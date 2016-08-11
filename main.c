#include "card_number.h"
#include <stdbool.h>
#include <stdio.h>

void main()
{
	unsigned char pointer = 0;
	unsigned int secondOutAscii = 0;
	unsigned short int firstPartOut = 0;

	unsigned char *vehicle_Number_List_WJN = getVehicle_Number_List_WJN();

	firstPartOut  = firstPartNumberOut(vehicle_Number_List_WJN,&pointer);
	printf("\r\nfirstPartOut is %u",firstPartOut);

	secondOutAscii = secondPartNumberOut(&vehicle_Number_List_WJN[pointer]);
	printf("\r\nsecond out ascii is %u\r\n",secondOutAscii);

	if(assembleAscii(&firstPartOut,&secondOutAscii,2)==1){
		printf("\r\nfirstFinalOut is %u",firstPartOut);
		printf("\r\nsecondFinalOut out ascii is %u\r\n",secondOutAscii);
	}else{
		printf("\r\ndeal cartNumber error");
	}
}
