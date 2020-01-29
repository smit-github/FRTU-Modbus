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
#include "modbus_FRTU_DI.h"
#include "modbus_Array_Defaults.h"


DI_HARDWARE_POINT_ADDRESSES DI_HPA_Data;
DI_DIAGNOSIS_POINT_ADDRESSES DI_DIAGNOSIS_Per_Card;
DI_SW_DIAGNOSIS_POINT_ADDRESSES DI_SW_DIAGNOSIS_Per_Channel;

void default_DI_Modbus_Frame(void)
{
	memcpy(DI_HPA_Data.HPA_DI_Whole_Channel, DI_FRTU_PART1_REGISTERS_TAB,DI_NO_OF_HAP_WORDBYTE);
	memcpy(&(DI_HPA_Data.HPA_DI_Whole_Channel[DI_FRTU_PART2_REGISTERS_ADDRESS]), DI_FRTU_PART2_REGISTERS_TAB,DI_NO_OF_HAP_WORDBYTE);

	memcpy(&(DI_DIAGNOSIS_Per_Card.DIAGNOSIS_DI_Whole_Card[0]), DI_FRTU_PART3_REGISTERS_TAB,DI_NO_OF_DIAGNOSIS_WORDBYTE*BYTEperWORD);

	memcpy(DI_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DI_Whole_Channel, DI_FRTU_PART4_REGISTERS_TAB,DI_NO_OF_SW_DIAGNOSIS_WORDBYTE*BYTEperWORD);


}

void test_Independent_DI(void)
{


	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.CASDU2 = 0x00;


	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA2 = 0x00;
	
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA3 = 0x00;

#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA3);
#endif

	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA1 = 0x02;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA3 = 0x00;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA3);
#endif

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA1 = 0x03;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA3 = 0x00;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA3);
#endif

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.CASDU2 = 0x00;
       
        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA1 = 0x04;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA3 = 0x00;
#ifdef DEBUG

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA3);
#endif
        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA1 = 0x05;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.TI = 0x1F;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA3 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.DI_bitAllocation.bits.Inv = 0;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.DI_bitAllocation.bits.Inv_1 = 1;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA3);
#endif
        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA1 = 0x06;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.TI = 0x1F;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA3 = 0x00;

#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA3);
#endif

	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.DI_bitAllocation.bits.Inv = 0;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.DI_bitAllocation.bits.Inv_1 = 1;

#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.DI_bitAllocation.DI_Bits);
#endif
        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA1 = 0x07;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA3 = 0x00;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA3);

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA1 = 0x08;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA3 = 0x00;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA3);

#endif
        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA1 = 0x09;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA3 = 0x00;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA3);
#endif

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA1 = 10;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA3 = 0x00;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA3);
#endif

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA1 = 11;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA3 = 0x00;

#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA3);
#endif

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA1 = 12;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA3 = 0x00;


#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA3);

#endif

#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.DI_bitAllocation.DI_Bits);
#endif
        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.IOA1 = 13;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.IOA3 = 0x00;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[12].HPA.IOA3);
#endif

	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA1 = 14;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA3 = 0x00;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA3);
#endif

	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA1 = 15;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA3 = 0x00;
#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA3);
#endif

	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA1 = 16;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA3 = 0x00;


#ifdef DEBUG
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA3);

				printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.DI_bitAllocation.DI_Bits);
#endif

}

