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

#define AI_NO_OF_CHANNELS 8
#define AI_NO_OF_HAP 22
#define AI_NO_OF_HAP_BYTES AI_NO_OF_HAP*AI_NO_OF_CHANNELS

typedef union
{	
	struct
	{
		uint8_t Zero:1;	//LSB
		uint8_t Block:1;
		uint8_t DataClass:2;
		uint8_t Unused:4;
	}AI_Bits;
	uint8_t AI_Bits_Byte;
		
}AI_Bits_Allocation;

typedef union
{	
	struct
	{

		uint8_t first_Byte:8;
		uint8_t second_Byte:8; 
		uint8_t third_Byte:8;
		uint8_t fourth_Byte:8; 
	}Bytes;
	
	float AI_Float_Byte;	
}AI_Float_Byte_Allocation;


typedef union
{	
	struct
	{

		int8_t fourth_Byte:8; 
		int8_t third_Byte:8;
        	int8_t second_Byte:8; 
		int8_t first_Byte:8;

		
	}Bytes;
	
	uint32_t AI_Long_Byte;	
}AI_Long_Byte_Allocation;

//AI_Float_Byte_Allocation AI_Thresold_Additive;

void floatToLong_Swaped_Float(AI_Long_Byte_Allocation *AI_Long,AI_Float_Byte_Allocation *AI_Float);


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

		uint8_t Input_Type;
		AI_Bits_Allocation AI_bits;

		AI_Long_Byte_Allocation AI_Thresold_Additive;
		AI_Long_Byte_Allocation AI_Thresold_Uncond;

		uint16_t X_Zero;		
		uint16_t X_100;	

		AI_Long_Byte_Allocation Y_Zero;	
		AI_Long_Byte_Allocation Y_100;	

		AI_Long_Byte_Allocation PLAUSIBLE;
		AI_Long_Byte_Allocation	ZERO_RANGE_HIGH;
		AI_Long_Byte_Allocation ZERO_RANGE_LOW;
		AI_Long_Byte_Allocation ZERO_RANGE_YZ;
		
	} HPA;
	uint16_t HPA_AI_Per_Channel[AI_NO_OF_HAP];
} AI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES ;


typedef union
{
	struct
	{
		AI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES HPA_AI_Per_Channel[AI_NO_OF_CHANNELS];
	} HPA_whole;
	uint16_t HPA_AI_Whole_Channel[AI_NO_OF_HAP_BYTES];
	
} AI_HARDWARE_POINT_ADDRESSES ;



void floatToLong_Swaped_Float(AI_Long_Byte_Allocation *AI_Long,AI_Float_Byte_Allocation *AI_Float)
{
     AI_Long->Bytes.first_Byte = AI_Float->Bytes.first_Byte;
    AI_Long->Bytes.second_Byte = AI_Float->Bytes.second_Byte;
    AI_Long->Bytes.third_Byte = AI_Float->Bytes.third_Byte;
    AI_Long->Bytes.fourth_Byte = AI_Float->Bytes.fourth_Byte;

}


//extern DI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES DI_HPA_Data[16];
extern AI_HARDWARE_POINT_ADDRESSES AI_HPA_Data;

