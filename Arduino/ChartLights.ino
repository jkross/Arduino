// Cascaded TLC5916 drivers

#if 0
#include <RTClib.h>
#endif
#include "perfStat.h"
#include "timer.h"	
#include "scheduler.h"
#include "ledDriver.h"
#include "snapshotTime.h"
#include "ledTimer.h"
#include "morseTimer.h"
#include "debug.h"

scheduler *myScheduler;
ledDriver *myDriver;
snapshotTime *snapp;

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

  myScheduler = new scheduler();							//  Initialize the LED timing scheduler and the LED driver hardware abstraction
  myDriver = new ledDriver();
  
  snapp = new snapshotTime();								// Create the time snapshot class and initialize
  snapp->set(millis());
  ticks_t now = snapp->get();

  ledTimer::loadPatterns(myScheduler, myDriver, now);		// Load up the normal flashing patterns
  morseTimer::loadPatterns(myScheduler, myDriver, now);		// Load up the lights which flash in Morse code
  myDriver->setPwm(PWM_PCT);								// Set the global LED brightness
  myDriver->writeData();									// Set the initial LED pattern based on the initialization calls 
}

// 
// Main loop - call back all expired timers and update the LED pattern
//
void 
loop()
{
	snapp->set(millis());									// set the current snapshot time
	do {								
		myScheduler->dispatch(snapp->get());				// Dispatch all expired timers
		myDriver->writeData();								// Write updated LED pattern
		snapp->set(millis());								// Update current time after dispatch to check for any new expired timers
	} while (myScheduler->expired(snapp->get()));			// Continue dispatching while there is any expired timer - expect this to rarely loop
	delay(myScheduler->remaining(snapp->get()));			// Delay until the next timer is due
}


