#pragma once
// You thought it would be this easy??  Get a Mid to read the Morse.
struct morseLightDesc morseLightList[] = {
	{ /*pin*/  8, /*letters*/  "Lorem ipsum dolor sit amet "		/* Bl B M        38-46-22.58N   76-33-47.61W  VaLeighGirl */ },
	{ /*pin*/ 25, /*letters*/  "consectetur adipiscing elit "		/* Fl B M        38-58-33.98N   76-29-57.50W  Drifmier */ },
};
#define NUM_MORSE_LIGHT_LIST sizeof(morseLightList)/sizeof(morseLightList[0])
