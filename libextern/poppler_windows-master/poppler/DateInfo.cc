//========================================================================
//
// DateInfo.cc
//
// Copyright (C) 2008, 2018 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2009 Carlos Garcia Campos <carlosgc@gnome.org>
// Copyright (C) 2015 André Guerreiro <aguerreiro1985@gmail.com>
// Copyright (C) 2015 André Esser <bepandre@hotmail.com>
// Copyright (C) 2016, 2018 Adrian Johnson <ajohnson@redneon.com>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

//========================================================================
//
// Based on code from pdfinfo.cc
//
// Copyright 1998-2003 Glyph & Cog, LLC
//
//========================================================================

#include <config.h>

#include "glibc.h"
#include "DateInfo.h"

#include <stdio.h>
#include <string.h>

/* See PDF Reference 1.3, Section 3.8.2 for PDF Date representation */
bool parseDateString(const char *dateString, int *year, int *month, int *day, int *hour, int *minute, int *second, char *tz, int *tzHour, int *tzMinute)
{
    if ( dateString == nullptr ) return false;
    if ( strlen(dateString) < 2 ) return false;

    if ( dateString[0] == 'D' && dateString[1] == ':' )
        dateString += 2;

    *month = 1;
    *day = 1;
    *hour = 0;
    *minute = 0;
    *second = 0;
    *tz = 0x00;
    *tzHour = 0;
    *tzMinute = 0;

    if ( sscanf( dateString,
                 "%4d%2d%2d%2d%2d%2d%c%2d%*c%2d",
                 year, month, day, hour, minute, second,
                 tz, tzHour, tzMinute ) > 0 ) {
        /* Workaround for y2k bug in Distiller 3 stolen from gpdf, hoping that it won't
        * be used after y2.2k */
        if ( *year < 1930 && strlen (dateString) > 14)
        {
           int century, years_since_1900;
           if ( sscanf( dateString,
                        "%2d%3d%2d%2d%2d%2d%2d",
                        &century, &years_since_1900, month, day, hour, minute, second) == 7 )
           {
               *year = century * 100 + years_since_1900;
           }
           else
           {
               return false;
           }
       }

       if (*year <= 0) return false;

       return true;
   }

   return false;
}

GooString *timeToDateString(time_t *timeA)
{
  const time_t timet = timeA ? *timeA : time(nullptr);

  struct tm localtime_tm;
  localtime_r (&timet, &localtime_tm);

  char buf[50];
  strftime (buf, sizeof(buf), "D:%Y%m%d%H%M%S", &localtime_tm);
  GooString *dateString = new GooString(buf);

  // strftime "%z" does not work on windows (it prints zone name, not offset)
  // calculate time zone offset by comparing local and gmtime time_t value for same
  // time.
  const time_t timeg = timegm(&localtime_tm);
  const time_t offset = difftime(timeg, timet); // find time zone offset in seconds
  if (offset > 0) {
    dateString->appendf("+{0:02d}'{1:02d}", offset/3600, (offset%3600)/60);
  } else if (offset < 0) {
    dateString->appendf("-{0:02d}'{1:02d}", -offset/3600, (-offset%3600)/60);
  } else {
    dateString->append("Z");
  }

  return dateString;
}

// Convert PDF date string to time. Returns -1 if conversion fails.
time_t dateStringToTime(const GooString *dateString) {
  int year, mon, day, hour, min, sec, tz_hour, tz_minute;
  char tz;
  struct tm tm;
  time_t time;

  if (!parseDateString (dateString->c_str(), &year, &mon, &day, &hour, &min, &sec, &tz, &tz_hour, &tz_minute))
    return -1;

  tm.tm_year = year - 1900;
  tm.tm_mon = mon - 1;
  tm.tm_mday = day;
  tm.tm_hour = hour;
  tm.tm_min = min;
  tm.tm_sec = sec;
  tm.tm_wday = -1;
  tm.tm_yday = -1;
  tm.tm_isdst = -1; /* 0 = DST off, 1 = DST on, -1 = don't know */

  /* compute tm_wday and tm_yday and check date */
  time = timegm (&tm);
  if (time == (time_t)-1)
    return time;

  time_t offset = (tz_hour*60 + tz_minute)*60;
  if (tz == '-')
    offset *= -1;
  time -= offset;

  return time;
}
