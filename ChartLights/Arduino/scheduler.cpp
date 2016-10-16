//
//  Class to schedule timers and dispatch on timer expiration
//
#ifndef WIN32
#include <Arduino.h>
#include <pnew.cpp>
#endif // !WIN32
#include "scheduler.h"

scheduler::scheduler(lfsr *lfsrp)
{
	_lfsr = lfsrp;
}

//
//  Add timer to the list of timers, sorted by the timer's "<" operator
//
void
scheduler::queueTimer(timer* t)
{
	timerList.push(t);
}

//
//  Query for any scheduled timers
//
bool
scheduler::isEmpty()
{
	return timerList.empty();
}

//
// Return whether any timer has expired
//
bool
scheduler::isAnyExpired(ticks_t now)
{
	// Check the first timer...
	return timerList.top()->isExpired(now);
}

//
// Dispatch all the expired timers
//
void
scheduler::dispatch(ticks_t now)
{
	timer  *top;
	bool	another;

	//
	// Invoke each expired and re-schedule it if the
	// invoke function requests re-scheduling.
	//
	while (!isEmpty() && isAnyExpired(now)) {
		top = timerList.top();						// get the first on the list
		timerList.pop();							// and remove from the priority list
		another = top->invoke(now, _lfsr->nextDurationFuzz());
		if (another) {								 // re-schedule if requested - invoke set a new time
			queueTimer(top);
		}
	}
}

//
//  Return the number of ticks until the next timer expiration
//
ticks_t
scheduler::remaining(ticks_t now)
{
	ticks_t remain = 0;
	if (!isEmpty()) {
		timer *top = timerList.top();
		remain = top->remaining(now);
	}
	return remain;
}
