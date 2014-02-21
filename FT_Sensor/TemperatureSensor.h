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


/// The statemachine for the alarm
typedef enum
{
    ALARM_ACTIVE = 0, ///< The alarm is triggered.
    ALARM_ACKED,      ///< The alarm is triggered, and is ack:ed.
    ALARM_NOT_ACTIVE  ///< The alarm is not triggered, all is fine.
} AlarmStates;



/**
 * A Temperature sensor class for the DS18B20 and LVTS (i.e. LM35).
 *
 * This will class will wrap the different sensors and give them the
 * same interface so they can be put in a array and just looped from main.
 *
 * There is also some some alarm logic so that main know when things is wrong.
 * The alarm is active as long as it is not ack:ed (Acknowledged) or until what
 * triggered the alarm ends, like the temperature goes back to normal.
 *
 *
 * Sensor read alarm is triggered if there is a read error.
 * @see SENSOR_ALARM_SENSOR
 * \dot
 * digraph finite_state_machine {
 *   label="State machine for SENSOR_ALARM_SENSOR"
 *   rankdir=LR;
 *   size="8,5"
 *
 *   node [shape = circle, label="ALARM_NOT_ACTIVE" ] NA;
 *   node [shape = circle, label="ALARM_ACTIVE" ]     AN;
 *   node [shape = circle, label="ALARM_ACKED" ]      ACK;
 *
 *   node [shape = point ]; qi
 *   qi -> NA;
 *
 *   NA  -> AN  [ label = "failcnt > 5" ];
 *   AN  -> ACK [ label = "alarmAck()" ];
 *   ACK -> NA  [ label = "failcnt == 0" ];
 *   AN  -> NA  [ label = "failcnt == 0" ];
 * }
 * \enddot
 *
 * Low level alarm is if the value is lower than the alarm low level.
 * @see SENSOR_ALARM_LOW
 * \dot
 * digraph finite_state_machine {
 *   label="State machine for SENSOR_ALARM_LOW"
 *   rankdir=LR;
 *   size="8,5"
 *
 *   node [shape = circle, label="ALARM_NOT_ACTIVE" ] NA;
 *   node [shape = circle, label="ALARM_ACTIVE" ]     AN;
 *   node [shape = circle, label="ALARM_ACKED" ]      ACK;
 *
 *   node [shape = point ]; qi
 *   qi -> NA;
 *
 *   NA  -> AN  [ label = "value < alarmLow - alarmHyst" ];
 *   AN  -> ACK [ label = "alarmAck()" ];
 *   ACK -> NA  [ label = "value > alarmLow" ];
 *   AN  -> NA  [ label = "value > alarmLow" ];
 * }
 * \enddot
 *
 * High level alarm is if the value is higher than alarm high level.
 * @see SENSOR_ALARM_HIGH
 * \dot
 * digraph finite_state_machine {
 *   label="State machine for SENSOR_ALARM_HIGH"
 *   rankdir=LR;
 *   size="8,5"
 *
 *   node [shape = circle, label="ALARM_NOT_ACTIVE" ] NA;
 *   node [shape = circle, label="ALARM_ACTIVE" ]     AN;
 *   node [shape = circle, label="ALARM_ACKED" ]      ACK;
 *
 *   node [shape = point ]; qi
 *   qi -> NA;
 *
 *   NA  -> AN  [ label = "value > alarmHigh + alarmHyst" ];
 *   AN  -> ACK [ label = "alarmAck()" ];
 *   ACK -> NA  [ label = "value < alarmHigh" ];
 *   AN  -> NA  [ label = "value < alarmHigh" ];
 * }
 * \enddot
 */
class TemperatureSensor
{
    private:
        FT_SensorType type; ///< What kind of sensor is this object?
        DS18B20* ds;        ///< Ref to DS18B20 if correct type.
        LVTS*    lvts;      ///< Ref to LVTS if correct type.

        AlarmStates alarmSensor; ///< Current state for the sensor read alarm.
        AlarmStates alarmLow;    ///< Current state for the low level alarm.
        AlarmStates alarmHigh;   ///< Current state for the high level alarm.

        unsigned int failcnt; ///< If sensor read fails, then this value inc. Zero is ok.

        //Variables for the value
        double valueWork;   ///< Active value that we work with right now

    public:
        TemperatureSensor();
        ~TemperatureSensor();

        void init(int pin, FT_SensorType type);

        bool getTemperature(double* value);

        SensorAlarmNumber alarmCheck();
        void alarmAck(SensorAlarmNumber num); //Alarm Acknowledgement

};

#endif  // __TEMPERATURESENSOR_H
