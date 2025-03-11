#include<LPC21xx.h>
#include"delay_headers.h"

#define LED 1<<19 //P0.19

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
void CAN_RX(CAN_MSG *);

int main()
{
	CAN_MSG v1;
	
	IODIR0 = LED;
	IOSET0 = LED;
	
	CAN_INIT();
	
	while(1)
		{
			CAN_RX(&v1);
			if(v1.ID == 0x02)
			{
				IOCLR0 = LED;
				delay_sec(5);
				IOSET0 = LED;
			}
		}
}

void CAN_INIT(void)
{
	PINSEL1 |= 0X00014000;
	VPBDIV = 1;
	C2MOD = 0X01;
	AFMR  = 0X02;
	C2BTR = 0X001C001D;
	C2MOD = 0X00;
}

void CAN_RX(CAN_MSG *v1)
{
	while((C2GSR&0x1)==0);
	v1->ID = C2RID;
	v1->DLC =(C2RFS>>16)&0xF; // Only collecting the DLC 
	v1->RTR =(C2RFS>>30)&0x1; // only collecting the RTR
	if(v1->RTR==0){ //If data frame COLLECT THE DATA
		v1->ByteA=C2RDA;
		v1->ByteB=C2RDB;
	}
	C2CMR=(1<<2);//Free receiver buffer(imp)
}
