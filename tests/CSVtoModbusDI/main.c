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

#include "modbus_FRTU.h"
#include "modbus_Array_Defaults.h"
#include "modbus_FRTU_DI.h"
#include "csv_FRTU.h"


const int EXCEPTION_RC = 2;

enum {
    TCP,
    TCP_PI,
    RTU
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
    uint8_t value,NoOfData;
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


//#ifdef TEST
    if (argc == 2) {
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
    } 
    else if (argc == 3) {

	 strcpy(InputFile_whole,argv[2]);
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
    }else {
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
//#endif
    /** HOLDING REGISTERS **/



	IOM_Single_Seperator();
	default_DI_Modbus_Frame();
	test_Independent_DI();
	



	NoOfData = extract_Diagnostic_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_CONFIGURATION_IOM_DATA,&s_DI_CHANNEL_CONFIGURATION_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);
	for(int i = 0; i< NoOfData ; i++)	
	{
	DI_CSVtoModbus_HPAPerChannelFrame(&s_DI_CHANNEL_CONFIGURATION_IOM_DATA,&DI_HPA_Data,i);
	}


    printf("1/4 FRTU_DI_write_registers: ");
    rc = modbus_write_registers(ctx, DI_FRTU_PART1_REGISTERS_ADDRESS,
DI_FRTU_PART1_REGISTERS_NB, &(DI_HPA_Data.HPA_DI_Whole_Channel[0])/*DI_FRTU_PART1_REGISTERS_TAB*/);
    
    //ASSERT_TRUE(rc == DI_FRTU_PART1_REGISTERS_NB, "");
    
    printf("2/4 FRTU_DI_write_registers: ");
    rc = modbus_write_registers(ctx, DI_FRTU_PART2_REGISTERS_ADDRESS,
                                DI_FRTU_PART2_REGISTERS_NB, &(DI_HPA_Data.HPA_DI_Whole_Channel[0+DI_NO_OF_HAP_WORDBYTE/2]));
    
   // ASSERT_TRUE(rc == DI_FRTU_PART2_REGISTERS_NB, "");





	NoOfData = extract_Diagnostic_Data_From_CSV(e_DI_IOM_TYPE,e_DI_DIAGNOSTIC_IOM_DATA,&s_DI_DIAGNOSTIC_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);

	for(int i = 0; i< NoOfData ; i++)	
	{						DI_CSVtoModbus_DiagnosisPerCardFrame(&s_DI_DIAGNOSTIC_IOM_DATA,&DI_DIAGNOSIS_Per_Card,i);
	}


	printf("3/4 FRTU_DO_write_registers: ");
	rc = modbus_write_registers(ctx, DI_FRTU_PART3_REGISTERS_ADDRESS,
	DI_NO_OF_DIAGNOSIS_WORDBYTE, DI_DIAGNOSIS_Per_Card.DIAGNOSIS_DI_Whole_Card);

	// ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");


	NoOfData = extract_Diagnostic_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA,&S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);

	for(int i = 0; i< NoOfData ; i++)	
	{						DI_CSVtoModbus_DiagnosticsPerChannelFrame(&S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA,&DI_SW_DIAGNOSIS_Per_Channel,i);
	}


	printf("4/4 FRTU_DO_write_registers: ");
	rc = modbus_write_registers(ctx, DI_FRTU_PART4_REGISTERS_ADDRESS,
	DI_NO_OF_SW_DIAGNOSIS_WORDBYTE, &(DI_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DI_Whole_Channel[0]));

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


