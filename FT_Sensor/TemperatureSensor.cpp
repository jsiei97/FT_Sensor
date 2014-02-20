/**
 * @file TemperatureSensor.cpp
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

//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#include "TemperatureSensor.h"
//#include "StringHelp.h"
#include "SensorTypes.h"


TemperatureSensor::TemperatureSensor()
{
    ds   = NULL;
    lvts = NULL;
    this->type=SENSOR_NONE;

    failcnt = 0;
    valueWork = 0;
}

TemperatureSensor::~TemperatureSensor()
{
    if(ds!=NULL)
        delete ds;

    if(lvts!=NULL)
        delete lvts;
}

void TemperatureSensor::init(int pin, FT_SensorType type)
{
    this->type=type;
    /// @todo Make sure it is called only once, or fix multiple new.
    switch ( type )
    {
        case SENSOR_DS18B20:
            if(ds!=NULL)
                delete ds;

            ds = new DS18B20(pin);
            break;
        case SENSOR_LVTS_LM34: //Fall through ok
        case SENSOR_LVTS_LM35:
            if(lvts!=NULL)
                delete lvts;

            lvts = new LVTS(pin, type);
            break;
        default :
            break;
    }

}

bool TemperatureSensor::getTemperature(double* value)
{
    bool ret = false;
    switch ( type )
    {
        case SENSOR_DS18B20:
            ret = ds->getTemperature(value);
            break;
        case SENSOR_LVTS_LM34: //Fall through ok
        case SENSOR_LVTS_LM35:
            ret = lvts->getTemperature(value);
            break;
        case SENSOR_NONE:
            break;
    }

    if(ret)
    {
        failcnt=0;
        valueWork = *value;
    }
    else
    {
        failcnt++;
    }

    return ret;
}



SensorAlarmNumber TemperatureSensor::alarmCheck()
{
    SensorAlarmNumber num = SENSOR_ALARM_NO;

    if(failcnt>5)
    {
        num = SENSOR_ALARM_SENSOR;
    }
 
    return num;
}
