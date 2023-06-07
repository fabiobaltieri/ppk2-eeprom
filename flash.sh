#!/bin/bash

nrfutil pkg generate --hw-version 52 --sd-req=0x00 \
	--application build/zephyr/zephyr.hex \
	--application-version 1 ppk2-eeprom.zip

nrfutil dfu usb-serial -pkg ppk2-eeprom.zip -p /dev/ttyACM0
