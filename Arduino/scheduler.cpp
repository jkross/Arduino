//
//  Class to schedule timers and dispatch on timer expiration
//
#ifndef WIN32
#include <Arduino.h>
#include <pnew.cpp>
#endif // !WIN32
#include "scheduler.h"

scheduler::scheduler()
{
	_lfsr = new lfsr();
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
scheduler::empty()
{
	return timerList.empty();
}

//
//  Return the number of scheduled timers
//
int
scheduler::size()
{
	return timerList.size();
}

//
// Return whether any timer has expired
//
bool
scheduler::expired(ticks_t now)
{
	// Check the first timer...
	return timerList.top()->expired(now);
}

//
// Dispatch all the expired timers
//
void
scheduler::dispatch(ticks_t now)
{
	timer *top;
	//
	// First put all the expired timers on their own list
	//	NB: may not be required given new ordering of timers jkr 16.10.11
	//
	while (!timerList.empty() && expired(now)) {
		top = timerList.top();
		timerList.pop();
		expiredDeque.push_front(top);
	}
	//
	// Invoke each expired timer on the expired list, and re-schedule it if the
	// invoke function requests re-scheduling.
	//
	while (!expiredDeque.empty()) {
		top = expiredDeque.back();
		expiredDeque.pop_back();
		bool another = top->invoke(now, (_lfsr->next() & 0x1));
		if (another) {								 // re-schedule if requested - invoke set a new time
			queueTimer(top);
			//ticks_t next = timerList.top()->ticks; // DBG
			//lSize = size(); // DBG
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
	if (!timerList.empty()) {
		timer *top = timerList.top();
		remain = top->remaining(now);
	}
	return remain;
}
