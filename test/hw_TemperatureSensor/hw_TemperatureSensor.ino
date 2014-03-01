/**
 * @file hw_TemperatureSensor.ino
 * @author Johan Simonsson
 * @brief Test program for the TemperatureSensor class
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


#include "TemperatureSensor.h"
#include "SensorTypes.h"

#define SENSOR_CNT 2
TemperatureSensor sensor[SENSOR_CNT];

void setup()
{
    Serial.begin(9600);
    Serial.println("TemperatureSensor test!");

    //First a LM35 connect on A0
    sensor[0].init(A0, SENSOR_LVTS_LM35);
    sensor[0].setValueMaxCnt(30);
    sensor[0].setValueDiff(1.0);
    sensor[0].setAlarmLevels(2.0, true, 20.0, true, 25.0);

    //Then a DS18B20 on A1
    sensor[1].init(A1, SENSOR_DS18B20);
    sensor[1].setValueMaxCnt(30);
    sensor[1].setValueDiff(0.5);
    sensor[1].setAlarmLevels(2.0, true, 20.0, true, 25.0);
}

void loop()
{
    double temperature = 0;

    for( int i=0 ; i<SENSOR_CNT ; i++ )
    {
        if( sensor[i].getTemperature(&temperature) )
        {
            Serial.print("S");
            Serial.print(i);
            Serial.print(" - Temperature: ");
            Serial.print(temperature);
            Serial.println(" *C");
        }

        switch ( sensor[i].alarmCheck() )
        {
            case SENSOR_ALARM_NO:
                break;
            case SENSOR_ALARM_SENSOR:
                Serial.print("S");
                Serial.print(i);
                Serial.println(" - Alarm: Sensor error");
                break;
            case SENSOR_ALARM_HIGH:
                Serial.print("S");
                Serial.print(i);
                Serial.println(" - Alarm: High value");
                break;
            case SENSOR_ALARM_LOW:
                Serial.print("S");
                Serial.print(i);
                Serial.println(" - Alarm: Low value");
                break;
        }
    }

    delay(1000);
}
