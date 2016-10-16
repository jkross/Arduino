//
//  Base class for timers of various flavors
//
#ifndef WIN32
#include <Arduino.h>
#endif
#include "debug.h"
#include "timer.h"
#include <new>

timer::timer(ticks_t i)
{
	ticks = i;
}

//
//  Called when timer expires.  Base class doesn't request re-scheduling.
//
bool
timer::invoke(ticks_t now, int fuzz) {
	return false;		// default to one-shot
}

//
//  Return time remaining until timer expires
//
ticks_t
timer::remaining(ticks_t now) {
  return (ticks - now);
}

//
//  Query: Has this timer expired?
//
bool
timer::isExpired(ticks_t now)
{
	// The global clock wraps after reaching 2^32 ticks.
	// If the distance between timers is less than "HalfWrap" we consider the timer expired
	// Therefore a timer can only be scheduled "HalfWrap" ticks into the future.  Further than that and it
	// will appear that the timer has already expired.
	// A 32-bit timer which ticks every millisecond has a HalfWrap period of > 24.8 days (plenty for LED blink patterns)
	return (now - ticks) < HalfWrap;
}

//
//  Comparison operator
//
bool
timer::operator <(const timer* right) const
{
	bool ret = (right->ticks - ticks) > HalfWrap;
  	return ret;
}
