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
