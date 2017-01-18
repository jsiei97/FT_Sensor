/**
 * @file DS18B20Helper.h
 * @author Johan Simonsson
 * @brief Help functions for the DS18B20
 *
 * It is placed here to simplify testing
 */


#ifndef  __DS18B20HELPER_H
#define  __DS18B20HELPER_H

#include <stdint.h>

class DS18B20Helper
{
    private:
    public:
        static double convertTemperature(uint16_t);


};

#endif  // __DS18B20HELPER_H
