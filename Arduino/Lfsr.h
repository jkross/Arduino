//
//  Class to manage LED patterns for a fixed light sequence
//
#pragma once
#include <stdint.h>

#define LFSR_BITS			16			// shift register 16 bits wide

typedef uint16_t lfsr_val_t;			// NB: change base type if bit width changes

class lfsr {
private:
	lfsr_val_t	_start_state;			// BUGBUG: not needed
	lfsr_val_t	_lfsr;					// current shift register state
public:
	lfsr();
	lfsr_val_t next(void);
};
