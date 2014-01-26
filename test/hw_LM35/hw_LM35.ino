/**
 * @file hw_LM35.ino
 * @author Johan Simonsson  
 * @brief Test program for LVTS::lm35
 */

/*
 * Copyright (C) 2013 Johan Simonsson
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

#include "LVTS.h"

LVTS lvts(A0, LVTS_LM35);

void setup() 
{
    Serial.begin(9600); 
    Serial.println("LVTS LM35 test!");

    //INTERNAL: an built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328
    analogReference(INTERNAL); //1.1V
}

void loop() 
{
    double temperature = 0;

    if(!lvts.getTemperature(&temperature))
    {
        Serial.println("Error: read fail...");
    } 

    Serial.print("Temperature: "); 
    Serial.print(temperature);
    Serial.println(" *C");

    delay(1000);
}
