#include "Wire.h"
#include <Arduino.h>

#define SDA_PIN 18
#define SCL_PIN 19
#define ADJUST_PIN 34
#define POT_VOLT_PIN 35
#define LED_BUILTIN 15

// I2C Address of device
#define DS1807_DEFAULT_ADDRESS 0x28 // A0,A1 & A2 are connected to GND

// Command definitions
#define DS1807_CMD_WRPOT0 0xA9
#define DS1807_CMD_WRPOT1 0xAA
#define DS1807_CMD_WRBOTH 0xAF
#define DS1807_CMD_ACT_ZC 0xBD
#define DS1807_CMD_DEACT_ZC 0xBD

// Common WIPER values
#define DS1807_WIPER_MAX 0x00
#define DS1807_WIPER_MID 0x20
#define DS1807_WIPER_MIN 0x3F
#define DS1807_WIPER_MUTE 0x40

int val = 0;  
int pot_val = 0;  

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	Serial.println("test");

	delay(1000);


	val = analogRead(ADJUST_PIN);
	pot_val = analogRead(POT_VOLT_PIN);
	Wire.beginTransmission(DS1807_DEFAULT_ADDRESS);
	Wire.write((uint8_t)DS1807_CMD_WRBOTH);
	uint8_t temp = ((val / 64) & 0xFF);
	Wire.write(temp);
	
	bool result = Wire.endTransmission() == 0;

	Serial.println(val);
	Serial.println(pot_val);


}
