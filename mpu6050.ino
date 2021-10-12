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
