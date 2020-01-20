/* tests/unit-test.h.  Generated from unit-test.h.in by configure.  */
/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/* Constants defined by configure.ac */
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
# ifndef _MSC_VER
# include <stdint.h>
# else
# include "stdint.h"
# endif
#endif

typedef union
{
	struct
	{
		uint8_t CASDU2;
		uint8_t CASDU1;				

		uint8_t IOA2;
		uint8_t IOA1;						

		uint8_t TI;
		uint8_t IOA3;

		uint8_t Flutter_Number;
		uint8_t State_Of_Flutter;
		

		uint16_t Flutter_time;

		uint16_t Software_Debouce_time;

		uint16_t Intermeditate_Position_time;

		uint16_t Faulty_Position_time;

		uint16_t AR_Signaling_time;

		uint8_t RESERVED_1;
		uint8_t ON_Before_Time;
		
	} HPA;
	uint16_t HPA_DI_Per_Channel[10];
} DI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES ;


typedef union
{
	struct
	{
		DI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES HPA_DI_Per_Channel[16];
	} HPA_whole;
	uint16_t HPA_DI_Whole_Channel[160];
	
} DI_HARDWARE_POINT_ADDRESSES ;

//extern DI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES DI_HPA_Data[16];
extern DI_HARDWARE_POINT_ADDRESSES DI_HPA_Data;


