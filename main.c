#include "card_number.h"
#include <stdbool.h>
#include <stdio.h>
FIRST_Part_Vehicle_NumberDef *numberList;
unsigned char *vehicle_Number_List_WJN;
void main()
{
	unsigned char position = -1;
	unsigned int mask = -1;
	//printf("size is %d",sizeof(mask));
	unsigned short int secondPartAscii = 0;
	unsigned int start = -1;
	vehicle_Number_List_WJN = getVehicle_Number_List_WJN();
	unsigned short int firstTwoAscii = uint8ToUint16(vehicle_Number_List_WJN[0],vehicle_Number_List_WJN[1]);
	unsigned char isIn = CheckIfInVehicleNumberList(firstTwoAscii,&position);
	unsigned short int firstOutAscii = 0;
	numberList = getVehicleNumHead();
	if(!isIn)
	{
		printf("当前车牌未识别");
		return;
	}
	/*-----------------------first part begin ----------------------------*/
	mask = (numberList+position)->character_mask;
	start = (numberList+position)->out_start_Ascii;
	printf("\r\nmask number in the list %d, start position is %d\r\n",mask,start);
	if((numberList+position)->second_Ascii_len==1)
	{
		if(position>=32)
		{
			secondPartAscii = (firstTwoAscii)&0x00FF;
		}
		else
		{
			//直接读取下一个字节
			secondPartAscii = g_Vehicle_Number_List_Normal[2];
		}
		firstOutAscii= getsecondPartAsciiRealPositionInList(mask,secondPartAscii)+start;
	}
	else/*武警车牌*/
	{
		firstOutAscii = combineAsciiToDec(vehicle_Number_List_WJN[2],vehicle_Number_List_WJN[3])+start;
		
	}
	printf("\r\nfirstOutAscii is %d\r\n",firstOutAscii);
	/*-----------------------first part end ----------------------------*/
	/*-----------------------second part begin ----------------------------*/
	
	/*-----------------------second part end ----------------------------*/
	printf("\r\nascii number in the list %d, position is %d\r\n",isIn,position);
}
