//
//  Abstraction for setting LED states and writing to serial LED drivers (TLC5916)
//
#ifndef WIN32
#include <Arduino.h>
#endif // !WIN32
#include "ledDriver.h"

ledDriver::ledDriver() {
#ifndef WIN32
	// Turn off all LEDs
	for (int i = 0; i < NUM_BANKS; i++) {
		_data[i] = 0;
	}
	_pwmPct = PWM_PCT; 					// Initial brightness setting - use PWM driving output-enable
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(oePin, OUTPUT);
	digitalWrite(latchPin, LOW);		// don't latch
	digitalWrite(oePin, HIGH);			// disable
#endif	// !WIN32
}

//
//  Set the pulse width percent to be used on the next writeData() call
//
void
ledDriver::setPwm(int pwmPct)
{
	// value will be used next writeData() call
	_pwmPct = pwmPct;
}

//
//  Shift out the LED values and set the brightness (PWM)
//
void
ledDriver::writeData() {
#ifndef WIN32
	digitalWrite(oePin, HIGH); 			// disable LEDs and don't let driver switch to non-standard mode
	// Shift out starting with last bank, most significant bit
	for (int bank = NUM_BANKS - 1; bank >= 0; bank--)
	{
		shiftOut(dataPin, clockPin, MSBFIRST, _data[bank]);
	}
	digitalWrite(latchPin, HIGH); 		// toggle latch pin to latch data from shift register to output drivers
	digitalWrite(latchPin, LOW);
	analogWrite(oePin, PWM_PCT_ACT_LOW(_pwmPct));	// set brightness using PWM on ~OE pin
#endif // WIN32
}

//
// Set pin i on or off (val)
//
void ledDriver::set(int i, bool val) {
	int bank = i / LEDS_PER_BANK;		// pick bank
	int bit = i % LEDS_PER_BANK;		// pick bit in the bank
	if (val) {							// if non-zero, set the bit
		_data[bank] |= 1 << bit;
	}
	else {								// otherwise, clear the bit
		_data[bank] &= ~(1 << bit);
	}
}
