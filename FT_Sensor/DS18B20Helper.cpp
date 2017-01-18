/**
 * @file DS18B20Helper.cpp
 * @author Johan Simonsson
 * @brief Help functions for the DS18B20
 *
 * It is placed here to simplify testing
 */

#include <stdint.h>
//#include <stdio.h>

#include "DS18B20Helper.h"

double DS18B20Helper::convertTemperature(uint16_t value)
{
    double result = 0.0;

    uint16_t dec = value & 0x000F;
    value = value >> 4;

    if(value & 0x800)
    {
        //The negative values
        value += 0xF000;        // Fill will 1
        value--;                // Remove the "one"
        value = value ^ 0xFFFF; // invert
        result = value;         // switch type
        result = -result;       // And make it negative

        double decD = dec/16.0; // Then put the decimals back
        result += decD;
    }
    else
    {
        //The positive cases
        result = value;

        double decD = dec/16.0;
        result += decD;
    }

    //printf("%04x \n", value);
    return result;
}

