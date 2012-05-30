/**
  * Implements a simple interface to the time function of the PCF8583 RTC chip
  * 
  * Works around the device's limited year storage by keeping the year in the
  * first two bytes of user accessible storage
  * 
  * Assumes device is attached in the standard location - Analog pins 4 and 5
  * Device address is the 8 bit address (as in the device datasheet - normally A0)
  * 
  * Copyright (c) 2009, Erik DeBill
  * 
  * 
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  * 
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  * 
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  */


#ifndef PCF8583_H
#define PCF8583_H

#include "Arduino.h"
#include "Wire.h"
#include <Time.h>

#define RTC_ADDRESS (0xA0 >> 1)


class PCF8583 {
  public:
    PCF8583();
    static time_t get();
    static void set(time_t t);
    static tmElements_t getTime();
    static void setTime(tmElements_t tm);
    static int bcdToByte(byte bcd);
    static byte intToBcd(int in);
};

extern PCF8583 RTC;

#endif  //PCF8583_H
