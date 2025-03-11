#include<lpc21xx.h>
#include"LCD_4Bit_header.h"

#define sw1 14
#define sw2 15
#define sw3 16

/* 
unsigned char CGRAM_LUT[]={0x03,0x05,0x09,0x11,0x09,0x05,0x03,0x00,
								0x00,0x1f,0x01,0x01,0x01,0x1f,0x00,0x00}
*/

typedef unsigned int u32;
typedef struct can2
{
	u32 ID;
	u32 RTR;
	u32 DLC;
	u32 ByteA;
	u32 ByteB;
}CAN_MSG;

void CAN_INIT(void);
void LCD_START(void);
void CAN_TX(CAN_MSG v1);

int main()
{
	CAN_MSG v1; // assign variable v1 to can2 struct 
	
	LCD_INIT();
	CAN_INIT();
	
	LCD_CMD(0X80);
	LCD_STR("Body Control");
	
	v1.RTR = 0; //Data Frama
	v1.DLC = 4; //4 byte length
	
	while(1)
	{
		if(((IOPIN0>>sw1)&1)==0)
			{
				LCD_START();
				LCD_STR("LEFT INDICATOR");
				v1.ID=0x01;
				CAN_TX(v1);
			}
		else if(((IOPIN0>>sw2)&1)==0)
			{
				LCD_START();
				LCD_STR("RIGHT INDICATOR");
				v1.ID=0x02;
				CAN_TX(v1);
			}
		else if(((IOPIN0>>sw3)&1)==0)
			{
				LCD_START();
				LCD_STR("WIPER IN ON");
				v1.ID=0x03;
				CAN_TX(v1);
			}
	}
}

void CAN_INIT(void)
{
	PINSEL1 |= 0x00014000; //P0.23 , P0.24
	VPBDIV = 1;
	C2MOD = 0X01; //GOING TO RESET MODE
	AFMR  = 0X02; // enabled CAN controller all receiver messages are accepted(data/remote)  
	C2BTR = 0X001C001D; // Baud RATE B125Kbps @ PLCK=60MHz
	C2MOD = 0X00; //Going to normal mode
}

void LCD_START(void)
{
	LCD_CMD(0X01); // TO CLEAR THE DISPLAY
	delay_ms(100);
	LCD_CMD(0X80); // START ADDRESS OF LCD
	LCD_STR("Body Control");
	LCD_CMD(0XC0);
}

void CAN_TX(CAN_MSG v1)
{
	C2TID1 = v1.ID;
	C2TFI1 = (v1.DLC<<16);
	if(v1.RTR == 0) //IF data frame
	{
		C2TFI1 &= ~(1<<30); //RTR =  0;
		C2TDA1 = v1.ByteA;
		C2TDB2 = v1.ByteB;
	}
	else
	{
		C2TFI1 |= (1<<30); //RTR = 1;
	}
	C2CMR = (1<<0)|(1<<5); //Start TXmission & select Tx Buf1
	while((C2GSR &(1<<3))== 0 ); //WAIT FOR Data TXmission
}
