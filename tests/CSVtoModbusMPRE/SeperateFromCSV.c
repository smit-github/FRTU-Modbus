#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>    
#include <errno.h>

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
#define NO_OF_MPRE 2
#define NO_OF_NODES 20
#define NO_OF_Ch_PER_NODE 50
#define NO_OF_CPU 2

#define AI_Status 1
#define DI_Status 2
#define DO_Status 3



IOM_Header_CSV_Data IOM_Header_data;
CPU_Header_CSV_Data CPU_Header_data;
MPRE_Header_CSV_Data MPRE_Header_data;
MPRE_Nodes_Header_CSV_Data MPRE_Nodes_Header_data;

QuantityOfCSVData DataAvailablity_CPU;
const string string_IOM_Differentiate[6] = {
	"CPU",
	"M:PRE 1",
	"M:PRE 2",
	"IOM "
};

const string Node_Strings[21] =
{
	"Node-01",
	"Node-02",
	"Node-03",
	"Node-04",
	"Node-05",
	"Node-06",
	"Node-07",
	"Node-08",
	"Node-09",
	"Node-10",
	"Node-11",
	"Node-12",
	"Node-13",
	"Node-14",
	"Node-15",
	"Node-16",
	"Node-17",
	"Node-18",
	"Node-19",
	"Node-20"

};


const string string_MPER_Nodes[4] = {
	"Node Diagnostics",
	"Receive Measured Value",
	"Receive Binary Value",	
	"Receive Measured Value Without Processing"
};


bool IOM_status[11];
int IOM_LineNumber_CSV[14] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};
				 


char InputFile_CSV[50] = "AI_DO_DI";


int SeperateCSV(void)
{ 
	FILE *fp;
	char buf[BUFSIZE];
	char * cmd;
	uint8_t NoOfIOMs;

	NoOfIOMs = 0;







	cmd = malloc(snprintf(NULL,0,"csvgrep -c 1 -m %s %s.csv -l | sed 1d > IOMs_text.csv",string_IOM_Differentiate[2],InputFile_CSV)+1);
	sprintf(cmd,"csvgrep -c 1 -m %s %s.csv -l | sed 1d > IOMs_text.csv",string_IOM_Differentiate[2],InputFile_CSV);
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
#ifdef TEST
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
				IOM_LineNumber_CSV[i] = 0xFFFF;
			}

			NoOfIOMs++;
		}
	}

return NoOfIOMs;
}


void fetchCPUHeaderCSVandSeperate(CPU_Header_CSV_Data* data,IOM_Header_CSV_Data* IOM_data,MPRE_Header_CSV_Data* MPREdata, QuantityOfCSVData*  NoOfData)
{

	FILE *fp,*fPtr;
	
	uint8_t count = 0;
	char buf[BUFSIZE];
	char * cmd, *Tmp;			
	char *token, *p;
	uint8_t channel_no,NoOfIOMs = 0;

	NoOfData->NoOfCPUs = 0;
	NoOfData->CPUAvailable = 0;
	NoOfData->NoOfIOMs = 0;
	NoOfData->IOMAvailable = 0;
	NoOfData->NoOfCPUs = 0;
	NoOfData->CPUAvailable = 0;
	NoOfData->NoOfMPREs = 0;
	NoOfData->MPREAvailable[0] = 0;
	NoOfData->MPREAvailable[1] = 0;
	NoOfData->NoOfNodesForMPRE[0] = 0;
	NoOfData->NoOfNodesForMPRE_Available[0]  = 0;
	NoOfData->NoOfNodesForMPRE[1]  = 0;
	NoOfData->NoOfNodesForMPRE_Available[1]  = 0;

	cmd = malloc(snprintf(NULL,0," grep -n -e \"IOM\" -e \"CPU\" -e \"M:PRE\" %s.csv  > Header_text.csv",InputFile_CSV)+1);
	sprintf(cmd,"grep -n -e \"IOM\" -e \"CPU\" -e \"M:PRE\" %s.csv  > Header_text.csv",InputFile_CSV);
	if (system(cmd) == 256) 
	{
		printf("Failed to run command\n" );
		exit(1);
	}

	fp = fopen("Header_text.csv", "r");
	
	if (fp == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}



	for(int i =0;i<NO_OF_CPU;i++)
	{
		cmd = malloc(snprintf(NULL,0,"find -type f -name 'CPU%02d.csv' -delete",i+1)+1);
		sprintf(cmd,"find -type f -name 'CPU%02d.csv' -delete",i+1);
#ifdef TEST
		printf("%s\n",cmd);
#endif

		system(cmd);
	}
	for(int i =0;i<NO_OF_MPRE;i++)
	{
		cmd = malloc(snprintf(NULL,0,"find -type f -name 'MPRE%02d.csv' -delete",i+1)+1);
		sprintf(cmd,"find -type f -name 'MPRE%02d.csv' -delete",i+1);
#ifdef TEST
		printf("%s\n",cmd);
#endif

		system(cmd);
		for(int j =0;j<NO_OF_NODES;j++)
		{
			cmd = malloc(snprintf(NULL,0,"find -type f -name 'MPRE%02d_Nodes_%02d.csv' -delete",i+1,j+1)+1);
			sprintf(cmd,"find -type f -name 'MPRE%02d_Nodes_%02d.csv' -delete",i+1,j+1);
			
	#ifdef TEST
			printf("%s\n",cmd);
	#endif
			system(cmd);

			for(int k =0;k<4;k++)
			{
				cmd = malloc(snprintf(NULL,0,"find -type f -name 'MPRE%02d_Nodes_%02d_%02d.csv' -delete",i+1,j+1,k+1)+1);
				sprintf(cmd,"find -type f -name 'MPRE%02d_Nodes_%02d_%02d.csv' -delete",i+1,j+1,k+1);
				
		#ifdef TEST
				printf("%s\n",cmd);
		#endif
				system(cmd);
			}
		}

	
	}
	for(int i =0;i<NO_OF_IOMS;i++)
	{
		cmd = malloc(snprintf(NULL,0,"find -type f -name 'IOM%02d.csv' -delete",i+1)+1);
		sprintf(cmd,"find -type f -name 'IOM%02d.csv' -delete",i+1);
#ifdef DEBUG
		printf("%s\n",cmd);
#endif

		system(cmd);
		for(int j =0;j<3;j++)
		{
			cmd = malloc(snprintf(NULL,0,"find -type f -name 'IOM%02d_%02d.csv' -delete",i+1,j+1)+1);
			sprintf(cmd,"find -type f -name 'IOM%02d_%02d.csv' -delete",i+1,j+1);
	#ifdef TEST
			printf("%s\n",cmd);
	#endif
			system(cmd);
		}

	
	}





	
	while (fgets(buf, BUFSIZE, fp) != NULL) 
	{
#ifdef TEST
		printf("%s",buf);
		printf(" count : %d\n",count);
#endif	
		p = buf;

		data->Line_Number[count] = 0xFFFF;
		data->Line_Number[count+1] = 0xFFFF;

		token = strsep(&p, ":\n");//1st field
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


	for(int i = 0;i < count;i++)
	{

		if(strstr(data->IOM_Heading[i], string_IOM_Differentiate[eHeaderCPU]))		
		{


			NoOfData->NoOfCPUs++;
			NoOfData->CPUAvailable = 1;
			//MPREdata->Line_Number[NoOfMPRE] = data->Line_Number[i] ;

			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" %s.csv > CPU%02d.csv",data->Line_Number[i],data->Line_Number[i+1]-1,InputFile_CSV,i+1)+1);
			sprintf(cmd,"sed -n \"%d,%d p\" %s.csv > CPU%02d.csv",data->Line_Number[i],
			data->Line_Number[i+1]-1,InputFile_CSV,i+1);
			#ifdef TEST
			printf("%s\n",cmd);
			#endif

			if (system(cmd) == 256) 
			{
				printf("Failed to run command\n" );
				exit(1);
			}
		}
		else if(strstr(data->IOM_Heading[i], string_IOM_Differentiate[eHeaderMPRE1]))		
		{

			NoOfData->NoOfMPREs++;
			NoOfData->MPREAvailable[e_MPRE_1] = 1;
			MPREdata->Line_Number[e_MPRE_1] = data->Line_Number[i] ;
#ifdef TEST
			printf("LineNumber: %d\n", MPREdata->Line_Number[e_MPRE_1]);
#endif



			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" %s.csv > MPRE%02d.csv",data->Line_Number[i], data->Line_Number[i+1]-1,InputFile_CSV,e_MPRE_1+1) +1);
			sprintf(cmd,"sed -n \"%d,%d p\" %s.csv > MPRE%02d.csv", data->Line_Number[i], data->Line_Number[i+1]-1,InputFile_CSV,e_MPRE_1+1);
			#ifdef TEST
			printf("%s\n",cmd);
			#endif

			if (system(cmd) == 256) 
			{
				printf("Failed to run command\n" );
				exit(1);
			}
		}
		else if(strstr(data->IOM_Heading[i], string_IOM_Differentiate[eHeaderMPRE2]))		
		{

			NoOfData->NoOfMPREs++;
			NoOfData->MPREAvailable[e_MPRE_2] = 1;
			MPREdata->Line_Number[NoOfData->NoOfMPREs] = data->Line_Number[i] ;
#ifdef TEST
			printf("LineNumber: %d\n", MPREdata->Line_Number[NoOfData->NoOfMPREs]);
#endif



			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" %s.csv > MPRE%02d.csv",data->Line_Number[i], data->Line_Number[i+1]-1,InputFile_CSV,e_MPRE_2+1 )+1);
			sprintf(cmd,"sed -n \"%d,%d p\" %s.csv > MPRE%02d.csv", data->Line_Number[i], data->Line_Number[i+1]-1,InputFile_CSV,e_MPRE_2+1);
			#ifdef TEST
			printf("%s\n",cmd);
			#endif

			if (system(cmd) == 256) 
			{
				printf("Failed to run command\n" );
				exit(1);
			}
		}
		else if(strstr(data->IOM_Heading[i], string_IOM_Differentiate[eHeaderIOM]))	
		{
			NoOfData->NoOfIOMs++;
			NoOfData->IOMAvailable = 1;
			IOM_data->Line_Number[NoOfIOMs]=data->Line_Number[i];
			printf("LineNumber: %d\n", IOM_data->Line_Number[NoOfIOMs]);

			if(data->IOM_Heading[i] != NULL) 
			{	
				IOM_data->IOM_Heading[NoOfIOMs] = malloc(strlen(data->IOM_Heading[i])+1);
				strcpy(IOM_data->IOM_Heading[NoOfIOMs],data->IOM_Heading[i]);			
			}

			if(data->IOM_Description[i] != NULL) 
			{	
				IOM_data->IOM_Description[NoOfIOMs] = malloc(strlen(data->IOM_Description[i])+1);
				strcpy(IOM_data->IOM_Description[NoOfIOMs],data->IOM_Description[i]);			
			}
			IOM_data->slaveID[NoOfIOMs]=data->slaveID[i];
			printf("Slave ID: %d\n", IOM_data->slaveID[NoOfIOMs]);

			NoOfIOMs++;


			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" %s.csv > IOM%02d.csv",data->Line_Number[i], data->Line_Number[i+1]-1,InputFile_CSV,NoOfIOMs)+1);
		sprintf(cmd,"sed -n \"%d,%d p\" %s.csv > IOM%02d.csv",data->Line_Number[i],data->Line_Number[i+1]-1,InputFile_CSV,NoOfIOMs);




		#ifdef TEST
			printf("%s\n",cmd);
		#endif

			if (system(cmd) == 256) 
			{
				printf("Failed to run command\n" );
				exit(1);
			}

			
		}	
	}
	/* close */
	fclose(fp);
}

void fetchNodesandSeperate(MPRE_Header_CSV_Data* MPREdata, MPRE_Nodes_Header_CSV_Data* NodesData,uint8_t NoOfMPRES, QuantityOfCSVData *DataAvailablity)
{

	FILE *fp,*fPtr;
	
	uint8_t count = 0;
	char buf[BUFSIZE];
	char * cmd, *Tmp;			
	char *token, *p;
	
	uint8_t channel_no,NoOfNodes =0,NoOfMPRE = 0;

	
	DataAvailablity->NoOfNodesForMPRE_Available[NoOfMPRES]=0;
	DataAvailablity->NoOfNodesForMPRE[NoOfMPRES]=0;

	cmd = malloc(snprintf(NULL,0," grep -own -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\"  -e \"%s\"  -e \"%s\"  -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" %s%02d.csv  > Nodes_Header_text.csv", Node_Strings[0], Node_Strings[1], Node_Strings[2], Node_Strings[3], Node_Strings[4], Node_Strings[5], Node_Strings[6], Node_Strings[7], Node_Strings[8], Node_Strings[9], Node_Strings[10], Node_Strings[11], Node_Strings[12], Node_Strings[13], Node_Strings[14], Node_Strings[15], Node_Strings[16], Node_Strings[17], Node_Strings[18], Node_Strings[19],"MPRE",NoOfMPRES+1)+1);



	sprintf(cmd," grep -own -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\"  -e \"%s\"  -e \"%s\"  -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" %s%02d.csv  > Nodes_Header_text.csv", Node_Strings[0], Node_Strings[1], Node_Strings[2], Node_Strings[3], Node_Strings[4], Node_Strings[5], Node_Strings[6], Node_Strings[7], Node_Strings[8], Node_Strings[9], Node_Strings[10], Node_Strings[11], Node_Strings[12], Node_Strings[13], Node_Strings[14], Node_Strings[15], Node_Strings[16], Node_Strings[17], Node_Strings[18], Node_Strings[19],"MPRE",NoOfMPRES+1);

#ifdef TEST
		printf("%s",cmd);

#endif

	if (system(cmd) == 256) 
	{
		printf("Failed to run command\n" );
		exit(1);
	}

	fp = fopen("Nodes_Header_text.csv", "r");

	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}

	
	while (fgets(buf, BUFSIZE, fp) != NULL) 
	{
#ifdef TEST
		printf("%s",buf);
		printf(" count : %d\n",count);
#endif
		
		p = buf;

		NodesData->Line_Number[count] = 0xFFFF;
		NodesData->Line_Number[count+1] = 0xFFFF;

		token = strsep(&p, ":\n");//1st field
		if(token != NULL) NodesData->Line_Number[count] = atoi(token);
		token = strsep(&p, ",\n");//2nd field
		if(token != NULL) 
		{	
			NodesData->IOM_Heading[count] = malloc(strlen(token)+1);
			strcpy(NodesData->IOM_Heading[count], token);	
		}		
		
#ifdef TEST
		printf("DEBUG:\nLine_Number:'%d',\n IOM_Heading:'%s',\n count:%d\n",
			NodesData->Line_Number[count], 
			NodesData->IOM_Heading[count], 
			count);
		
#endif
		count++;
	}



	for(uint8_t i = 0;i < count;i++)
	{

		if(strstr(NodesData->IOM_Heading[i], "Node-"))	
		{
#ifdef TEST
			printf("DEBUG:\nLine_Number:'%d',\n IOM_Heading:'%s',\n count:%d\n",
			NodesData->Line_Number[i], 
			NodesData->IOM_Heading[i], 
			i);

			printf("LineNumber: %d\n", NodesData->Line_Number[i]);
#endif
	
			DataAvailablity->NoOfNodesForMPRE_Available[NoOfMPRES]=1;
			DataAvailablity->NoOfNodesForMPRE[NoOfMPRES]++;
			
		

			NoOfNodes++;


			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" %s%02d.csv > MPRE%02d_Nodes_%02d.csv", NodesData->Line_Number[i], NodesData->Line_Number[i+1]-1,"MPRE",NoOfMPRES+1, NoOfMPRES+1,NoOfNodes)+1);
		sprintf(cmd,"sed -n \"%d,%d p\" %s%02d.csv > MPRE%02d_Nodes_%02d.csv", NodesData->Line_Number[i],NodesData->Line_Number[i+1]-1,"MPRE",NoOfMPRES+1, NoOfMPRES+1,NoOfNodes);




		#ifdef TEST
			printf("%s\n",cmd);
		#endif

			if (system(cmd) == 256) 
			{
				printf("Failed to run command\n" );
				exit(1);
			}

			
		}	
	}



	/* close */
	fclose(fp);

}


void fetchNodesParametersandSeperate(uint8_t Node_number,uint8_t MPREChannelNo)
{

	FILE *fp,*fPtr;
	
	uint8_t count = 0;
	char buf[BUFSIZE];
	char * cmd, *Tmp;			
	char *token, *p;
	uint16_t Line_Number[40];
	char *Heading[40];
	
	uint8_t channel_no,NoOfNodes =0,NoOfMPRE = 0;


	cmd = malloc(snprintf(NULL,0," grep -own -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" MPRE%02d_Nodes_%02d.csv > NodesParameter_Header_text.csv", string_MPER_Nodes[0], string_MPER_Nodes[1], string_MPER_Nodes[2], string_MPER_Nodes[3], MPREChannelNo+1, Node_number+1)+1);



	sprintf(cmd," grep -own -e \"%s\" -e \"%s\" -e \"%s\" -e \"%s\" MPRE%02d_Nodes_%02d.csv  > NodesParameter_Header_text.csv",string_MPER_Nodes[0],string_MPER_Nodes[1], string_MPER_Nodes[2], string_MPER_Nodes[3],MPREChannelNo+1, Node_number+1);


	if (system(cmd) == 256) 
	{
		printf("Failed to run command\n" );
		exit(1);
	}

	fp = fopen("NodesParameter_Header_text.csv", "r");

	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}

	
	while (fgets(buf, BUFSIZE, fp) != NULL) 
	{
#ifdef TEST
		printf("%s",buf);
		printf(" count : %d\n",count);
#endif		
		p = buf;

		Line_Number[count] = 0xFFFF;
		Line_Number[count+1] = 0xFFFF;

		token = strsep(&p, ":\n");//1st field
		if(token != NULL) Line_Number[count] = atoi(token);
		token = strsep(&p, ",\n");//2nd field
		if(token != NULL) 
		{	
			Heading[count] = malloc(strlen(token)+1);
			strcpy(Heading[count], token);	
		}
#ifdef TEST
		printf("DEBUG:\nLine_Number:'%d',\n IOM_Heading:'%s',\n count:%d\n",
			Line_Number[count], 
			Heading[count], 
			count);
		
#endif		
		count++;


	}


	for(int i = 0;i < count;i++)
	{

		NoOfNodes++;

#ifdef TEST
		printf("SMIT:\nLine_Number:'%d',\n IOM_Heading:'%s',\n count:%d\n",
			Line_Number[i], 
			Heading[i], 
			count);
		
#endif
	
		if(strstr(Heading[i], string_MPER_Nodes[0]))	
		{
			
#ifdef TEST
		printf("DEBUG:\nLine_Number:'%d',\n IOM_Heading:'%s',\n count:%d\n",
			Line_Number[count], 
			Heading[count], 
			count);
		

			printf("LineNumber Heading 1: %d\n", Line_Number[i]);		
#endif

			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" MPRE%02d_Nodes_%02d.csv > MPRE%02d_Nodes_%02d_01.csv", Line_Number[i],Line_Number[i+1]-1, MPREChannelNo+1, Node_number+1,MPREChannelNo+1, Node_number+1)+1);
		sprintf(cmd,"sed -n \"%d,%d p\" MPRE%02d_Nodes_%02d.csv > MPRE%02d_Nodes_%02d_01.csv", Line_Number[i],Line_Number[i+1]-1,MPREChannelNo+1, Node_number+1, MPREChannelNo+1, Node_number+1);
		}
		else if(strcmp(Heading[i], string_MPER_Nodes[1]) == 0)		
		{
			
#ifdef TEST
			printf("LineNumber Heading 2: %d\n", Line_Number[i]);

#endif


			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" MPRE%02d_Nodes_%02d.csv > MPRE%02d_Nodes_%02d_02.csv",Line_Number[i],Line_Number[i+1]-1,MPREChannelNo+1, Node_number+1,MPREChannelNo+1, Node_number+1)+1);
		sprintf(cmd,"sed -n \"%d,%d p\" MPRE%02d_Nodes_%02d.csv > MPRE%02d_Nodes_%02d_02.csv", Line_Number[i],Line_Number[i+1]-1,MPREChannelNo+1, Node_number+1,MPREChannelNo+1, Node_number+1);
		}
		else if(strcmp(Heading[i], string_MPER_Nodes[2]) == 0)		
		{
			
#ifdef TEST
			printf("LineNumber Heading 3: %d\n", Line_Number[i]);

#endif


			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" MPRE%02d_Nodes_%02d.csv > MPRE%02d_Nodes_%02d_03.csv",Line_Number[i],Line_Number[i+1]-1,MPREChannelNo+1, Node_number+1,MPREChannelNo+1, Node_number+1)+1);
		sprintf(cmd,"sed -n \"%d,%d p\" MPRE%02d_Nodes_%02d.csv > MPRE%02d_Nodes_%02d_03.csv", Line_Number[i],Line_Number[i+1]-1,MPREChannelNo+1, Node_number+1,MPREChannelNo+1, Node_number+1);
		}
		else if(strcmp(Heading[i], string_MPER_Nodes[3]) == 0)	
		{
			
#ifdef TEST
			printf("LineNumber Heading 4: %d\n", Line_Number[i]);
#endif

			cmd = malloc(snprintf(NULL,0,"sed -n \"%d,%d p\" MPRE%02d_Nodes_%02d.csv > MPRE%02d_Nodes_%02d_04.csv",Line_Number[i],Line_Number[i+1]-1,MPREChannelNo+1, Node_number+1,MPREChannelNo+1, Node_number+1)+1);
		sprintf(cmd,"sed -n \"%d,%d p\" MPRE%02d_Nodes_%02d.csv > MPRE%02d_Nodes_%02d_04.csv", Line_Number[i],Line_Number[i+1]-1,MPREChannelNo+1, Node_number+1,MPREChannelNo+1, Node_number+1);
		}



		#ifdef TEST
			printf("%s\n",cmd);
		#endif

			if (system(cmd) == 256) 
			{
				printf("Failed to run command\n" );
				exit(1);
			}

			
			
	}



	/* close */
	fclose(fp);


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
	fclose(fp);





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



void ExtractAsPerIOMType(IOM_Header_CSV_Data *data, uint8_t NoofIOM)
{

	uint8_t value,NoOfData;
	int rc;
	


	switch(data->File_Identifier[NoofIOM])
	{

		case AI_Status:
		{

			m_modbus_set_slave(data->slaveID[NoofIOM]);

			NoOfData = extract_AI_Data_From_CSV(e_AI_IOM_TYPE,e_AI_CHANNEL_CONFIGURATION_IOM_DATA,&s_AI_CHANNEL_CONFIGURATION_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);
			for(int i = 0; i< NoOfData ; i++)	
			{
				AI_CSVtoModbus_HPAPerChannelFrame(&s_AI_CHANNEL_CONFIGURATION_IOM_DATA,&AI_HPA_Data,i);
			}


			printf("1/3 FRTU AI Channel Wise HAP: ");
			m_modbus_write_registers(AI_FRTU_PART1_REGISTERS_ADDRESS,
			AI_FRTU_PART1_REGISTERS_NB, &(AI_HPA_Data.HPA_AI_Whole_Channel[0]));

			//ASSERT_TRUE(rc == DI_FRTU_PART1_REGISTERS_NB, "");

			printf("2/3 FRTU AI Channel Wise HAP: ");
			 m_modbus_write_registers(AI_FRTU_PART2_REGISTERS_ADDRESS,
			AI_FRTU_PART2_REGISTERS_NB, &(AI_HPA_Data.HPA_AI_Whole_Channel[AI_NO_OF_HAP_BYTES/2]));



			NoOfData = extract_AI_Data_From_CSV(e_AI_IOM_TYPE,e_AI_DIAGNOSTIC_IOM_DATA,&s_AI_DIAGNOSTIC_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);
			for(int i = 0; i< NoOfData ; i++)	
			{
				AI_CSVtoModbus_DiagnosisPerCardFrame(&s_AI_DIAGNOSTIC_IOM_DATA,&DIAGNOSIS_AI_Per_Card,i);
			}




			printf("3/3 FRTU AI Card Wise Diagnostics: ");
			m_modbus_write_registers(AI_FRTU_PART3_REGISTERS_ADDRESS,
				AI_NO_OF_DIAGNOSIS_WORDBYTE, DIAGNOSIS_AI_Per_Card.DIAGNOSIS_AI_Whole_Card);

			// ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");
		}
		break;

		case DI_Status:
		{


			m_modbus_set_slave(data->slaveID[NoofIOM]);

			NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_CONFIGURATION_IOM_DATA,&s_DI_CHANNEL_CONFIGURATION_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);
			for(int i = 0; i< NoOfData ; i++)	
			{
			DI_CSVtoModbus_HPAPerChannelFrame(&s_DI_CHANNEL_CONFIGURATION_IOM_DATA,&DI_HPA_Data,i);
			}



    printf("1/4 FRTU_DI_write_registers: ");
    m_modbus_write_registers( DI_FRTU_PART1_REGISTERS_ADDRESS,
DI_FRTU_PART1_REGISTERS_NB, &(DI_HPA_Data.HPA_DI_Whole_Channel[0])/*DI_FRTU_PART1_REGISTERS_TAB*/);
    
    //ASSERT_TRUE(rc == DI_FRTU_PART1_REGISTERS_NB, "");
    
    printf("2/4 FRTU_DI_write_registers: ");
    m_modbus_write_registers(DI_FRTU_PART2_REGISTERS_ADDRESS,
                                DI_FRTU_PART2_REGISTERS_NB, &(DI_HPA_Data.HPA_DI_Whole_Channel[0+DI_NO_OF_HAP_WORDBYTE/2]));
    
   // ASSERT_TRUE(rc == DI_FRTU_PART2_REGISTERS_NB, "");








			NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_DIAGNOSTIC_IOM_DATA,&s_DI_DIAGNOSTIC_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);

			for(int i = 0; i< NoOfData ; i++)	
			{						DI_CSVtoModbus_DiagnosisPerCardFrame(&s_DI_DIAGNOSTIC_IOM_DATA,&DI_DIAGNOSIS_Per_Card,i);
			}

	printf("3/4 FRTU_DO_write_registers: ");
	m_modbus_write_registers(DI_FRTU_PART3_REGISTERS_ADDRESS,
	DI_NO_OF_DIAGNOSIS_WORDBYTE, DI_DIAGNOSIS_Per_Card.DIAGNOSIS_DI_Whole_Card);

	// ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");






			NoOfData = extract_DI_Data_From_CSV(e_DI_IOM_TYPE,e_DI_CHANNEL_DIAGNOSTIC_IOM_DATA,&S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA);
			printf("No. Data to Extract : %d\n",NoOfData);

			for(int i = 0; i< NoOfData ; i++)	
			{						DI_CSVtoModbus_DiagnosticsPerChannelFrame(&S_DI_CHANNEL_DIAGNOSTIC_IOM_DATA,&DI_SW_DIAGNOSIS_Per_Channel,i);
			}


	printf("4/4 FRTU_DO_write_registers: ");
	m_modbus_write_registers(DI_FRTU_PART4_REGISTERS_ADDRESS,
	DI_NO_OF_SW_DIAGNOSIS_WORDBYTE, &(DI_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DI_Whole_Channel[0]));

	// ASSERT_TRUE(rc == DO_FRTU_PART4_REGISTERS_NB, "");



		}
		break;


		case DO_Status:
		{

	
		m_modbus_set_slave(data->slaveID[NoofIOM]);

		NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_CHANNEL_CONFIGURATION_IOM_DATA,&s_DO_CHANNEL_CONFIGURATION_IOM_DATA);
		printf("No. Data to Extract : %d\n",NoOfData);
		for(int i = 0; i< NoOfData ; i++)	
		{
				DO_CSVtoModbus_HPAPerChannelFrame(&s_DO_CHANNEL_CONFIGURATION_IOM_DATA,&DO_HPA_Data,i);
		}


		

	    printf("1/4 FRTU_DO_write_registers: ");
	    m_modbus_write_registers(DO_FRTU_PART1_REGISTERS_ADDRESS,DO_FRTU_PART1_REGISTERS_NB,
					&(DO_HPA_Data.HPA_DO_Whole_Channel[0]));

    printf("2/4 FRTU_DO_write_registers: ");
     m_modbus_write_registers(DO_FRTU_PART2_REGISTERS_ADDRESS,
                                DO_FRTU_PART2_REGISTERS_NB, &(DO_HPA_Data.HPA_DO_Whole_Channel[0+DO_NO_OF_HAP_WORDBYTE/2]));
    
   // ASSERT_TRUE(rc == DO_FRTU_PART2_REGISTERS_NB, "");


		NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_DIAGNOSTIC_IOM_DATA,&s_DO_DIAGNOSTIC_IOM_DATA);
		printf("No. Data to Extract : %d\n",NoOfData);

		for(int i = 0; i< NoOfData ; i++)	
		{						DO_CSVtoModbus_DiagnosisPerCardFrame(&s_DO_DIAGNOSTIC_IOM_DATA,&DO_DIAGNOSIS_Per_Card,i);
		}


    printf("3/4 FRTU_DO_write_registers: ");
    m_modbus_write_registers(DO_FRTU_PART3_REGISTERS_ADDRESS,
                                DO_FRTU_PART3_REGISTERS_NB, &(DO_DIAGNOSIS_Per_Card.DIAGNOSIS_DO_Whole_Card[0]));
    
   // ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");


		NoOfData = extract_DO_Data_From_CSV(e_DO_IOM_TYPE,e_DO_CHANNEL_DIAGNOSTIC_IOM_DATA,&s_DO_CHANNEL_DIAGNOSTIC_IOM_DATA);
		printf("No. Data to Extract : %d\n",NoOfData);

		for(int i = 0; i< NoOfData ; i++)	
		{						DO_CSVtoModbus_DiagnosticsPerChannelFrame(&s_DO_CHANNEL_DIAGNOSTIC_IOM_DATA,&DO_SW_DIAGNOSIS_Per_Channel,i);
		}

    printf("4/4 FRTU_DO_write_registers: ");
    m_modbus_write_registers(DO_FRTU_PART4_REGISTERS_ADDRESS,
                                DO_FRTU_PART4_REGISTERS_NB, &(DO_SW_DIAGNOSIS_Per_Channel.SW_DIAGNOSIS_DO_Whole_Channel[0]));
    
   // ASSERT_TRUE(rc == DO_FRTU_PART3_REGISTERS_NB, "");


	}
	break;
		
	}
}






