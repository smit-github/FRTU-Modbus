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

#include "modbus_FRTU_AI.h"
"

#define BYTEperWORD 2
AI_HARDWARE_POINT_ADDRESSES AI_HPA_Data;
AI_DIAGNOSIS_POINT_ADDRESSES AI_DIAGNOSIS_Per_Card;

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

void floatToModbus(float FloatingValue,AI_Long_Byte_Allocation *AI_Long);

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


	//memcpy(AI_HPA_Data.HPA_AI_Whole_Channel, AI_FRTU_PART1_REGISTERS_TAB,160);

	memcpy(AI_HPA_Data.HPA_AI_Whole_Channel, AI_FRTU_PART1_REGISTERS_TAB,AI_NO_OF_HAP_BYTES);
	memcpy(&(AI_HPA_Data.HPA_AI_Whole_Channel[AI_FRTU_PART2_REGISTERS_ADDRESS]), AI_FRTU_PART2_REGISTERS_TAB,AI_NO_OF_HAP_BYTES);

	memcpy(&(AI_DIAGNOSIS_Per_Card.DIAGNOSIS_AI_Whole_Card[0]), AI_FRTU_PART3_REGISTERS_TAB,AI_NO_OF_DIAGNOSIS_WORDBYTE*BYTEperWORD);

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

	


    /* Many registers */
    printf("1/3 FRTU AI Channel Wise HAP: ");
    rc = modbus_write_registers(ctx, AI_FRTU_PART1_REGISTERS_ADDRESS,
AI_FRTU_PART1_REGISTERS_NB, &(AI_HPA_Data.HPA_AI_Whole_Channel[0]));
    
    //ASSERT_TRUE(rc == DI_FRTU_PART1_REGISTERS_NB, "");
    
    printf("2/3 FRTU AI Channel Wise HAP: ");
    rc = modbus_write_registers(ctx, AI_FRTU_PART2_REGISTERS_ADDRESS,
                                AI_FRTU_PART2_REGISTERS_NB, &(AI_HPA_Data.HPA_AI_Whole_Channel[AI_NO_OF_HAP_BYTES/2]));
    
   // ASSERT_TRUE(rc == DI_FRTU_PART2_REGISTERS_NB, "");


/*

AI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_AI_Per_Card[1].DIAGNOSIS_BYTES.CASDU1 =1;
AI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_AI_Per_Card[1].DIAGNOSIS_BYTES.CASDU2 =0;
AI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_AI_Per_Card[1].DIAGNOSIS_BYTES.IOA1 =1;
AI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_AI_Per_Card[1].DIAGNOSIS_BYTES.IOA2 =0;
AI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_AI_Per_Card[1].DIAGNOSIS_BYTES.IOA3 =0;

*/

    printf("3/3 FRTU AI Card Wise Diagnostics: ");
    rc = modbus_write_registers(ctx, AI_FRTU_PART3_REGISTERS_ADDRESS,
                                AI_NO_OF_DIAGNOSIS_WORDBYTE, AI_DIAGNOSIS_Per_Card.DIAGNOSIS_AI_Whole_Card);
    
   // ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");

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




void floatToModbus(float FloatingValue,AI_Long_Byte_Allocation *AI_Long)
{
	AI_Float_Byte_Allocation AI_Float;

	AI_Float.AI_Float_Byte = FloatingValue;
	 floatToLong_Swaped_Float( AI_Long,&AI_Float);
	


}
