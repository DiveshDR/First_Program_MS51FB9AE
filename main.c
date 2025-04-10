#include "MS51_16K.H"

#include<stdio.h>


//----------------------------------------------------------------------------------------------//
void main (void)
{

	//P0 = 0x00;
	  ALL_GPIO_PUSHPULL_MODE;
	  P06 = 0x00;

	P3 = 0xB0;
	P17 = P1^7;
	P13 = P1^3;
	
  while(1)
  {
   
		P06 = 0xff;
    Timer2_Delay(24000000,16,400,1000);        
    P06 = 0x00;
    Timer2_Delay(24000000,16,400,1000);
			
		P3 = 0xff;
		P17 = 0x00;
		Timer2_Delay(24000000,16,400,2000);  
	P3 = 0x00;		
		P17 = 0xff;
		
		if(P3==0xff);
//	{P13=0x00; }
	if(P3==0x00);
	{P13=0
  }
	
	
}
