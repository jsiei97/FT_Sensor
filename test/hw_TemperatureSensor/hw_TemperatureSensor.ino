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
TemperatureSensor *sensor = new TemperatureSensor[SENSOR_CNT];

void setup()
{
    Serial.begin(9600);
    Serial.println("TemperatureSensor test!");

    //Since we use a LVTS
    //INTERNAL: an built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328
    analogReference(INTERNAL); //1.1V

    //First a LM35 connect on A0
    sensor[0].init(A0, SENSOR_LVTS_LM35);
    sensor[0].setValueMaxCnt(30);
    sensor[0].setValueDiff(1.0);
    sensor[0].setAlarmLevels(1.0, true, 21.0, true, 25.0);

    //Then a DS18B20 on A1
    sensor[1].init(A1, SENSOR_DS18B20);
    sensor[1].setValueMaxCnt(30);
    sensor[1].setValueDiff(0.5);
    sensor[1].setAlarmLevels(1.0, true, 21.0, true, 25.0);
}

void loop()
{
    double temperature = 0;

    for( int i=0 ; i<SENSOR_CNT ; i++ )
    {
        if( sensor[i].getTemperature(&temperature) )
        {
            Serial.print((millis()/1000));         
            Serial.print(": S");
            Serial.print(i);
            Serial.print(" - Temperature: ");
            Serial.print(temperature);
            Serial.println(" *C");
        }

        //Now loop and Ack them...
        int maxCnt = 20;
        SensorAlarmNumber num;
        do
        {
            num = sensor[i].alarmCheck();
            switch ( num )
            {
                case SENSOR_ALARM_SENSOR:
                    Serial.print("S");
                    Serial.print(i);
                    Serial.println(" - Alarm: Sensor error");
                    sensor[i].alarmAck(num);
                    break;
                case SENSOR_ALARM_HIGH:
                    Serial.print("S");
                    Serial.print(i);
                    Serial.println(" - Alarm: High value");
                    sensor[i].alarmAck(num);
                    break;
                case SENSOR_ALARM_LOW:
                    Serial.print("S");
                    Serial.print(i);
                    Serial.println(" - Alarm: Low value");
                    sensor[i].alarmAck(num);
                    break;
            }
            maxCnt--;
        }
        while( num != SENSOR_ALARM_NO && maxCnt != 0);
    }

    delay(1000);
}
