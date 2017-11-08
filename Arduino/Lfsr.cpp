//
//  Quickly generate pseudo random numbers using a Linear Feedback Shift Register
//
#include "lfsr.h"
#define LFSR_START_STATE	0xDEAD
#define	LFSR_POLY			0xB400		// this polynomial will not recycle until ~2^16 iterations


lfsr::lfsr() {
	_start_state = LFSR_START_STATE;	// BUGBUG: should be _lfsr
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
