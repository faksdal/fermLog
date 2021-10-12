/*


*/

//
#include <LiquidCrystal_I2C.h>
#include "Wire.h"
#include "mpu6050.h"

LiquidCrystal_I2C lcd(0x27,20,4);






#define	MPU_6050_ADDRESS	0x68

mpu6050 mpu(MPU_6050_ADDRESS);
//LiquidCrystal_I2C lcd(0x27,20,4);



void setup()
{
	Serial.begin(115200);
	
	// initialize the 16x2 I2C LCD
	lcd.init();
	lcd.backlight();

	mpu.initMPU6050(250, 2);

}

void loop()
{

}
