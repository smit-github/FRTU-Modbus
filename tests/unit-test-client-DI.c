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

#include "unit-test.h"
#include "modbus_FRTU_DI.h"

#define BYTEperWORD 2


DI_HARDWARE_POINT_ADDRESSES DI_HPA_Data;
DI_DIAGNOSIS_POINT_ADDRESSES DI_DIAGNOSIS_Per_Card;
DI_SW_DIAGNOSIS_POINT_ADDRESSES DI_SW_DIAGNOSIS_Per_Channel;

const int EXCEPTION_RC = 2;

enum {
    TCP,
    TCP_PI,
    RTU
};

int test_server(modbus_t *ctx, int use_backend);
int send_crafted_request(modbus_t *ctx, int function,
                         uint8_t *req, int req_size,
                         uint16_t max_value, uint16_t bytes,
                         int backend_length, int backend_offset);
int equal_dword(uint16_t *tab_reg, const uint32_t value);

#define BUG_REPORT(_cond, _format, _args ...) \
    printf("\nLine %d: assertion error for '%s': " _format "\n", __LINE__, # _cond, ## _args)

#define ASSERT_TRUE(_cond, _format, __args...) {  \
    if (_cond) {                                  \
        printf("OK\n");                           \
    } else {                                      \
        BUG_REPORT(_cond, _format, ## __args);    \
        goto close;                               \
    }                                             \
};

int equal_dword(uint16_t *tab_reg, const uint32_t value) {
    return ((tab_reg[0] == (value >> 16)) && (tab_reg[1] == (value & 0xFFFF)));
}

int main(int argc, char *argv[])
{
    const int NB_REPORT_SLAVE_ID = 10;
    uint8_t *tab_rp_bits = NULL;
    uint16_t *tab_rp_registers = NULL;
    uint16_t *tab_rp_registers_bad = NULL;
    modbus_t *ctx = NULL;
    int i;
    uint8_t value;
    int nb_points;
    int rc;
    float real;
    uint32_t old_response_to_sec;
    uint32_t old_response_to_usec;
    uint32_t new_response_to_sec;
    uint32_t new_response_to_usec;
    uint32_t old_byte_to_sec;
    uint32_t old_byte_to_usec;
    int use_backend;
    int success = FALSE;
    int old_slave;



    if (argc > 1) {
        if (strcmp(argv[1], "tcp") == 0) {
            use_backend = TCP;
        } else if (strcmp(argv[1], "tcppi") == 0) {
            use_backend = TCP_PI;
        } else if (strcmp(argv[1], "rtu") == 0) {
            use_backend = RTU;
        } else {
            printf("Usage:\n  %s [tcp|tcppi|rtu] - Modbus client for unit testing\n\n", argv[0]);
            exit(1);
        }
    } else {
        /* By default */
        use_backend = TCP;
    }

    if (use_backend == TCP) {
        ctx = modbus_new_tcp("127.0.0.1", 1502);
    } else if (use_backend == TCP_PI) {
        ctx = modbus_new_tcp_pi("::1", "1502");
    } else {
        ctx = modbus_new_rtu("/dev/ttyUSB0", 115200, 'E', 8, 1);
    }
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);

    if (use_backend == RTU) {
        modbus_set_slave(ctx, SERVER_ID);
    }

    modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    modbus_get_response_timeout(ctx, &new_response_to_sec, &new_response_to_usec);

    printf("** UNIT TESTING **\n");

    /** HOLDING REGISTERS **/


	memcpy(DI_HPA_Data.HPA_DI_Whole_Channel, DI_FRTU_PART1_REGISTERS_TAB,DI_NO_OF_HAP_WORDBYTE);
	memcpy(&(DI_HPA_Data.HPA_DI_Whole_Channel[DI_FRTU_PART2_REGISTERS_ADDRESS]), DI_FRTU_PART2_REGISTERS_TAB,DI_NO_OF_HAP_WORDBYTE);

	memcpy(&(DI_DIAGNOSIS_Per_Card.DIAGNOSIS_DI_Whole_Card[0]), DI_FRTU_PART3_REGISTERS_TAB,DI_NO_OF_DIAGNOSIS_WORDBYTE*BYTEperWORD);

	memcpy(DI_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DI_Whole_Channel, DI_FRTU_PART4_REGISTERS_TAB,DI_NO_OF_SW_DIAGNOSIS_WORDBYTE*BYTEperWORD);


	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.CASDU2 = 0x00;


	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA2 = 0x00;
	
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA3 = 0x00;


	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[0].HPA.IOA3);


	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA1 = 0x02;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA3 = 0x00;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[1].HPA.IOA3);
  

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA1 = 0x03;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA3 = 0x00;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[2].HPA.IOA3);


        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.CASDU2 = 0x00;
       
        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA1 = 0x04;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA3 = 0x00;


	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[3].HPA.IOA3);

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA1 = 0x05;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.TI = 0x1F;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA3 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.DI_bitAllocation.bits.Inv = 0;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.DI_bitAllocation.bits.Inv_1 = 1;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[4].HPA.IOA3);

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA1 = 0x06;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.TI = 0x1F;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA3 = 0x00;


	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.IOA3);
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.DI_bitAllocation.bits.Inv = 0;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.DI_bitAllocation.bits.Inv_1 = 1;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[5].HPA.DI_bitAllocation.DI_Bits);

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA1 = 0x07;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[6].HPA.IOA3 = 0x00;

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

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[7].HPA.IOA3);

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA1 = 0x09;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA3 = 0x00;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[8].HPA.IOA3);

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA1 = 10;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA3 = 0x00;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[9].HPA.IOA3);

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA1 = 11;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA3 = 0x00;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[10].HPA.IOA3);

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA1 = 12;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA3 = 0x00;


	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.IOA3);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[11].HPA.DI_bitAllocation.DI_Bits);

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

	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA1 = 14;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA3 = 0x00;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[13].HPA.IOA3);

	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA1 = 15;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA3 = 0x00;

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[14].HPA.IOA3);

	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.CASDU1 = 0x01;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.CASDU2 = 0x00;

        DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA1 = 16;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA2 = 0x00;
	DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA3 = 0x00;



	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.CASDU1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.CASDU2);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA1);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA2);
	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.IOA3);

	printf("%d\n",DI_HPA_Data.HPA_whole.HPA_DI_Per_Channel[15].HPA.DI_bitAllocation.DI_Bits);


    /* Many registers */
    printf("1/4 FRTU_DI_write_registers: ");
    rc = modbus_write_registers(ctx, DI_FRTU_PART1_REGISTERS_ADDRESS,
DI_FRTU_PART1_REGISTERS_NB, &(DI_HPA_Data.HPA_DI_Whole_Channel[0]));
    
    //ASSERT_TRUE(rc == DI_FRTU_PART1_REGISTERS_NB, "");
    
    printf("2/4 FRTU_DI_write_registers: ");
    rc = modbus_write_registers(ctx, DI_FRTU_PART2_REGISTERS_ADDRESS,
                                DI_FRTU_PART2_REGISTERS_NB, &(DI_HPA_Data.HPA_DI_Whole_Channel[0+DI_NO_OF_HAP_WORDBYTE/2]));
    
   // ASSERT_TRUE(rc == DI_FRTU_PART2_REGISTERS_NB, "");


DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[1].DIAGNOSIS_BYTES.CASDU1 =1;
DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[1].DIAGNOSIS_BYTES.CASDU2 =0;
DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[1].DIAGNOSIS_BYTES.IOA1 =1;
DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[1].DIAGNOSIS_BYTES.IOA2 =0;
DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[1].DIAGNOSIS_BYTES.IOA3 =0;



    printf("3/4 FRTU_DO_write_registers: ");
    rc = modbus_write_registers(ctx, DI_FRTU_PART3_REGISTERS_ADDRESS,
                                DI_NO_OF_DIAGNOSIS_WORDBYTE, DI_DIAGNOSIS_Per_Card.DIAGNOSIS_DI_Whole_Card);
    
   // ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");


DI_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[1].SW_DIAGNOSIS_BYTES.CASDU1 =1;
DI_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[1].SW_DIAGNOSIS_BYTES.CASDU2 =0;

DI_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA1 =1;
DI_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA2 =0;
DI_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA3 =0;

    printf("4/4 FRTU_DO_write_registers: ");
    rc = modbus_write_registers(ctx, DI_FRTU_PART4_REGISTERS_ADDRESS,
                                DI_NO_OF_SW_DIAGNOSIS_WORDBYTE, &(DI_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DI_Whole_Channel[0]));
    
   // ASSERT_TRUE(rc == DO_FRTU_PART4_REGISTERS_NB, "");
    /* End of many registers */

    modbus_close(ctx);
    modbus_free(ctx);
    ctx = NULL;
  

close:
    /* Free the memory */
    free(tab_rp_bits);
    free(tab_rp_registers);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return (success) ? 0 : -1;
}


