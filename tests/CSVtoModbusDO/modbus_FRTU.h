/* tests/unit-test.h.  Generated from unit-test.h.in by configure.  */
/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/* Constants defined by configure.ac */


#include <stdint.h>

#define TEST_DO
#ifdef TEST_DO
	#define SERVER_ID         7
#endif

//#define TEST_DI
#ifdef TEST_DI
	#define SERVER_ID         3
#endif

//#define TEST_AI
#ifdef TEST_AI
	#define SERVER_ID         1
#endif


#define INVALID_SERVER_ID 18

#define BYTEperWORD 2



