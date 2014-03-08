/**
 * @file SensorTypes.h
 * @author Johan Simonsson
 * @brief SensorType has the supported list of sensors
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

#ifndef  __SENSORTYPES_H
#define  __SENSORTYPES_H


/// A enumeration of supported sensors
typedef enum
{
    SENSOR_NONE = 0,  ///< No sensor
    SENSOR_DS18B20,   ///< DS18B20 a OneWire temperature sensor.
    SENSOR_LVTS_LM34, ///< LM34 a low voltage temperature sensor. 10mV per degF.
    SENSOR_LVTS_LM35  ///< LM35 a low voltage temperature sensor. 10mV per degC.
} FT_SensorType;


#endif  // __SENSORTYPES_H
