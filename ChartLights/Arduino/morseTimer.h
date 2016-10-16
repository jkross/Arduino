//
//  Class to manage LED patterns for a Morse code blinking sequence
//
#pragma once
#include "scheduler.h"
#include "ledDriver.h"
#include "timer.h"
#include "morsePatterns.h"
#include "morse.h"



class morseTimer : public timer {
private:
	morseLightDesc*	_mldp;				// describe the light, positon and characters
	ledDriver*		_driver;			// hardware abstraction
	uint8_t			_state_char;		// the character we are on
	uint8_t			_state;				// position in the character
	const morseChar* _mchar;			// current characters Morse duration pattern

	int index(char c);							// index into Morse table for given character
	const morseChar * getCharDesc(int state_char);	// get the Morse character description
	int decodeStateDur(uint8_t state);
	// for the given state return number of "morse units" in duration
	uint16_t stepDuration(int state, int fuzz);	// return duration in milliseconds for specified state
	void advanceState(void);					// advance the state machine

public:
	morseTimer(morseLightDesc * mldp);
	virtual bool init(ticks_t now, ledDriver* driver);
	virtual bool invoke(ticks_t now, int fuzz);										// called on timer expiration
	static void loadPatterns(scheduler * schedp, ledDriver * driverp, lfsr * lfsrp, ticks_t now);// blinking pattern init
};
