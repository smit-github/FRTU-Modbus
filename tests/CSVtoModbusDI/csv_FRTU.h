/* tests/unit-test.h.  Generated from unit-test.h.in by configure.  */
/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Constants defined by configure.ac */


#define DOUBLE_POINT_DI_TI 31
#define SINGLE_POINT_DI_TI 30

typedef char * string;

extern char InputFile_whole[50];



typedef struct{
	char Name[16][50];
	uint8_t CASDU1[16];
	uint8_t CASDU2[16];
	uint8_t IOA1[16];
	uint8_t IOA2[16];
	uint8_t IOA3[16];
	uint8_t TI[16];
	uint8_t Inversion[16];

	//only if Double Point
	uint8_t Inversion_1[16];	
	uint16_t int_posit_t[16];
	uint16_t faulty_pos_t[16];
	uint8_t ON_before_OFF[16];
	//end

	uint8_t block[16];
	uint8_t Flutt_Number[16];
	uint16_t Flutt_dp_qds[16];
	uint16_t Flutt_t[16];
	uint16_t SW_Filter_t[16];
	uint16_t State_of_flutter_information[16];
	char Longtext[16][50];

	
}
IOMDiagnostics;

extern IOMDiagnostics  s_DI_DIAGNOSTIC_IOM_DATA,s_DI_CHANNEL_CONFIGURATION_IOM_DATA,S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA;	
int extract_Diagnostic_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOMDiagnostics *data);




enum DI_TYPE_OF_DATA
{
	e_DI_DIAGNOSTIC_IOM_DATA,
	e_DI_CHANNEL_CONFIGURATION_IOM_DATA,
	e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA
		
};

enum IOM_TYPE
{
	e_DI_IOM_TYPE,

		
};

void IOM_Single_Seperator(void);
void DI_CSVtoModbus_DiagnosisPerCardFrame(IOMDiagnostics* csv_data,DI_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t CardNo);

void DI_CSVtoModbus_HPAPerChannelFrame(IOMDiagnostics* csv_data,DI_HARDWARE_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo);

void DI_CSVtoModbus_DiagnosticsPerChannelFrame(IOMDiagnostics* csv_data,DI_SW_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo);









