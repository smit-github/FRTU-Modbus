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
#include "modbus_FRTU_AI.h"
#include "modbus_FRTU_DO.h"

#include "csv_FRTU.h"

#include "SeperateFromCSV.h"


#define AI_Status 1
#define DI_Status 2
#define DO_Status 3



const int EXCEPTION_RC = 2;

uint8_t No_of_IOMs;

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

	 strcpy(InputFile_CSV,argv[2]);
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

	
	No_of_IOMs = SeperateCSV();
	printf("No. Of IOMs: %d\n", No_of_IOMs);

	fetchIOMHeaderCSV(&IOM_Header_data);

	Recognize_IOMs(&IOM_Header_data,No_of_IOMs);

	for(i = 0; i< No_of_IOMs; i++)
	{
			
		sprintf(InputFile_whole,"IOM%02d",i+1);
		printf("File Name: %s\n", InputFile_whole);
		IOM_Single_Seperator();

		ExtractAsPerIOMType(IOM_Header_data.File_Identifier[i]);
	}







#ifdef TEST_DO

	default_DO_Modbus_Frame();
	test_Independent_DO();

	NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_CHANNEL_CONFIGURATION_IOM_DATA,&s_DO_CHANNEL_CONFIGURATION_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);
	for(int i = 0; i< NoOfData ; i++)	
	{
		//DO_CSVtoModbus_HPAPerChannelFrame(&s_DO_CHANNEL_CONFIGURATION_IOM_DATA,&DO_HPA_Data,i);
	}


    printf("1/4 FRTU_DO_write_registers: ");
    rc = modbus_write_registers(ctx, DO_FRTU_PART1_REGISTERS_ADDRESS,DO_FRTU_PART1_REGISTERS_NB,
				&(DO_HPA_Data.HPA_DO_Whole_Channel[0]));
    
    //ASSERT_TRUE(rc == DO_FRTU_PART1_REGISTERS_NB, "");
    
    printf("2/4 FRTU_DO_write_registers: ");
    rc = modbus_write_registers(ctx, DO_FRTU_PART2_REGISTERS_ADDRESS,
                                DO_FRTU_PART2_REGISTERS_NB, &(DO_HPA_Data.HPA_DO_Whole_Channel[0+DO_NO_OF_HAP_WORDBYTE/2]));
    
   // ASSERT_TRUE(rc == DO_FRTU_PART2_REGISTERS_NB, "");



	NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_DIAGNOSTIC_IOM_DATA,&s_DO_DIAGNOSTIC_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);

	for(int i = 0; i< NoOfData ; i++)	
	{						DO_CSVtoModbus_DiagnosisPerCardFrame(&s_DO_DIAGNOSTIC_IOM_DATA,&DO_DIAGNOSIS_Per_Card,i);
	}


    printf("3/4 FRTU_DO_write_registers: ");
    rc = modbus_write_registers(ctx, DO_FRTU_PART3_REGISTERS_ADDRESS,
                                DO_FRTU_PART3_REGISTERS_NB, &(DO_DIAGNOSIS_Per_Card.DIAGNOSIS_DO_Whole_Card[0]));
    
   // ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");


	NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_CHANNEL_DIAGNOSTIC_IOM_DATA,&s_DO_CHANNEL_DIAGNOSTIC_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);

	for(int i = 0; i< NoOfData ; i++)	
	{						DO_CSVtoModbus_DiagnosticsPerChannelFrame(&s_DO_CHANNEL_DIAGNOSTIC_IOM_DATA,&DO_SW_DIAGNOSIS_Per_Channel,i);
	}


    printf("4/4 FRTU_DO_write_registers: ");
    rc = modbus_write_registers(ctx, DO_FRTU_PART4_REGISTERS_ADDRESS,
                                DO_FRTU_PART4_REGISTERS_NB, &(DO_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DO_Whole_Channel[0]));
    
   // ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");

#endif






	
#ifdef TEST_AI
	default_AI_Modbus_Frame();

	NoOfData = extract_AI_Data_From_CSV(e_AI_IOM_TYPE,e_AI_CHANNEL_CONFIGURATION_IOM_DATA,&s_AI_CHANNEL_CONFIGURATION_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);
	for(int i = 0; i< NoOfData ; i++)	
	{
		AI_CSVtoModbus_HPAPerChannelFrame(&s_AI_CHANNEL_CONFIGURATION_IOM_DATA,&AI_HPA_Data,i);
	}

	/* Many registers */
	printf("1/3 FRTU AI Channel Wise HAP: ");
	rc = modbus_write_registers(ctx, AI_FRTU_PART1_REGISTERS_ADDRESS,
	AI_FRTU_PART1_REGISTERS_NB, &(AI_HPA_Data.HPA_AI_Whole_Channel[0]));

	//ASSERT_TRUE(rc == DI_FRTU_PART1_REGISTERS_NB, "");

	printf("2/3 FRTU AI Channel Wise HAP: ");
	rc = modbus_write_registers(ctx, AI_FRTU_PART2_REGISTERS_ADDRESS,
	AI_FRTU_PART2_REGISTERS_NB, &(AI_HPA_Data.HPA_AI_Whole_Channel[AI_NO_OF_HAP_BYTES/2]));


	NoOfData = extract_AI_Data_From_CSV(e_AI_IOM_TYPE,e_AI_DIAGNOSTIC_IOM_DATA,&s_AI_DIAGNOSTIC_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);
	for(int i = 0; i< NoOfData ; i++)	
	{
		AI_CSVtoModbus_DiagnosisPerCardFrame(&s_AI_DIAGNOSTIC_IOM_DATA,&DIAGNOSIS_AI_Per_Card,i);
	}

	printf("3/3 FRTU AI Card Wise Diagnostics: ");
	rc = modbus_write_registers(ctx, AI_FRTU_PART3_REGISTERS_ADDRESS,
		AI_NO_OF_DIAGNOSIS_WORDBYTE, DIAGNOSIS_AI_Per_Card.DIAGNOSIS_AI_Whole_Card);

	// ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");
    
#endif

#ifdef TEST_DI

	default_DI_Modbus_Frame();
	test_Independent_DI();

	NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_CONFIGURATION_IOM_DATA,&s_DI_CHANNEL_CONFIGURATION_IOM_DATA);
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





	NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_DIAGNOSTIC_IOM_DATA,&s_DI_DIAGNOSTIC_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);

	for(int i = 0; i< NoOfData ; i++)	
	{						DI_CSVtoModbus_DiagnosisPerCardFrame(&s_DI_DIAGNOSTIC_IOM_DATA,&DI_DIAGNOSIS_Per_Card,i);
	}


	printf("3/4 FRTU_DO_write_registers: ");
	rc = modbus_write_registers(ctx, DI_FRTU_PART3_REGISTERS_ADDRESS,
	DI_NO_OF_DIAGNOSIS_WORDBYTE, DI_DIAGNOSIS_Per_Card.DIAGNOSIS_DI_Whole_Card);

	// ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");


	NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA,&S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA);
	printf("No. Data to Extract : %d\n",NoOfData);

	for(int i = 0; i< NoOfData ; i++)	
	{						DI_CSVtoModbus_DiagnosticsPerChannelFrame(&S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA,&DI_SW_DIAGNOSIS_Per_Channel,i);
	}


	printf("4/4 FRTU_DO_write_registers: ");
	rc = modbus_write_registers(ctx, DI_FRTU_PART4_REGISTERS_ADDRESS,
	DI_NO_OF_SW_DIAGNOSIS_WORDBYTE, &(DI_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DI_Whole_Channel[0]));

	// ASSERT_TRUE(rc == DO_FRTU_PART4_REGISTERS_NB, "");

#endif
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


