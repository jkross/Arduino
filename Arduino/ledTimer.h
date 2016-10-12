//
//  Class to manage LED patterns for a fixed light sequence
//
#pragma once
#include "scheduler.h"
#include "ledDriver.h"
#include "patterns.h"
#include "timer.h"



class ledTimer : public timer {
private:
	uint8_t			_state;				// manage state machine
	uint8_t			_pin;				// this light's pin number
	uint8_t		    _seqIndex;			// this light's sequence pattern index
	ledDriver*		_driver;			// hardware abstraction for setting LED values

	uint16_t stepDuration(int state, int fuzz);

public:
	ledTimer(uint8_t pin, int seqIndex);
	virtual bool init(ticks_t now, ledDriver* driver);
	virtual bool invoke(ticks_t now, int fuzz);										// called on timer expiration
	static void loadPatterns(scheduler * schedp, ledDriver * driverp, ticks_t now);	// blinking pattern init
};
