/**
 * @file hw_DS18B20.ino
 * @author Johan Simonsson
 * @brief Test program for DS18B20
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

#include "DS18B20.h"

DS18B20 ds(A0);

void setup()
{
    Serial.begin(9600);
    Serial.println("DS18B20 test!");
}

void loop()
{
    double temperature = 0;

    if(!ds.getTemperature(&temperature))
    {
        Serial.println("Error: read fail...");
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");

    delay(1000);
}
