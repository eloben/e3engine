/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 11-Dec-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Time.h
This file declares utility methods for time management. Time features functions to retrieve the current time & 
current Cpu time and conversion functions between TimeValue and Date.
*/

#ifndef E3_TIME_H
#define E3_TIME_H

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
Time
----------------------------------------------------------------------------------------------------------------------*/
namespace Time
{
/*----------------------------------------------------------------------------------------------------------------------
Value

Please note that this class has the following usage contract:

1. Value can also be interpreted as an amount of time. The time is saved internally in microseconds as a I64. 
2. Accessors methods always return absolute values e.g. GetSeconds will get 90.5 seconds rather than 1 minute, 30
seconds and 500 microseconds.
----------------------------------------------------------------------------------------------------------------------*/
class Value
{
public:
  // Constants
  static const I64 kOneMillisecond = 1000;
  static const I64 kOneSecond = 1000000;
  static const I64 kOneMinute = 60000000;
  static const I64 kOneHour = 3600000000;
  static const I64 kOneDay = 86400000000;

  Value(I64 usecs = 0)
    : mTv(usecs) {}

  // Operators
  inline operator const I64&(void) const { return mTv; }

  inline operator       I64&(void) { return mTv; }

  // Accessors
  inline D64 GetMilliseconds() const { return static_cast<D64>(mTv) / kOneMillisecond; }
  inline D64 GetSeconds() const { return static_cast<D64>(mTv) / kOneSecond; }
  inline D64 GetMinutes() const { return static_cast<D64>(mTv) / kOneMinute; }
  inline D64 GetHours() const { return static_cast<D64>(mTv) / kOneHour; }
  inline D64 GetDays() const { return static_cast<D64>(mTv) / kOneDay; }

private:
  I64	  mTv;
};

/*----------------------------------------------------------------------------------------------------------------------
Date
----------------------------------------------------------------------------------------------------------------------*/
struct Date
{
  enum WeekDay
  {
    eWeekDaySunday,
    eWeekDayMonday,
    eWeekDayTuesday,
    eWeekDayWednesday,
    eWeekDayThursday,
    eWeekDayFriday,
    eWeekDaySaturday
  };

  U16 year;
  U16 month;
  U16	day;
  U16	weekDay;
  U16 hour;
  U16 minute;
  U16	second;
  U16 millisecond;

  Date::Date()
    : year(0)
    , month(0)
    , day(0)
    , weekDay(0)
    , hour(0)
    , minute(0)
    , second(0)
    , millisecond(0) {}
};

/*----------------------------------------------------------------------------------------------------------------------
Time API methods
----------------------------------------------------------------------------------------------------------------------*/
E_API Value GetCpuTime();
E_API void  GetDate(Value tv, Date& date);
E_API Value GetDateTime(const Date& date);
E_API Value GetCurrentTime();
E_API Value GetCurrentTimeUtc();
}

/*----------------------------------------------------------------------------------------------------------------------
Time types
----------------------------------------------------------------------------------------------------------------------*/
typedef Time::Value TimeValue;
}

#endif
