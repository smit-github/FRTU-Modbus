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
#include "modbus_FRTU_DO.h"
#include "modbus_Array_Defaults.h"


DO_HARDWARE_POINT_ADDRESSES DO_HPA_Data;
DO_DIAGNOSIS_POINT_ADDRESSES DO_DIAGNOSIS_Per_Card;
DO_SW_DIAGNOSIS_POINT_ADDRESSES DO_SW_DIAGNOSIS_Per_Channel;


void default_DO_Modbus_Frame(void)
{
	memcpy(DO_HPA_Data.HPA_DO_Whole_Channel, DO_FRTU_PART1_REGISTERS_TAB,DO_NO_OF_HAP_WORDBYTE);
	memcpy(&(DO_HPA_Data.HPA_DO_Whole_Channel[DO_FRTU_PART2_REGISTERS_ADDRESS]), DO_FRTU_PART2_REGISTERS_TAB,DO_NO_OF_HAP_WORDBYTE);

	memcpy(&(DO_DIAGNOSIS_Per_Card.DIAGNOSIS_DO_Whole_Card[0]), DO_FRTU_PART3_REGISTERS_TAB,DO_NO_OF_DIAGNOSIS_WORDBYTE*BYTEperWORD);

	memcpy(DO_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DO_Whole_Channel, DO_FRTU_PART4_REGISTERS_TAB,DO_NO_OF_SW_DIAGNOSIS_WORDBYTE*BYTEperWORD);

}


void test_Independent_DO(void)
{


	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.CASDU1 = 0x01;
	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.CASDU2 = 0x00;
	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.IOA1 = 0x01;
	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.IOA2 = 0x00;
	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.IOA3 = 0x00;




DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.CASDU1 =1;
DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.CASDU2 =0;

DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.IOA1 =1;
DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.IOA2 =0;
DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.IOA3 =0;




DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.CASDU1 =1;
DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.CASDU2 =0;

DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA1 =1;
DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA2 =0;
DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA3 =0;
}


