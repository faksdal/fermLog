


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

		if(__LCD_PRESENT__){
			if(i % 150 == 0)
				lcd.print(".");
		}
		

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
	// this means that if we lay the device level flat, x and y should be 0 and z should be the value stored in accSensitivityScalingFactor
	//
	calibrateValueAccX = (ax / CALIBRATION_READINGS);
	calibrateValueAccY = (ay / CALIBRATION_READINGS);
	calibrateValueAccZ = (az / CALIBRATION_READINGS); // - accSensitivityScalingFactor;
	
	
	Serial.print("accSensitivityScalingFactor: ");
	Serial.print(accSensitivityScalingFactor);
	Serial.print(" | gyroSensitivityScalingFactor: ");
	Serial.print(gyroSensitivityScalingFactor);
	
	Serial.print(" | calibrateValueAccX: ");
	Serial.print(calibrateValueAccX);
	Serial.print(" | calibrateValueAccY: ");
	Serial.print(calibrateValueAccY);
	Serial.print(" | calibrateValueAccZ: ");
	Serial.println(calibrateValueAccZ);

}
