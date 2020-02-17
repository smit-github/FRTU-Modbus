/* tests/unit-test.h.  Generated from unit-test.h.in by configure.  */
/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Constants defined by configure.ac */


#define DOUBLE_POINT_DI_TI 31
#define SINGLE_POINT_DI_TI 30


#define DOUBLE_POINT_DO_TI 3
#define SINGLE_POINT_DO_TI 1

#define PER_NODE_MAX_CHANNEL 50
#define NODE_PER_MPER 20

#define TOTAL_MODBUS_ADDRESS_PER_MPER PER_NODE_MAX_CHANNEL*NODE_PER_MPER

typedef char * string;

extern char InputFile_whole[50];

void IOM_Single_Seperator(void);


enum CPU_TYPE_OF_DATA
{
	e_CPU_MODULE_GENERAL_DATA,
	e_CPU_MODULE_DIAGNOSTIC_DATA		
};

enum DI_TYPE_OF_DATA
{
	e_DI_DIAGNOSTIC_IOM_DATA,
	e_DI_CHANNEL_CONFIGURATION_IOM_DATA,
	e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA
		
};

enum NODES_PARAMETER_TYPE_OF_DATA
{
	e_NODES_Para_DIAGNOSTIC_IOM_DATA,
	e_NODES_Para_CONFIGURATION_RMValue_DATA,
	e_NODES_Para_CONFIGURATION_RBValue_DATA,
	e_NODES_Para_CONFIGURATION_RBValueWOProcessing_DATA,

		
};

enum MPRES_PARAMETER_TYPE_OF_DATA
{
	e_MPRE_1,
	e_MPRE_2,		
};

enum AI_TYPE_OF_DATA
{
	e_AI_DIAGNOSTIC_IOM_DATA,
	e_AI_CHANNEL_CONFIGURATION_IOM_DATA		
};

enum DO_TYPE_OF_DATA
{
	e_DO_DIAGNOSTIC_IOM_DATA,
	e_DO_CHANNEL_CONFIGURATION_IOM_DATA,
	e_DO_CHANNEL_DIAGNOSTIC_IOM_DATA
		
};

enum IOM_TYPE
{
	e_DI_IOM_TYPE,	//0
	e_AI_IOM_TYPE,	//1
	e_DO_IOM_TYPE	//2

		
};



/*******************DI CSV******************************/

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
IOM_DI_CSV_Data;

extern IOM_DI_CSV_Data  s_DI_DIAGNOSTIC_IOM_DATA,s_DI_CHANNEL_CONFIGURATION_IOM_DATA,S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA;	

int extract_DI_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOM_DI_CSV_Data *data);



void DI_CSVtoModbus_DiagnosisPerCardFrame(IOM_DI_CSV_Data* csv_data,DI_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t CardNo);

void DI_CSVtoModbus_HPAPerChannelFrame(IOM_DI_CSV_Data* csv_data,DI_HARDWARE_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo);

void DI_CSVtoModbus_DiagnosticsPerChannelFrame(IOM_DI_CSV_Data* csv_data,DI_SW_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo);

/*******************AI CSV******************************/



typedef struct{
	char Name[8][50];
	uint8_t CASDU1[8];
	uint8_t CASDU2[8];
	uint8_t IOA1[8];
	uint8_t IOA2[8];
	uint8_t IOA3[8];
	uint8_t TI[8];

	uint8_t Zero_range[8];
	uint8_t block[8];

	float thresh_additive[8];
	float thresh_uncond[8];

	float Y_Zero[8];	
	float Y_100[8];	

	float MV_conv_free_Xl[8];
	float zero_range_Xzu[8];
	float zero_range_Xzl[8];
	float zero_range_Yz[8];

	char Longtext[16][50];

	uint8_t Input_Type[8];		//newly Added
	uint8_t Input_Unit[8][50];	//newly Added
	
}
IOM_AI_CSV_Data;


int extract_AI_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOM_AI_CSV_Data *data);
extern IOM_AI_CSV_Data s_AI_DIAGNOSTIC_IOM_DATA,s_AI_CHANNEL_CONFIGURATION_IOM_DATA;	



void AI_CSVtoModbus_DiagnosisPerCardFrame(IOM_AI_CSV_Data* csv_data,AI_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t CardNo);

void AI_CSVtoModbus_HPAPerChannelFrame(IOM_AI_CSV_Data* csv_data,AI_HARDWARE_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo);


/*******************DO CSV******************************/



typedef struct{
	char Name[8][50];
	uint8_t CASDU1[8];
	uint8_t CASDU2[8];
	uint8_t IOA1[8];
	uint8_t IOA2[8];
	uint8_t IOA3[8];
	uint8_t TI[8];

	uint16_t select_execute_t[8];
 	uint16_t output_t_0 [8];
	uint16_t output_t_1[8];
	uint16_t Default_on_Time[8];
	uint16_t ON_before_OFF[8];

	char Longtext[16][50];
	
}
IOM_DO_CSV_Data;


int extract_DO_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOM_DO_CSV_Data *data);

extern IOM_DO_CSV_Data  s_DO_DIAGNOSTIC_IOM_DATA,s_DO_CHANNEL_CONFIGURATION_IOM_DATA,s_DO_CHANNEL_DIAGNOSTIC_IOM_DATA;	


void DO_CSVtoModbus_DiagnosisPerCardFrame(IOM_DO_CSV_Data* csv_data,DO_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t CardNo);

void DO_CSVtoModbus_HPAPerChannelFrame(IOM_DO_CSV_Data* csv_data,DO_HARDWARE_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo);

void DO_CSVtoModbus_DiagnosticsPerChannelFrame(IOM_DO_CSV_Data* csv_data,DO_SW_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo);

/*******************CPU DIAGNOSTIC CSV******************************/



typedef struct{
	char Name[8][50];
	uint8_t CASDU1[8];
	uint8_t CASDU2[8];
	uint8_t IOA1[8];
	uint8_t IOA2[8];
	uint8_t IOA3[8];
	uint8_t TI[8];
}
CPU_DIAGNOSTIC_CSV_Data;


int extract_CPU_DIAG_Data_From_CSV(CPU_DIAGNOSTIC_CSV_Data *data);

extern CPU_DIAGNOSTIC_CSV_Data s_CPU_DIAGNOSTIC_DATA;


/*******************CPU MODULE CSV******************************/



typedef struct{
	char* Own_IP_Address;
	char* Subnet_Mask;
	char* Default_Gateway;
	char* SNMP_IP_Address;
	char* NTP_Server_IP_Address;
	uint8_t Sync_with_Time_in_minute;
	uint8_t T0;
	uint8_t T1;
	uint8_t T2;
	uint8_t T3;
	uint8_t k;
	uint8_t w;
	char* Remote_Station_1_IP_Address;
	char* Remote_Station_2_IP_Address;
	char* SNMP_Read_Community;
	char* SNMP_Write_Community;
	char* HTTP_Web_Server_User_Name;
	char* HTTP_Web_Server_Password;
	uint8_t CASDU1;
	uint8_t CASDU2;
}
CPU_MODULE_CSV_Data;


int extract_CPU_MODULE_Data_From_CSV(CPU_MODULE_CSV_Data *data);

extern CPU_MODULE_CSV_Data s_CPU_MODULE_CSV_Data;



/*******************Nodes Parameters CSV******************************/

typedef struct{
	char Name[PER_NODE_MAX_CHANNEL][50];
	uint8_t CASDU1[PER_NODE_MAX_CHANNEL];
	uint8_t CASDU2[PER_NODE_MAX_CHANNEL];
	uint8_t Function_Code[PER_NODE_MAX_CHANNEL];
	uint8_t IOA1[PER_NODE_MAX_CHANNEL];
	uint8_t IOA2[PER_NODE_MAX_CHANNEL];
	uint8_t IOA3[PER_NODE_MAX_CHANNEL];
	uint16_t Modbus_Address[PER_NODE_MAX_CHANNEL];
	uint8_t Thresh_Additive[PER_NODE_MAX_CHANNEL];
	uint8_t Thresh_Uncond[PER_NODE_MAX_CHANNEL];
	uint8_t Station_Number[PER_NODE_MAX_CHANNEL];
	uint8_t TI[PER_NODE_MAX_CHANNEL];

	float X_Zero[PER_NODE_MAX_CHANNEL];	
	float X_100[PER_NODE_MAX_CHANNEL];	

	float Y_Zero[PER_NODE_MAX_CHANNEL];	
	float Y_100[PER_NODE_MAX_CHANNEL];	
	char Description[PER_NODE_MAX_CHANNEL][50];
	
}
IOM_Nodes_Para_CSV_Data;

extern IOM_Nodes_Para_CSV_Data  s_Nodes_Para_DIAGNOSTIC_IOM_DATA[2],s_Nodes_Para_CHANNEL_CONFIGURATION_IOM_DATA[2];	


int extract_Nodes_Parameters_Data_From_CSV(char IOM_Type_of_data,IOM_Nodes_Para_CSV_Data *data,char *FileName);
//int extract_DI_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOM_DI_CSV_Data *data);


uint8_t GetChannelNumber(char* Channel_number_string);




