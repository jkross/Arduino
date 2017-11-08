#pragma once
#include <Wire.h>
#include <RTClib.h>

class rtc {
private:
	RTC_DS1307 _rtc;

	const char _daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
public:

	rtc();
	void setup();
	void test();

};