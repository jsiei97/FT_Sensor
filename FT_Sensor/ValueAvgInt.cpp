/**
 * @file ValueAvgInt.cpp
 * @author Johan Simonsson
 * @brief A basic filter
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

#include "ValueAvgInt.h"

ValueAvgInt::ValueAvgInt()
{
    init();
}

/**
 * Reset and init this filter to start over with a new session.
 */
void ValueAvgInt::init()
{
    firstTime = true;
    higest = 0;
    smallest = 0;
    sum = 0;
    cnt = 0;
}

/**
 * Add a new value to the filter
 *
 * @param[in] data is some data to be used int the filter
 */
void ValueAvgInt::addValue(int data)
{
    sum += data;
    cnt++;
    if(true == firstTime)
    {
        higest = data;
        smallest = data;
    }
    else
    {
        if(data > higest)
        {
            higest = data;
        }
        else if(data < smallest)
        {
            smallest = data;
        }
    }

    firstTime = false;
}

/**
 * Get the result from the filter.
 *
 * @return the calculated value
 */
int ValueAvgInt::getValue()
{
    if(cnt == 0)
    {
        return 0;
    }

    //If we have more than 7 numbers, then
    //get rid of the two extreems and calc avg on the rest.
    if(cnt >= 7)
    {
        sum -= smallest;
        sum -= higest;

        cnt -= 2;
    }

    return sum/cnt;
}

