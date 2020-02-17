typedef char * string;



typedef struct 
{
uint8_t NoOfIOMs;
uint8_t IOMAvailable;
uint8_t NoOfCPUs;
uint8_t CPUAvailable;
uint8_t NoOfMPREs;
uint8_t MPREAvailable[2];
uint8_t NoOfNodesForMPRE[2];
uint8_t NoOfNodesForMPRE_Available[2];
}QuantityOfCSVData;



extern QuantityOfCSVData DataAvailablity;



extern char InputFile_CSV[50];
typedef struct{
	uint16_t Line_Number[10];
	char *IOM_Heading[10];
	char *IOM_Description[10];	
	uint8_t slaveID[10];
	uint8_t File_Identifier[10];
}
IOM_Header_CSV_Data;

typedef struct{
	uint16_t Line_Number[3];
	char *IOM_Heading[3];
	char *IOM_Description[3];
	uint8_t File_Identifier[3];
}
MPRE_Header_CSV_Data;

typedef struct{
	uint16_t Line_Number[21];
	char *IOM_Heading[21];
	uint8_t File_Identifier[21];
}
MPRE_Nodes_Header_CSV_Data;

typedef struct{
	uint16_t Line_Number[15];
	char *IOM_Heading[15];
	char *IOM_Description[15];
	uint8_t slaveID[15];
	uint8_t File_Identifier[15];	
}
CPU_Header_CSV_Data;

enum HeaderAvail{
	eHeaderCPU,
	eHeaderMPRE1,
	eHeaderMPRE2,
	eHeaderIOM
};


int SeperateCSV(void);

void Recognize_IOMs(IOM_Header_CSV_Data* data, uint8_t IOM_fileNumber);

void fetchCPUHeaderCSVandSeperate(CPU_Header_CSV_Data* data,IOM_Header_CSV_Data* IOM_data,MPRE_Header_CSV_Data* MPREdata, QuantityOfCSVData*  NoOfData);


void ExtractAsPerIOMType(IOM_Header_CSV_Data *data, uint8_t NoofIOM);
void fetchNodesParametersandSeperate(uint8_t Node_number,uint8_t MPREChannelNo);


void fetchNodesandSeperate(MPRE_Header_CSV_Data* MPREdata, MPRE_Nodes_Header_CSV_Data* NodesData,uint8_t NoOfMPRES, QuantityOfCSVData *DataAvailablity);




extern IOM_Header_CSV_Data IOM_Header_data;
extern CPU_Header_CSV_Data CPU_Header_data;
extern MPRE_Header_CSV_Data MPRE_Header_data;
extern MPRE_Nodes_Header_CSV_Data MPRE_Nodes_Header_data;





