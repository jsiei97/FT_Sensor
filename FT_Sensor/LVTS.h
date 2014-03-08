/**
 * @file LVTS.h
 * @author Johan Simonsson
 * @brief Low Voltage Temperature Sensor Class
 */

/*
 * Copyright (C) 2014 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef  __LVTS_H
#define  __LVTS_H

#include "SensorTypes.h"

/**
 * Low Voltage Temperature Sensor Class
 *
 * This is a wrapper class for analog low voltage temperature sensors
 * like the LM35 and LM34.
 */
class LVTS
{
    private:
        int pin;            ///< What IO pin is this sensor using?
        FT_SensorType type; ///< What sensor type is this, i.e. LM35

        static double lm34(int reading, bool *ok);
        static double lm35(int reading, bool *ok);
        static double F2C(double degC);

    public:
        LVTS(int pin, FT_SensorType type);
        bool getTemperature(double* value);

};

#endif  // __LVTS_H
