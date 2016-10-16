#pragma once

#define RUN_TESTS
#define DEBUG

#ifdef 	RUN_TESTS
#define TEST(X) X
#else  	// !RUN_TESTS
#define TEST(X)
#endif  // !RUN_TESTS

#ifdef DEBUG
#define DEBUG_PRINT
//#define DEBUG_PRINT_VERBOSE
#endif // DEBUG

#ifdef DEBUG_PRINT
#define SPA(A)	  Serial.print(A)
#define SPAB(A,B) Serial.print(A); Serial.print(B)
#define SPLN(A)	  Serial.println(A)
#else	//!DEBUG_PRINT
#define SPA(A)
#define SPAB(A,B)
#define SPLN(A) 
#endif // !DEBUG_PRINT

#ifdef DEBUG_PRINT_VERBOSE
#define SPAV(A)	   Serial.print(A)
#define SPABV(A,B) Serial.print(A); Serial.print(B)
#define SPLNV(A)   Serial.println(A)
#else	//!DEBUG_PRINT_VERBOSE
#define	SPAV(A)
#define SPABV(A,B)
#define SPLNV(A) 
#endif // !DEBUG_PRINT_VERBOSE



