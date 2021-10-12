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
