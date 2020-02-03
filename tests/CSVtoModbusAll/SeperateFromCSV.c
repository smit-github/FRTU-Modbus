#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>     /* atoi */
#include "SeperateFromCSV.h"
#include "modbus_FRTU.h"
#include "modbus_Array_Defaults.h"
#include "modbus_FRTU_DI.h"
#include "modbus_FRTU_AI.h"
#include "modbus_FRTU_DO.h"

#include "csv_FRTU.h"






#define BUFSIZE 1024

#define DEBUG


#define lengthofAI 18
#define lengthofDI 45
#define lengthofDO 27

#define NO_OF_IOMS 10

#define AI_Status 1
#define DI_Status 2
#define DO_Status 3



IOM_Header_CSV_Data IOM_Header_data;


const string string_IOM_Differentiate[4] = {
	"CPU    ",
	"CPU Diagnostics",
	"IOM "
};



bool IOM_status[11];
int IOM_LineNumber_CSV[11] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};
				 


char InputFile_CSV[50] = "AI_DO_DI";


int SeperateCSV(void)
{ 
	FILE *fp;
	char buf[BUFSIZE];
	char * cmd;
	uint8_t NoOfIOMs;

	NoOfIOMs = 0;


	cmd = malloc(snprintf(NULL,0,"csvgrep -c 1 -m IOM %s.csv -l | sed 1d > IOMs_text.csv",InputFile_CSV)+1);
	sprintf(cmd,"csvgrep -c 1 -m IOM %s.csv -l | sed 1d > IOMs_text.csv",InputFile_CSV);
	if (system(cmd) == 256) 
	{
		printf("Failed to run command\n" );
		exit(1);
	}

	for(int i =0;i<NO_OF_IOMS;i++)
	{
		cmd = malloc(snprintf(NULL,0,"find -type f -name 'IOM%02d.csv' -delete",i+1)+1);
		sprintf(cmd,"find -type f -name 'IOM%02d.csv' -delete",i+1);
#ifdef DEBUG
		printf("%s\n",cmd);
#endif
#ifdef TEST
		fp = popen(cmd, "r");
		if (fp == NULL) {
		printf("Failed to run command\n" );
			exit(1);
		}
		/* close */
		pclose(fp);
#endif
		system(cmd);

		cmd = malloc(snprintf(NULL,0,"grep \"IOM %02d:\" IOMs_text.csv | csvcut -c 1",i+1)+1);
		sprintf(cmd,"grep \"IOM %02d:\" IOMs_text.csv | csvcut -c 1",i+1);
#ifdef DEBUG
		printf("%s\n",cmd);
#endif
		fp = popen(cmd, "r");
		if (fp == NULL) {
		printf("Failed to run command\n" );
			exit(1);
		}
		
		while (fgets(buf, BUFSIZE, fp) != NULL) {
		// Do whatever you want here...
#ifdef DEBUG
			printf("OUPUT: %s", buf);
#endif		
		}

		/* close */
		pclose(fp);
		
		
		if(atoi(buf) != 0) 
		{
			IOM_status[i] = 1;
			IOM_LineNumber_CSV[i] = atoi(buf);
#ifdef DEBUG
			printf("%d\n",IOM_status[i]);
			printf("line numbers: %d\n",IOM_LineNumber_CSV[i]);
#endif
		}
		memset ( buf, 0, BUFSIZE*sizeof (char) );
	}
	

	for(int i =0;i<NO_OF_IOMS;i++)
	{	
		if(IOM_status[i] == 1)
		{

			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" %s.csv > IOM%02d.csv",IOM_LineNumber_CSV[i]+1,IOM_LineNumber_CSV[i+1],InputFile_CSV,i+1)+1);
		sprintf(cmd,"sed -n \"%d,%d p\" %s.csv > IOM%02d.csv",IOM_LineNumber_CSV[i]+1,
IOM_LineNumber_CSV[i+1],InputFile_CSV,i+1);
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

		NoOfIOMs++;
		}
	}

return NoOfIOMs;
}


void fetchIOMHeaderCSV(IOM_Header_CSV_Data* data)
{

	FILE *fp,*fPtr;
	
	uint8_t count = 0;
	char buf[BUFSIZE];
	char * cmd, *Tmp;
	
			
	char *token, *p;
	uint8_t channel_no;
	

#ifdef TESTs
	cmd = malloc(snprintf(NULL,0,"csvgrep -c 2 -m \"%s\" %s.csv  ","IOM","IOMs_text")+1);
	sprintf(cmd,"csvgrep -c 2 -m \"%s\" %s.csv   ","IOM","IOMs_text");
	printf("%s\n",cmd);

	fp = popen(cmd, "r");
#endif

	fp = fopen("IOMs_text.csv", "r");
	
	if (fp == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	
	while (fgets(buf, BUFSIZE, fp) != NULL) 
	{
		printf("%s",buf);
		printf(" count : %d\n",count);
		
		p = buf;

		token = strsep(&p, ",\n");//1st field
		if(token != NULL) data->Line_Number[count] = atoi(token);
		token = strsep(&p, ",\n");//2nd field
		if(token != NULL) 
		{	
			data->IOM_Heading[count] = malloc(strlen(token)+1);
			strcpy(data->IOM_Heading[count], token);			
		}
		token = strsep(&p, ",\n");//3rd field
		if(token != NULL) 
		{
			data->IOM_Description[count] = malloc(strlen(token)+1);
			strcpy(data->IOM_Description[count], token);			
		}
		token = strsep(&p, ",\n");//4th field
		if(token != NULL) data->slaveID[count] = atoi(token);
			
		count++;
	}
	
	
	/* close */
	pclose(fp);	
}


void Recognize_IOMs(IOM_Header_CSV_Data* data, uint8_t IOM_fileNumber)
{
	for(int i = 0; i < IOM_fileNumber;i++)
	{
		if(strstr(data->IOM_Heading[i], "AI-"))
			data->File_Identifier[i] = AI_Status;
		else if(strstr(data->IOM_Heading[i], "DI-"))
			data->File_Identifier[i] = DI_Status;
		else if(strstr(data->IOM_Heading[i], "DO-"))
			data->File_Identifier[i] = DO_Status;

		printf("IOM Description: %d\n", data->File_Identifier[i]);
	}

}



void ExtractAsPerIOMType(uint8_t typeOfIOM)
{

	uint8_t value,NoOfData;
	switch(typeOfIOM)
	{

		case AI_Status:
		{
			NoOfData = extract_AI_Data_From_CSV(e_AI_IOM_TYPE,e_AI_CHANNEL_CONFIGURATION_IOM_DATA,&s_AI_CHANNEL_CONFIGURATION_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);
			for(int i = 0; i< NoOfData ; i++)	
			{
				AI_CSVtoModbus_HPAPerChannelFrame(&s_AI_CHANNEL_CONFIGURATION_IOM_DATA,&AI_HPA_Data,i);
			}

			NoOfData = extract_AI_Data_From_CSV(e_AI_IOM_TYPE,e_AI_DIAGNOSTIC_IOM_DATA,&s_AI_DIAGNOSTIC_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);
			for(int i = 0; i< NoOfData ; i++)	
			{
				AI_CSVtoModbus_DiagnosisPerCardFrame(&s_AI_DIAGNOSTIC_IOM_DATA,&DIAGNOSIS_AI_Per_Card,i);
			}
		}
		break;

		case DI_Status:
		{

			NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_CONFIGURATION_IOM_DATA,&s_DI_CHANNEL_CONFIGURATION_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);
			for(int i = 0; i< NoOfData ; i++)	
			{
			DI_CSVtoModbus_HPAPerChannelFrame(&s_DI_CHANNEL_CONFIGURATION_IOM_DATA,&DI_HPA_Data,i);
			}

			NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_DIAGNOSTIC_IOM_DATA,&s_DI_DIAGNOSTIC_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);

			for(int i = 0; i< NoOfData ; i++)	
			{						DI_CSVtoModbus_DiagnosisPerCardFrame(&s_DI_DIAGNOSTIC_IOM_DATA,&DI_DIAGNOSIS_Per_Card,i);
			}

			NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA,&S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);

			for(int i = 0; i< NoOfData ; i++)	
			{						DI_CSVtoModbus_DiagnosticsPerChannelFrame(&S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA,&DI_SW_DIAGNOSIS_Per_Channel,i);
			}
		}
		break;


		case DO_Status:
		{

		NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_CHANNEL_CONFIGURATION_IOM_DATA,&s_DO_CHANNEL_CONFIGURATION_IOM_DATA);
		printf("No. Data to Extract : %d\n",NoOfData);
		for(int i = 0; i< NoOfData ; i++)	
		{
				DO_CSVtoModbus_HPAPerChannelFrame(&s_DO_CHANNEL_CONFIGURATION_IOM_DATA,&DO_HPA_Data,i);
		}


		NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_DIAGNOSTIC_IOM_DATA,&s_DO_DIAGNOSTIC_IOM_DATA);
		printf("No. Data to Extract : %d\n",NoOfData);

		for(int i = 0; i< NoOfData ; i++)	
		{						DO_CSVtoModbus_DiagnosisPerCardFrame(&s_DO_DIAGNOSTIC_IOM_DATA,&DO_DIAGNOSIS_Per_Card,i);
		}


		NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_CHANNEL_DIAGNOSTIC_IOM_DATA,&s_DO_CHANNEL_DIAGNOSTIC_IOM_DATA);
		printf("No. Data to Extract : %d\n",NoOfData);

		for(int i = 0; i< NoOfData ; i++)	
		{						DO_CSVtoModbus_DiagnosticsPerChannelFrame(&s_DO_CHANNEL_DIAGNOSTIC_IOM_DATA,&DO_SW_DIAGNOSIS_Per_Channel,i);
		}
		}
		break;
		
	}
}



