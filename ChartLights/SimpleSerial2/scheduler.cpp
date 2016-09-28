#include <Arduino.h>
#include "scheduler.h"
#include <pnew.cpp>

void 
scheduler::schedule(timer* t)
{
	timerList.push(t);
}

bool 
scheduler::empty()
{
	return timerList.empty();
}

bool 
scheduler::expired(ticks_t now, timer* t)
{
	return timer::expired(now, t->ticks);
}

ticks_t
scheduler::dispatch(ticks_t now)
{
	timer *top;
	do {
		top = timerList.top();
		if (!expired(now, top))
			break;
		timerList.pop();
		bool another = top->invoke(now);
		if (another) {
			schedule(top);
		}
	} while (!timerList.empty());
	return top->remaining(now);
}

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

void 
scheduler::dumpDestructive()
{
  while (!timerList.empty()) {
    ticks_t i = timerList.top()->ticks;
    timerList.pop();
    Serial.print("dump: ");
    Serial.print(i, DEC);
    Serial.println();
  }
}

#include "snapshotTime.h"
snapshotTime* gTime;

void 
scheduler::testTimers() {
	gTime = new snapshotTime();
	gTime->set(((ticks_t)2));
#if 0
	schedule(new timer(3, gTime));
	schedule(new timer(10, gTime));
	schedule(new timer(3, gTime));
	schedule(new timer(1, gTime));
	schedule(new timer(-1, gTime));
  dumpDestructive();
#endif
  schedule(new timer(3, gTime));
  schedule(new timer(10, gTime));
  schedule(new timer(3, gTime));
  schedule(new timer(1, gTime));
  schedule(new timer(-1, gTime));

	ticks_t now = gTime->get();
	while (!empty())
	{
		// cout << "now: " << now << endl;
		Serial.print("now: ");
		Serial.print(now, HEX);
		Serial.println();
		dispatch(now);
		gTime->set(now++);
	}
  Serial.println("testTimers end");
}
