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
