#pragma once
#include <stdint.h>

class ledDriver {
private:
	static const int NUM_BANKS = 5;	// number of TLC5916 drivers
	const int LEDS_PER_BANK = 8;	// number of LEDs per driver
	const int latchPin = 12;  		// connected to LE  of TLC5916	active high
	const int clockPin = 8;   		// connected to CLK of TLC5916
	const int dataPin = 11;   		// connected to SDI of TLC5916
	const int oePin = 3;      		// connected to ~OE of TLC5916	active low

							  		// Since ~OE is active low we calcuate our PWM percent as the inverse %
#define PWM_PCT_ACT_LOW(_PCT) ((int)255*(1-(_PCT/100.0)))
#define PWM_PCT 10

	uint8_t _data[NUM_BANKS];		// 8 LEDs per driver, 8 bits per byte
	int		_pwmPct;				// brightness setting
public:
	ledDriver();					// constructor

	void setPwm(int pwmPct);		// set brightness

	void writeData();				// write LED patterns to drivers

	void set(int i, bool val);		// set LED on/off
};
