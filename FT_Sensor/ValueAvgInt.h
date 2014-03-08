/**
 * @file ValueAvgInt.h
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

#ifndef  __VALUEAVG_H
#define  __VALUEAVG_H

/**
 * A basic filter
 *
 * The filter will ignore the most extreme values, 
 * and then calculate the average value on the rest.
 *
 * The usage is first to call init(), 
 * then add some 10-20 values with addValue(int).
 * And the result can be colleced with getValue().
 */
class ValueAvgInt
{
    private:
        bool firstTime; ///< Indicate first value
        int higest;     ///< The biggest extreme value, it will not be used in the final calculation
        int smallest;   ///< The smallest extreme value, it will not be used in the final calculation
        int sum;        ///< The current sum of values
        int cnt;        ///< How many values is sent into the filter

    public:
        ValueAvgInt();
        void init();
        void addValue(int data);
        int getValue();

};

#endif  // __VALUEAVG_H
