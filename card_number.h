
#ifndef __CARD_NUMBER_H
#define __CARD_NUMBER_H

#define VEHICLE_CARD_NUMBER 42

typedef struct
{
	unsigned short int first_Ascii;/*车牌第一位的ascii,有一个字节也有两个字节*/
	unsigned char second_Ascii_len;/*输入的车牌号前两个或者一个ascii字节后面跟着的字节长度*/
	unsigned short int out_start_Ascii;/*前两位车牌号的输出字节码的其实位置，最终输出由第二位决定*/
	int character_mask;/*低26位用做相应字母的使用标记，1表示使用，0表示未使用，在实际使用的时候，需要将其中标记的字母重新排序*/
	//uint16_t second_number;/*车牌第二位的ascii,有一个字节也有两个字节*/
}FIRST_Part_Vehicle_NumberDef;



FIRST_Part_Vehicle_NumberDef *getVehicleNumHead(void);
unsigned char *getVehicle_Number_List_WJN(void);
unsigned short int uint8ToUint16(unsigned char data1,unsigned char data2);
unsigned char combineAsciiToDec(unsigned char H_Data, unsigned char L_Data);
unsigned char getsecondPartAsciiRealPositionInList(unsigned int mask,unsigned short int ascii);
unsigned char CheckIfInVehicleNumberList(unsigned int headData,unsigned char *position);

extern unsigned char g_Vehicle_Number_List_Normal[10];//辽E1230E
extern unsigned char g_Vehicle_Number_List_Force[10] ;//沈M1230E
extern unsigned char g_Vehicle_Number_List_WJN[10] ;//WJ141230E
extern unsigned char g_Vehicle_Number_List_WJF[10];//WJ141230E

extern FIRST_Part_Vehicle_NumberDef g_FirstTwoVehicleNumberList[VEHICLE_CARD_NUMBER];

#endif
