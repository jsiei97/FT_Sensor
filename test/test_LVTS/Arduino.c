/**
 * @file Arduino.c
 * @author Johan Simonsson
 * @brief Stub the hw dependent code on the Arduino
 */
#include "Arduino.h"

unsigned long my_local_millis = 0;
unsigned long millis(void)
{
    return my_local_millis;
}


unsigned long my_local_analog = 0;
int analogRead(unsigned int pin)
{
    //Remove unused parameter warning
    pin++;
    return my_local_analog;
}



void pinMode(unsigned int pin, unsigned int mode)
{
    //Remove unused parameter warning
    pin++;
    mode++;
}
