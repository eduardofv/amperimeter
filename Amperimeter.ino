/*
Amperimeter with a ACS712 module and a LCD
Output is also sent to Serial in tab separeated values format:

average read from sensor \t voltage \t current

In my tests the output voltage for no-load (0 current) from
the ACS712 looks unreliable. Supposedly the bias (or offset)
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
 
 The circuit:
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

References:
https://www.electronicshub.org/interfacing-acs712-current-sensor-with-arduino/
https://learn.sparkfun.com/tutorials/acs712-low-current-sensor-hookup-guide
https://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay
Look for ACS712 module datasheets, like
https://rees52.com/arduino-sensors/1162-acs712-30a-hall-current-sensor-module-sr045

*/

//LCD
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//ACS712 
const unsigned int curr_sens_pin = A0;
const unsigned int curr_sens_avg_count = 1000; //How many for avg
const float curr_sens_max = 5.0; //Vin for ADC
const float curr_sens_factor = 1000.0; //amps
const unsigned int curr_sens_calibration_steps = 100;

float curr_sens_slope = 185.0; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module
float curr_sens_v_bias = 2.494; //offset - hay que calibrar

//Other
#define LOOP_DELAY 200
#define VERSION "1.0"


float get_current_sensor_voltage(){
  unsigned long vsens=0;
  for(int i=0; i < curr_sens_avg_count; i++){
    vsens += analogRead(curr_sens_pin);
  }
  float vsens_f = float(vsens);
  vsens_f = vsens_f / float(curr_sens_avg_count);  
  float vf = (vsens_f / 1024.0) * curr_sens_max;

  Serial.print(vsens_f);
  Serial.print("\t");
  Serial.print(vf, 3);
  Serial.print("\t");

  return vf;
}

float get_current_sensor(){
  float vf = get_current_sensor_voltage();
  float curr = curr_sens_factor * (vf - curr_sens_v_bias) / curr_sens_slope;
  
  Serial.println(curr, 2);
  
  return curr; 
}


void calibrate(){
  lcd.clear();
  lcd.print("Amperimeter");
  lcd.print(" ");
  lcd.print(VERSION);
  lcd.setCursor(0, 1);
  lcd.print("Disconnect load");
  delay(2000);
  
  lcd.clear();
  lcd.print("Zeroing...");
  lcd.blink();
  float vf = 0.0;
  for(int i=0; i < curr_sens_calibration_steps; i++)
    vf += get_current_sensor_voltage();
  vf /= curr_sens_calibration_steps;
  curr_sens_v_bias = vf;
  lcd.noBlink();
  
  lcd.clear();
  lcd.println("curr_sens_v_bias");
  char buf[17]; 
  dtostrf(vf, 4, 4, buf);
  lcd.setCursor(0, 1);
  lcd.print(buf);
  delay(2000);
}


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Amperimeter");
  Serial.begin(115200);
  Serial.println();
  calibrate();
}


void loop() {
  float current = get_current_sensor();
  lcd.clear();
  lcd.print("Amperes");
  lcd.setCursor(0, 1);

  char buf[17]; 
  dtostrf(current, 4, 2, buf);
  if(current >= 0.0)
    lcd.print(" ");
  lcd.print(buf);
  delay(LOOP_DELAY);
}
