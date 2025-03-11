#include<LPC21xx.h>
#include"delay_headers.h"

#define LED 1<<18 // CENTER LED OF BOARD P0.18
#define M1 1<<0  // Motor rotate clockwise
#define M2 1<<1  // Motor rotate Anti-clockwise
#define M 1<<30   //P1.31 M->For Motor

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
void MOTOR_OFF(void); // Motor turn OFF
void WIPER(void);

//void MOTOR_C(void); //Motor rotate clockwise
//void MOTOR_A(void); //Motor rotate Anti-clockwise

int main()
{
	CAN_MSG v1;
	
	IODIR0 |= M1 |M2 | LED;
	IODIR1 |= M ;
	
	MOTOR_OFF();
	CAN_INIT();
	while(1)
	{
		CAN_RX(&v1);
		if ( v1.ID == 0x03)
		{
			WIPER();
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
	v1->ID  = C2RID;
	v1->DLC =(C2RFS>>16)&0xF; // Only collecting the DLC 
	v1->RTR =(C2RFS>>30)&0x1; // only collecting the RTR
	if(v1->RTR == 0){ //If data frame COLLECT THE DATA
		v1->ByteA = C2RDA;
		v1->ByteB = C2RDB;
	}
	C2CMR=(1<<2);//Free receiver buffer(imp)
}

void MOTOR_OFF(void)
{
	IOSET0 = M1 | M2 | LED;
	IOSET1 = M;
}

void WIPER(void)
{
	int i=5;
	while(i--)
	{
		//Motor rotate clockwise
		IOCLR0 = M1 | LED ;
		IOCLR1 = M ;
		IOSET0 = M2;
		delay_ms(1000);
		//Motor rotate Anti-clockwise
		IOCLR0 = M2 | LED;
		IOCLR1 = M;
		IOSET0 = M1;
		delay_ms(1000);
		
		i--;
	}
	MOTOR_OFF();
}
