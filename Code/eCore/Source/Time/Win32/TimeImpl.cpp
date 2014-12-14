/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 11-Dec-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file TimeImpl.h
This file defines the Windows version of the Time utility methods.
*/

#include <CorePch.h>
#include "TimeImpl.h"

namespace E 
{
namespace Time
{
/*----------------------------------------------------------------------------------------------------------------------
Time assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_TIME_MONTH_VALUE       "Month numerical values must be between 1 and 12"
#define E_ASSERT_MSG_TIME_DAY_VALUE         "Day numerical values must be between 1 and 31"
#define E_ASSERT_MSG_TIME_HOUR_VALUE        "Hour numerical values must be between 0 and 23"
#define E_ASSERT_MSG_TIME_SEXAGESIMAL_VALUE "Sexagesimal values must be between 0 and 59"
#define E_ASSERT_MSG_TIME_MILLESIMAL_VALUE  "Millesimal values must be between 0 and 999"

/*----------------------------------------------------------------------------------------------------------------------
Impl methods
----------------------------------------------------------------------------------------------------------------------*/

/** Retrieves the current CPU time. The CPU time is arbitrary and platform dependent. In some platforms in ex. It is 
the time past since computer was started. The time requested by CPU time functions is expected to be linear
(a time requested after another will be equal or greater than the first one). This time functions returns high 
resolution time values (nanoseconds). The time taken by this function to retrieve the CPU time is platform dependent.
*/
TimeValue Impl::GetCpuTime()
{
  TimeValue tv;

  LARGE_INTEGER ticks0;
  LARGE_INTEGER freq;
  QueryPerformanceCounter(&ticks0);                                           // Ticks from CPU tick count start
  QueryPerformanceFrequency(&freq);                                           // CPU tick frequency
  tv = ((I64)(ticks0.QuadPart / static_cast<D64>(freq.QuadPart) * 1000000));  // Result in nanoseconds

  return tv;
}

TimeValue Impl::GetCurrentTime()
{
  // Retrieve the current system date and time. The information is in Universal Time Coordinates (UTC) format.
  FILETIME utc_ft;
  GetSystemTimeAsFileTime(&utc_ft); 

  FILETIME local_ft;
  FileTimeToLocalFileTime(&utc_ft, &local_ft);

  ULARGE_INTEGER m;
  m.LowPart = local_ft.dwLowDateTime;
  m.HighPart = local_ft.dwHighDateTime;

  m.QuadPart = (m.QuadPart - kOffsetFrom1601To1970) / 10; // switch to microseconds from 1970

  return TimeValue(m.QuadPart);
}

TimeValue Impl::GetCurrentTimeUtc()
{
  // Retrieve the current system date and time. The information is in Universal Time Coordinates (UTC) format.
  FILETIME utc_ft;
  GetSystemTimeAsFileTime(&utc_ft);

  ULARGE_INTEGER m;
  m.LowPart = utc_ft.dwLowDateTime;
  m.HighPart = utc_ft.dwHighDateTime;

  m.QuadPart = (m.QuadPart - kOffsetFrom1601To1970) / 10; // Switch to microseconds from 1970

  return TimeValue(m.QuadPart);
}

void Impl::GetDate(TimeValue tv, Date& date)
{
  ULARGE_INTEGER us;
  us.QuadPart = (tv * 10) + kOffsetFrom1601To1970;

  FILETIME ft;
  ft.dwLowDateTime = us.LowPart;
  ft.dwHighDateTime = us.HighPart;

  SYSTEMTIME st;
  FileTimeToSystemTime(&ft,&st);

  date.year = st.wYear;
  date.month = st.wMonth;
  date.day = st.wDay;
  date.weekDay = st.wDayOfWeek; // It goes from 0 to 6 (starting with Sunday)
  date.hour = st.wHour;
  date.minute = st.wMinute;
  date.second = st.wSecond;
  date.millisecond = st.wMilliseconds;
}

TimeValue Impl::GetDateTime(const Date& date)
{
  E_ASSERT_MSG(date.month >= 1 && date.month <= 12, E_ASSERT_MSG_TIME_MONTH_VALUE);
  E_ASSERT_MSG(date.day >= 1 && date.day <= 31, E_ASSERT_MSG_TIME_DAY_VALUE);
  E_ASSERT_MSG(date.hour >= 0 && date.hour <= 23, E_ASSERT_MSG_TIME_HOUR_VALUE);
  E_ASSERT_MSG(date.minute >= 0 && date.minute <= 59, E_ASSERT_MSG_TIME_SEXAGESIMAL_VALUE);
  E_ASSERT_MSG(date.second >= 0 && date.second <= 59, E_ASSERT_MSG_TIME_SEXAGESIMAL_VALUE);
  E_ASSERT_MSG(date.millisecond >= 0 && date.millisecond <= 999, E_ASSERT_MSG_TIME_MILLESIMAL_VALUE);

  SYSTEMTIME st;
  st.wYear         = date.year;
  st.wMonth        = date.month;
  st.wDay          = date.day;
  st.wDayOfWeek    = 0; // does not affect this computation
  st.wHour         = date.hour;
  st.wMinute       = date.minute;
  st.wSecond       = date.second;
  st.wMilliseconds = date.millisecond;

  // We need to put this information in a large 64-bit integer. This can be done by converting
  // the SYSTEMTIME value into a FILESYSTEM value, which uses a ULARGE_INTEGER type.
  FILETIME ft;
  SystemTimeToFileTime(&st,&ft);

  ULARGE_INTEGER m;
  m.LowPart = ft.dwLowDateTime;
  m.HighPart = ft.dwHighDateTime;

  return (m.QuadPart - kOffsetFrom1601To1970) / 10; // from 1e-7 to 1e-6
}
}
}
