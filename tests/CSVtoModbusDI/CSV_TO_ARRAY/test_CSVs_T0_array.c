#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modbus_FRTU_DI.h"

#define BUFSIZE 128

#define AI_Status 1
#define DI_Status 2
#define DO_Status 3

#define DEBUG


DI_DIAGNOSIS_POINT_ADDRESSES DI_DIAGNOSIS_Per_Card;


char IOM_status[4] ;
int IOM_LineNumber[10] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};
typedef char * string;

string InputFile = "IOM03_02";

string string_to_compare ="Ch-";

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
						 

IOMDiagnostics  s_DI_DIAGNOSTIC_IOM_DATA,s_DI_CHANNEL_CONFIGURATION_IOM_DATA;	



enum DI_TYPE_OF_DATA
{
	e_DI_DIAGNOSTIC_IOM_DATA,
	e_DI_CHANNEL_CONFIGURATION_IOM_DATA,
		
};

enum IOM_TYPE
{
	e_DI_IOM_TYPE,

		
};
const string string_DI_IOM[4] = {
	"I/O Module Diagnostics",
	"Channelwise Configuration",
	"Channelwise Diagnostics"
};


void extract_Diagnostic_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOMDiagnostics *data);
void DI_CSVtoModbus_DiagnosisPerCardFrame(IOMDiagnostics* csv_data,DI_DIAGNOSIS_POINT_ADDRESSES* modbusFrame);
int main( int argc, char *argv[] )
{

	extract_Diagnostic_Data_From_CSV(e_DI_IOM_TYPE,e_DI_DIAGNOSTIC_IOM_DATA,&s_DI_DIAGNOSTIC_IOM_DATA);

	DI_CSVtoModbus_DiagnosisPerCardFrame(&s_DI_DIAGNOSTIC_IOM_DATA,&DI_DIAGNOSIS_Per_Card);



#ifdef DEBUG
		//check print
		printf("DEBUG:\nCASDU1:'%d', CASDU2: '%d, IOA1:'%d', IOA2:'%d', IOA3:'%d', TI:'%d'\n",
		 DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.CASDU1,
		 DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.CASDU2,
		 DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.IOA1,
		 DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.IOA2,
		 DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.IOA3,
		 DI_DIAGNOSIS_Per_Card.Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.TI);

#endif

	extract_Diagnostic_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_CONFIGURATION_IOM_DATA,&s_DI_CHANNEL_CONFIGURATION_IOM_DATA);
	return 0;
}

void DI_CSVtoModbus_DiagnosisPerCardFrame(IOMDiagnostics* csv_data,DI_DIAGNOSIS_POINT_ADDRESSES* modbusFrame)
{
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.CASDU1 = csv_data->CASDU1[0];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.CASDU2 = csv_data->CASDU2[0];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.IOA1 = csv_data->IOA1[0];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.IOA2 = csv_data->IOA2[0];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.IOA3 = csv_data->IOA3[0];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[0].DIAGNOSIS_BYTES.TI = csv_data->TI[0];
}

void extract_Diagnostic_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOMDiagnostics *data)
{

	FILE *fp,*fPtr;
	char buf[BUFSIZE];
	char * cmd;
	int count;	
	char *token, *p;
	
	count = 0;

	if(IOM_Type == e_DI_IOM_TYPE)
	{
		if(IOM_Type_of_data == e_DI_DIAGNOSTIC_IOM_DATA)
		{

			string_to_compare = malloc(snprintf(NULL,0,"SW-")+1);
			sprintf(string_to_compare,"SW-");
			
			InputFile = malloc(snprintf(NULL,0,"IOM03_01")+1);
			sprintf(InputFile,"IOM03_01");				
		}
		else if(IOM_Type_of_data == e_DI_CHANNEL_CONFIGURATION_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"IOM03_01")+1);
			sprintf(InputFile,"IOM03_02_edit");	
		}
	}


	cmd = malloc(snprintf(NULL,0,"csvgrep -c 1 -m \"%s\" %s.csv  | sed 1d ",string_to_compare,InputFile)+1);
	sprintf(cmd,"csvgrep -c 1 -m \"%s\" %s.csv  | sed 1d ",string_to_compare,InputFile);
	printf("%s",cmd);

	fp = popen(cmd, "r");

	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}


	while (fgets(buf, BUFSIZE, fp) != NULL) 
	{

		printf("%s",buf);
		// Do whatever you want here...
		p = buf;
		token = strsep(&p, ",\n");//1st field
		strcpy(data->Name[count], token);
		token = strsep(&p, ",\n");//2nd field
		data->CASDU1[count] = atoi(token);
		token = strsep(&p, ",\n");//3rd field
		data->CASDU2[count] = atoi(token);
		token = strsep(&p, ",\n");//5th field
		data->IOA1[count] = atoi(token);
		token = strsep(&p, ",\n");//6th field
		data->IOA2[count] = atoi(token);
		token = strsep(&p, ",\n");//7th field
		data->IOA3[count] = atoi(token);
		token = strsep(&p, ",\n");//8th field
		data->TI[count] = atoi(token);
		token = strsep(&p, ",\n");//9th field
		if(token != NULL)
			data->select_execute_t[count] = atoi(token);
		else
			data->select_execute_t[count] = 25;




#ifdef DEBUG
		//check print
		printf("DEBUG:\nName:'%s', CASDU1:'%d', CASDU2:%d, IOA1:'%d', IOA2:'%d', IOA3:'%d', TI:'%d', select_execute_t:'%d', count:%d\n",
		data->Name[count], data->CASDU1[count], data->CASDU2[count], data->IOA1[count], data->IOA2[count], data->IOA3[count], data->TI[count], data->select_execute_t[count],count);

#endif
		count++;		
	}

	/* close */
	pclose(fp);
}




