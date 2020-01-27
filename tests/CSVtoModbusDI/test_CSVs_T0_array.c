#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "modbus_FRTU_DI.h"
#include "csv_FRTU.h"



char InputFile_whole[50] = "IOM02";

string InputFile;

#define BUFSIZE 128

#define AI_Status 1
#define DI_Status 2
#define DO_Status 3

#define DEBUG


char IOM_status[4] ;
int IOM_LineNumber[10] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};




string string_to_compare ="Ch-";



						 

IOMDiagnostics  s_DI_DIAGNOSTIC_IOM_DATA,s_DI_CHANNEL_CONFIGURATION_IOM_DATA,S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA;	

const string string_DI_IOM[4] = {
	"I/O Module Diagnostics",
	"Channelwise Configuration",
	"Channelwise Diagnostics"
};

void IOM_Single_Seperator(void)
{

	FILE *fp,*fPtr;
	char buf[BUFSIZE];
	char * cmd;
	int 	i;	


	cmd = malloc(snprintf(NULL,0,"find -type f -name 'IOM0_text.csv' -delete")+1);
	sprintf(cmd,"find -type f -name 'IOM0_text.csv' -delete");

	if (system(cmd) == 256) 
	{
		printf("Failed to run command\n" );
		exit(1);
	}

	for(int i =0;i<4;i++)
	{
		cmd = malloc(snprintf(NULL,0,"csvgrep -c 1 -m \"%s\" %s.csv -l | sed 1d ",string_DI_IOM[i],InputFile_whole)+1);


		sprintf(cmd,"csvgrep -c 1 -m \"%s\" %s.csv -l | sed 1d ",string_DI_IOM[i],InputFile_whole);

#ifdef TEST
		if (system(cmd) == 256) 
		{
			printf("Failed to run command\n" );
			exit(1);
		}
#endif


		fPtr = fopen("IOM0_text.csv", "a+");

		fp = popen(cmd, "r");
		
		if (fp == NULL) {
			printf("Failed to run command\n" );
			exit(1);
		}
		if (fPtr == NULL) {
			printf("Failed to run command\n" );
			exit(1);
		}

		while (fgets(buf, BUFSIZE, fp) != NULL) {
			
			// Do whatever you want here...
#ifdef DEBUG
			printf("OUPUT: %s", buf);
#endif
			fputs(buf, fPtr);		
					
		}

		/* close */
		fclose(fPtr);
		pclose(fp);

		if(atoi(buf) != 0) 
		{
			IOM_status[i] = 1;
			IOM_LineNumber[i] = atoi(buf);
#ifdef DEBUG
			printf("%d\n",IOM_status[i]);
			printf("line numbers: %d\n",IOM_LineNumber[i]);
#endif

		}
		memset ( buf, 0, BUFSIZE*sizeof (char) );
		
	}

	for(int i =0;i<4;i++)
	{	
		if(IOM_status[i] == 1)
		{


			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" %s.csv > %s_%02d.csv",IOM_LineNumber[i]+1,IOM_LineNumber[i+1],InputFile_whole,InputFile_whole,i+1)+1);
		sprintf(cmd,"sed -n \"%d,%d p\" %s.csv > %s_%02d.csv",IOM_LineNumber[i]+1,
IOM_LineNumber[i+1],InputFile_whole,InputFile_whole,i+1);
#ifdef DEBUG
			printf("%s\n",cmd);
#endif

			if (system(cmd) == 256) 
			{
				printf("Failed to run command\n" );
				exit(1);
			}
			else
			{
			 	IOM_status[i] = 0;
			}
		}
	}

}

void DI_CSVtoModbus_DiagnosisPerCardFrame(IOMDiagnostics* csv_data,DI_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t CardNo)
{
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.CASDU1 = csv_data->CASDU1[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.CASDU2 = csv_data->CASDU2[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA1 = csv_data->IOA1[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA2 = csv_data->IOA2[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA3 = csv_data->IOA3[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.TI = csv_data->TI[CardNo];
}


void DI_CSVtoModbus_HPAPerChannelFrame(IOMDiagnostics* csv_data,DI_HARDWARE_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo)
{
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.CASDU1 = csv_data->CASDU1[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.CASDU2 = csv_data->CASDU2[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.IOA1 = csv_data->IOA1[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.IOA2 = csv_data->IOA2[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.IOA3 = csv_data->IOA3[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.TI = csv_data->TI[ChannelNo];

if(csv_data->TI[ChannelNo] == SINGLE_POINT_DI_TI)
{
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.DI_bitAllocation.DI_Bits = csv_data->block[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.DI_bitAllocation.DI_Bits |= csv_data->Inversion[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.DI_bitAllocation.DI_Bits |= csv_data->Flutt_dp_qds[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.DI_bitAllocation.DI_Bits |= csv_data->State_of_flutter_information[ChannelNo];


	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.Flutter_Number = csv_data->Flutt_Number[ChannelNo];


	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.Flutter_time = csv_data->Flutt_t[ChannelNo];

	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.Software_Debouce_time = csv_data->SW_Filter_t[ChannelNo];


}
else if(csv_data->TI[ChannelNo] == DOUBLE_POINT_DI_TI)
{

	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.Intermeditate_Position_time = csv_data->int_posit_t[ChannelNo];

	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.DI_bitAllocation.DI_Bits = csv_data->block[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.DI_bitAllocation.DI_Bits |= csv_data->Inversion[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.DI_bitAllocation.DI_Bits |= csv_data->Inversion_1[ChannelNo];
	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.DI_bitAllocation.DI_Bits |= csv_data->Flutt_dp_qds[ChannelNo];

	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.Faulty_Position_time = csv_data->faulty_pos_t[ChannelNo];

	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.ON_Before_Time
 = csv_data->ON_before_OFF[ChannelNo];


	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.Flutter_Number = csv_data->Flutt_Number[ChannelNo];


	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.Flutter_time = csv_data->Flutt_t[ChannelNo];


	modbusFrame->HPA_whole.HPA_DI_Per_Channel[ChannelNo].HPA.Software_Debouce_time = csv_data->SW_Filter_t[ChannelNo];
}




}

void DI_CSVtoModbus_DiagnosticsPerChannelFrame(IOMDiagnostics* csv_data,DI_SW_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo)
{
	modbusFrame-> Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.CASDU1 = csv_data->CASDU1[ChannelNo];
modbusFrame->
Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.CASDU2 = csv_data->CASDU2[ChannelNo];
modbusFrame->
Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.IOA1 = csv_data->IOA1[ChannelNo];
modbusFrame->
Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.IOA2 = csv_data->IOA2[ChannelNo];
modbusFrame->
Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.IOA3 = csv_data->IOA3[ChannelNo];
modbusFrame->
Diagnosis_whole.DIAGNOSIS_SW_DI_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.TI = csv_data->TI[ChannelNo];

}


int extract_Diagnostic_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOMDiagnostics *data)
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
			
			InputFile = malloc(snprintf(NULL,0,"IOM02_01")+1);
			sprintf(InputFile,"IOM02_01");				
		}
		else if(IOM_Type_of_data == e_DI_CHANNEL_CONFIGURATION_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"IOM02_02")+1);
			sprintf(InputFile,"IOM02_02");	
		}
		else if(IOM_Type_of_data == e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"IOM02_03")+1);
			sprintf(InputFile,"IOM02_03");	
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
		p = buf;
		token = strsep(&p, ",\n");//1st field
		if(token != NULL)		strcpy(data->Name[count], token);
		token = strsep(&p, ",\n");//2nd field
		if(token != NULL)		data->CASDU1[count] = atoi(token);
		token = strsep(&p, ",\n");//3rd field
		if(token != NULL)		data->CASDU2[count] = atoi(token);
		token = strsep(&p, ",\n");//4th field
		if(token != NULL)		data->IOA1[count] = atoi(token);
		token = strsep(&p, ",\n");//5th field
		if(token != NULL)		data->IOA2[count] = atoi(token);
		token = strsep(&p, ",\n");//6th field
		if(token != NULL)		data->IOA3[count] = atoi(token);
		token = strsep(&p, ",\n");//7th field
		if(token != NULL)		data->TI[count] = atoi(token);
		

		if(data->TI[count] == DOUBLE_POINT_DI_TI)
		{
			token = strsep(&p, ",\n");//8th field
			if(token != NULL)	data->int_posit_t[count] = atoi(token);
			token = strsep(&p, ",\n");//9th field
			if(token != NULL)	data->Inversion[count] = atoi(token);
			token = strsep(&p, ",\n");//10th field
			if(token != NULL)	data->Inversion_1[count] = atoi(token);
			token = strsep(&p, ",\n");//11th field
			if(token != NULL)	data->block[count] = atoi(token);
			token = strsep(&p, ",\n");//12th field
			if(token != NULL)	data->faulty_pos_t[count] = atoi(token);
			token = strsep(&p, ",\n");//13th field
			if(token != NULL)	data->ON_before_OFF[count] = atoi(token);
			token = strsep(&p, ",\n");//14th field
			if(token != NULL)	data->Flutt_Number[count] = atoi(token);
			token = strsep(&p, ",\n");//15th field
			if(token != NULL)	data->Flutt_dp_qds[count] = atoi(token);
			token = strsep(&p, ",\n");//16th field
			if(token != NULL)	data->Flutt_t[count] = atoi(token);
			token = strsep(&p, ",\n");//17th field
			if(token != NULL)	data->SW_Filter_t[count] = atoi(token);
			token = strsep(&p, ",\n");//18th field
			if(token != NULL)	strcpy(data->Longtext[count], token);


#ifdef DEBUG
		//check print
		printf("DEBUG:\nName:'%s',\n CASDU1:'%d',\n CASDU2:'%d',\n IOA1:'%d',\n IOA2:'%d',\n IOA3:'%d',\n TI:'%d',\n int_posit_t:'%d',\n Inversion_0:'%d',\n Inversion_1:'%d',\n block:'%d',\n faulty_pos_t:'%d',\n ON_before_OFF:'%d',\n Flutt_Number:'%d',\n Flutt_dp_qds:'%d',\n Flutt_t:'%d',\n SW_Filter_t:'%d',\n Longtext:'%s',\n count:%d.\n",

data->Name[count], 
data->CASDU1[count], 
data->CASDU2[count], 
data->IOA1[count], 
data->IOA2[count], 
data->IOA3[count], 
data->TI[count], 
data->int_posit_t[count], 
data->Inversion[count], 
data->Inversion_1[count], 
data->block[count], 
data->faulty_pos_t[count], 
data->ON_before_OFF[count], 
data->Flutt_Number[count],
data->Flutt_dp_qds[count],
data->Flutt_t[count],
data->SW_Filter_t[count],
data->Longtext[count],
count
);

#endif

		}
		else if(data->TI[count] == SINGLE_POINT_DI_TI)	
		{	
			token = strsep(&p, ",\n");//8th field
			if(token != NULL)	data->Inversion[count] = atoi(token);
			
			token = strsep(&p, ",\n");//9th field
			if(token != NULL)	data->block[count] = atoi(token);
			token = strsep(&p, ",\n");//10th field
			if(token != NULL)	data->Flutt_Number[count] = atoi(token);
			token = strsep(&p, ",\n");//11th field
			if(token != NULL)	data->Flutt_dp_qds[count] = atoi(token);
			token = strsep(&p, ",\n");//12th field
			if(token != NULL)	data->Flutt_t[count] = atoi(token);
			token = strsep(&p, ",\n");//13th field
			if(token != NULL)	data->SW_Filter_t[count] = atoi(token);
			token = strsep(&p, ",\n");//14th field
			if(token != NULL)	data->State_of_flutter_information[count] = atoi(token);
			token = strsep(&p, ",\n");//15th field
			if(token != NULL)	strcpy(data->Longtext[count], token);


#ifdef DEBUG
		//check print
		printf("DEBUG:\nName:'%s',\n CASDU1:'%d',\n CASDU2:%d,\n IOA1:'%d',\n IOA2:'%d',\n IOA3:'%d',\n TI:'%d',\n Inversion:'%d',\n block:'%d',\n Flutt_Number:'%d',\n Flutt_dp_qds:'%d',\n Flutt_t:'%d',\n SW_Filter_t:'%d',\n State_of_flutter_information:'%d',\n Longtext:'%s',\n count:%d.\n",
		data->Name[count], data->CASDU1[count], data->CASDU2[count], data->IOA1[count], data->IOA2[count], data->IOA3[count], data->TI[count], data->Inversion[count], data->block[count], data->Flutt_Number[count], data->Flutt_dp_qds[count], data->Flutt_t[count], data->SW_Filter_t[count], data->State_of_flutter_information[count],data->Longtext[count],count);

#endif
		}


		count++;		
	}

	/* close */
	pclose(fp);

return count;
}




