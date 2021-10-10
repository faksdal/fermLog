#include "mpu6050.h"

//
// calculate angles from calibrated data
//
void mpu6050::mpuAngleCalculations(void)
{
	//
	// calibrate raw data, if they're not already calibrated
	if(!dataCalibrated)
		addCalibrationData();
	
	
	//
	// gyro output is a signed 16-bit integer in units [LSB / °/s], a value per degrees per second
	// this means we have to divide the gyro output with our [gyroSensitivityScalingFactor] to get output in degrees per second (°/s)
	// furthermore, if we multiply with the time between to readings, we get an output in degrees (°)
	// we use the variable [loopTime] to keep track of time...
	//
	pitchAngleGyro	+= rawGyroX / gyroSensitivityScalingFactor * loopTime;	// this gives us [pitchAngleGyro] in degrees (°)
	rollAngleGyro	+= rawGyroY / gyroSensitivityScalingFactor * loopTime;	// this gives us [rollAngleGyro] in degrees (°)
	yawAngleGyro	+= rawGyroY / gyroSensitivityScalingFactor * loopTime;	// this gives us [yawAngleGyro] in degrees (°)
	
	/*
	Serial.print("pitchAngleGyro: ");
	Serial.print(pitchAngleGyro, 3);
	Serial.print("°");
	Serial.print(" | rollAngleGyro: ");
	Serial.print(rollAngleGyro, 3);
	Serial.print("°");
	Serial.print(" | yawAngleGyro: ");
	Serial.print(yawAngleGyro, 3);
	Serial.println("°");
	*/
	
	
	
	//
	// accelerometer output is in LSB/g's (g is gravitational acceleration of apx. 9.81 m/s²)
	// if we divide the output by [accSensitivityScalingFactor] we get the output in g
	// these values, from the three axis', can now be used to determine the attitude of the sensor
	//
	pitchAngleAcc	= rawAccX / accSensitivityScalingFactor;	// this gives output in g's
	rollAngleAcc	= rawAccX / accSensitivityScalingFactor;	// this gives output in g's
	yawAngleAcc		= rawAccX / accSensitivityScalingFactor;	// this gives output in g's
	
	pitch	= (.9 * pitchAngleGyro) + (.1 * pitchAngleAcc);
	roll	= (.9 * rollAngleGyro) + (.1 * rollAngleAcc);
	
}



//
// adds the calibration data to the last read values
//
void mpu6050::addCalibrationData(void)
{
	if(!dataCalibrated){
		//
		// I need to calibrate the accelerometer such that the values are correct when it lays flat down on the table
		// we should get a reading of zero on the x- and y-axis, and a reading of accSensitivityScalingFactor (here 16384) for the z-axis
		//
		//rawAccX -= calibrateValueAccX;
		//rawAccY -= calibrateValueAccY;
		//rawAccZ -= calibrateValueAccZ;

		rawGyroX -= calibrateValueGyroX;
		rawGyroY -= calibrateValueGyroY;
		rawGyroZ -= calibrateValueGyroZ;
	
		dataCalibrated = true;
	}
}



//
// reading raw gyro, temperature and accelerometer data
//
void mpu6050::readMPU6050Data(void)
{
    // initiating comms
    Wire.beginTransmission(mpu6050Address);

    // setting the requested start register
    Wire.write(0x3B);

    // end the transmision
    Wire.endTransmission();

    // we're requesting 14 bytes
    Wire.requestFrom(mpu6050Address, 14);

    // wait for all bytes to be received
    while(Wire.available() < 14);

    // filling in the raw data
	rawAccX    = Wire.read() << 8 | Wire.read();
	rawAccY    = Wire.read() << 8 | Wire.read();
	rawAccZ    = Wire.read() << 8 | Wire.read();
	rawTemp    = Wire.read() << 8 | Wire.read();
	rawGyroX   = Wire.read() << 8 | Wire.read();
	rawGyroY   = Wire.read() << 8 | Wire.read();
	rawGyroZ   = Wire.read() << 8 | Wire.read();
	
	dataCalibrated = false;

    // We need to know how much time has elapsed since the last gyro read.
    // This value is used later to integrate angular rate over time and convert it to
    // angular displacement
    lastTime    = currentTime;
    currentTime = micros();
    loopTime = (currentTime - lastTime) / 1000000.0;
}



//
//
//
uint8_t mpu6050::getRegister(uint8_t registerToGet)
{
    Wire.beginTransmission(mpu6050Address);
    Wire.write(registerToGet);
    Wire.endTransmission(false);
    Wire.requestFrom(mpu6050Address, 1, true);

    return(Wire.read());
}



//
//
//
int mpu6050::setRegister(uint8_t registerToSet, uint8_t registerValue)
{
    // initiate the comms
    Wire.beginTransmission(mpu6050Address);

    // write the register number
    Wire.write(registerToSet);

    // write the new value
    Wire.write(registerValue);

    // nothing is written until we end the transmission
    // this returns a non-zero value upon error as described here: https://www.arduino.cc/en/Reference/WireEndTransmission
    return(Wire.endTransmission());
}
	


void mpu6050::calibrateMPU6050(void)
{
	int32_t	gx, gy, gz, ax, ay, az;
	

	// zero out the calibration data before we begin
	calibrateValueAccX	= 0;
	calibrateValueAccY	= 0;
	calibrateValueAccZ	= 0;
	calibrateValueTemp	= 0;
	calibrateValueGyroX	= 0;
	calibrateValueGyroY	= 0;
	calibrateValueGyroZ	= 0;

	ax = gx = ay = gy = az = gz = 0;

	// read the device 'CALIBRATION_READINGS' number of times to establish a basevalue
    // it is important the device remains level and still during this process
    //lcdPrint(true, 0, 0, "Calibrating");
    //lcdPrint(false, 0, 1, ".");

	
	// TODO: discard the first hundred readings
	for(int i = 0; i < CALIBRATION_READINGS; i++){
		if(i % 200 == 0)
			lcd.print(".");

		readMPU6050Data();
		gx += rawGyroX;
		gy += rawGyroY;
		gz += rawGyroZ;

		ax += rawAccX;
		ay += rawAccY;
		az += rawAccZ;
	}
	
	//
	// divide by number of readings to get a mean value
	//
	// gyro output should be 0 when not in motion
	//
	calibrateValueGyroX = (gx / CALIBRATION_READINGS);
	calibrateValueGyroY = (gy / CALIBRATION_READINGS);
	calibrateValueGyroZ = (gz / CALIBRATION_READINGS);

	//
	// accelerometer output is depending on full scale range
	// this means that if we lay the device level flat, x and y should be 0 and z should be the vlaue stored in accSensitivityScalingFactor
	//
	calibrateValueAccX = (ax / CALIBRATION_READINGS);
	calibrateValueAccY = (ay / CALIBRATION_READINGS);
	calibrateValueAccZ = (az / CALIBRATION_READINGS) - accSensitivityScalingFactor;

}



//
// initialises the mpu, returning a non-zero value if fail
//
// gyroSensitivity is one of four values, 250, 500, 1000 or 2000, referring to degrees per second [°/s]
// this is described on p.12 i MPU-6000/MPU-6050 Product Specification rev.3.4
//
// accSensitivity is one of four values, 2, 4, 8 or 16, referring to g's
// described on p.13 in MPU-6000/MPU-6050 Product Specification rev.3.4
//
int mpu6050::initMPU6050(uint16_t _gyroSensitivity, uint16_t _accSensitivity)
{
	int retVal;
	
		
    // the device will start in sleep mode as described in the MPU-6000/MPU6050 Register Map and Description document p.9
    // this sequence starts up the device
    // initiate communication with the MPU-6050
    Wire.beginTransmission(mpu6050Address);

    // tell which register we want to access
    Wire.write(MPU6050_REG_PWR_MGMT_1);

    // write a '0' to start up the MPU
    Wire.write(0);

    // the endTransmission function returns a non-zero value if it detects an error
    // most likely it's an address issue
	retVal = Wire.endTransmission();
	
	// if no error we can proceed with setting up the device
	if(retVal == 0){
		gyroSensitivity	= _gyroSensitivity;
		accSensitivity	= _accSensitivity;
	
		if(gyroSensitivity == 250){
			//gyroFS_SEL &= ~(3 << 4);
			gyroFS_SEL = 0;
			gyroSensitivityScalingFactor = 131;
			retVal = setRegister(MPU6050_REG_GYRO_CONFIG, 0b00000000);
		}
		else if(gyroSensitivity == 500){
			gyroFS_SEL = 1;
			gyroSensitivityScalingFactor = 65.5;
			retVal = setRegister(MPU6050_REG_GYRO_CONFIG, 0b00001000);
		}
		else if(gyroSensitivity == 1000){
			gyroFS_SEL = 2;
			gyroSensitivityScalingFactor = 32.8;
			retVal = setRegister(MPU6050_REG_GYRO_CONFIG, 0b00010000);
		}
		else if(gyroSensitivity == 2000){
			gyroFS_SEL = 3;
			gyroSensitivityScalingFactor = 16.4;
			retVal = setRegister(MPU6050_REG_GYRO_CONFIG, 0b00011000);
		}
		
		
		if(accSensitivity == 2){
			accFS_SEL = 0;
			accSensitivityScalingFactor = 16384;
			retVal = setRegister(MPU6050_REG_ACCEL_CONFIG, 0b00000000);
		}
		else if(accSensitivity == 4){
			accFS_SEL = 1;
			accSensitivityScalingFactor = 8192;
			retVal = setRegister(MPU6050_REG_ACCEL_CONFIG, 0b00001000);
		}
		else if(accSensitivity == 8){
			accFS_SEL = 2;
			accSensitivityScalingFactor = 4096;
			retVal = setRegister(MPU6050_REG_ACCEL_CONFIG, 0b00010000);
		}
		else if(accSensitivity == 16){
			accFS_SEL = 3;
			accSensitivityScalingFactor = 2048;
			retVal = setRegister(MPU6050_REG_ACCEL_CONFIG, 0b00011000);
		}
		
		//set up the MPU-6050 to our use
		//
		//
		// set FIFO register, p.16 https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
		//
		// writing this to 0, no data in the FIFO buffer
		setRegister(MPU6050_REG_FIFO_EN, 0);
		
		//
		// set CONFIG register, p.13 https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
		// DLPF_CFG is set to 6, this is the heaviest filter
		setRegister(MPU6050_REG_CONFIG, 0b00000110);
		
		//
		// set SMPTR_DIV register, p.11 https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
		setRegister(MPU6050_REG_SMPTR_DIV, 0);
	}
	
    return(retVal);
}

	


//
// mpu6050 constructor
//
mpu6050::mpu6050(uint8_t address)
{
	mpu6050Address = address;

	// initialize the i2c bus
	Wire.begin();
	Wire.setClock(400000);
	delay(7);

	//
	// zero the variables
	rawAccX				= 0;
	rawAccY				= 0;
	rawAccZ				= 0;
	rawTemp				= 0;
	rawGyroX			= 0;
	rawGyroY			= 0;
	rawGyroZ			= 0;
	
	angleAccX = angleAccY = angleAccZ = angleGyroX = angleGyroY = angleGyroZ = 0.0;
	
	totalVectorAcc		= 0.0;
	pitch				= 0.0;
	roll				= 0.0;
	yaw					= 0.0;
	pitchAngleGyro		= 0.0;
	rollAngleGyro		= 0.0;
	yawAngleGyro		= 0.0;
	pitchAngleAcc		= 0.0;
	rollAngleAcc		= 0.0;
	yawAngleAcc			= 0.0;
	
	setGyroAngles		= false;
	dataCalibrated		= false;
}
