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

#define AI_NO_OF_DIAGNOSIS_CARD 12
#define AI_NO_OF_DIAGNOSIS_DATA 3 
#define AI_NO_OF_DIAGNOSIS_WORDBYTE AI_NO_OF_DIAGNOSIS_CARD*AI_NO_OF_DIAGNOSIS_DATA

//DO NOT CHANGE 
typedef union
{	
	struct
	{
		uint8_t Zero:1;	//LSB
		uint8_t Block:1;
		uint8_t DataClass:2;
		uint8_t Unused:4;
	}bits;
	uint8_t AI_Bits_Byte;
		
}AI_Bits_Allocation;

//DO NOT CHANGE 
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

//DO NOT CHANGE 
typedef union
{	
	struct
	{
        	int8_t second_Byte:8; 
		int8_t first_Byte:8;
		int8_t fourth_Byte:8; 
		int8_t third_Byte:8;
		
	}Bytes;
	
	uint32_t AI_Long_Byte;	
}AI_Long_Byte_Allocation;


void floatToLong_Swaped_Float(AI_Long_Byte_Allocation *AI_Long,AI_Float_Byte_Allocation *AI_Float);
void LongTofloat_Swaped_Float(AI_Long_Byte_Allocation *AI_Long,AI_Float_Byte_Allocation *AI_Float);


void floatToModbus(float FloatingValue,AI_Long_Byte_Allocation *AI_Long);
void test_Independent_AI(void);
void default_AI_Modbus_Frame(void);


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
	uint16_t HPA_AI_Per_Channel[AI_NO_OF_HAP/*22*/];
} AI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES ;


typedef union
{
	struct
	{
		AI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES HPA_AI_Per_Channel[AI_NO_OF_CHANNELS/*8*/];
	} HPA_whole;
	uint16_t HPA_AI_Whole_Channel[AI_NO_OF_HAP_BYTES/*22x8*/];
	
} AI_HARDWARE_POINT_ADDRESSES ;



typedef union
{
	struct
	{
		uint8_t CASDU2;	//LSB
		uint8_t CASDU1;	//MSB			

		uint8_t IOA2;	//LSB
		uint8_t IOA1;	//MSB					

		uint8_t TI;	//LSB
		uint8_t IOA3;	//MSB		

	} DIAGNOSIS_BYTES;
	uint16_t DIAGNOSIS_AI_Per_Card[AI_NO_OF_DIAGNOSIS_DATA/*3*/];
} AI_DIAGNOSIS_POINT_PER_CARD_ADDRESSES ;

typedef union
{
	struct
	{
		AI_DIAGNOSIS_POINT_PER_CARD_ADDRESSES DIAGNOSIS_AI_Per_Card[AI_NO_OF_DIAGNOSIS_CARD/*12*/];
	} Diagnosis_whole;
	uint16_t DIAGNOSIS_AI_Whole_Card[AI_NO_OF_DIAGNOSIS_WORDBYTE/*12x3=36*/];
} AI_DIAGNOSIS_POINT_ADDRESSES ;




extern AI_HARDWARE_POINT_ADDRESSES AI_HPA_Data;
extern AI_DIAGNOSIS_POINT_ADDRESSES DIAGNOSIS_AI_Per_Card;





