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
