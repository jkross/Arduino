//
//  Quickly generate pseudo random numbers using a Linear Feedback Shift Register
//
#include "lfsr.h"
#define LFSR_START_STATE	0xDEAD
#define	LFSR_POLY			0xB400		// this polynomial will not recycle until ~2^16 iterations


lfsr::lfsr() {
	_lfsr = LFSR_START_STATE;			// starting state
}

lfsr_val_t
lfsr::next() {
	lfsr_val_t lsb = _lfsr & 1;   		// Get the output bit (i.e. the LSB)
	_lfsr >>= 1;                  		// Shift the register
	if (lsb) {					  		// If the output bit is 1, apply xor mask.
		_lfsr ^= LFSR_POLY;
	}
	return _lfsr;
}

// A couple of helper routines follow to randomize start times (big variation) and LED
// flash/blank durations (small variation)

// Pick random start times for lights, spread over a unit of time.
// Choose a power of two bucket size for efficient mod operations, and a total time which 
//   can also be efficiently divided by our bucket size.
#define START_BUCKETS	16							// power of 2
// Spread starts over ~2s
#define START_FUZZ (2048 / START_BUCKETS)			// make it an even divide

// Pick the next starting offset
unsigned long
lfsr::nextStartOffset() {
	return (next() % START_BUCKETS) * START_FUZZ;
}

// Pick whether we add FUZZ to our duration or not
// Choose 10ms as our unit

#define	FUZZ	10

// Pick the next duration offset
unsigned long
lfsr::nextDurationFuzz() {
	return (next() & 0x1) ? FUZZ : 0;
}