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
	
	pitch	= (.9996 * pitchAngleGyro) + (.0004 * pitchAngleAcc);
	roll	= (.9 * rollAngleGyro) + (.1 * rollAngleAcc);

	lcd.clear();
	lcd.print(pitch);
	
}
