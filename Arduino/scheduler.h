//
//  Class to schedule timers and dispatch on timer expiration
//
#pragma once

#include "timer.h"

using namespace std;

#include <iterator>
#include <queue>
#include "Lfsr.h"

class Compare {
  public:
    bool operator()(timer *left, timer* right) {
      bool ret = left->operator <(right);
      return ret;
    }
};

class scheduler {
    priority_queue<timer *, vector<timer *>, Compare> timerList;			// Ordered queue of timers
	deque<timer *> expiredDeque;											// Work queue of expired timers
	lfsr* _lfsr;															// Pseudo-random generator

  public:
	scheduler();

    void queueTimer(timer* t);												// Add timer to list of pending timers

    bool empty();															// Query: Are there any pending timers?

	int size();																// Return number of scheduled timers

	bool expired(ticks_t now);												// Query: Has any timer expired?

	void dispatch(ticks_t now);												// Invoke all expired timers

    ticks_t remaining(ticks_t now);											// Return ticks until next timer expiration
};


// ToDo:  	empty -> isEmpty()
// 			expired -> isAnyExpired()
