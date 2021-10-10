/*


*/
LiquidCrystal_I2C lcd(0x27,20,4);

#include "lcdI2C.h"
#include "mpu6050.h"




#define	MPU_6050_ADDRESS	0x68

mpu6050 mpu(MPU_6050_ADDRESS);



void setup()
{
	Serial.begin(115200);

	mpu.initMPU6050(250, 2);

	//lcd.print(".");
}

void loop()
{

}
