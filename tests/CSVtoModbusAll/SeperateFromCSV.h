typedef char * string;


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
	uint16_t Line_Number[15];
	char *IOM_Heading[15];
	char *IOM_Description[15];
	uint8_t slaveID[15];
	uint8_t File_Identifier[15];	
}
CPU_Header_CSV_Data;



int SeperateCSV(void);

void Recognize_IOMs(IOM_Header_CSV_Data* data, uint8_t IOM_fileNumber);
uint8_t fetchCPUHeaderCSVandSeperate(CPU_Header_CSV_Data* data,IOM_Header_CSV_Data* IOM_data);
void ExtractAsPerIOMType(IOM_Header_CSV_Data *data, uint8_t NoofIOM);

extern IOM_Header_CSV_Data IOM_Header_data;
extern CPU_Header_CSV_Data CPU_Header_data;



