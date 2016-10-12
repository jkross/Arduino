//
//  Class to manage LED patterns for a fixed light sequence
//
#include <Arduino.h>
#include "debug.h"
#include "ledTimer.h"
#include "lightList.h"


//  Constructor
//  	to save space we store the index of the sequence durations, not the actual sequence per LED
ledTimer::ledTimer(uint8_t pin, int seqIndex)
	: timer(0)
{
	_state = 0;
	_pin = pin;
	_seqIndex = seqIndex;
}

//
//  Set up - schedule for when we will start our pattern
//  	Initial start time is a delay (driver initialized to all off)
//
bool
ledTimer::init(ticks_t start, ledDriver* driver)
{
	_driver = driver;
	ticks = start;
	return true; // invoke(now, 0);
}

#define MULTIPLIER 1				// BUGBUG: todel
#define FUZZ	   10

//
//  Calculate duration for specified step in state machine, adding random fuzz if requested
//  	to save space we store the index of the sequence durations, not the actual sequence per LED
//
uint16_t
ledTimer::stepDuration(int state, int fuzz) {
	uint16_t duration = uniqueLightSeq[_seqIndex].sequence[state];
	duration += fuzz ? FUZZ : 0;
	return duration;
}

//
//  Called when the timer expires.
//  * set LED state
//  * calculate next time to be invoked
//
//  Always request a next time, cycling through pattern indefinitely
//
bool
ledTimer::invoke(ticks_t now, int fuzz)
{
	do {
		int bitval = (_state & 0x1) ^ 0x1;			// alternate between on/off (first duration is the "on" duration)
		_driver->set(_pin, bitval);					// record desired LED value (on/off)

		ticks = ticks + stepDuration(_state, fuzz);	// calculate the next time to be invoked

		_state = (_state + 1) % PATTERN_SEQ_LEN;	// advance the state machine
		//SPAB("now ", now); SPAB(" pin: ", _ppattern->pin); SPAB(" val: ", bitval); SPAB(" ticks ", ticks); SPLN();
	} while (expired(now));							// catch up if we need to
	return true;									// schedule again using absolute time in "ticks"
}

//  BUGBUG:  refactor this, removing duplicate in *Timer.cpp
//  For efficiency we pick a power of two for the # of delay bins (we take modulo of pseudo-random number)
#define START_BUCKETS	16							// power of 2
// Spread starts over ~2s
#define START_FUZZ (2048 / START_BUCKETS)			// make it an even divide

//
//  Load up all the LED values from our table
//
void
ledTimer::loadPatterns(scheduler* schedp, ledDriver *driverp, ticks_t now)
{
	lfsr *lfsrp = new lfsr();		// BUGBUG: pass in one instance
	for (int i = 0; i < NUM_LIGHT_LIST; i++) {
		// Again, to save space, we implicitly set the pin from the table index number
		//   and skip the couple of lights that are not fixed blinking patterns
		if (lightList[i].seqId != LIGHT_DESC_NO_BLINK) {
			ledTimer *light = new ledTimer(i, lightList[i].seqId);				// create the timer object
			ticks_t startOffset = (lfsrp->next() % START_BUCKETS) * START_FUZZ;	// compute random start time
			if (light->init(now + startOffset, driverp)) {
				schedp->queueTimer(light);										// queue @ start time
			}
		}
	}
}
