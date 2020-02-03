#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "modbus_FRTU_DI.h"
#include "modbus_FRTU_AI.h"
#include "modbus_FRTU_DO.h"
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

		 

IOM_DI_CSV_Data  s_DI_DIAGNOSTIC_IOM_DATA,s_DI_CHANNEL_CONFIGURATION_IOM_DATA,S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA;	


IOM_AI_CSV_Data s_AI_DIAGNOSTIC_IOM_DATA,s_AI_CHANNEL_CONFIGURATION_IOM_DATA;

IOM_DO_CSV_Data  s_DO_DIAGNOSTIC_IOM_DATA,s_DO_CHANNEL_CONFIGURATION_IOM_DATA,s_DO_CHANNEL_DIAGNOSTIC_IOM_DATA;		


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

void DI_CSVtoModbus_DiagnosisPerCardFrame(IOM_DI_CSV_Data* csv_data,DI_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t CardNo)
{
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.CASDU1 = csv_data->CASDU1[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.CASDU2 = csv_data->CASDU2[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA1 = csv_data->IOA1[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA2 = csv_data->IOA2[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA3 = csv_data->IOA3[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DI_Per_Card[CardNo].DIAGNOSIS_BYTES.TI = csv_data->TI[CardNo];
}

void AI_CSVtoModbus_DiagnosisPerCardFrame(IOM_AI_CSV_Data* csv_data,AI_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t CardNo)
{
	modbusFrame->Diagnosis_whole.DIAGNOSIS_AI_Per_Card[CardNo].DIAGNOSIS_BYTES.CASDU1 = csv_data->CASDU1[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_AI_Per_Card[CardNo].DIAGNOSIS_BYTES.CASDU2 = csv_data->CASDU2[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_AI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA1 = csv_data->IOA1[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_AI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA2 = csv_data->IOA2[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_AI_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA3 = csv_data->IOA3[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_AI_Per_Card[CardNo].DIAGNOSIS_BYTES.TI = csv_data->TI[CardNo];
}


void DO_CSVtoModbus_DiagnosisPerCardFrame(IOM_DO_CSV_Data* csv_data,DO_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t CardNo)
{
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DO_Per_Card[CardNo].DIAGNOSIS_BYTES.CASDU1 = csv_data->CASDU1[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DO_Per_Card[CardNo].DIAGNOSIS_BYTES.CASDU2 = csv_data->CASDU2[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DO_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA1 = csv_data->IOA1[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DO_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA2 = csv_data->IOA2[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DO_Per_Card[CardNo].DIAGNOSIS_BYTES.IOA3 = csv_data->IOA3[CardNo];
	modbusFrame->Diagnosis_whole.DIAGNOSIS_DO_Per_Card[CardNo].DIAGNOSIS_BYTES.TI = csv_data->TI[CardNo];
}

void DI_CSVtoModbus_HPAPerChannelFrame(IOM_DI_CSV_Data* csv_data,DI_HARDWARE_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo)
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



void AI_CSVtoModbus_HPAPerChannelFrame(IOM_AI_CSV_Data* csv_data,AI_HARDWARE_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo)
{
	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.CASDU1 = csv_data->CASDU1[ChannelNo];
	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.CASDU2 = csv_data->CASDU2[ChannelNo];
	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.IOA1 = csv_data->IOA1[ChannelNo];
	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.IOA2 = csv_data->IOA2[ChannelNo];
	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.IOA3 = csv_data->IOA3[ChannelNo];
	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.TI = csv_data->TI[ChannelNo];

	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.AI_bits.AI_Bits_Byte = 0x00;
	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.AI_bits.bits.Block = csv_data->block[ChannelNo];

	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.AI_bits.bits.Zero = csv_data->Zero_range[ChannelNo];


	floatToModbus(csv_data->thresh_additive[ChannelNo],&(modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.AI_Thresold_Additive));
	
	floatToModbus(csv_data->thresh_uncond[ChannelNo],&(modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.AI_Thresold_Uncond));

	floatToModbus(csv_data->Y_Zero[ChannelNo],&(modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.Y_Zero));
	floatToModbus(csv_data->Y_100[ChannelNo],&(modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.Y_100));
	
	floatToModbus(csv_data->MV_conv_free_Xl[ChannelNo],&(modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.PLAUSIBLE));
	
	floatToModbus(csv_data->zero_range_Xzu[ChannelNo],&(modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.ZERO_RANGE_HIGH));
	
	floatToModbus(csv_data->zero_range_Xzl[ChannelNo],&(modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.ZERO_RANGE_LOW));
	
	floatToModbus(csv_data->zero_range_Yz[ChannelNo],&(modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.ZERO_RANGE_YZ));




//newly Added
	modbusFrame->HPA_whole.HPA_AI_Per_Channel[ChannelNo].HPA.Input_Type = csv_data->Input_Type[ChannelNo];
}




void DO_CSVtoModbus_HPAPerChannelFrame(IOM_DO_CSV_Data* csv_data,DO_HARDWARE_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo)
{
	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.CASDU1 = csv_data->CASDU1[ChannelNo];
	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.CASDU2 = csv_data->CASDU2[ChannelNo];
	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.IOA1 = csv_data->IOA1[ChannelNo];
	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.IOA2 = csv_data->IOA2[ChannelNo];
	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.IOA3 = csv_data->IOA3[ChannelNo];
	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.TI = csv_data->TI[ChannelNo];


	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.select_execute_t  = csv_data->select_execute_t[ChannelNo];


	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.default_on_t = csv_data->Default_on_Time[ChannelNo];

	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.output_t_0 = csv_data->output_t_0[ChannelNo];

	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.output_t_1 = csv_data->output_t_1[ChannelNo];

	modbusFrame->HPA_whole.HPA_DO_Per_Channel[ChannelNo].HPA.DO_bitAllocation.bits.On_before_off = csv_data->ON_before_OFF[ChannelNo];

}



void DI_CSVtoModbus_DiagnosticsPerChannelFrame(IOM_DI_CSV_Data* csv_data,DI_SW_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo)
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


void DO_CSVtoModbus_DiagnosticsPerChannelFrame(IOM_DO_CSV_Data* csv_data,DO_SW_DIAGNOSIS_POINT_ADDRESSES* modbusFrame, uint8_t ChannelNo)
{
	modbusFrame-> Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.CASDU1 = csv_data->CASDU1[ChannelNo];
	modbusFrame->
	Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.CASDU2 = csv_data->CASDU2[ChannelNo];
	modbusFrame->
	Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.IOA1 = csv_data->IOA1[ChannelNo];
	modbusFrame->
	Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.IOA2 = csv_data->IOA2[ChannelNo];
	modbusFrame->
	Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.IOA3 = csv_data->IOA3[ChannelNo];
	modbusFrame->
	Diagnosis_whole.DIAGNOSIS_SW_DO_Per_Channel[ChannelNo].SW_DIAGNOSIS_BYTES.TI = csv_data->TI[ChannelNo];
}



int extract_DI_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOM_DI_CSV_Data *data)
{

	FILE *fp,*fPtr;
	char buf[BUFSIZE];
	char * cmd;
	int count;	
	char *token, *p;
	uint8_t channel_no;
	
	count = 0;

	if(IOM_Type == e_DI_IOM_TYPE)
	{
		if(IOM_Type_of_data == e_DI_DIAGNOSTIC_IOM_DATA)
		{

			string_to_compare = malloc(snprintf(NULL,0,"SW-")+1);
			sprintf(string_to_compare,"SW-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_DI_DIAGNOSTIC_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_DI_DIAGNOSTIC_IOM_DATA+1);				
		}
		else if(IOM_Type_of_data == e_DI_CHANNEL_CONFIGURATION_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_DI_CHANNEL_CONFIGURATION_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_DI_CHANNEL_CONFIGURATION_IOM_DATA+1);
		}
		else if(IOM_Type_of_data == e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA+1);
		}
	}
	else if(IOM_Type == e_AI_IOM_TYPE)
	{
		if(IOM_Type_of_data == e_AI_DIAGNOSTIC_IOM_DATA)
		{

			string_to_compare = malloc(snprintf(NULL,0,"SW-")+1);
			sprintf(string_to_compare,"SW-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_AI_DIAGNOSTIC_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_AI_DIAGNOSTIC_IOM_DATA+1);				
		}
		else if(IOM_Type_of_data == e_AI_CHANNEL_CONFIGURATION_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_AI_CHANNEL_CONFIGURATION_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_AI_CHANNEL_CONFIGURATION_IOM_DATA+1);
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


	if(IOM_Type == e_DI_IOM_TYPE)
	{
		while (fgets(buf, BUFSIZE, fp) != NULL) 
		{

			printf("%s",buf);
			p = buf;
			token = strsep(&p, ",\n");//1st field
			if(token != NULL)	
			{
				if(strcmp(token, "Ch-01") == 0) channel_no = 0;
				else
				if(strcmp(token, "Ch-02") == 0) channel_no = 1;
				else
				if(strcmp(token, "Ch-03") == 0) channel_no = 2;
				else
				if(strcmp(token, "Ch-04") == 0) channel_no = 3;
				else
				if(strcmp(token, "Ch-05") == 0) channel_no = 4;
				else
				if(strcmp(token, "Ch-06") == 0) channel_no = 5;
				else
				if(strcmp(token, "Ch-07") == 0) channel_no = 6;
				else
				if(strcmp(token, "Ch-08") == 0) channel_no = 7;
				else
				if(strcmp(token, "Ch-09") == 0) channel_no = 8;
				else
				if(strcmp(token, "Ch-10") == 0) channel_no = 9;
				else
				if(strcmp(token, "Ch-11") == 0) channel_no = 10;
				else
				if(strcmp(token, "Ch-12") == 0) channel_no = 11;
				else
				if(strcmp(token, "Ch-13") == 0) channel_no = 12;
				else
				if(strcmp(token, "Ch-14") == 0) channel_no = 13;
				else
				if(strcmp(token, "Ch-15") == 0) channel_no = 14;
				else
				if(strcmp(token, "Ch-16") == 0) channel_no = 15;
				
				strcpy(data->Name[channel_no], token);
			}
			
			token = strsep(&p, ",\n");//2nd field
			if(token != NULL)		data->CASDU1[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//3rd field
			if(token != NULL)		data->CASDU2[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//4th field
			if(token != NULL)		data->IOA1[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//5th field
			if(token != NULL)		data->IOA2[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//6th field
			if(token != NULL)		data->IOA3[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//7th field
			if(token != NULL)		data->TI[channel_no] = atoi(token);
			

			if(data->TI[channel_no] == DOUBLE_POINT_DI_TI)
			{
				token = strsep(&p, ",\n");//8th field
				if(token != NULL)	data->int_posit_t[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//9th field
				if(token != NULL)	data->Inversion[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//10th field
				if(token != NULL)	data->Inversion_1[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//11th field
				if(token != NULL)	data->block[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//12th field
				if(token != NULL)	data->faulty_pos_t[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//13th field
				if(token != NULL)	data->ON_before_OFF[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//14th field
				if(token != NULL)	data->Flutt_Number[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//15th field
				if(token != NULL)	data->Flutt_dp_qds[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//16th field
				if(token != NULL)	data->Flutt_t[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//17th field
				if(token != NULL)	data->SW_Filter_t[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//18th field
				if(token != NULL)	strcpy(data->Longtext[channel_no], token);


	#ifdef DEBUG
			//check print
			printf("DEBUG:\nName:'%s',\n CASDU1:'%d',\n CASDU2:'%d',\n IOA1:'%d',\n IOA2:'%d',\n IOA3:'%d',\n TI:'%d',\n int_posit_t:'%d',\n Inversion_0:'%d',\n Inversion_1:'%d',\n block:'%d',\n faulty_pos_t:'%d',\n ON_before_OFF:'%d',\n Flutt_Number:'%d',\n Flutt_dp_qds:'%d',\n Flutt_t:'%d',\n SW_Filter_t:'%d',\n Longtext:'%s',\n count:%d.\n",

	data->Name[channel_no], 
	data->CASDU1[channel_no], 
	data->CASDU2[channel_no], 
	data->IOA1[channel_no], 
	data->IOA2[channel_no], 
	data->IOA3[channel_no], 
	data->TI[channel_no], 
	data->int_posit_t[channel_no], 
	data->Inversion[channel_no], 
	data->Inversion_1[channel_no], 
	data->block[channel_no], 
	data->faulty_pos_t[channel_no], 
	data->ON_before_OFF[channel_no], 
	data->Flutt_Number[channel_no],
	data->Flutt_dp_qds[channel_no],
	data->Flutt_t[channel_no],
	data->SW_Filter_t[channel_no],
	data->Longtext[channel_no],
	count
	);

	#endif

			}
			else if(data->TI[channel_no] == SINGLE_POINT_DI_TI)	
			{	
				token = strsep(&p, ",\n");//8th field
				if(token != NULL)	data->Inversion[channel_no] = atoi(token);
				
				token = strsep(&p, ",\n");//9th field
				if(token != NULL)	data->block[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//10th field
				if(token != NULL)	data->Flutt_Number[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//11th field
				if(token != NULL)	data->Flutt_dp_qds[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//12th field
				if(token != NULL)	data->Flutt_t[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//13th field
				if(token != NULL)	data->SW_Filter_t[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//14th field
					if(token != NULL)	data->State_of_flutter_information[channel_no] = atoi(token);
					token = strsep(&p, ",\n");//15th field
					if(token != NULL)	strcpy(data->Longtext[channel_no], token);


		#ifdef DEBUG
				//check print
				printf("DEBUG:\nName:'%s',\n CASDU1:'%d',\n CASDU2:%d,\n IOA1:'%d',\n IOA2:'%d',\n IOA3:'%d',\n TI:'%d',\n Inversion:'%d',\n block:'%d',\n Flutt_Number:'%d',\n Flutt_dp_qds:'%d',\n Flutt_t:'%d',\n SW_Filter_t:'%d',\n State_of_flutter_information:'%d',\n Longtext:'%s',\n count:%d.\n",
				data->Name[channel_no], data->CASDU1[channel_no], data->CASDU2[channel_no], data->IOA1[channel_no], data->IOA2[channel_no], data->IOA3[channel_no], data->TI[channel_no], data->Inversion[channel_no], data->block[channel_no], data->Flutt_Number[channel_no], data->Flutt_dp_qds[channel_no], data->Flutt_t[channel_no], data->SW_Filter_t[channel_no], data->State_of_flutter_information[channel_no],data->Longtext[channel_no],count);

		#endif
			}


			count++;		
		}
	}


	/* close */
	pclose(fp);

return count;
}


int extract_AI_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOM_AI_CSV_Data *data)
{

	FILE *fp,*fPtr;
	char buf[BUFSIZE];
	char * cmd;
	int count;	
	char *token, *p;
	char * pEnd;
	uint8_t channel_no;
	
	count = 0;


	if(IOM_Type == e_AI_IOM_TYPE)
	{
		if(IOM_Type_of_data == e_AI_DIAGNOSTIC_IOM_DATA)
		{

			string_to_compare = malloc(snprintf(NULL,0,"SW-")+1);
			sprintf(string_to_compare,"SW-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_AI_DIAGNOSTIC_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_AI_DIAGNOSTIC_IOM_DATA+1);				
		}
		else if(IOM_Type_of_data == e_AI_CHANNEL_CONFIGURATION_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_AI_CHANNEL_CONFIGURATION_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_AI_CHANNEL_CONFIGURATION_IOM_DATA+1);
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


	if(IOM_Type == e_AI_IOM_TYPE)
	{
		if(IOM_Type_of_data == e_AI_DIAGNOSTIC_IOM_DATA)
		{
			while (fgets(buf, BUFSIZE, fp) != NULL) 
			{

				printf("%s",buf);
				p = buf;
				token = strsep(&p, ",\n");//1st field
				if(token != NULL)	strcpy(data->Name[count], token);
				token = strsep(&p, ",\n");//2nd field
				if(token != NULL)	data->CASDU1[count] = atoi(token);
				token = strsep(&p, ",\n");//3rd field
				if(token != NULL)	data->CASDU2[count] = atoi(token);
				token = strsep(&p, ",\n");//4th field
				if(token != NULL)	data->IOA1[count] = atoi(token);
				token = strsep(&p, ",\n");//5th field
				if(token != NULL)	data->IOA2[count] = atoi(token);
				token = strsep(&p, ",\n");//6th field
				if(token != NULL)	data->IOA3[count] = atoi(token);
				token = strsep(&p, ",\n");//7th field
				if(token != NULL)	data->TI[count] = atoi(token);
				
				#ifdef DEBUG
				//check print
				printf("DEBUG:\nName:'%s',\n CASDU1:'%d',\n CASDU2:'%d',\n IOA1:'%d',\n IOA2:'%d',\n IOA3:'%d',\n TI:'%d',\n count:%d.\n",

				data->Name[count], 
				data->CASDU1[count], 
				data->CASDU2[count], 
				data->IOA1[count], 
				data->IOA2[count], 
				data->IOA3[count], 
				data->TI[count], 
				count
				);

				#endif
				count++;
			}
		}
		else if(IOM_Type_of_data == e_AI_CHANNEL_CONFIGURATION_IOM_DATA)
		{
			while (fgets(buf, BUFSIZE, fp) != NULL) 
			{

				printf("%s",buf);
				p = buf;
				token = strsep(&p, ",\n");//1st field
				if(token != NULL)	
				{
					if(strcmp(token, "Ch-01") == 0) channel_no = 0;
					else
					if(strcmp(token, "Ch-02") == 0) channel_no = 1;
					else
					if(strcmp(token, "Ch-03") == 0) channel_no = 2;
					else
					if(strcmp(token, "Ch-04") == 0) channel_no = 3;
					else
					if(strcmp(token, "Ch-05") == 0) channel_no = 4;
					else
					if(strcmp(token, "Ch-06") == 0) channel_no = 5;
					else
					if(strcmp(token, "Ch-07") == 0) channel_no = 6;
					else
					if(strcmp(token, "Ch-08") == 0) channel_no = 7;
					
					strcpy(data->Name[channel_no], token);
				}
				token = strsep(&p, ",\n");//2nd field
				if(token != NULL)	data->CASDU1[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//3rd field
				if(token != NULL)	data->CASDU2[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//4th field
				if(token != NULL)	data->IOA1[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//5th field
				if(token != NULL)	data->IOA2[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//6th field
				if(token != NULL)	data->IOA3[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//7th field
				if(token != NULL)	data->TI[channel_no] = atoi(token);
				

				token = strsep(&p, ",\n");//8th field
				if(token != NULL)	data->Zero_range[channel_no] = atoi(token);
				token = strsep(&p, ",\n");//9th field
				if(token != NULL)	
					data->thresh_additive[channel_no] = atof (token);
				token = strsep(&p, ",\n");//10th field
				if(token != NULL)	
					data->thresh_uncond[channel_no] = atof (token);
					
				token = strsep(&p, ",\n");//11th field
				if(token != NULL)	data->block[channel_no] = atoi(token);
				
				token = strsep(&p, ",\n");//12th field
				if(token != NULL)	
					data->Y_Zero[channel_no] = atof (token);
					
				token = strsep(&p, ",\n");//13th field
				if(token != NULL)	
					data->Y_100[channel_no] = atof (token);
					
				token = strsep(&p, ",\n");//14th field
				if(token != NULL)	
					data->MV_conv_free_Xl[channel_no] =  atof (token);
									
				token = strsep(&p, ",\n");//15th field
				if(token != NULL)	
					data->zero_range_Xzu[channel_no] = atof (token);
					
				token = strsep(&p, ",\n");//16th field
				if(token != NULL)	
					data->zero_range_Xzl[channel_no] =  atof (token);;
					
				token = strsep(&p, ",\n");//17th field
				if(token != NULL)	
					data->zero_range_Yz[channel_no] =  atof (token);
					
				token = strsep(&p, ",\n");//18th field
				if(token != NULL)	strcpy(data->Longtext[channel_no], token);
				
				token = strsep(&p, ",\n");//19th field
				if(token != NULL)	data->Input_Type[channel_no] = atoi(token);
		#ifdef DEBUG
				//check print
				printf("DEBUG:\nName:'%s',\n CASDU1:'%d',\n CASDU2:'%d',\n IOA1:'%d',\n IOA2:'%d',\n IOA3:'%d',\n TI:'%d',\n Zero range:'%d',\n thresh_additive:'%f' ,\n thresh_uncond:'%f',\n block:'%d',\n Y_0:'%f',\n Y_100:'%f',\n MV_conv_free_Xl:'%f',\n zero_range_Xzu:'%f',\n zero_range_Xzl:'%f',\n zero_range_Yz:'%f',\n Longtext:'%s',\n Input_Type:'%d',\n channel no:%d.\n",

		data->Name[channel_no], 
		data->CASDU1[channel_no], 
		data->CASDU2[channel_no], 
		data->IOA1[channel_no], 
		data->IOA2[channel_no], 
		data->IOA3[channel_no], 
		data->TI[channel_no], 
		data->Zero_range[channel_no],
		data->thresh_additive[channel_no], 
		data->thresh_uncond[channel_no], 
		data->block[channel_no], 
		data->Y_Zero[channel_no], 
		data->Y_100[channel_no], 
		data->MV_conv_free_Xl[channel_no],
		data->zero_range_Xzu[channel_no],
		data->zero_range_Xzl[channel_no],
		data->zero_range_Yz[channel_no],
		data->Longtext[channel_no],
		data->Input_Type[channel_no],
		channel_no
		);

		#endif


		count++;			
			}
		}
	}
	
	/* close */
	pclose(fp);

	return count;	
}




int extract_DO_Data_From_CSV(char IOM_Type, char IOM_Type_of_data,IOM_DO_CSV_Data *data)
{


	FILE *fp,*fPtr;
	char buf[BUFSIZE];
	char * cmd;
	int count;	
	char *token, *p;
	uint8_t channel_no;
	
	count = 0;

	if(IOM_Type == e_DO_IOM_TYPE)
	{
		if(IOM_Type_of_data == e_DO_DIAGNOSTIC_IOM_DATA)
		{

			string_to_compare = malloc(snprintf(NULL,0,"SW-")+1);
			sprintf(string_to_compare,"SW-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_DO_DIAGNOSTIC_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_DO_DIAGNOSTIC_IOM_DATA+1);				
		}
		else if(IOM_Type_of_data == e_DO_CHANNEL_CONFIGURATION_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_DO_CHANNEL_CONFIGURATION_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_DO_CHANNEL_CONFIGURATION_IOM_DATA+1);
		}
		else if(IOM_Type_of_data == e_DO_CHANNEL_DIAGNOSTIC_IOM_DATA)
		{
			string_to_compare = malloc(snprintf(NULL,0,"Ch-")+1);
			sprintf(string_to_compare,"Ch-");
			
			InputFile = malloc(snprintf(NULL,0,"%s_%02d",InputFile_whole, e_DO_CHANNEL_DIAGNOSTIC_IOM_DATA+1)+1);


			sprintf(InputFile,"%s_%02d",InputFile_whole, e_DO_CHANNEL_DIAGNOSTIC_IOM_DATA+1);
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


	if(IOM_Type == e_DO_IOM_TYPE)
	{

		if(IOM_Type_of_data == e_DO_CHANNEL_CONFIGURATION_IOM_DATA)
		{
		while (fgets(buf, BUFSIZE, fp) != NULL) 
		{

			printf("%s",buf);
			p = buf;
			token = strsep(&p, ",\n");//1st field
			if(token != NULL)	
			{
				if(strcmp(token, "Ch-01") == 0) channel_no = 0;
				else
				if(strcmp(token, "Ch-02") == 0) channel_no = 1;
				else
				if(strcmp(token, "Ch-03") == 0) channel_no = 2;
				else
				if(strcmp(token, "Ch-04") == 0) channel_no = 3;
				else
				if(strcmp(token, "Ch-05") == 0) channel_no = 4;
				else
				if(strcmp(token, "Ch-06") == 0) channel_no = 5;
				else
				if(strcmp(token, "Ch-07") == 0) channel_no = 6;
				else
				if(strcmp(token, "Ch-08") == 0) channel_no = 7;
		
				strcpy(data->Name[channel_no], token);
			}
			token = strsep(&p, ",\n");//2nd field
			if(token != NULL)		data->CASDU1[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//3rd field
			if(token != NULL)		data->CASDU2[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//4th field
			if(token != NULL)		data->IOA1[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//5th field
			if(token != NULL)		data->IOA2[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//6th field
			if(token != NULL)		data->IOA3[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//7th field
			if(token != NULL)		data->TI[channel_no] = atoi(token);
			
			token = strsep(&p, ",\n");//8th field
			if(token != NULL)	data->select_execute_t[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//9th field
			if(token != NULL)	data->output_t_0 [channel_no] = atoi(token);
			token = strsep(&p, ",\n");//10th field
			if(token != NULL)	data->output_t_1[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//11th field
			if(token != NULL)	data->Default_on_Time[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//12th field
			if(token != NULL)	data->ON_before_OFF[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//13th field
			if(token != NULL)	strcpy(data->Longtext[channel_no], token);


	#ifdef DEBUG
			//check print
			printf("DEBUG:\nName:'%s',\n CASDU1:'%d',\n CASDU2:'%d',\n IOA1:'%d',\n IOA2:'%d',\n IOA3:'%d',\n TI:'%d',\n select_execute_t:'%d',\n output_t_0:'%d',\n output_t_1:'%d',\n Default on Time:'%d',\n ON_before_OFF:'%d',\n Longtext:'%s',\n count:%d.\n",

	data->Name[channel_no], 
	data->CASDU1[channel_no], 
	data->CASDU2[channel_no], 
	data->IOA1[channel_no], 
	data->IOA2[channel_no], 
	data->IOA3[channel_no], 
	data->TI[channel_no], 
	data->select_execute_t[channel_no], 
	data->output_t_0[channel_no], 
	data->output_t_1[channel_no], 
	data->Default_on_Time[channel_no], 
	data->ON_before_OFF[channel_no], 
	data->Longtext[channel_no],
	count
	);

	#endif



		count++;		
		}
		}
		else if(IOM_Type_of_data == e_DO_CHANNEL_DIAGNOSTIC_IOM_DATA || IOM_Type_of_data ==  e_DO_DIAGNOSTIC_IOM_DATA)
		{
		while (fgets(buf, BUFSIZE, fp) != NULL) 
		{

			printf("%s",buf);
			p = buf;
			token = strsep(&p, ",\n");//1st field
if(token != NULL)	
			{
				if(strcmp(token, "Ch-01") == 0) channel_no = 0;
				else
				if(strcmp(token, "Ch-02") == 0) channel_no = 1;
				else
				if(strcmp(token, "Ch-03") == 0) channel_no = 2;
				else
				if(strcmp(token, "Ch-04") == 0) channel_no = 3;
				else
				if(strcmp(token, "Ch-05") == 0) channel_no = 4;
				else
				if(strcmp(token, "Ch-06") == 0) channel_no = 5;
				else
				if(strcmp(token, "Ch-07") == 0) channel_no = 6;
				else
				if(strcmp(token, "Ch-08") == 0) channel_no = 7;
		
				strcpy(data->Name[channel_no], token);
			}
			token = strsep(&p, ",\n");//2nd field
			if(token != NULL)		data->CASDU1[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//3rd field
			if(token != NULL)	 	data->CASDU2[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//4th field
			if(token != NULL)		data->IOA1[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//5th field
			if(token != NULL)		data->IOA2[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//6th field
			if(token != NULL)		data->IOA3[channel_no] = atoi(token);
			token = strsep(&p, ",\n");//7th field
			if(token != NULL)		data->TI[channel_no] = atoi(token);
			
				#ifdef DEBUG
				//check print
				printf("DEBUG:\nName:'%s',\n CASDU1:'%d',\n CASDU2:'%d',\n IOA1:'%d',\n IOA2:'%d',\n IOA3:'%d',\n TI:'%d',\n count:%d.\n",

				data->Name[channel_no], 
				data->CASDU1[channel_no], 
				data->CASDU2[channel_no], 
				data->IOA1[channel_no], 
				data->IOA2[channel_no], 
				data->IOA3[channel_no], 
				data->TI[channel_no], 
				count
				);

				#endif
		count++;
		}

		}
	}


	/* close */
	pclose(fp);

return count;
}




