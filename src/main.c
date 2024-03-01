/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/eeprom.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

#define EEPROM_BUF_SZ 0x100

static const struct device *cal_eeprom = DEVICE_DT_GET(
		DT_NODELABEL(cal_eeprom));

static struct gpio_dt_spec reset_trigger_gpio = GPIO_DT_SPEC_GET(
		ZEPHYR_USER_NODE, reset_trigger_gpios);

static struct {
	uint8_t off;
	char *name;
} cal_offsets[] = {
	{0, "r0"},
	{4, "r1"},
	{8, "r2"},
	{12, "r3"},
	{16, "r4"},

	{20, "gs0"},
	{24, "gs1"},
	{28, "gs2"},
	{32, "gs3"},
	{36, "gs4"},

	{40, "o0"},
	{44, "o1"},
	{48, "o2"},
	{52, "o3"},
	{56, "o4"},

	{60, "s0"},
	{64, "s1"},
	{68, "s2"},
	{72, "s3"},
	{76, "s4"},

	{80, "i0"},
	{84, "i1"},
	{88, "i2"},
	{92, "i3"},
	{96, "i4"},

	{100, "gi0"},
	{104, "gi1"},
	{108, "gi2"},
	{112, "gi3"},
	{116, "gi4"},

	{128, "ug0"},
	{132, "ug1"},
	{136, "ug2"},
	{140, "ug3"},
	{144, "ug4"},
};

#define OFFSET_COUNT ARRAY_SIZE(cal_offsets)

static int cmd_reset_bl(const struct shell *sh, size_t argc, char **argv)
{
	gpio_pin_configure_dt(&reset_trigger_gpio, GPIO_OUTPUT_ACTIVE);

	return 0;
}
SHELL_CMD_REGISTER(reset_bl, NULL, "Reset into bootloader", cmd_reset_bl);

static int cmd_float_bytes(const struct shell *sh, size_t argc, char **argv)
{
	float f;
	uint8_t *x = (uint8_t *)&f;

	f = strtof(argv[1], NULL);

	shell_print(sh, "float: %.9f", (double)f);
	shell_print(sh, "bytes: %02x %02x %02x %02x", x[0], x[1], x[2], x[3]);
	return 0;
}
SHELL_CMD_ARG_REGISTER(float_bytes, NULL,
		       "Convert a float to bytes\nfloat_bytes <float>",
		       cmd_float_bytes, 2, 0);

static int cmd_cal_read(const struct shell *sh, size_t argc, char **argv)
{
	uint8_t buf[EEPROM_BUF_SZ];
	int err;
	int i;

	err = eeprom_read(cal_eeprom, 0, buf, sizeof(buf));
	if (err) {
		shell_error(sh, "EEPROM read failed (err %d)", err);
		return err;
	}

	for (i = 0; i < OFFSET_COUNT; i++) {
		uint8_t offset;
		float *f;
		uint8_t *x;

		offset = cal_offsets[i].off;

		f = (float *)&buf[offset];
		x = &buf[offset];

		shell_print(sh, "%4s (%3d): %15.9f, %02x %02x %02x %02x",
			    cal_offsets[i].name,
			    offset,
			    (double)*f,
			    x[0], x[1], x[2], x[3]);
	}

	return 0;
}
SHELL_CMD_REGISTER(cal_read, NULL, "Read the calibration values", cmd_cal_read);

static int cmd_cal_write(const struct shell *sh, size_t argc, char **argv)
{
	bool dry_run;
	char *name;
	uint8_t offset;
	float f;
	int i;
	int err;
	uint8_t *x = (uint8_t *)&f;

	dry_run = true;
	if (argc == 4 && strncmp(argv[3], "y", 1) == 0) {
		dry_run = false;
	}

	name = argv[1];

	f = strtof(argv[2], NULL);
	
	for (i = 0; i < OFFSET_COUNT; i++) {
		if (strcmp(name, cal_offsets[i].name) == 0) {
			break;
		}
	}
	if (i == OFFSET_COUNT) {
		shell_error(sh, "invalid argument: %s", name);
		return 0;
	}

	offset = cal_offsets[i].off;

	shell_print(sh, "updating %s (%d) to %.9f, %02x %02x %02x %02x",
		    name,
		    offset,
		    (double)f,
		    x[0], x[1], x[2], x[3]);

	if (dry_run) {
		shell_print(sh, "dry run: add a \"y\" argument to actually write");
		return 0;
	}

	err = eeprom_write(cal_eeprom, offset, x, sizeof(x));
	if (err) {
		shell_error(sh, "EEPROM write failed (err %d)", err);
		return err;
	}

	shell_print(sh, "done");

	return 0;
}

SHELL_CMD_ARG_REGISTER(cal_write, NULL,
		       "Write a calibration file\ncal_write <name> <val> [y]",
		       cmd_cal_write, 3, 1);
