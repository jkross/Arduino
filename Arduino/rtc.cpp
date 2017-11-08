// Simple date conversions and calculations

#include <Arduino.h>
#include "debug.h"

//#define RTC_TEST
#include "rtc.h"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

#ifdef RTC_TEST

void showDate(const char* txt, const DateTime& dt) {
	Serial.print(txt);
	Serial.print(' ');
	Serial.print(dt.year(), DEC);
	Serial.print('/');
	Serial.print(dt.month(), DEC);
	Serial.print('/');
	Serial.print(dt.day(), DEC);
	Serial.print(' ');
	Serial.print(dt.hour(), DEC);
	Serial.print(':');
	Serial.print(dt.minute(), DEC);
	Serial.print(':');
	Serial.print(dt.second(), DEC);

	Serial.print(" = ");
	Serial.print(dt.unixtime());
	Serial.print("s / ");
	Serial.print(dt.unixtime() / 86400L);
	Serial.print("d since 1970");

	Serial.println();
}

void showTimeSpan(const char* txt, const TimeSpan& ts) {
	Serial.print(txt);
	Serial.print(" ");
	Serial.print(ts.days(), DEC);
	Serial.print(" days ");
	Serial.print(ts.hours(), DEC);
	Serial.print(" hours ");
	Serial.print(ts.minutes(), DEC);
	Serial.print(" minutes ");
	Serial.print(ts.seconds(), DEC);
	Serial.print(" seconds (");
	Serial.print(ts.totalseconds(), DEC);
	Serial.print(" total seconds)");
	Serial.println();
}

void dtTest() {
#ifndef ESP8266
	while (!Serial); // for Leonardo/Micro/Zero
#endif
	//Serial.begin(57600);

	DateTime dt0(0, 1, 1, 0, 0, 0);
	showDate("dt0", dt0);

	DateTime dt1(1, 1, 1, 0, 0, 0);
	showDate("dt1", dt1);

	DateTime dt2(2009, 1, 1, 0, 0, 0);
	showDate("dt2", dt2);

	DateTime dt3(2009, 1, 2, 0, 0, 0);
	showDate("dt3", dt3);

	DateTime dt4(2009, 1, 27, 0, 0, 0);
	showDate("dt4", dt4);

	DateTime dt5(2009, 2, 27, 0, 0, 0);
	showDate("dt5", dt5);

	DateTime dt6(2009, 12, 27, 0, 0, 0);
	showDate("dt6", dt6);

	DateTime dt7(dt6.unixtime() + 3600); // One hour later.
	showDate("dt7", dt7);

	DateTime dt75 = dt6 + TimeSpan(0, 1, 0, 0); // One hour later with TimeSpan addition.
	showDate("dt7.5", dt75);

	DateTime dt8(dt6.unixtime() + 86400L); // One day later.
	showDate("dt8", dt8);

	DateTime dt85 = dt6 + TimeSpan(1, 0, 0, 0); // One day later with TimeSpan addition.
	showDate("dt8.5", dt85);

	DateTime dt9(dt6.unixtime() + 7 * 86400L); // One week later.
	showDate("dt9", dt9);

	DateTime dt95 = dt6 + TimeSpan(7, 0, 0, 0); // One week later with TimeSpan addition.
	showDate("dt9.5", dt95);

	DateTime dt10 = dt6 + TimeSpan(0, 0, 42, 42); // Fourty two minutes and fourty two seconds later.
	showDate("dt10", dt10);

	DateTime dt11 = dt6 - TimeSpan(7, 0, 0, 0);  // One week ago.
	showDate("dt11", dt11);

	TimeSpan ts1 = dt6 - dt5;
	showTimeSpan("dt6-dt5", ts1);

	TimeSpan ts2 = dt10 - dt6;
	showTimeSpan("dt10-dt6", ts2);
}
#endif // RTC_TEST

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif


rtc::rtc() {}

void 
rtc::setup () {
  TEST((Serial.println("rtc setup begin")));
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  // Serial.begin(57600);
  if (! _rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! _rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  TEST((Serial.println("rtc setup end")));
}

void 
rtc::test () {
    DateTime now = _rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(_daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();

}
