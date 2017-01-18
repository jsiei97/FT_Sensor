/**
 * @file DS18B20.cpp
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

#include "Arduino.h"
#include "DS18B20.h"
#include "OneWire.h"
#include "DS18B20Helper.h"

/**
 * Init with what pin the sensor is connected to.
 *
 * @param[in] pin is the IO pin
 */
DS18B20::DS18B20(int pin)
{
    this->pin  = pin;
    pinMode(pin, INPUT);
}

/**
 * Returns a temperature from a OneWire sensor.
 *
 * Please note that there is a need for a 750ms delay in the middle
 * that is removed so this function starts a new reading
 * and returns the result from the last reading.
 *
 * @param[out] value Temperature reading
 * @return true if ok, false if fail.
 */
bool DS18B20::getTemperature(double* value)
{
    OneWire ds(pin);

    byte i;
    byte present = 0;
    byte type_s;
    byte data[12];
    byte addr[8];
    double celsius;

    if( !ds.search(addr) )
    {
        //Serial.println("No more addresses.");
        //Serial.println();
        ds.reset_search();
        delay(250);
        return false;
    }

    //1w adress
    /*
    Serial.print("ROM =");
    for( i = 0; i < 8; i++ )
    {
        Serial.print(addr[i], HEX);
        Serial.write(' ');
    }
    */

    if( OneWire::crc8(addr, 7) != addr[7] )
    {
        //Serial.println("CRC is not valid!");
        return 0;
    }
    //Serial.println();

    // the first ROM byte indicates which chip
    switch(addr[0]) {
        /// @todo Add support for DS18S20, can't test right now
        //case 0x10:
            //Serial.println("  Chip = DS18S20");  // or old DS1820
            //type_s = 1;
            //break;
        case 0x28:
            //Serial.println("  Chip = DS18B20");
            type_s = 0;
            break;
        case 0x22:
            //Serial.println("  Chip = DS1822");
            type_s = 0;
            break;
        default:
            //Serial.println("Device is not a DS18x20 family device.");
            return 0;
    }

    ds.reset();
    ds.select(addr);
    // To  initiate  a  temperature  measurement
    // and A-to-D conversion, the master must issue a Convert
    // T [44h] command.
    ds.write(0x44,0);

    // Please note that every conversion takes 750ms,
    // and if we don't wait we get the last value...
    // But since we wait 1000ms in the main loop that delay
    // will give us the new value the next time.
    //
    // More or less wait now or later,
    // so if we have many DS18B20 then it is enough to wait once in the main loop

    //delay(1000);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.

    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);         // Read Scratchpad

    //Serial.print("  Data = ");
    //Serial.print(present,HEX);
    //Serial.print(" ");

    // we need 9 bytes
    for( i = 0; i < 9; i++ )
    {
        data[i] = ds.read();
        //Serial.print(data[i], HEX);
        //Serial.print(" ");
    }
    //Serial.print(" CRC=");
    //Serial.print(OneWire::crc8(data, 8), HEX);
    //Serial.println();

    // convert the data to actual temperature
    uint16_t raw = (data[1] << 8) | data[0];

    Serial.print(" RAW=");
    Serial.print(raw, HEX);
    Serial.print(" ");

    if( type_s )
    {
        raw = raw << 3; // 9 bit resolution default
        if( data[7] == 0x10 )
        {
            // count remain gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    }
    else
    {
        /// @todo Test if 9,10 and 11 bit resoltion works
        byte cfg = (data[4] & 0x60);
        if( cfg == 0x00 )
        {
            // 9 bit resolution, 93.75 ms
            //Serial.print(" 9 bit");
            raw = raw & 0xFFF8;
        }
        else if( cfg == 0x20 )
        {
            // 10 bit res, 187.5 ms
            //Serial.print("10 bit");
            raw = raw & 0xFFFC;
        }
        else if( cfg == 0x40 )
        {
            // 11 bit res, 375 ms
            //Serial.print("10 bit");
            raw = raw & 0xFFFE;
        }
        // default is 12 bit resolution, 750 ms conversion time
    }

    //Serial.print(" RAW=");
    //Serial.print(raw, HEX);
    //Serial.print(" ");

    celsius = DS18B20Helper::convertTemperature(raw);

    //The internal DS18B20 error temperature,
    //dont return this value!
    if(celsius == 85.0)
    {
        return false;
    }

    //Datasheet tells us
    //-"Measures temperatures from -55°C to +125°C"
    //so if it not in that range... something is wrong!
    if(celsius <= 125 && celsius >= -55)
    {
        *value = celsius;
        return true;
    }

    //The calc above did not go well :(
    return false;
}

