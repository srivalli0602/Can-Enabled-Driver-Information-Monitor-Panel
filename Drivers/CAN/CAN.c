#include <lpc21xx.h>
#include "can_defines.h"
#include "can.h"
void Init_CAN1(void)   
{   
    //cfg p0.25 pin as CAN1_RX pin(RD1),TD1 is exclusive    
    PINSEL1|=RD1_PIN; //using defines from can_defines.h   
                      // #define RD1_PIN 0x00040000 ,    
                      //as RD1/ (i.e CAN1_RX)/p0.25   
    //Reset CAN1 controller   
    C1MOD=1;   
    //All received messages are accepted       
    AFMR=2;    
    //Set baud Rate for CAN   
    C1BTR=BTR_LVAL; //using defines from can_defines.h   
    //Enable CAN1 controller   
    C1MOD=0;    
}

void CAN1_Tx(struct CAN_Frame txFrame)     
{      
  // Checking that the TX buffer is empty in C1GSR     
     while(((C1GSR>>TBS1_BIT)&1)==0); //if status is 1 then empty    

  // place 11-bit tx id in C1T1D1     
		C1TID1=txFrame.ID;      
  // place cfg whether data/remote frame & no of data bytes in message      
    C1TFI1=txFrame.vbf.RTR<<30|txFrame.vbf.DLC<<16;     
  // For Data Frame place 1 to 8 bytes data into Data Tx Buffers      
		if(txFrame.vbf.RTR!=1)     
    {      
          //Place data bytes 1-4 in C1TDA1      
			C1TDA1= txFrame.Data1;     
        //Place data bytes 5-8 in C1TDB1     
      C1TDB1= txFrame.Data2;     
		}
    //Select Tx Buf1 & Start Xmission using       
     C1CMR=STB1_BIT_SET|TR_BIT_SET;      
		//monitor tx status in C1GSR     
      while((C1GSR&TCS1_BIT_READ)==0);     
}

void CAN1_Rx(struct CAN_Frame *rxFrame)        
{        
//wait for CAN frame recv status        
	while((C1GSR&RBS_BIT_READ)==0);        
//read 11-bit CANid of recvd frame.         
	rxFrame->ID=C1RID;        
// read & extract data/remote frame status        
	rxFrame->vbf.RTR=(C1RFS>>30)&1;        
//read & extract data length        
	rxFrame->vbf.DLC=(C1RFS>>16)&0x0f;        
//check if recvd frame is data frame,extract data bytes        
	if(rxFrame->vbf.RTR==0)        
  {         
//read 1-4 bytes from C1RDA        
		rxFrame->Data1=C1RDA;        
//read 5-8 bytes from C1RDB        
    rxFrame->Data2=C1RDB;        
}        

// Release receive buffer         

      C1CMR=1<<RRB_BIT;         

}

int tx_check(void)
{
	return((C1GSR&TCS1_BIT_READ)); //if status is 1 then empty    
}

int rx_check(void)
{
	return((C1GSR&RBS_BIT_READ)?1:0);     
}