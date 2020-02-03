#include <inttypes.h>
#include <stdint.h>


#define DI_NO_OF_CHANNELS 16
#define DI_NO_OF_HAP 10
#define DI_NO_OF_HAP_WORDBYTE DI_NO_OF_HAP*DI_NO_OF_CHANNELS

#define DI_NO_OF_DIAGNOSIS_CARD 5
#define DI_NO_OF_DIAGNOSIS_DATA 3 
#define DI_NO_OF_DIAGNOSIS_WORDBYTE DI_NO_OF_DIAGNOSIS_CARD*DI_NO_OF_DIAGNOSIS_DATA

#define DI_NO_OF_SW_POINTS 2
#define DI_NO_OF_SW_DIAGNOSIS_CHANNEL DI_NO_OF_SW_POINTS* DI_NO_OF_CHANNELS
 //2 SOFTWARE POINTS OF 16 CHANNEL EACH SO 32
#define DI_NO_OF_SW_DIAGNOSIS_DATA 3 
#define DI_NO_OF_SW_DIAGNOSIS_WORDBYTE DI_NO_OF_SW_DIAGNOSIS_CHANNEL*DI_NO_OF_SW_DIAGNOSIS_DATA


void default_DI_Modbus_Frame(void);
void test_Independent_DI(void);




typedef union 
{
	struct
	{	
		uint8_t Inv:1;//LSB
		uint8_t Inv_1:1;
		uint8_t Block:1;
		uint8_t flutter_ODS:1;
		uint8_t State_Of_flutter:1;
		uint8_t class_2:2;
		uint8_t unused:1;	//MSB		
	}bits;
	uint8_t DI_Bits;
}bitAllocation_DI;


typedef union
{
	struct
	{
		uint8_t CASDU2;	//LSB
		uint8_t CASDU1;	//MSB			

		uint8_t IOA2;	//LSB
		uint8_t IOA1;	//MSB					

		uint8_t TI;	//LSB
		uint8_t IOA3;	//MSB

		uint8_t Flutter_Number;	
		bitAllocation_DI DI_bitAllocation;		

		uint16_t Flutter_time;

		uint16_t Software_Debouce_time;

		uint16_t Intermeditate_Position_time;

		uint16_t Faulty_Position_time;

		uint16_t AR_Signaling_time;

		uint8_t RESERVED_1;
		uint8_t ON_Before_Time;
		
	} HPA;
	uint16_t HPA_DI_Per_Channel[DI_NO_OF_HAP/*10*/];
} DI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES ;


typedef union
{
	struct
	{
		DI_HARDWARE_POINT_PER_CHANNEL_ADDRESSES HPA_DI_Per_Channel[DI_NO_OF_CHANNELS/*16*/];
	} HPA_whole;
	uint16_t HPA_DI_Whole_Channel[DI_NO_OF_HAP_WORDBYTE/*16x10*/];
	
} DI_HARDWARE_POINT_ADDRESSES ;



typedef union
{
	struct
	{
		uint8_t CASDU2;	//LSB
		uint8_t CASDU1;	//MSB			

		uint8_t IOA2;	//LSB
		uint8_t IOA1;	//MSB					

		uint8_t TI;	//LSB
		uint8_t IOA3;	//MSB		

	} DIAGNOSIS_BYTES;
	uint16_t DIAGNOSIS_DI_Per_Card[DI_NO_OF_DIAGNOSIS_DATA/*3*/];
} DI_DIAGNOSIS_POINT_PER_CARD_ADDRESSES ;

typedef union
{
	struct
	{
		DI_DIAGNOSIS_POINT_PER_CARD_ADDRESSES DIAGNOSIS_DI_Per_Card[DI_NO_OF_DIAGNOSIS_CARD/*2*/];
	} Diagnosis_whole;
	uint16_t DIAGNOSIS_DI_Whole_Card[DI_NO_OF_DIAGNOSIS_WORDBYTE/*6*/];
} DI_DIAGNOSIS_POINT_ADDRESSES ;


typedef union
{
	struct
	{
		uint8_t CASDU2;	//LSB
		uint8_t CASDU1;	//MSB			

		uint8_t IOA2;	//LSB
		uint8_t IOA1;	//MSB					

		uint8_t TI;	//LSB
		uint8_t IOA3;	//MSB		

	} SW_DIAGNOSIS_BYTES;
	uint16_t SW_DIAGNOSIS_DI_Per_Channel[DI_NO_OF_SW_DIAGNOSIS_DATA/*3*/];
} DI_SW_DIAGNOSIS_POINT_PER_CHANNEL_ADDRESSES ;

typedef union
{
	struct
	{
		DI_SW_DIAGNOSIS_POINT_PER_CHANNEL_ADDRESSES DIAGNOSIS_SW_DI_Per_Channel[DI_NO_OF_SW_DIAGNOSIS_CHANNEL/*32*/];
	} Diagnosis_whole;
	uint16_t SW_DIAGNOSIS_DI_Whole_Channel[DI_NO_OF_SW_DIAGNOSIS_WORDBYTE/*32x3*/];
} DI_SW_DIAGNOSIS_POINT_ADDRESSES;

extern DI_HARDWARE_POINT_ADDRESSES DI_HPA_Data;
extern DI_DIAGNOSIS_POINT_ADDRESSES DI_DIAGNOSIS_Per_Card;
extern DI_SW_DIAGNOSIS_POINT_ADDRESSES DI_SW_DIAGNOSIS_Per_Channel;

