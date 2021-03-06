#include <reg51.h>										//include public header files
#include <string.h>
#include "type.h"
#include "syscfg.h"
#include "sysctrl.h"
#include "key.h"
#include "debug.h"
#include "message.h"
#include "uart.h"
#include "slave_cmd.h"
#include "bt.h"
#if 0//defined(FUNC_BLUETOOTH_CJ_EN) && !defined(AU6210K_NR_D_8_CSRBT)
//#ifdef	FUNC_UARTDBG_EN
//	#error	"MV: UART Conflict!"
//#endif

extern VOID UnMute(VOID);
#define MAX_CMD_DATA_LEN	32						//命令数据域最大长度
#define RX_QUEUE_LENGTH 	(MAX_CMD_DATA_LEN + 10)		//串口中断接收缓冲区长度
static BYTE BluetoothConnectedCount = 0;
static BYTE RxQueue[RX_QUEUE_LENGTH];
static BYTE RxQueueHead;
static BYTE RxQueueTail;
BYTE test_data;
BYTE gSlaveCmd[MAX_CMD_DATA_LEN + 1];					//长度域、命令码域、数据域
static BYTE SlaveRxIndex = -2;
static BYTE SlaveHead;
static BYTE dataflag = 0x55;
extern BOOL PlayPhoneNumBreakoff;
extern BYTE PlayPairFlag;
#if defined(FUNC_AUTO_BTSTANDBY)
extern TIMER   BT_standby;
#endif
BYTE Connect;
// Initialize 8051 ex UART
VOID SlaveUartInit()
{
#if 0
	SetGpioRegBit(GPIO_B_IE, 0x02);	
	ClrGpioRegBit(GPIO_B_OE, 0x02);
    ClrGpioRegBit(GPIO_B_IE, 0x01);	
	SetGpioRegBit(GPIO_B_OE, 0x01);	

	SetGpioReg(GPIO_CFG0_REG, MASK_FAST_UART_TO_B0B1); 	
	
	ModuleClkEn(MASK_UART_CLOCK);
	RxQueueHead = 0;
	RxQueueTail = 0;
	SlaveRxIndex = -2;
	
	RstFastUart();
	FastUartOpen(57600);  
	FastUartSendByte('S');
	FastUartSendByte('T');
	FastUartSendByte('A');
	FastUartSendByte('R');
	FastUartSendByte('T');
	FastUartSendByte('.');
	FastUartSendByte('.');
	FastUartSendByte('.');
	FastUartSendByte('\r');
	FastUartSendByte('\n');
#endif
    BluetoothConnectedCount = 0;
    RxQueueHead = 0;
	RxQueueTail = 0;
	SlaveRxIndex = -2;
	gBlutoothStatus = BT_IDLE;
	Connect = 0;
	//I51UartOpen(BAUDRATE_2400, UART_USE_A1A2);
	I51UartSendByte('S');
	I51UartSendByte('T');
	I51UartSendByte('A');
	I51UartSendByte('R');
	I51UartSendByte('T');
	I51UartSendByte('I');
	I51UartSendByte('N');
	I51UartSendByte('G');
	I51UartSendByte('\r');
	I51UartSendByte('\n');
	ES = 0;                 //串口中断允许
    REN= 0;        //允许接收
}


static BYTE GetRxQueueLen()
{
	return ((RxQueueTail + RX_QUEUE_LENGTH - RxQueueHead) % RX_QUEUE_LENGTH);
}
BYTE USEGetRxQueueLen()
{
	return GetRxQueueLen();
}


BYTE GetRxQueueData()
{
	BYTE val;

	val = RxQueue[RxQueueHead];
	RxQueueHead = ((RxQueueHead + 1) % RX_QUEUE_LENGTH);
	return val;
}

BOOL SlaveRcvCmd()
{
	BYTE Temp;
	BYTE CheckSum;
	BYTE i;
	TIMER Timer;

	TimeOutSet(&Timer, 500);
	while(GetRxQueueLen() > 0)
	{	I51UartSendByte(0x19);
		if(IsTimeOut(&Timer))
		{
			//DBG(("timeout\n"));
			return FALSE;
		}		
		Temp = GetRxQueueData();	
		if(SlaveRxIndex == -2)
		{
			if(Temp==0xAA)
			{
				SlaveRxIndex =-1;
				//I51UartSendByte(0xAA);
				
			}
		}
		else if(SlaveRxIndex == -1)
		{
			if(Temp==0x00)
			{   
				SlaveRxIndex = 0;
				I51UartSendByte(Temp);
			}
		}
		else 
		{	
			//SlaveHead=0;
			I51UartSendByte(Temp);
			gSlaveCmd[SlaveRxIndex++] = Temp;		//Cmd + Data[] + CS
			//如果长度域超过最大长度，则重新接收
			if((gSlaveCmd[0] > MAX_CMD_DATA_LEN))
			{	I51UartSendByte(0xab);	
				SlaveRxIndex = -2;
				continue;
			}
			if(SlaveRxIndex >= gSlaveCmd[0] + 2)	//CS接收完
			{   I51UartSendByte(0xcd);
				CheckSum = 0;
				for(i = 0; i < SlaveRxIndex-1; i++)
				{
					CheckSum += gSlaveCmd[i];
				}
				CheckSum = 0xFF - CheckSum + 1;
				SlaveRxIndex = -2;
				dataflag = 9;
				I51UartSendByte(dataflag);
				I51UartSendByte(0x99);
				return TRUE;  //如需要CheckSum则注释此行
				if(CheckSum == gSlaveCmd[SlaveRxIndex-1])
				{
					return TRUE;
				}
				else
				{

	 				return FALSE;
				}	
			}
		}
	}
	return FALSE;
}

VOID BluetoothPlayPhoneNum()
{
	BYTE Cmdbuf;
	BYTE PhoneNumSum;
	BYTE BluetoothPhoneNum[MAX_CMD_DATA_LEN + 1];
	if (gSlaveCmd[3] == 0x30)
		PhoneNumSum = 15;
	else
		PhoneNumSum = 14;
	for (Cmdbuf=0;Cmdbuf<=PhoneNumSum;Cmdbuf++)		//防止再次接收命令时gBluetoothCmd[Cmdbuf]被改变导致号码错误
	{
		BluetoothPhoneNum[Cmdbuf] = gSlaveCmd[Cmdbuf];
	}
	for(Cmdbuf=3;Cmdbuf<=PhoneNumSum && PlayPhoneNumBreakoff == FALSE;Cmdbuf++)
	{
#if defined(AU6210K_NR_D_9X_XJ_HTS)
		if (Cmdbuf == 3)
		{
			SPI_PlaySelectNum(SPIPLAY_SONG_PLAY,0);
		}
#endif
		switch(BluetoothPhoneNum[Cmdbuf])
		{
			case 0x30:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_0,0);
				break;
			case 0x31:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_1,0);
				break;
			case 0x32:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_2,0);
				break;
			case 0x33:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_3,0);
				break;
			case 0x34:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_4,0);
				break;
			case 0x35:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_5,0);
				break;
			case 0x36:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_6,0);
				break;
			case 0x37:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_7,0);
				break;
			case 0x38:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_8,0);
				break;
			case 0x39:
				SPI_PlaySelectNum(SPIPLAY_SONG_NUM_9,0);
				break;
			default:
				break;
		}
	}
	InDacChannelSel(BLUETOOTH_CJ_IO_TYPE); 
	SetBluetoothVolumeCJ(gSys.Volume);
	PlayPhoneNumBreakoff = FALSE;
}

VOID BluetoothRefreshStatus()
{
	PlayPhoneNumBreakoff = FALSE;
	SlaveRcvCmd();
  
	    //I51UartSendByte(dataflag);

	if (dataflag == 9)
	{   I51UartSendByte(0xCC);
	    I51UartSendByte(gSlaveCmd[1]);
		I51UartSendByte(gSlaveCmd[2]);
	    dataflag = 0x55;
		switch (gSlaveCmd[1])
		{   
        
			case 0x01: //Connecting status
				{
					if (gSlaveCmd[2] == 0x05)
					{
						PlayPairFlag = 0;
						Connect = 1;
						gBlutoothStatus = BT_CONNECTED;
						BluetoothConnectedCount++;
						if (BluetoothConnectedCount > 2)
							BluetoothConnectedCount = 2;
						//I51UartSendByte(gSlaveCmd[1]);
						//I51UartSendByte(gSlaveCmd[2]);
						#ifdef FUNC_SPI_KEY_SOUND_EN
						if (BluetoothConnectedCount == 1)
						{
							SPI_PlaySelectNum(SPIPLAY_SONG_PHONE_ONE_CONNECTED,0);
							InDacChannelSel(BLUETOOTH_CJ_IO_TYPE); 
							SetBluetoothVolumeCJ(gSys.Volume);
							UnMute();
						}
						else if (BluetoothConnectedCount == 2)
						{
							SPI_PlaySelectNum(SPIPLAY_SONG_PHONE_TWO_CONNECTED,0);
							InDacChannelSel(BLUETOOTH_CJ_IO_TYPE);	 
							SetBluetoothVolumeCJ(gSys.Volume);
							UnMute();
						}
						#endif	
					} 
					else if (gSlaveCmd[2] == 0x07)
					{
						#ifdef FUNC_SPI_KEY_SOUND_EN
						if (BluetoothConnectedCount == 2)
						{   Connect = 0;
						    
							#if defined(FUNC_AUTO_BTSTANDBY)
							TimeOutSet(&BT_standby, 15000);
							#endif
							SPI_PlaySelectNum(SPIPLAY_SONG_PHONE_ONE_DISCONNECTED,0);
							InDacChannelSel(BLUETOOTH_CJ_IO_TYPE); 
							SetBluetoothVolumeCJ(gSys.Volume);
							UnMute();
						}
						else if (BluetoothConnectedCount == 1)
						{   Connect = 0;
						#if defined(FUNC_AUTO_BTSTANDBY)
						    TimeOutSet(&BT_standby, 15000);
						#endif
							SPI_PlaySelectNum(SPIPLAY_SONG_PHONE_TWO_DISCONNECTED,0);
							InDacChannelSel(BLUETOOTH_CJ_IO_TYPE);
							SetBluetoothVolumeCJ(gSys.Volume);
							UnMute();
						}
						#endif	
						if (BluetoothConnectedCount > 0)
							BluetoothConnectedCount--;
						if (BluetoothConnectedCount == 0)
						{
							gBlutoothStatus = BT_IDLE;
						}
					}
				}
				break;
			case 0x02:	//Call status
				{
					if (gSlaveCmd[2] == 0x00 || gSlaveCmd[2] == 0x01) //Call 
					{   					    
						if (gSlaveCmd[3] == 0x02) //Incoming call
						{   Connect = 1;
							gBlutoothStatus = BT_INCOMING_CALL;
						}
						else if (gSlaveCmd[3] == 0x03)
						{   Connect = 1;
							gBlutoothStatus = BT_MAKE_CALL;
						}
						else if (gSlaveCmd[3] == 0x04) //Answer call
						{   Connect = 1;
							gBlutoothStatus = BT_IN_THE_CALL;
							PlayPhoneNumBreakoff = TRUE;
						}	
						else if (gSlaveCmd[3] == 0x00) //End call
						{   Connect = 1;
							gBlutoothStatus = BT_CONNECTED;
							PlayPhoneNumBreakoff = TRUE;
						}
					} 
				}
				break;
			case 0x03:	//phone num
				{
					if (gSlaveCmd[2] == 0x00)
					{   Connect = 1;
						gBlutoothStatus = BT_INCOMING_CALL;
						PlayPhoneNumBreakoff = FALSE;
					#ifdef FUNC_SPI_KEY_SOUND_EN
						BluetoothPlayPhoneNum();
					    UnMute();
					#endif
					}
				}
				break;
			default:
				break;
		}
	}
}

VOID SlaveSendResp(WORD Cmd, BYTE* Buf, BYTE Len)
{
	BYTE CheckSum;

	CheckSum = Len + (BYTE0(Cmd) | 0x80) + BYTE1(Cmd);
	
	//I51UartSendByte(0x55);
	//I51UartSendByte(0xAA);
	//I51UartSendByte(Len);
	//I51UartSendByte(BYTE0(Cmd) | 0x80);
	//I51UartSendByte(BYTE1(Cmd));
	while(Len--)
	{
		CheckSum += (*Buf);
		//I51UartSendByte(*(Buf++));
	}

	//I51UartSendByte(0 - CheckSum);
}

VOID TESTSlaveSendRespSecond()
{
	
	I51UartSendByte(test_data);

}
//	Uart received event handle function
//#pragma disable 
VOID OnDataReceived()
{
	BYTE temp = SBUF;
	test_data = SBUF;
	if (((RxQueueTail + 1) % RX_QUEUE_LENGTH) != RxQueueHead)		//queue is not full
	{
		RxQueue[RxQueueTail++] = temp;			//save data into queue
		RxQueueTail %= RX_QUEUE_LENGTH;		//queue tail++
		
	}

}


#endif