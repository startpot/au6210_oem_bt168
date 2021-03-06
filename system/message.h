#ifndef __MESSAGE_H__
#define __MESSAGE_H__


typedef enum _MSG_FIFO_INDEX
{
	MSG_FIFO_DEV_CTRL,
	MSG_FIFO_KEY,
	MSG_FIFO_RTC,

	MSG_FIFO_COUNT			//message fifo count

} MSG_FIFO_INDEX;


//define key event.
typedef enum _MESSAGE
{
	MSG_NONE = 0x00,

	MSG_PLAY_PAUSE,		
	MSG_PLAY_1,
	MSG_PLAY_1CP,
	MSG_PP_STOP,
	MSG_STOP,	
	
	MSG_NEXT,
	MSG_PRE,
	MSG_FF_START,
	MSG_FB_START,
	MSG_FF_FB_END,	
	
	MSG_EQ_SW,
	MSG_REPEAT,
	MSG_FOLDER_EN,
	MSG_NEXT_FOLDER,
	MSG_PRE_FOLDER,	
		MSG_VOL_ADD_END,
		MSG_VOL_SUB_END,

	MSG_VOL_ADD,
	MSG_VOL_SUB,
	MSG_MODE_SW,
	MSG_MUTE,
	MSG_NUM_0,	

	MSG_NUM_1,
	MSG_NUM_2,
	MSG_NUM_3,
	MSG_NUM_4,
	MSG_NUM_5,	
	
	MSG_NUM_6,
	MSG_NUM_7,
	MSG_NUM_8,
	MSG_NUM_9,
	MSG_NUM_10ADD, 	
	
	MSG_RECV_NUM,
	MSG_IR_SELECT,
	MSG_SAVE_STATION, 
	MSG_FREQ_UP, 
	MSG_FREQ_DN, 
	
	MSG_10TRACK_ADD,
	MSG_10TRACK_ADDCP,
	MSG_10TRACK_SUB,   	
	MSG_10TRACK_SUBCP,
	MSG_FREQUP_10TRK, 
	
	MSG_FREQDN_10TRK, 
	MSG_NEXT1,
	MSG_PREV1,	   		
	MSG_RANDOM,
 	MSG_EQ_CH_SUB,

 	MSG_INTRO,	
	MSG_NUM_0CP,
	MSG_NUM_1CP,
	MSG_NUM_2CP,
	MSG_NUM_3CP,
	
	MSG_NUM_4CP,  		
	MSG_NUM_5CP,
	MSG_NUM_6CP,
	MSG_NUM_7CP,
	MSG_NUM_8CP,
	
	MSG_NUM_9CP,  		
	MSG_NUM_10ADDCP,
	MSG_NUM_SAVE_STAITON,
	MSG_MODE_USB,
	MSG_MODE_SD,
	
	MSG_MODE_FM,   
	MSG_MODE_AUX,
	MSG_MODE_MP3,
	MSG_POWER,
	MSG_SCAN,		 	

	MSG_RTC_SW,
	MSG_RTC_DOWN,  		
	MSG_RTC_UP,
	MSG_RTC_SET,   
	MSG_ALARM_MODE,
	 
	MSG_ALARM_SET,
	MSG_ALARM_SW,
	MSG_PWR_KEY_SP,		
	MSG_SLEEP,	
	MSG_CLOCK,

	MSG_DIMMER,
	MSG_SNOOZE,	
	MSG_ALARM1_SET,
	MSG_ALARM2_SET,
	MSG_SYS_TIMING_EN,
	
	MSG_SYS_TIMING_SET,
	MSG_SYS_TIMING_READ,
	MSG_FM_STERO_SW,
	MSG_SYS_ON,
	MSG_SYS_OFF,

	MSG_BLUETOOTH_ANSWER,
	MSG_BLUETOOTH_REJECT,	
	MSG_BT_REJECT,

	MSG_BT_CHANGE_PHONE,

	MSG_TREBUP,
	MSG_TREBDN,
	MSG_BASSUP,
	MSG_BASSDN,
	MSG_DEFAULT,
	MSG_100TRACK_ADD,
    MSG_200TRACK_ADD,
	
	MSG_UPDATE_FLASH,
	MSG_IR_ON_OFF,
	MSG_V_ADD,
	MSG_V_SUB,
	
    MSG_BT_RECALL,
    MSG_BT_DISCONNECT
	
} MESSAGE;


// message module initial
VOID MessageInit(VOID);

// send message process
BOOL MessageSend(BYTE MsgFifoIndex, BYTE Event);

// get message process
BYTE MessageGet(BYTE MsgFifoIndex);

#endif 

