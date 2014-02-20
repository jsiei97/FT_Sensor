/**
 * @file TemperatureSensor.h
 * @author Johan Simonsson
 * @brief A temperature sensor class with alarm logic
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

#ifndef  __TEMPERATURESENSOR_H
#define  __TEMPERATURESENSOR_H


#include "DS18B20.h"
#include "LVTS.h"
#include "SensorTypes.h"


typedef enum
{
    SENSOR_ALARM_NO = 0, ///< No active alarm
    SENSOR_ALARM_SENSOR, ///< There is a sensor error
    SENSOR_ALARM_HIGH,   ///< High level alarm
    SENSOR_ALARM_LOW,    ///< Low level alarm
} SensorAlarmNumber;


class TemperatureSensor
{
    private:
        FT_SensorType type; ///< What kind of sensor is this object?
        DS18B20* ds;        ///< Ref to DS18B20 if correct type.
        LVTS*    lvts;      ///< Ref to LVTS if correct type.

        unsigned int failcnt; ///< If sensor read fails, then this value inc. Zero is ok.

        //Variables for the value
        double valueWork;   ///< Active value that we work with right now

    public:
        TemperatureSensor();
        ~TemperatureSensor();

        void init(int pin, FT_SensorType type);

        bool getTemperature(double* value);

        SensorAlarmNumber alarmCheck();
        //void alarmAck(SensorAlarmNumber num); //Alarm Acknowledgement

};

#endif  // __TEMPERATURESENSOR_H
