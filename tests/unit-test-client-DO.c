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

#define BYTEperWORD 2
#include "unit-test.h"

#include "modbus_FRTU.h"
#include "modbus_FRTU_DO.h"


DO_HARDWARE_POINT_ADDRESSES DO_HPA_Data;
DO_DIAGNOSIS_POINT_ADDRESSES DO_DIAGNOSIS_Per_Card;

DO_SW_DIAGNOSIS_POINT_ADDRESSES DO_SW_DIAGNOSIS_Per_Channel;




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
        modbus_set_slave(ctx, 07);
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



	memcpy(DO_HPA_Data.HPA_DO_Whole_Channel, DO_FRTU_PART1_REGISTERS_TAB,DO_NO_OF_HAP_WORDBYTE);
	memcpy(&(DO_HPA_Data.HPA_DO_Whole_Channel[DO_FRTU_PART2_REGISTERS_ADDRESS]), DO_FRTU_PART2_REGISTERS_TAB,DO_NO_OF_HAP_WORDBYTE);

	memcpy(&(DO_DIAGNOSIS_Per_Card.DIAGNOSIS_DO_Whole_Card[0]), DO_FRTU_PART3_REGISTERS_TAB,DO_NO_OF_DIAGNOSIS_WORDBYTE*BYTEperWORD);

	memcpy(DO_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DO_Whole_Channel, DO_FRTU_PART4_REGISTERS_TAB,DO_NO_OF_SW_DIAGNOSIS_WORDBYTE*BYTEperWORD);


	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.CASDU1 = 0x01;
	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.CASDU2 = 0x00;
	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.IOA1 = 0x01;
	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.IOA2 = 0x00;
	DO_HPA_Data.HPA_whole.HPA_DO_Per_Channel[0].HPA.IOA3 = 0x00;


    /* Many registers */
    printf("1/4 FRTU_AI_write_registers: ");
    rc = modbus_write_registers(ctx, DO_FRTU_PART1_REGISTERS_ADDRESS,
DO_FRTU_PART1_REGISTERS_NB, &(DO_HPA_Data.HPA_DO_Whole_Channel[0]));
    
    //ASSERT_TRUE(rc == DI_FRTU_PART1_REGISTERS_NB, "");
    
    printf("2/4 FRTU_DI_write_registers: ");
    rc = modbus_write_registers(ctx, DO_FRTU_PART2_REGISTERS_ADDRESS,
                                DO_FRTU_PART2_REGISTERS_NB, &(DO_HPA_Data.HPA_DO_Whole_Channel[DO_NO_OF_HAP_WORDBYTE/2]));




DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.CASDU1 =1;
DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.CASDU2 =0;

DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.IOA1 =1;
DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.IOA2 =0;
DO_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DO_Per_Card[1].DIAGNOSIS_BYTES.IOA3 =0;



    printf("3/4 FRTU_DO_write_registers: ");
    rc = modbus_write_registers(ctx, DO_FRTU_PART3_REGISTERS_ADDRESS,
                                DO_FRTU_PART3_REGISTERS_NB, DO_DIAGNOSIS_Per_Card.DIAGNOSIS_DO_Whole_Card);
    
   // ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");


DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.CASDU1 =1;
DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.CASDU2 =0;

DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA1 =1;
DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA2 =0;
DO_SW_DIAGNOSIS_Per_Channel.Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[1].SW_DIAGNOSIS_BYTES.IOA3 =0;

    printf("4/4 FRTU_DO_write_registers: ");
    rc = modbus_write_registers(ctx, DO_FRTU_PART4_REGISTERS_ADDRESS,
                                DO_FRTU_PART4_REGISTERS_NB, &(DO_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DO_Whole_Channel[0]));
    
   // ASSERT_TRUE(rc == DO_FRTU_PART4_REGISTERS_NB, "");



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


