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
