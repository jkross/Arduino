// Cascaded TLC5916 drivers

#if 0
#include <RTClib.h>
#endif
#include "perfStat.h"
#include "timer.h"
#include "lfsr.h"
#include "scheduler.h"
#include "ledDriver.h"
#include "ledTimer.h"
#include "morseTimer.h"
#include "debug.h"

scheduler *myScheduler;
ledDriver *myDriver;
lfsr	  *myLfsr;

#if 0
#include "rtc.h"
rtc* realTime;
#endif

// 
// Initialize board
//
void 
setup ()
{
  //Serial.begin(115200);		// removed to save memory

 #if 0
  realTime = new rtc();
  realTime->setup();
#endif

  myLfsr = new lfsr();										// Pseudo Random generator
  myScheduler = new scheduler(myLfsr);						// Initialize the LED timing scheduler and the LED driver hardware abstraction
  myDriver = new ledDriver();
  
  ticks_t now = millis();

  ledTimer::loadPatterns(myScheduler, myDriver, myLfsr, now);	// Load up the normal flashing patterns
  morseTimer::loadPatterns(myScheduler, myDriver, myLfsr, now);	// Load up the lights which flash in Morse code
  myDriver->writeData();										// Set the initial LED pattern based on the initialization calls 
}

// 
// Main loop - call back all expired timers and update the LED pattern
//
void 
loop()
{
	ticks_t now;
	now = millis();											// set the current snapshot time
	do {								
		myScheduler->dispatch(now);							// Dispatch all expired timers
		myDriver->writeData();								// Write updated LED pattern
		now = millis();										// Update current time after dispatch to check for any new expired timers
	} while (myScheduler->isAnyExpired(now));					// Continue dispatching while there is any expired timer - expect this to rarely loop
	delay(myScheduler->remaining(now));						// Delay until the next timer is due
}


