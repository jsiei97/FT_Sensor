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
    valueSent = 0;
    valueDiffMax = 0;
    valueSendMax = 0;
    valueSendCnt = valueSendMax;
    valueOffset = 0.0;

    alarmLowActive = false;
    alarmHighActive = false;

    alarmSensor = ALARM_NOT_ACTIVE;
    alarmLow = ALARM_NOT_ACTIVE;
    alarmHigh = ALARM_NOT_ACTIVE;

    alarmHyst = 0.0;
    alarmLowLevel  = 10.0;
    alarmHighLevel = 80.0;
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

/**
 * Active alarm and set what alarm levels to be used.
 *
 * Please note that the low alarm is activated under alarmLowLevel-alarmHyst,
 * and deactivates over alarmLowLevel.
 * The high alarm activates over alarmHighLevel+alarmHyst,
 * and deactivates lower than alarmLowLevel.
 *
 * @param alarmHyst How big hysteresis around the alarm level.
 * @param activateLowAlarm true to activate low alarm.
 * @param alarmLevelLow alarm level for low.
 * @param activateHighAlarm true to active high alarm.
 * @param alarmLevelHigh alarm level for high.
 */
void TemperatureSensor::setAlarmLevels(
        double alarmHyst,
        bool activateLowAlarm, double alarmLevelLow,
        bool activateHighAlarm, double alarmLevelHigh)
{
    this->alarmHyst = alarmHyst;

    alarmLowActive = activateLowAlarm;
    this->alarmLowLevel = alarmLevelLow;

    alarmHighActive = activateHighAlarm;
    this->alarmHighLevel = alarmLevelHigh;
}

/**
 * Enable value diff to send value.
 *
 * Value needs to diff more than this value to be treated as a new value that should be send.
 *
 * @param diff the value
 */
void TemperatureSensor::setValueDiff(double diff)
{
    valueDiffMax = diff;
}

/**
 * Enable send after X counts even if value is the same
 *
 * To make sure that we sometimes get a value even if there is no change.
 *
 * @param cnt how many times can we call getTemperature before we always gets a value.
 */
void TemperatureSensor::setValueMaxCnt(int cnt)
{
    valueSendMax = cnt;
    valueSendCnt = valueSendMax;
}

/**
 * Calibration offset value to add to value.
 *
 * @param offset a number that will be added onto any read value
 */
void TemperatureSensor::setValueOffset(double offset)
{
    valueOffset = offset;
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
        *value += valueOffset;
        valueWork = *value;

        if(valueSendMax != 0)
        {
            if(0 == valueSendCnt)
            {
                valueSendCnt = valueSendMax;
                valueSent = valueWork;
                return true;
            }
            else
            {
                valueSendCnt--;
            }
        }

        if(valueDiffMax != 0.0)
        {
            double diff = valueWork-valueSent;
            if( diff > valueDiffMax || -diff > valueDiffMax )
            {
                valueSendCnt = valueSendMax;
                valueSent = valueWork;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        failcnt++;
    }

    return ret;
}


/**
 * Check if there is any active alarms.
 *
 * Please note that this should be called after getTemperature().
 *
 * $see getTemperature
 *
 * @return SensorAlarmNumber for the type of alarm.
 */
SensorAlarmNumber TemperatureSensor::alarmCheck()
{
    //if(!allowAlarm())
    //    return false;

    SensorAlarmNumber num = SENSOR_ALARM_NO;

    //First check the sensor error alarm.
    switch ( alarmSensor )
    {
        case ALARM_NOT_ACTIVE:
            if(failcnt>5)
            {
                num = SENSOR_ALARM_SENSOR;
                alarmSensor = ALARM_ACTIVE;
            }
            break;
        case ALARM_ACTIVE:
            if(failcnt==0)
            {
                alarmSensor = ALARM_NOT_ACTIVE;
            }
            else
            {
                num = SENSOR_ALARM_SENSOR;
            }
            break;
        case ALARM_ACKED:
            if(failcnt==0)
            {
                alarmSensor = ALARM_NOT_ACTIVE;
            }
            break;
    }

    if(num!=SENSOR_ALARM_NO)
        return num;

    //Then check the low value alarm.
    switch ( alarmLow )
    {
        case ALARM_NOT_ACTIVE:
            if(valueWork < alarmLowLevel - alarmHyst)
            {
                num = SENSOR_ALARM_LOW;
                alarmLow = ALARM_ACTIVE;
            }
            break;
        case ALARM_ACTIVE:
            if(valueWork > alarmLowLevel)
            {
                alarmLow = ALARM_NOT_ACTIVE;
            }
            else
            {
                num = SENSOR_ALARM_LOW;
            }
            break;
        case ALARM_ACKED:
            if(valueWork > alarmLowLevel)
            {
                alarmLow = ALARM_NOT_ACTIVE;
            }
            break;
    }

    if(num!=SENSOR_ALARM_NO)
        return num;


    //Then check the high value alarm.
    switch ( alarmHigh )
    {
        case ALARM_NOT_ACTIVE:
            if(valueWork > alarmHighLevel + alarmHyst)
            {
                num = SENSOR_ALARM_HIGH;
                alarmHigh = ALARM_ACTIVE;
            }
            break;
        case ALARM_ACTIVE:
            if(valueWork < alarmHighLevel)
            {
                alarmLow = ALARM_NOT_ACTIVE;
            }
            else
            {
                num = SENSOR_ALARM_HIGH;
            }
            break;
        case ALARM_ACKED:
            if(valueWork < alarmHighLevel)
            {
                alarmHigh = ALARM_NOT_ACTIVE;
            }
            break;
    }

    return num;
}

/**
 * Acknowledge alarm, dont send any more at this time.
 *
 * @param num The alarm to ack
 */
void TemperatureSensor::alarmAck(SensorAlarmNumber num)
{
    switch ( num )
    {
        case SENSOR_ALARM_SENSOR:
            if( alarmSensor == ALARM_ACTIVE )
            {
                alarmSensor = ALARM_ACKED;
            }
            break;
        case SENSOR_ALARM_LOW:
            if( alarmLow == ALARM_ACTIVE )
            {
                alarmLow = ALARM_ACKED;
            }
            break;
        case SENSOR_ALARM_HIGH:
            if( alarmHigh == ALARM_ACTIVE )
            {
                alarmHigh = ALARM_ACKED;
            }
            break;
        case SENSOR_ALARM_NO:
            break;
    }
}
