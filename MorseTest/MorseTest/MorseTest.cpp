// MorseTest.cpp : Defines the entry point for the console application.
//

#include <stdint.h>
#include "testLedDriver.h"
#include "scheduler.h"
#include "morseTimer.h"


int main()
{
	testLedDriver *myDriver = new testLedDriver();
	scheduler *myScheduler = new scheduler();
	snapshotTime* snapp = new snapshotTime();
	ticks_t now = 0;
	
	morseTimer::loadPatterns(myScheduler, myDriver, now);

	do {
		do {
			myScheduler->dispatch(now);
			myDriver->writeData();
			// snapp->set(millis());
		} while (myScheduler->expired(now));  // while there is any expired timer
		ticks_t remaining = myScheduler->remaining(now);
		now += remaining;
	} while (true);
	return 0;
}

