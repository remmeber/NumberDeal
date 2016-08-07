#include "card_number.h"

FIRST_Part_Vehicle_NumberDef g_FirstTwoVehicleNumberList[VEHICLE_CARD_NUMBER] =
{
	{0x4EAC,1,0x0001,0x000040F7},/*��*/
	{0x6D25,1,0x000c,0x00000017},/*��*/
	{0x5180,1,0x0014,0x000A02FF},/*��*/
	{0x664B,1,0x0021,0x00001EBF},/*��*/
	{0x8499,1,0x002E,0x00001EFF},/*��*/
	{0x8FBD,1,0x003C,0x0020BEFF},/*��*/
	{0x5409,1,0x004C,0x000002FF},/*��*/
	{0x9ED1,1,0x0058,0x0002BEFF},/*��*/
	{0x6CAA,1,0x0068,0x000000FF},/*��*/
	{0x82CF,1,0x0070,0x00003EFF},/*��*/
	{0x6D59,1,0x007F,0x00000EFF},/*��*/
	{0x7696,1,0x008C,0x0007BEFF},/*��*/
	{0x95FD,1,0x009E,0x000006FF},/*��*/
	{0x8D63,1,0x00AA,0x00001EFF},/*��*/
	{0x9C81,1,0x00B8,0x0037BEFF},/*³*/
	{0x8C6B,1,0x00CC,0x0017BEFF},/*ԥ*/
	{0x9102,1,0x00DF,0x0007BEFF},/*��*/
	{0x6E58,1,0x00F1,0x00103EFF},/*��*/
	{0x7CA4,1,0x0101,0x03FFBEFF},/*��*/
	{0x6842,1,0x011A,0x0002BEFF},/*��*/
	{0x743C,1,0x012A,0x0000001F},/*��*/
	{0x6E1D,1,0x0132,0x000000A7},/*��*/
	{0x5DDD,1,0x013B,0x03FF2EBF},/*��*/
	{0x8D35,1,0x0150,0x000002FF},/*��*/
	{0x4E91,1,0x015C,0x0007BEFF},/*��*/
	{0x85CF,1,0x016E,0x000002FF},/*��*/
	{0x9655,1,0x017A,0x003006FF},/*��*/
	{0x7518,1,0x0188,0x0000BEFF},/*��*/
	{0x9752,1,0x0198,0x000000FF},/*��*/
	{0x5B81,1,0x01A3,0x0000000F},/*��*/
	{0x65B0,1,0x01AB,0x0003BEFF},/*��*/
	{0x574A,2,0x024E,0xFFFFFFFF},/*�侯 ע��ȫF��ʾ��01-32*/

	/*---------����������δ�ҵ���������ȡ��2λ������ѯ------------*/
	{0x0056,1,0x01C4,0x002A547F},/*��*/
	{0x004B,1,0x01D3,0x02127EFF},/*��*/
	{0x0048,1,0x01E5,0x0002487F},/*��*/
	{0x0042,1,0x01F1,0x0122740F},/*��*/
	{0x0053,1,0x01FE,0x0122740F},/*��*/
	{0x004C,1,0x020B,0x0122740F},/*��*/
	{0x004A,1,0x0218,0x0122740F},/*��*/
	{0x004E,1,0x0125,0x0122740F},/*��*/
	{0x0047,1,0x0232,0x0122740F},/*��*/
	{0x0043,1,0x0241,0x0122740F}/*��*/
};

unsigned char g_Vehicle_Number_List_WJN[10] = {0x57,0x4A,0x31,0x34,0x31,0x32,0x33,0x30,0x45};//WJ141230E
unsigned char g_Vehicle_Number_List_Normal[10] = {0x8F,0xBD,0x45,0x31,0x32,0x33,0x30,0x45};//��E1230E
unsigned char g_Vehicle_Number_List_Force[10] = {0x53,0x4D,0x31,0x32,0x33,0x30,0x45};//��M1230E
unsigned char g_Vehicle_Number_List_WJF[10] = {0x57,0x4A,0x31,0x34,0x68,0xEE,0x32,0x33,0x30,0x45};//WJ141230E

FIRST_Part_Vehicle_NumberDef *getVehicleNumHead(void)
{
	return &g_FirstTwoVehicleNumberList[0];
}

unsigned char *getVehicle_Number_List_WJN(void)
{
	return &g_Vehicle_Number_List_WJN[0];
}

/*
 *author 			�����
 *description �ж������ͷ��λAscii�Ƿ����б���
 *param 			����2���ֽڵ�����
 *output 			���б��е�λ��
 *return 			�Ƿ�����б���
 */
unsigned char CheckIfInVehicleNumberList(unsigned int headData,unsigned char *position)
{
	unsigned char _in = 0;
	FIRST_Part_Vehicle_NumberDef *temp = getVehicleNumHead();
	int i = 0;
	for(; i<32; i++) /*ǰ32�ĳ��ƶ��ǿ����������ֽڽ����ж�*/
	{
		if(headData == temp->first_Ascii)
		{
			_in = 1;
			*position = i;
			break;
		}
		temp++;
	}
	if(_in==0)
	{
		unsigned int convertData = (headData>>8)&0x00FF;
		for(; i<VEHICLE_CARD_NUMBER; i++) /*����32�Ժ�ĳ��ƣ�ֻҪһ���ֽڼ���*/
		{
			if(convertData == temp->first_Ascii)
			{
				_in = 1;
				*position = i;
				break;
			}
			temp++;
		}
	}
	return _in;
}

unsigned char getsecondPartAsciiRealPositionInList(unsigned int mask,unsigned short int ascii)
{
	unsigned char position = -1;
	int i = 0;
	unsigned char len = sizeof(mask)*8;
	for(; i<len; i++)
	{
		if(mask&1==1)
		{
			position++;
		}
		if((unsigned char)ascii - 0x41 == i)
		{
			break;
		}
		mask>>=1;
	}
	if(i==len)//�������б���
		return -1;
	return position;
}

unsigned short int uint8ToUint16(unsigned char data1,unsigned char data2)
{
	unsigned short int assembData = 0;
	assembData |= data1;
	assembData <<= 8;
	assembData |= data2;
	return assembData;
}
/*
 *author ruanhugang
 *description ��Ascii��3132ת��ʮ����12��ע���λҪ��30����λҪ��ȥ31
 *param H_Data ��λ�ֽ�
 *param L_Data ��λ�ֽ�
 *return ����ʮ���ƽ��
 */
unsigned char combineAsciiToDec(unsigned char H_Data, unsigned char L_Data)
{
	return (H_Data-0x30)*10+(L_Data-0x31);
}



