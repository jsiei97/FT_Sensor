/**
 * @file DS18B20.h
 * @author Johan Simonsson
 * @brief OneWire DS18B20 Temperature Sensor Class
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

#ifndef  __DS18B20_H
#define  __DS18B20_H


/// OneWire DS18B20 Temperature Sensor Class
class DS18B20
{
    private:
        int pin; ///< What IO pin is this sensor using?

    public:
        DS18B20(int pin);
        bool getTemperature(double* value);

};

#endif  // __DS18B20_H
