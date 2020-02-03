/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>

#include "modbus_FRTU.h"
#include "modbus_FRTU_AI.h"
#include "modbus_Array_Defaults.h"



AI_HARDWARE_POINT_ADDRESSES AI_HPA_Data;
AI_DIAGNOSIS_POINT_ADDRESSES DIAGNOSIS_AI_Per_Card;



void default_AI_Modbus_Frame(void)
{

	memcpy(AI_HPA_Data.HPA_AI_Whole_Channel, AI_FRTU_PART1_REGISTERS_TAB,AI_NO_OF_HAP_BYTES);
	memcpy(&(AI_HPA_Data.HPA_AI_Whole_Channel[AI_FRTU_PART2_REGISTERS_ADDRESS]), AI_FRTU_PART2_REGISTERS_TAB,AI_NO_OF_HAP_BYTES);

	memcpy(&(DIAGNOSIS_AI_Per_Card.DIAGNOSIS_AI_Whole_Card[0]), AI_FRTU_PART3_REGISTERS_TAB,AI_NO_OF_DIAGNOSIS_WORDBYTE*BYTEperWORD);
}



void test_Independent_AI(void)
{

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.CASDU1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.CASDU2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.IOA1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.IOA2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.IOA3 = 0x00;

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.TI = 34;
	

	printf("%d\n",AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.CASDU1);
	printf("%d\n",AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.CASDU2);

	printf("%d\n",AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.IOA1);
	printf("%d\n",AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.IOA2);
	printf("%d\n",AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[0].HPA.IOA3);
	 
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[1].HPA.CASDU1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[1].HPA.CASDU2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[1].HPA.IOA1 = 0x02;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[1].HPA.IOA2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[1].HPA.IOA3 = 0x00;

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[1].HPA.TI = 34;

	floatToModbus( -6000.0,&(AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[1].HPA.Y_Zero));


	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[2].HPA.CASDU1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[2].HPA.CASDU2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[2].HPA.IOA1 = 0x03;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[2].HPA.IOA2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[2].HPA.IOA3 = 0x00;

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[2].HPA.TI = 11;

	floatToModbus( -7000.0,&(AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[2].HPA.Y_Zero));

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[3].HPA.CASDU1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[3].HPA.CASDU2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[3].HPA.IOA1 = 0x04;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[3].HPA.IOA2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[3].HPA.IOA3 = 0x00;
	floatToModbus( -8000.0,&(AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[3].HPA.Y_Zero));

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[4].HPA.CASDU1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[4].HPA.CASDU2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[4].HPA.IOA1 = 0x05;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[4].HPA.IOA2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[4].HPA.IOA3 = 0x00;
	floatToModbus( -9000.12,&(AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[4].HPA.Y_Zero));

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[5].HPA.CASDU1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[5].HPA.CASDU2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[5].HPA.IOA1 = 0x06;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[5].HPA.IOA2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[5].HPA.IOA3 = 0x00;
	floatToModbus( -11230.34,&(AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[5].HPA.Y_Zero));

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[6].HPA.CASDU1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[6].HPA.CASDU2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[6].HPA.IOA1 = 0x07;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[6].HPA.IOA2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[6].HPA.IOA3 = 0x00;

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[6].HPA.TI = 13;

	floatToModbus( -12345.67,&(AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[6].HPA.Y_Zero));

	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[7].HPA.CASDU1 = 0x01;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[7].HPA.CASDU2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[7].HPA.IOA1 = 0x08;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[7].HPA.IOA2 = 0x00;
	AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[7].HPA.IOA3 = 0x00;
	floatToModbus( -23456.87,&(AI_HPA_Data.HPA_whole.HPA_AI_Per_Channel[7].HPA.Y_Zero));

}

void floatToModbus(float FloatingValue,AI_Long_Byte_Allocation *AI_Long)
{
	AI_Float_Byte_Allocation AI_Float;

	AI_Float.AI_Float_Byte = FloatingValue;
	 floatToLong_Swaped_Float( AI_Long,&AI_Float);
	
}


void floatToLong_Swaped_Float(AI_Long_Byte_Allocation *AI_Long,AI_Float_Byte_Allocation *AI_Float)
{
     AI_Long->Bytes.first_Byte = AI_Float->Bytes.first_Byte;
    AI_Long->Bytes.second_Byte = AI_Float->Bytes.second_Byte;
    AI_Long->Bytes.third_Byte = AI_Float->Bytes.third_Byte;
    AI_Long->Bytes.fourth_Byte = AI_Float->Bytes.fourth_Byte;

}

void LongTofloat_Swaped_Float(AI_Long_Byte_Allocation *AI_Long,AI_Float_Byte_Allocation *AI_Float)
{
     AI_Float->Bytes.first_Byte = AI_Long->Bytes.first_Byte;
    AI_Float->Bytes.second_Byte = AI_Long->Bytes.second_Byte;
    AI_Float->Bytes.third_Byte = AI_Long->Bytes.third_Byte;
    AI_Float->Bytes.fourth_Byte = AI_Long->Bytes.fourth_Byte;

}
