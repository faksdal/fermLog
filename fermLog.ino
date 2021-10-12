/*


*/

//
#include <LiquidCrystal_I2C.h>
#include "Wire.h"
#include "mpu6050.h"

void lcdPrint(bool clearScreen, int8_t x, int8_t y, char *msg);
void lcdPrint(bool clearScreen, int8_t x, int8_t y, int msg, uint16_t numberFormat);

LiquidCrystal_I2C lcd(0x27,20,4);


#define	MPU_6050_ADDRESS	0x68
#define __LCD_PRESENT__		true

mpu6050 mpu(MPU_6050_ADDRESS);



void setup()
{
	Serial.begin(115200);

	if(__LCD_PRESENT__){
		// initialize the 16x2 I2C LCD
		lcd.init();
		lcd.backlight();
		
		// print a welcome message
		lcdPrint(true, 3, 0, "fermLog v1");
		lcdPrint(false, 3, 1, "Jon Leithe");
		delay(750);
		lcd.clear();
	}
	
	// initializing the MPU-6050 and tell the user if we're ok or not
	if(!mpu.initMPU6050(250, 2)){
		if(__LCD_PRESENT__){
			lcdPrint(true, 0, 0, "MPU found at");
			lcdPrint(false, 0, 1, "hex 0x");
			lcdPrint(false, -1, -1, mpu.getMPUAddress(), HEX);
			delay(1250);
		}
	}
	else{
		if(__LCD_PRESENT__){
			lcdPrint(true, 0, 0, "NO MPU at");
			lcdPrint(false, 0, 1, "hex 0x");
			int t = mpu.getMPUAddress();
			lcdPrint(false, -1, -1, mpu.getMPUAddress(), HEX);
			delay(1250);
			lcdPrint(true, 0, 0, "Exiting...");
		}
		return;
	}
	lcd.clear();
		
	
	mpu.calibrateMPU6050();
	lcd.clear();

}

void loop()
{
	mpu.readMPU6050Data();
	mpu.addCalibrationData();
	mpu.mpuAngleCalculations();
}
