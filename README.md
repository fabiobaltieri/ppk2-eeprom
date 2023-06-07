# PPK2 calibration EEPROM utility

Calibration utility to read and write calibration values on a PPK2 power
profiler.

## Disclaimer

This tool is UNOFFICIAL, it has been written using only the publicly available
documentation and by decoding the EEPROM content by comparing values against
the ones reported in the desktop tool. Use it at your own risk, you assume all
responsibility for any damage caused to the device.

## Usage

Flash the firmware to a PPK device using `nrfutil` or any other flashing tool,
binary releases can be found at
https://github.com/fabiobaltieri/ppk2-eeprom/releases.

Example: `nrfutil dfu usb-serial -pkg ppk2-eeprom.zip -p /dev/ttyACM0`

The device will reconnect as a USB CDC-ACM serial port with a shell available
on it.

The following commands are available:

- `reset_bl`: reset back into the bootloader
- `cal_read`: read and print all the calibration values from the EEPROM
- `cal_write`: write one calibration value into the EEPROM
- `float_bytes`: convert a float to bytes
- `eeprom`: provides raw EEPROM access

To reprogram the device back to the PPK firmware, run the `reset_bl` or
temporarily connect TP34 to GND. The device will reboot into the bootloader and
the PPK desktop application will prompt to reprogram it.

## Example Usage

```
uart:~$ cal_read 
  r0 (  0):  1004.569885254, 79 24 7b 44
  r1 (  4):   102.184196472, 4f 5e cc 42
  r2 (  8):    10.382900238, 5c 20 26 41
  r3 ( 12):     0.972299993, a7 e8 78 3f
  r4 ( 16):     0.056699999, 42 3e 68 3d
 gs0 ( 20):     0.000000000, 00 00 00 00
 gs1 ( 24):   154.061004639, 9e 0f 1a 43
 gs2 ( 28):    23.392099380, 05 23 bb 41
 gs3 ( 32):     3.371000051, 77 be 57 40
 gs4 ( 36):     0.109369040, e0 fc df 3d
  o0 ( 40):   151.841400146, 66 d7 17 43
  o1 ( 44):   103.388397217, dc c6 ce 42
  o2 ( 48):    87.856201172, 60 b6 af 42
  o3 ( 52):    66.804603577, f5 9b 85 42
  o4 ( 56):   118.277618408, 24 8e ec 42
  s0 ( 60):    -0.000000028, a7 84 f0 b2
  s1 ( 64):    -0.000000274, 46 1a 93 b4
  s2 ( 68):    -0.000001803, 9c fe f1 b5
  s3 ( 72):    -0.000021984, 48 6a b8 b7
  s4 ( 76):     0.001923220, 8f 14 fc 3a
  i0 ( 80):     0.000000118, 30 67 fd 33
  i1 ( 84):     0.000001097, a2 3c 93 35
  i2 ( 88):     0.000071752, 90 79 96 38
  i3 ( 92):    -0.000214770, e2 33 61 b9
  i4 ( 96):    -0.007002169, 74 72 e5 bb
 gi0 (100):     1.000000000, 00 00 80 3f
 gi1 (104):     0.957199991, 0f 0b 75 3f
 gi2 (108):     0.950299978, dc 46 73 3f
 gi3 (112):     0.936200023, ce aa 6f 3f
 gi4 (116):     0.944025695, ab ab 71 3f
 ug0 (128):     1.000000000, 00 00 80 3f
 ug1 (132):     1.000000000, 00 00 80 3f
 ug2 (136):     1.000000000, 00 00 80 3f
 ug3 (140):     1.000000000, 00 00 80 3f
 ug4 (144):     1.000000000, 00 00 80 3f
uart:~$ cal_write r3 0.988
updating r3 (12) to 0.987999976, 91 ed 7c 3f
dry run: add a "y" argument to actually write
uart:~$ cal_write r3 0.988 y
updating r3 (12) to 0.987999976, 91 ed 7c 3f
done
uart:~$ cal_read
  r0 (  0):  1004.569885254, 79 24 7b 44
  r1 (  4):   102.184196472, 4f 5e cc 42
  r2 (  8):    10.382900238, 5c 20 26 41
  r3 ( 12):     0.987999976, 91 ed 7c 3f
  r4 ( 16):     0.056699999, 42 3e 68 3d
 gs0 ( 20):     0.000000000, 00 00 00 00
 gs1 ( 24):   154.061004639, 9e 0f 1a 43
 gs2 ( 28):    23.392099380, 05 23 bb 41
 gs3 ( 32):     3.371000051, 77 be 57 40
 gs4 ( 36):     0.109369040, e0 fc df 3d
  o0 ( 40):   151.841400146, 66 d7 17 43
  o1 ( 44):   103.388397217, dc c6 ce 42
  o2 ( 48):    87.856201172, 60 b6 af 42
  o3 ( 52):    66.804603577, f5 9b 85 42
  o4 ( 56):   118.277618408, 24 8e ec 42
  s0 ( 60):    -0.000000028, a7 84 f0 b2
  s1 ( 64):    -0.000000274, 46 1a 93 b4
  s2 ( 68):    -0.000001803, 9c fe f1 b5
  s3 ( 72):    -0.000021984, 48 6a b8 b7
  s4 ( 76):     0.001923220, 8f 14 fc 3a
  i0 ( 80):     0.000000118, 30 67 fd 33
  i1 ( 84):     0.000001097, a2 3c 93 35
  i2 ( 88):     0.000071752, 90 79 96 38
  i3 ( 92):    -0.000214770, e2 33 61 b9
  i4 ( 96):    -0.007002169, 74 72 e5 bb
 gi0 (100):     1.000000000, 00 00 80 3f
 gi1 (104):     0.957199991, 0f 0b 75 3f
 gi2 (108):     0.950299978, dc 46 73 3f
 gi3 (112):     0.936200023, ce aa 6f 3f
 gi4 (116):     0.944025695, ab ab 71 3f
 ug0 (128):     1.000000000, 00 00 80 3f
 ug1 (132):     1.000000000, 00 00 80 3f
 ug2 (136):     1.000000000, 00 00 80 3f
 ug3 (140):     1.000000000, 00 00 80 3f
 ug4 (144):     1.000000000, 00 00 80 3f
uart:~$ cal_write r3 0.972299993
updating r3 (12) to 0.972299993, a7 e8 78 3f
dry run: add a "y" argument to actually write
uart:~$ cal_write r3 0.972299993 y
updating r3 (12) to 0.972299993, a7 e8 78 3f
done
uart:~$ reset_bl 
```
