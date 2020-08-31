# Arduino Amperimeter with a ACS-712 module and a LCD

## Description

A simple Amperimeter (or [Ammeter](https://en.wikipedia.org/wiki/Ammeter)) using a readily-available hall effect current sensor. Output is both to the LCD and to Serial in tab-separeated-value format:

`average_read_from_sensor \t voltage \t current`

In my tests the output voltage for no-load (0 current) from
the ACS-712 looks unreliable. Supposedly the bias (or offset)
should be 2.5 volts but it varies a lot. I've implemented a
calibration / zeroing procedure at startup to average the 
values read without load and take that value as the actual
bias. Reset Arduino if necessary to recalibrate.


Set curr_sens_slope the the adequate value for the ACS version
you have: 5A, 20A, 30A. Resolution would depend on this. I've
found the 30A version be more stable despite more limited 
resolution.  


Use a LCD w/Hitachi HD44780 driver compatible with 
LiquidCrystal lib

 
## The circuit:

LCD:

* LCD RS pin to digital pin 12
* LCD Enable pin to digital pin 11
* LCD D4 pin to digital pin 5
* LCD D5 pin to digital pin 4
* LCD D6 pin to digital pin 3
* LCD D7 pin to digital pin 2
* LCD R/W pin to ground
* LCD VSS pin to ground
* LCD VCC pin to 5V
* 10K resistor:
* ends to +5V and ground
* wiper to LCD VO pin (pin 3)

ACS 712:

* Vin: 5v from regulator
* GND: GND
* Output: Arduino A0

## References:

https://www.electronicshub.org/interfacing-acs712-current-sensor-with-arduino/
https://learn.sparkfun.com/tutorials/acs712-low-current-sensor-hookup-guide
https://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay

Look for ACS712 module datasheets, like

https://rees52.com/arduino-sensors/1162-acs712-30a-hall-current-sensor-module-sr045
