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



int SeperateCSV(void);

void Recognize_IOMs(IOM_Header_CSV_Data* data, uint8_t IOM_fileNumber);
void fetchIOMHeaderCSV(IOM_Header_CSV_Data* data);
void ExtractAsPerIOMType(uint8_t typeOfIOM);

extern IOM_Header_CSV_Data IOM_Header_data;

