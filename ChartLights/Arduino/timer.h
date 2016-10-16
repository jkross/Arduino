//
//  Base class for timers of various flavors
//
#pragma once

typedef unsigned long ticks_t;

const ticks_t HalfWrap = ~((ticks_t)0) >> 1;

class timer {
public:
	ticks_t ticks;						// time next invoke is due

	timer(ticks_t i);
	bool operator <(const timer* other) const;

	virtual bool invoke(ticks_t now, int fuzz);		// Called on expiration.  If returns true, re-schedule
	ticks_t remaining(ticks_t now);					// Time remaining until expiration
	bool isExpired(ticks_t now);						// Query: Has timer expired?

};
