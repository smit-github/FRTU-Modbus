/* tests/unit-test.h.  Generated from unit-test.h.in by configure.  */
/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Constants defined by configure.ac */

typedef struct{
	char Name[16][50];
	unsigned int CASDU1[16];
	unsigned int CASDU2[16];
	unsigned int IOA1[16];
	unsigned int IOA2[16];
	unsigned int IOA3[16];
	unsigned int TI[16];
	unsigned int select_execute_t[16];
	unsigned int output_t_0[16];
	unsigned int output_t_1[16];
	unsigned int Default_on_Time[16];
	unsigned int ON_before_OFF[16];

}
IOMDiagnostics;

extern IOMDiagnostics  s_DI_DIAGNOSTIC_IOM_DATA,s_DI_CHANNEL_CONFIGURATION_IOM_DATA;	
void extract_Diagnostic_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOMDiagnostics *data);


enum DI_TYPE_OF_DATA
{
	e_DI_DIAGNOSTIC_IOM_DATA,
	e_DI_CHANNEL_CONFIGURATION_IOM_DATA,
		
};

enum IOM_TYPE
{
	e_DI_IOM_TYPE,

		
};

void DI_CSVtoModbus_DiagnosisPerCardFrame(IOMDiagnostics* ,DI_DIAGNOSIS_POINT_ADDRESSES*);









