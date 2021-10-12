






void lcdPrint(bool clearScreen, int8_t x, int8_t y, char *msg)
{
    if(clearScreen)
        lcd.clear();

    lcd.setCursor(x, y);
    lcd.print(msg);
}



void lcdPrint(bool clearScreen, int8_t x, int8_t y, int msg, uint16_t numberFormat = DEC)
{
    if(clearScreen)
        lcd.clear();

    if(x > -1)
        lcd.setCursor(x, y);
        
    lcd.print(msg, numberFormat);
}
