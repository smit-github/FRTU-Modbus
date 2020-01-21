/* tests/unit-test.h.  Generated from unit-test.h.in by configure.  */
/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/* Constants defined by configure.ac */
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#include <stdbool.h>

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




#define DO_NO_OF_CHANNELS 8
#define DO_NO_OF_HAP 16
#define DO_NO_OF_HAP_WORDBYTE DO_NO_OF_HAP*DO_NO_OF_CHANNELS

#define DO_NO_OF_DIAGNOSIS_CARD 2
#define DO_NO_OF_DIAGNOSIS_DATA 3 
#define DO_NO_OF_DIAGNOSIS_WORDBYTE DO_NO_OF_DIAGNOSIS_DATA*DO_NO_OF_DIAGNOSIS_CARD


#define DO_NO_OF_SW_DIAGNOSIS_CHANNEL 40  //5 SOFTWARE POINTS OF 8 CHANNEL EACH SO 40
#define DO_NO_OF_SW_DIAGNOSIS_DATA 3 
#define DO_NO_OF_SW_DIAGNOSIS_WORDBYTE DO_NO_OF_SW_DIAGNOSIS_CHANNEL*DO_NO_OF_SW_DIAGNOSIS_DATA

typedef union 
{
	struct
	{	
		uint8_t Dataclass:2;//LSB
		uint8_t On_before_off:1;
		uint8_t unused:5;//MSB
	}bits;
	uint8_t DO_Bits;
}bitAllocation;




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

		uint16_t select_execute_t; 
		uint16_t default_on_t ;

		uint16_t output_t_0;
		uint16_t output_t_1;		

		uint8_t Unused;
	        bitAllocation DO_bitAllocation;			

		uint8_t RS_mon_source_1 ;//LSB
		uint8_t RS_mon_source_0 ;//MSB

		uint16_t RS_mon_t;
		uint16_t command_prolong_t ;
		uint16_t TERM_t;
		uint16_t sync_t_0 ;
		uint16_t sync_t_1 ;
		uint16_t sync_inp_t_0 ;
		uint16_t sync_inp_t_1 ;

	} HPA;
	uint16_t HPA_DO_Per_Channel[DO_NO_OF_HAP/*16*/];
} DO_HARDWARE_POINT_PER_CHANNEL_ADDRESSES ;

typedef union
{
	struct
	{
		DO_HARDWARE_POINT_PER_CHANNEL_ADDRESSES HPA_DO_Per_Channel[DO_NO_OF_CHANNELS/*8*/];
	} HPA_whole;
	uint16_t HPA_DO_Whole_Channel[DO_NO_OF_HAP_WORDBYTE/*128*/];
	
} DO_HARDWARE_POINT_ADDRESSES ;

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
	uint16_t DIAGNOSIS_DO_Per_Card[DO_NO_OF_DIAGNOSIS_DATA/*3*/];
} DO_DIAGNOSIS_POINT_PER_CARD_ADDRESSES ;

typedef union
{
	struct
	{
		DO_DIAGNOSIS_POINT_PER_CARD_ADDRESSES DIAGNOSIS_DO_Per_Card[DO_NO_OF_DIAGNOSIS_CARD/*2*/];
	} Diagnosis_whole;
	uint16_t DIAGNOSIS_DO_Whole_Card[DO_NO_OF_DIAGNOSIS_WORDBYTE/*6*/];
} DO_DIAGNOSIS_POINT_ADDRESSES ;


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

	} SW_DIAGNOSIS_BYTES;
	uint16_t SW_DIAGNOSIS_DO_Per_Channel[DO_NO_OF_SW_DIAGNOSIS_DATA/*3*/];
} DO_SW_DIAGNOSIS_POINT_PER_CHANNEL_ADDRESSES ;

typedef union
{
	struct
	{
		DO_SW_DIAGNOSIS_POINT_PER_CHANNEL_ADDRESSES DIAGNOSIS_SW_DO_Per_Channel[DO_NO_OF_SW_DIAGNOSIS_CHANNEL/*40*/];
	} Diagnosis_whole;
	uint16_t SW_DIAGNOSIS_DO_Whole_Channel[DO_NO_OF_SW_DIAGNOSIS_WORDBYTE/*120*/];
} DO_SW_DIAGNOSIS_POINT_ADDRESSES;



const uint16_t DO_FRTU_PART1_REGISTERS_ADDRESS = 0x0000;
const uint16_t DO_FRTU_PART1_REGISTERS_NB = 0x0040;
const uint16_t DO_FRTU_PART1_REGISTERS_NB_MAX = 0x20;
const uint16_t DO_FRTU_PART1_REGISTERS_TAB[] = {
0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

const uint16_t DO_FRTU_PART2_REGISTERS_ADDRESS = 0x0040;
const uint16_t DO_FRTU_PART2_REGISTERS_NB = 0x0040;
const uint16_t DO_FRTU_PART2_REGISTERS_NB_MAX = 0x20;
const uint16_t DO_FRTU_PART2_REGISTERS_TAB[] = {
0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

const uint16_t DO_FRTU_PART3_REGISTERS_ADDRESS = 0x015E;
const uint16_t DO_FRTU_PART3_REGISTERS_NB = 0x0006;
const uint16_t DO_FRTU_PART3_REGISTERS_NB_MAX = 0x20;
const uint16_t DO_FRTU_PART3_REGISTERS_TAB[] = { 
0xFFFF, 0xFFFF, 0xFF01, 0xFFFF, 0xFFFF, 0xFF01};

const uint16_t DO_FRTU_PART4_REGISTERS_ADDRESS = 0x00C8;
const uint16_t DO_FRTU_PART4_REGISTERS_NB = 0x0078;
const uint16_t DO_FRTU_PART4_REGISTERS_NB_MAX = 0x20;
const uint16_t DO_FRTU_PART4_REGISTERS_TAB[] = { 
0xFFFF, 0xFFFF, 0xFF01, 0xFFFF, 0xFFFF, 0xFF01, 0xFFFF, 0xFFFF, 0xFF01, 0xFFFF, 0xFFFF, 0xFF01, 0xFFFF, 0xFFFF, 0xFF01, 0xFFFF, 0xFFFF, 0xFF01, 0xFFFF, 0xFFFF, 0xFF01, 0xFFFF, 0xFFFF, 0xFF01, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

extern DO_HARDWARE_POINT_ADDRESSES DO_HPA_Data;
extern DO_DIAGNOSIS_POINT_ADDRESSES DO_DIAGNOSIS_Per_Card;
extern DO_SW_DIAGNOSIS_POINT_ADDRESSES DO_SW_DIAGNOSIS_Per_Channel;


