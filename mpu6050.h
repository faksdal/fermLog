/*
 * mpu6050.h
 *
 *  Created on: 11 Oct 2021
 *      Author: jole
 */

#ifndef MPU6050_H_
#define MPU6050_H_


// defining some MPU-6050 register names
/**********************************************************/
#define MPU6050_REG_SMPTR_DIV       0x19
#define MPU6050_REG_CONFIG          0x1A
#define MPU6050_REG_GYRO_CONFIG     0x1B
#define MPU6050_REG_ACCEL_CONFIG    0x1C
#define MPU6050_REG_FIFO_EN         0x23
#define MPU6050_REG_PWR_MGMT_1      0x6B
/**********************************************************/
//
// define how many readings we do for the gyro calibration
#define CALIBRATION_READINGS		2000


//
// sensitivity factors for the device
// these values varies with setup, see the documentation for more
/*
Gyro sensitivity as per p.12 in MPU-6000-Datasheet	
---------------------------------
| FS_SEL=0 |  131	| ± 250º/s  |
---------------------------------
| FS_SEL=1 |   65.5	| ± 250º/s  |
---------------------------------
| FS_SEL=2 |   32.8	| ± 250º/s  |
---------------------------------
| FS_SEL=3 |   16.4	| ± 250º/s  |
---------------------------------


Accelerometer sensitivity as per p.13 in MPU-6000-Datasheet
------------------------------
| FS_SEL=0 | 16384	| ± 2 g  |
------------------------------
| FS_SEL=1 |  8192	| ± 4 g  |
------------------------------
| FS_SEL=2 |  4096	| ± 8 g  |
------------------------------
| FS_SEL=3 |  2048	| ± 16 g |
------------------------------
*/



class mpu6050{

	bool	setGyroAngles, dataCalibrated;

	uint8_t	mpu6050Address, gyroFS_SEL, accFS_SEL;

	int16_t	rawAccX, rawAccY, rawAccZ, rawTemp, rawGyroX, rawGyroY, rawGyroZ;
	int16_t	calibrateValueAccX, calibrateValueAccY, calibrateValueAccZ, calibrateValueTemp, calibrateValueGyroX, calibrateValueGyroY, calibrateValueGyroZ;

	uint16_t gyroSensitivity, accSensitivity, accSensitivityScalingFactor;

	float	angleAccX, angleAccY, angleAccZ, angleGyroX, angleGyroY, angleGyroZ;
	float	totalVectorAcc;
	float	pitch, roll, yaw;
	float	pitchAngleGyro, rollAngleGyro, yawAngleGyro, pitchAngleAcc, rollAngleAcc, yawAngleAcc;
	float	gyroSensitivityScalingFactor;

	double	lastTime, currentTime, loopTime;


public:
			mpu6050(uint8_t address);

	int		initMPU6050(uint16_t _gyroSensitivity, uint16_t _accSensitivity);
	void	calibrateMPU6050(void);
	int		setRegister(uint8_t registerToSet, uint8_t registerValue);
	uint8_t	getRegister(uint8_t registerToGet);
	void	readMPU6050Data(void);
	void	addCalibrationData(void);
	void	mpuAngleCalculations(void);

	/*
	// defining some inline functions so that we have access to the private data members
	uint8_t	getMPUAddress(void) {return MPU_6050_ADDRESS;}
	int16_t	getAccX(void)			{return accX;}
	int16_t	getAccY(void)			{return accY;}
	int16_t	getAccZ(void)			{return accZ;}
	int16_t	getGyroX(void)			{return gyroX;}
	int16_t	getGyroY(void)			{return gyroY;}
	int16_t	getGyroZ(void)			{return gyroZ;}
	float	getPitchAngleGyro(void)	{return pitchAngleGyro;}
	float	getRollAngleGyro(void)	{return rollAngleGyro;}
	float	getYawAngleGyro(void)	{return yawAngleGyro;}
	float	getPitchAngleAcc(void)	{return pitchAngleAcc;}
	float	getRollAngleAcc(void)	{return rollAngleAcc;}
	float	getPitch(void)			{return pitch;}
	float	getRoll(void)			{return roll;}
	double	getElapsedTime(void)	{return elapsedTime;}
	*/

};

#endif /* MPU6050_H_ */
