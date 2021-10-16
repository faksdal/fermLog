/*


*/

//
#include <LiquidCrystal_I2C.h>
#include "Wire.h"
#include "mpu6050.h"

void lcdPrint(bool clearScreen, int8_t x, int8_t y, char *msg);
void lcdPrint(bool clearScreen, int8_t x, int8_t y, int msg, uint16_t numberFormat);

LiquidCrystal_I2C lcd(0x27,20,4);


#define __LCD_PRESENT__			true

#define	MPU_6050_ADDRESS		0x68

#define __GYRO_SENSITIVITY__	500
#define __ACC_SENSITIVITY__		4


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
	if(!mpu.initMPU6050(__GYRO_SENSITIVITY__, __ACC_SENSITIVITY__)){
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

	//set up the MPU-6050 to our use
	//
	//
	// set FIFO register, p.16 https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
	//
	// writing this to 0, no data in the FIFO buffer
	mpu.setRegister(MPU6050_REG_FIFO_EN, 0);
	delay(5);
	//
	// set CONFIG register, p.13 https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
	// DLPF_CFG is set to 6, this is the heaviest filter
	mpu.setRegister(MPU6050_REG_CONFIG, 0b00000110);
	delay(5);
	//
	// set SMPTR_DIV register, p.11 https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
	mpu.setRegister(MPU6050_REG_SMPTR_DIV, 0);
	delay(5);
	//
	// set GYRO_CONFIG register, p.14 https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
	// bits 4 and 3 are set to 0, giving a full scale range of ±250°/s
	// this gives an output of 131 per °/s of movement
	// meaning we have to divide our reading with 131 to get degrees
	mpu.setRegister(MPU6050_REG_GYRO_CONFIG, 0b00000000);
	delay(5);
	//
	// set ACCEL_CONFIG register, p.15 https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
	// bits 4 and 3 are set to 0, giving a full scale range of 2g (1g equals 9.81 m/s²)
	// this gives an output of 16384 for 1g of force
	mpu.setRegister(MPU6050_REG_ACCEL_CONFIG, 0b00000000);
	delay(5);
	
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
