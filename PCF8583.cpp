/**
 * Extended by Attila Sukosd (attila.sukosd@gmail.com)
 * to provide Time library hooks for synchronising with the system time.
 * May 30, 2012
 *
 *
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
    
#include "PCF8583.h"


// Provide device address as a full 8 bit address (like the datasheet)
PCF8583::PCF8583() {
	Wire.begin();
}

time_t PCF8583::get()
{
  tmElements_t tm = getTime();
  return(makeTime(tm));
}

void  PCF8583::set(time_t t)
{
  tmElements_t tm;
  breakTime(t, tm);
  setTime(tm);
}

tmElements_t PCF8583::getTime(){
  tmElements_t tm;
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write((byte)0xC0);	// Stop counting, don't mask
  Wire.endTransmission();

  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write((byte)0x02);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 5);

  tm.Second = bcdToByte(Wire.read());
  tm.Minute = bcdToByte(Wire.read());
  tm.Hour   = bcdToByte(Wire.read());
  byte incoming = Wire.read();				// Year/date counter
  tm.Day    = bcdToByte(incoming & 0x3f);
  tm.Year   = (int)((incoming >> 6) & 0x03);		// It will only hold 4 years...
  tm.Month  = bcdToByte(Wire.read() & 0x1f);	// 0 out the weekdays part

  //  But that's not all - we need to find out what the base year is
  //  so we can add the 2 bits we got above and find the real year
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write((byte)0x10);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 2);
  int year_base = 0;
  year_base = Wire.read();
  year_base = year_base << 8;
  year_base = year_base | Wire.read();
  tm.Year = tm.Year + year_base;

  return tm;
}

void PCF8583::setTime(tmElements_t tm) {
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write((byte)0xC0);   // stop counting, don't mask
  Wire.endTransmission();

  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write((byte)0x02);
  Wire.write(intToBcd(tm.Second));
  Wire.write(intToBcd(tm.Minute));
  Wire.write(intToBcd(tm.Hour));
  Wire.write(((byte)(tm.Year % 4) << 6) | intToBcd(tm.Day));
  Wire.write(intToBcd(tm.Month));
  Wire.endTransmission();

  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write((byte)0x10);
  int year_base = tm.Year - tm.Year % 4;
  Wire.write((byte)year_base >> 8);
  Wire.write((byte)year_base & 0x00ff);
  Wire.endTransmission();
}


int PCF8583::bcdToByte(byte bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0f);
}


byte PCF8583::intToBcd(int in) {
	return ((in / 10) << 4) + (in % 10);
}



PCF8583 RTC = PCF8583(); // create an instance for the user
