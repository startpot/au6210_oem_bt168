#include <string.h>
#include <stdio.h>

#include "type.h"										//include project header files
#include "utility.h"
#include "syscfg.h"
#include "sysctrl.h"
#include "timer.h"
#include "display.h"
#include "seg_panel.h"
#include "sled_disp.h"
#include "rtc.h"
#include "playctrl.h"
#include "breakpoint.h"
#include "device_audio.h"
#include "debug.h"
#include "radio_ctrl.h"
#include "device.h"

#include "power_monitor.h"

#ifdef FUNC_NPCA110X_EN
extern BOOL isDefaultBass;
#endif
#ifdef FUNC_SINGLE_LED_EN

#define SLedFlickQuick 			gBlink.Blink.HourSet			//复用做SLED MP3快进快退/Radio搜台闪烁标志(快速闪烁)
#define SLedQuickoff 			gBlink.Blink.HourBlankScreen
#define SLedFlickNormal 		gBlink.Blink.MinSet			//复用做SLED MP3播放闪烁标志(常速闪烁)
#define SLedNormaloff 			gBlink.Blink.MinBlankScreen
#define SLedFlickSlow 			gBlink.Blink.MuteBlink		//复用做SLED MP3 PAUSE/STOP/MUTE闪烁标志(慢速闪烁)
#define SLedSlowoff 			gBlink.Blink.MuteBlankScreen
#define SLedRepeatFlick			gBlink.Blink.RepeatBlink
#define SLedRepeatOff			gBlink.Blink.RepeatOff	

#ifdef	FUNC_GPIO_KEY_EN
BOOL SLedOnOffFlag = FALSE;		//按键端口与单个LED端口共用时LED状态标志
#endif

extern 	BYTE ChannelType;
// LED灯亮或灭.
// led ->LED灯,isLightOn -> TRUE: lighton.
VOID SLedLightOp(SLED_IDX Led, BOOL IsLightOn)
{	
#ifdef	FUNC_GPIO_KEY_EN
//按键端口与单个LED端口共用时端口设置输出
//	baGPIOCtrl[GPIO_B_IE] &= ~0x40;
//	baGPIOCtrl[GPIO_B_OE] |= 0x40;
#endif
	
	if(Led == LED_POWER)
	{
		if(IsLightOn)
		{
			//DBG(("SLED_POWER ON\n"));
#if defined(FUNC_SINGLE_LED_HIGHON_EN)
			SetGpioRegBit(SLED_POWER_PORT, MASK_SLED_POWER);
#else
		
			if(gSys.SystemMode != SYS_MODE_BLUETOOTH)
				SetGpioRegBit(SLED_POWER_PORT, MASK_SLED_POWER);
			else
				ClrGpioRegBit(SLED_POWER_PORT, MASK_SLED_POWER);
#endif
		}
		else
		{
			//DBG(("SLED_POWER OFF\n"));
#if defined(FUNC_SINGLE_LED_HIGHON_EN)
			ClrGpioRegBit(SLED_POWER_PORT, MASK_SLED_POWER);
#else
			SetGpioRegBit(SLED_POWER_PORT, MASK_SLED_POWER);
#endif
		}
		
#ifdef	FUNC_GPIO_KEY_EN
//		SLedOnOffFlag = IsLightOn;			
#endif
	}

#ifdef SLED_MP3_EN
	if(Led == LED_MP3)
	{
		DBG(("SLED_MP3 "));
		if(IsLightOn)
		{
			DBG(("ON\n"));
			ClrGpioRegBit(SLED_MP3_PORT, MASK_SLED_MP3);
		}
		else
		{
			DBG(("OFF\n"));
			SetGpioRegBit(SLED_MP3_PORT, MASK_SLED_MP3);
		}
	}
#endif

#ifdef SLED_USB_EN
	if(Led == LED_USB)
	{
		DBG(("SLED_USB "));
		if(IsLightOn)
		{
			DBG(("ON\n"));
			ClrGpioRegBit(SLED_USB_PORT, MASK_SLED_USB);
		}
		else
		{
			DBG(("OFF\n"));
			SetGpioRegBit(SLED_USB_PORT, MASK_SLED_USB);
		}
	}
#endif

#ifdef SLED_SD_EN
	if(Led == LED_SD)
	{
		DBG(("SLED_SD "));
		if(IsLightOn)
		{
			DBG(("ON\n"));
			ClrGpioRegBit(SLED_SD_PORT, MASK_SLED_SD);
		}
		else
		{
			DBG(("OFF\n"));
			SetGpioRegBit(SLED_SD_PORT, MASK_SLED_SD);
		}
	}
#endif

#ifdef SLED_AUX_EN
	if(Led == LED_AUX)
	{
		DBG(("SLED_AUX "));
		if(IsLightOn)
		{
			DBG(("ON\n"));
			ClrGpioRegBit(SLED_AUX_PORT, MASK_SLED_AUX);
		}
		else
		{
			DBG(("OFF\n"));
			SetGpioRegBit(SLED_AUX_PORT, MASK_SLED_AUX);
		}
	}
#endif

#ifdef SLED_RAD_EN
	if(Led == LED_RADIO)
	{
		DBG(("SLED_RADIO "));
		if(IsLightOn)
		{
			DBG(("ON\n"));
			ClrGpioRegBit(SLED_RADIO_PORT, MASK_SLED_RADIO);
		}
		else
		{
			DBG(("OFF\n"));
			SetGpioRegBit(SLED_RADIO_PORT, MASK_SLED_RADIO);
		}
	}
#endif

#ifdef SLED_REP_EN
	if(Led == LED_REPEAT)
	{
		DBG(("SLED_REPEAT "));
		if(IsLightOn)
		{
			DBG(("ON\n"));
			ClrGpioRegBit(SLED_REPEAT_PORT, MASK_SLED_REPEAT);
		}
		else
		{
			DBG(("OFF\n"));
			SetGpioRegBit(SLED_REPEAT_PORT, MASK_SLED_REPEAT);
		}
	}
#endif

#ifdef SLED_AUD_EN
	if(Led == LED_AUDIO)
	{
		DBG(("SLED_AUDIO "));
		if(IsLightOn)
		{
			DBG(("ON\n"));
			ClrGpioRegBit(SLED_AUDIO_PORT, MASK_SLED_AUDIO);
		}
		else
		{
			DBG(("OFF\n"));
			SetGpioRegBit(SLED_AUDIO_PORT, MASK_SLED_AUDIO);
		}
	}
#endif		
}


// 初始化.
VOID SLedInit(VOID)
{	
#if defined(FUNC_SINGLE_LED_HIGHON_EN)
	ClrGpioRegBit(SLED_PORT_PU, MASK_SLED_POWER);		// Pull-Down.
	ClrGpioRegBit(SLED_PORT_PD, MASK_SLED_POWER);		// Pull-Down.
	ClrGpioRegBit(SLED_PORT_IE, MASK_SLED_POWER);		// Input Disable
	SetGpioRegBit(SLED_PORT_OE, MASK_SLED_POWER);		// Output Enable. // D[2:7]
	SetGpioRegBit(SLED_PORT_OUT, MASK_SLED_POWER);		// Output 0
#else
#if 0//defined(AU6210K_ZB_BT007_CSR)
	ClrGpioRegBit(SLED_PORT_PU, MASK_SLED_POWER);		// Pull-Down.
	ClrGpioRegBit(SLED_PORT_PD, MASK_SLED_POWER);		// Pull-Down.
	ClrGpioRegBit(SLED_PORT_IE, MASK_SLED_POWER);		// Input Disable
	SetGpioRegBit(SLED_PORT_OE, MASK_SLED_POWER);		// Output Enable. // D[2:7]
	SetGpioRegBit(SLED_PORT_OUT, MASK_SLED_POWER);		// Output 0
#else
	SetGpioRegBit(SLED_PORT_PU, MASK_SLED_POWER);		// Pull-Down.
	SetGpioRegBit(SLED_PORT_PD, MASK_SLED_POWER);		// Pull-Down.
	ClrGpioRegBit(SLED_PORT_IE, MASK_SLED_POWER);		// Input Disable
	SetGpioRegBit(SLED_PORT_OE, MASK_SLED_POWER);		// Output Enable. // D[2:7]
	ClrGpioRegBit(SLED_PORT_OUT, MASK_SLED_POWER);		// Output 0
#endif	
#endif

	//SetGpioRegBit(SLED_PORT_DS, MASK_ALLSLED);
#if defined(AU6210K_LK_SJ_CSRBT)||defined(AU6210K_ZB_BT007_CSR)
#ifdef SLED_MP3_EN
#if defined(FUNC_SINGLE_LED_HIGHON_EN)
	ClrGpioRegBit(SLED_MP3_PU, MASK_SLED_MP3);		// Pull-Down.
	ClrGpioRegBit(SLED_MP3_PD, MASK_SLED_MP3);		// Pull-Down.
	ClrGpioRegBit(SLED_MP3_IE, MASK_SLED_MP3);		// Input Disable
	SetGpioRegBit(SLED_MP3_OE, MASK_SLED_MP3);		// Output Enable. // D[2:7]
	SetGpioRegBit(SLED_MP3_OUT, MASK_SLED_MP3);		// Output 0
#else
	SetGpioRegBit(SLED_MP3_PU, MASK_SLED_MP3);		// Pull-Down.
	SetGpioRegBit(SLED_MP3_PD, MASK_SLED_MP3);		// Pull-Down.
	ClrGpioRegBit(SLED_MP3_IE, MASK_SLED_MP3);		// Input Disable
	SetGpioRegBit(SLED_MP3_OE, MASK_SLED_MP3);		// Output Enable. // D[2:7]
	ClrGpioRegBit(SLED_MP3_OUT, MASK_SLED_MP3);		// Output 0
#endif	

#endif
#endif

#ifdef AU6210K_NR_D_8_CSRBT
	SetGpioRegBit(GPIO_D_PU, (1<<1));		// Pull-Down.
	SetGpioRegBit(GPIO_D_PD, (1<<1));		// Pull-Down.
	ClrGpioRegBit(GPIO_D_IE, (1<<1));		// Input Disable
	SetGpioRegBit(GPIO_D_OE, (1<<1));		// Output Enable. // D[2:7]
	ClrGpioRegBit(GPIO_D_OUT, (1<<1));		// Output 0

#endif
//	

//	#ifdef SLED_USB_EN
//	
//	#endif
//	#ifdef SLED_SD_EN
//	
//	#endif
//	#ifdef SLED_AUX_EN
//	
//	#endif
//	#ifdef SLED_RAD_EN
//	
//	#endif
//	#ifdef SLED_REP_EN
//	
//	#endif
//	#ifdef SLED_AUD_EN
//	
//	#endif
}


// 反初始化.
VOID SLedDeinit(VOID)
{	
	SLedLightOp(LED_POWER, LIGHTOFF);
#ifdef SLED_MP3_EN
	SLedLightOp(LED_MP3, LIGHTOFF);
#endif

#ifdef SLED_USB_EN
	SLedLightOp(LED_USB, LIGHTOFF);
#endif
#ifdef SLED_SD_EN
	SLedLightOp(LED_SD, LIGHTOFF);
#endif

#ifdef SLED_AUX_EN
	SLedLightOp(LED_AUX, LIGHTOFF);
#endif

#ifdef SLED_RAD_EN
	SLedLightOp(LED_RADIO, LIGHTOFF);
#endif

#ifdef SLED_REP_EN
	SLedLightOp(LED_REPEAT, LIGHTOFF);
#endif
#ifdef AU6210K_NR_D_8_CSRBT
	ClrGpioRegBit(GPIO_D_OUT, (1<<1));		// Output 0
#endif
/*
	baGPIOCtrl[GPIO_A_IE] |= 0x40;//A2
	baGPIOCtrl[GPIO_A_OE] &= ~0x40;
	baGPIOCtrl[GPIO_A_PU] |= 0x40;
	baGPIOCtrl[GPIO_A_PD] |= 0x40; 
	WaitUs(20);
*/

}


// 切换设备时调用.
VOID SLedDispDevSymbol(VOID)
{
	switch(gSys.SystemMode)
	{
#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
#ifdef FUNC_USB_EN
		case SYS_MODE_USB:
#ifdef SLED_MP3_EN
			SLedLightOp(LED_MP3, LIGHTON);
#endif
	 
#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTON);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTOFF);
#endif

#ifdef SLED_AUX_EN
			SLedLightOp(LED_AUX, LIGHTOFF);
#endif

#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTOFF);
#endif

#ifdef SLED_REP_EN
			SLedLightOp(LED_REPEAT, LIGHTOFF);
#endif
			break;
#endif

#ifdef FUNC_CARD_EN
		case SYS_MODE_SD:
#ifdef SLED_MP3_EN
			SLedLightOp(LED_MP3, LIGHTON);
#endif
	
#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTOFF);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTON);
#endif

#ifdef SLED_AUX_EN
			SLedLightOp(LED_AUX, LIGHTOFF);
#endif

#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTOFF);
#endif

#ifdef SLED_REP_EN
			SLedLightOp(LED_REPEAT, LIGHTOFF);
#endif
			break;
#endif
#endif
	
#ifdef FUNC_LINEIN_EN
		case SYS_MODE_LINEIN:
#ifdef SLED_MP3_EN
			SLedLightOp(LED_MP3, LIGHTOFF);
#endif
 
#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTOFF);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTOFF);
#endif
	
#ifdef SLED_AUX_EN
			SLedLightOp(LED_AUX, LIGHTON);
#endif

#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTOFF);
#endif

#ifdef SLED_REP_EN
			SLedLightOp(LED_REPEAT, LIGHTOFF);
#endif
			break;
#endif

#ifdef FUNC_RADIO_EN
		case SYS_MODE_RADIO:
#ifdef SLED_MP3_EN
			SLedLightOp(LED_MP3, LIGHTOFF);
#endif 

#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTOFF);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTOFF);
#endif
	
#ifdef SLED_AUX_EN
			SLedLightOp(LED_AUX, LIGHTOFF);
#endif

#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTON);
#endif

#ifdef SLED_REP_EN
			SLedLightOp(LED_REPEAT, LIGHTOFF);
#endif
			break;
#endif

#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
		case SYS_MODE_AUDIO:
		case SYS_MODE_READER:
		case SYS_MODE_AUDIO_READER:
#ifdef SLED_MP3_EN
			SLedLightOp(LED_MP3, LIGHTOFF);
#endif

#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTOFF);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTOFF);
#endif

#ifdef SLED_AUX_EN
			SLedLightOp(LED_AUX, LIGHTOFF);
#endif

#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTOFF);
#endif

#ifdef SLED_REP_EN		
			SLedLightOp(LED_REPEAT, LIGHTOFF);
#endif
			break;
#endif

#ifdef FUNC_RTC_EN
		case SYS_MODE_RTC:
#ifdef SLED_MP3_EN
			SLedLightOp(LED_MP3, LIGHTOFF);
#endif

#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTOFF);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTOFF);
#endif

#ifdef SLED_AUX_EN
			SLedLightOp(LED_AUX, LIGHTOFF);
#endif

#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTOFF);
#endif

#ifdef SLED_REP_EN		
			SLedLightOp(LED_REPEAT, LIGHTOFF);
#endif
			break;
#endif

		case SYS_MODE_NONE:
		default:
#ifdef SLED_MP3_EN
			SLedLightOp(LED_MP3, LIGHTOFF);
#endif

#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTOFF);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTOFF);
#endif

#ifdef SLED_AUX_EN
			SLedLightOp(LED_AUX, LIGHTOFF);
#endif

#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTOFF);
#endif

#ifdef SLED_REP_EN 		
			SLedLightOp(LED_REPEAT, LIGHTOFF);
#endif
			break;
	}

	SLedLightOp(LED_POWER, LIGHTON);
	gBlink.BlinkFlag = 0;
	TimeOutSet(&DispTmr, NORMAL_INTERVAL);
}


// MP3设备下播放状态改变时调用.
#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
VOID SLedDispPlayState(VOID)
{
	if((gSys.SystemMode != SYS_MODE_USB) && (gSys.SystemMode != SYS_MODE_SD))
	{
		return;
	}

	if(gPlayCtrl.State == PLAY_STATE_PLAY)//TF card playing led show
	{
#if defined(AU6210K_LK_SJ_CSRBT)||defined(AU6210K_ZB_BT007_CSR)
		SLedFlickQuick = 0;
		SLedQuickoff = 0;
		SLedFlickNormal = 0;
		SLedNormaloff = 0;
		SLedFlickSlow = 1;
		SLedSlowoff = 1;
#else
		SLedFlickQuick = 0;
		SLedQuickoff = 0;
		SLedFlickNormal = 1;
		SLedNormaloff = 1;
		SLedFlickSlow = 0;
		SLedSlowoff = 0;
#endif		
		SLedLightOp(LED_POWER, LIGHTOFF);
#ifdef SLED_MP3_EN
		SLedLightOp(LED_MP3, LIGHTOFF);
#endif
	
#ifdef SLED_USB_EN
		if(gSys.SystemMode == SYS_MODE_USB)
		{
			SLedLightOp(LED_USB, LIGHTOFF);
		}
		else //if(gSys.SystemMode == SYS_MODE_SD)
#endif
	
#ifdef SLED_SD_EN	
		{
			SLedLightOp(LED_SD, LIGHTOFF);
		}
#endif

#ifdef AU6210K_NR_D_8_CSRBT
		TimeOutSet(&DispTmr, SLOW_BLINK_INTERVAL);	
#else
		TimeOutSet(&DispTmr, NORMAL_BLINK_INTERVAL);	
#endif
	}
#ifdef FUNC_FAST_PLAY_EN
	else if((gPlayCtrl.State == PLAY_STATE_FF) || (gPlayCtrl.State == PLAY_STATE_FB))
	{
		//DBG(("SLED FF/FB\n"));
		SLedFlickQuick = 1;
		SLedQuickoff = 1;
		SLedFlickNormal = 0;
		SLedNormaloff = 0;
		SLedFlickSlow = 0;
		SLedSlowoff = 0;
		SLedLightOp(LED_POWER, LIGHTOFF);
#ifdef SLED_MP3_EN
		SLedLightOp(LED_MP3, LIGHTOFF);
#endif
	
#ifdef SLED_USB_EN
		if(gSys.SystemMode == SYS_MODE_USB)
		{
			SLedLightOp(LED_USB, LIGHTOFF);
		}
		else// if(gSys.SystemMode == SYS_MODE_SD)
#endif

#ifdef SLED_SD_EN
		{
			SLedLightOp(LED_SD, LIGHTOFF);
		}
#endif
		TimeOutSet(&DispTmr, QUICK_BLINK_INTERVAL);
	}
#endif
	else if((gPlayCtrl.State == PLAY_STATE_STOP) || (gPlayCtrl.State == PLAY_STATE_PAUSE))//TF pause led show
	{
		SLedFlickQuick = 0;
		SLedQuickoff = 0;
		SLedFlickNormal = 0;
		SLedNormaloff = 0;
		SLedFlickSlow = 0;
		SLedSlowoff = 0;
#ifdef AU6210K_ZB_BT007_CSR
		SLedLightOp(LED_POWER, LIGHTOFF);
#else
		SLedLightOp(LED_POWER, LIGHTON);
#endif
#ifdef SLED_MP3_EN
		SLedLightOp(LED_MP3, LIGHTON);
#endif

#ifdef SLED_USB_EN
		if(gSys.SystemMode == SYS_MODE_USB)
		{
			SLedLightOp(LED_USB, LIGHTON);
		}
		else //if(gSys.SystemMode == SYS_MODE_SD)
#endif

#ifdef SLED_SD_EN
		{
			SLedLightOp(LED_SD, LIGHTON);
		}
#endif
		TimeOutSet(&DispTmr, 0);
	}
	else if((gPlayCtrl.State == PLAY_STATE_IDLE) || (gPlayCtrl.State == PLAY_STATE_BROWSE))
	{
		SLedFlickQuick = 0;
		SLedQuickoff = 0;
		SLedFlickNormal = 0;
		SLedNormaloff = 0;
		SLedFlickSlow = 0;
		SLedSlowoff = 0;
		SLedLightOp(LED_POWER, LIGHTON);
#ifdef SLED_MP3_EN
		SLedLightOp(LED_MP3, LIGHTON);
#endif
		if(gSys.SystemMode == SYS_MODE_USB)
		{
#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTON);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTOFF);
#endif
		}
		else// if(gSys.SystemMode == SYS_MODE_SD)
		{
#ifdef SLED_USB_EN
			SLedLightOp(LED_USB, LIGHTOFF);
#endif

#ifdef SLED_SD_EN
			SLedLightOp(LED_SD, LIGHTON);
#endif
		}
		TimeOutSet(&DispTmr, 0);	
	}
}
#endif


// MP3设备下Repeat模式改变时调用.
// Repeat也可以根据具体Repeat模式不同设定闪烁的频率，但是必须要用单独的定时器，不能共用MP3播放状态的定时器。
// 目前代码显示部分共用一个定时器，所以Repeat模式闪烁的频率会取决于MP3播放状态，SLedDispRepeat/SLedDispPlayState
// 调用次序也会受到显示。
#ifdef SLED_REP_EN
VOID SLedDispRepeat(VOID)
{
	if((gSys.SystemMode != SYS_MODE_USB) && (gSys.SystemMode != SYS_MODE_SD))
	{
		return;
	}

	if(gPlayCtrl.RepeatMode == REPEAT_ALL)
	{
		SLedLightOp(LED_REPEAT, LIGHTOFF);
		SLedRepeatFlick = 0;
		SLedRepeatOff = 1;
	}
	else if(gPlayCtrl.RepeatMode == REPEAT_ONE)
	{
		SLedLightOp(LED_REPEAT, LIGHTON);
		SLedRepeatFlick = 0;
		SLedRepeatOff = 0;
		TimeOutSet(&DispTmr, 0);
	}
	else// if(gPlayCtrl.RepeatMode == REPEAT_RANDOM)
	{
		SLedLightOp(LED_REPEAT, LIGHTOFF);
		SLedRepeatFlick = 1;
		SLedRepeatOff = 1;
		TimeOutSet(&DispTmr, NORMAL_BLINK_INTERVAL);
	}
}
#endif


// LineIn设备显示.
#ifdef FUNC_LINEIN_EN
VOID SLedDispLineIn(VOID)//linein playing led show
{
	if(gSys.SystemMode != SYS_MODE_LINEIN)
	{
		return;
	}



#if defined(AU6210K_ZB_BT007_CSR) || defined(AU6210K_LK_SJ_CSRBT)
	if(gSys.MuteFg == TRUE)
	{

		gBlink.BlinkFlag = 0;
		SLedFlickQuick = 0;
		SLedQuickoff = 0;
		SLedFlickNormal = 0;
		SLedNormaloff = 0;
		SLedFlickSlow = 0;
		SLedSlowoff = 0;
#if defined(AU6210K_ZB_BT007_CSR)||defined(AU6210K_LK_SJ_CSRBT)
		SLedLightOp(LED_POWER, LIGHTOFF);
#else
		SLedLightOp(LED_POWER, LIGHTON);
#endif


#ifdef SLED_MP3_EN
#if 1//defined(AU6210K_LK_SJ_CSRBT)
		SLedLightOp(LED_MP3, LIGHTON);
#else
		SLedLightOp(LED_MP3, LIGHTOFF);
#endif
#endif

		
#ifdef SLED_AUX_EN
		SLedLightOp(LED_AUX, LIGHTOFF);
#endif
		TimeOutSet(&DispTmr, NORMAL_BLINK_INTERVAL);		
	}
	else
	{
		gBlink.BlinkFlag = 0;
		SLedFlickQuick = 0;
		SLedQuickoff = 0;
		SLedFlickNormal = 0;
		SLedNormaloff = 0;
		SLedFlickSlow = 1;
		SLedSlowoff = 1;

#if defined(AU6210K_LK_SJ_CSRBT)
		SLedLightOp(LED_POWER, LIGHTOFF);
#else
		SLedLightOp(LED_POWER, LIGHTON);
#endif
		
#ifdef SLED_MP3_EN
#if defined(AU6210K_LK_SJ_CSRBT)
		SLedLightOp(LED_MP3, LIGHTON);
#else
		SLedLightOp(LED_MP3, LIGHTOFF);
#endif
#endif
		
#ifdef SLED_AUX_EN
		SLedLightOp(LED_AUX, LIGHTON);
#endif
		TimeOutSet(&DispTmr, 0);
	}
#else
	gBlink.BlinkFlag = 0;

#endif


#if defined(AU6210K_NR_D_8_CSRBT)
	SLedLightOp(LED_POWER, LIGHTON);
#endif
}
#endif


// Radio设备显示.
#ifdef FUNC_RADIO_EN
VOID SLedDispRadio(VOID)
{
	if(gSys.SystemMode != SYS_MODE_RADIO)
	{
		return;
	}

	if(gRadioCtrl.State == RADIO_INIT)
	{
		gBlink.BlinkFlag = 0;
		SLedLightOp(LED_MP3, LIGHTON);
#ifdef SLED_RAD_EN
		SLedLightOp(LED_RADIO, LIGHTON);
#endif
		TimeOutSet(&DispTmr, 0);
	}
	else if(gRadioCtrl.State == RADIO_IDLE)
	{
		gBlink.BlinkFlag = 0;		
		if(gSys.MuteFg == TRUE)
		{
			SLedLightOp(LED_MP3, LIGHTOFF);
#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTOFF);
#endif
			SLedFlickSlow = 1;
			SLedSlowoff = 1;
			TimeOutSet(&DispTmr, SLOW_BLINK_INTERVAL);	
		}
		else
		{
			SLedLightOp(LED_MP3, LIGHTON);
#ifdef SLED_RAD_EN
			SLedLightOp(LED_RADIO, LIGHTON);
#endif
			TimeOutSet(&DispTmr, 0);
		}	
	}
	else
	{
#ifdef	RADIO_SEEK_WAIT_EN		
		if(RadioDisFlag == RadioDisCurChAutoscan)
		{			
			gBlink.BlinkFlag = 0;
			SLedLightOp(LED_MP3, LIGHTON);
			return;
		}
#endif
		
		SLedLightOp(LED_MP3, LIGHTOFF);
#ifdef SLED_RAD_EN
		SLedLightOp(LED_RADIO, LIGHTOFF);
#endif
		gBlink.BlinkFlag = 0;
		SLedFlickQuick = 1;
		SLedQuickoff = 1;
		TimeOutSet(&DispTmr, QUICK_BLINK_INTERVAL);
	}
}
#endif


// Radio设备显示.
#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
VOID SLedDispAudio(VOID)
{
	if((gSys.SystemMode != SYS_MODE_AUDIO)
	&& (gSys.SystemMode != SYS_MODE_READER)
	&& (gSys.SystemMode != SYS_MODE_AUDIO_READER))
	{
		return;
	}

	if(gAudioState == AUDIO_IDLE)
	{
		//DBG(("AUD,IDLE\n"));
		gBlink.BlinkFlag = 0;
		SLedLightOp(LED_POWER, LIGHTON);
#ifdef SLED_AUD_EN
		SLedLightOp(LED_AUDIO, LIGHTON);
#endif
		TimeOutSet(&DispTmr, 0);
	}
	else if(gAudioState == AUDIO_PLAY)
	{
		SLedLightOp(LED_POWER, LIGHTOFF);
#ifdef SLED_AUD_EN
		SLedLightOp(LED_AUDIO, LIGHTOFF);
#endif
		gBlink.BlinkFlag = 0;
		if(gSys.MuteFg == TRUE)
		{
			//DBG(("AUD,MUTE\n"));
			SLedFlickSlow = 1;
			SLedSlowoff = 1;
			TimeOutSet(&DispTmr, SLOW_BLINK_INTERVAL);
		}
		else
		{
			SLedFlickNormal = 1;
			SLedNormaloff = 1;
			TimeOutSet(&DispTmr, NORMAL_BLINK_INTERVAL);
		}
	}	
}
#endif


// RTC设备显示.
#ifdef FUNC_RTC_EN
VOID SLedDispRTC(VOID)
{
	if(gSys.SystemMode != SYS_MODE_RTC)
	{
		return;
	}

	SLedLightOp(LED_POWER, LIGHTON);	
}
#endif


// MUTE状态转变时调用.
VOID SLedDispMute(VOID)
{
	switch(gSys.SystemMode)
	{
#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
		case SYS_MODE_USB:
		case SYS_MODE_SD:
			if(gSys.MuteFg == TRUE)
			{
				//DBG(("SLED MUTE\n"));
				SLedFlickQuick = 0;
				SLedQuickoff = 0;
				SLedFlickNormal = 0;
				SLedNormaloff = 0;
				SLedFlickSlow = 1;
				SLedSlowoff = 1;
				SLedLightOp(LED_POWER, LIGHTOFF);
#ifdef SLED_MP3_EN
				SLedLightOp(LED_MP3, LIGHTOFF);
#endif

#ifdef SLED_USB_EN
				if(gSys.SystemMode == SYS_MODE_USB)
				{
					SLedLightOp(LED_USB, LIGHTOFF);
				}
				else
#endif

#ifdef SLED_SD_EN
				{
					SLedLightOp(LED_SD, LIGHTOFF);
				}
#endif
				TimeOutSet(&DispTmr, SLOW_BLINK_INTERVAL);
			}
			else
			{
				SLedFlickQuick = 0;
				SLedQuickoff = 0;
				SLedFlickNormal = 0;
				SLedNormaloff = 0;
				SLedFlickSlow = 0;
				SLedSlowoff = 0;
				SLedLightOp(LED_POWER, LIGHTON);
#ifdef SLED_MP3_EN
				SLedLightOp(LED_MP3, LIGHTON);
#endif

#ifdef SLED_USB_EN
				if(gSys.SystemMode == SYS_MODE_USB)
				{
					SLedLightOp(LED_USB, LIGHTON);
				}
				else
#endif

#ifdef SLED_SD_EN
				{
					SLedLightOp(LED_SD, LIGHTON);
				}
#endif
				TimeOutSet(&DispTmr, 0);
			}
			break;
#endif
	
#ifdef FUNC_LINEIN_EN
		case SYS_MODE_LINEIN://linein pause led show
			if(gSys.MuteFg == TRUE)
			{
				gBlink.BlinkFlag = 0;
				SLedFlickNormal= 0;
				SLedNormaloff = 0;
				SLedLightOp(LED_POWER, LIGHTON);
#ifdef SLED_AUX_EN
				SLedLightOp(LED_AUX, LIGHTOFF);
#endif
				TimeOutSet(&DispTmr, SLOW_BLINK_INTERVAL);
			}
			else
			{
				gBlink.BlinkFlag = 0;
				SLedLightOp(LED_POWER, LIGHTON);
#ifdef SLED_AUX_EN
				SLedLightOp(LED_AUX, LIGHTON);
#endif
				TimeOutSet(&DispTmr, 0);
			}
			break;
#endif

#ifdef FUNC_RADIO_EN
		case SYS_MODE_RADIO:
			if(gSys.MuteFg == TRUE)
			{
				gBlink.BlinkFlag = 0;
				SLedFlickSlow = 1;
				SLedSlowoff = 1;
				SLedLightOp(LED_POWER, LIGHTOFF);
#ifdef SLED_RAD_EN
				SLedLightOp(LED_RADIO, LIGHTOFF);
#endif
				TimeOutSet(&DispTmr, SLOW_BLINK_INTERVAL);
			}
			else
			{
				gBlink.BlinkFlag = 0;
				SLedLightOp(LED_POWER, LIGHTON);
#ifdef SLED_RAD_EN
				SLedLightOp(LED_RADIO, LIGHTON);
#endif
				TimeOutSet(&DispTmr, 0);
			}
			break;
#endif

#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
		case SYS_MODE_AUDIO:
		case SYS_MODE_READER:
		case SYS_MODE_AUDIO_READER:
			if(gSys.MuteFg == TRUE)
			{
				gBlink.BlinkFlag = 0;
				SLedFlickSlow = 1;
				SLedSlowoff = 1;
				SLedLightOp(LED_POWER, LIGHTOFF);
#ifdef SLED_AUD_EN
				SLedLightOp(LED_AUDIO, LIGHTOFF);
#endif
				TimeOutSet(&DispTmr, SLOW_BLINK_INTERVAL);
			}
			else
			{
				gBlink.BlinkFlag = 0;
				SLedLightOp(LED_POWER, LIGHTON);
#ifdef SLED_AUD_EN
				SLedLightOp(LED_AUDIO, LIGHTON);
#endif
				TimeOutSet(&DispTmr, 0);
			}
			break;
#endif

#ifdef FUNC_RTC_EN
		case SYS_MODE_RTC:

		//	break;
#endif
		default:
			break;
	}

}

VOID BassLed_CallBak(VOID)
{
#ifdef FUNC_NPCA110X_EN
	DBG1(("isDefaultBass = %x",isDefaultBass));
	if(isDefaultBass)
	{
		ClrGpioRegBit(GPIO_D_OUT, (1<<1));		// Output 0
	}
	else
	{
		SetGpioRegBit(GPIO_D_OUT, (1<<1));		// Output 0
	}
#endif
}

// 显示.
VOID SLedDisplay(VOID)
{
	BOOL Light;
#if defined(AU6210K_AT_BT809)
#ifdef FUNC_POWER_MONITOR_EN
	if(GetPwrDisp() == PWR_MNT_DISP_EMPTY_V)
	{
		if(gBlink.BlinkFlag)
		{
			if(!IsTimeOut(&DispTmr))
			{
				return;
			}
			if(SLedFlickQuick)
			{
				//DBG(("SLedFlickQuick\n"));
				TimeOutSet(&DispTmr, QUICK_BLINK_INTERVAL);
				SLedQuickoff = !SLedQuickoff;
				Light = SLedQuickoff;
			}
			//SLedLightOp(LED_POWER, Light);
			SLedLightOp(LED_MP3, Light);

		}
		else
		{
			SLedFlickQuick = 1;
			SLedQuickoff = 1;
			SLedFlickNormal = 0;
			SLedNormaloff = 0;
			SLedFlickSlow = 0;
			SLedSlowoff = 0;
			SLedLightOp(LED_POWER, LIGHTOFF);
			SLedLightOp(LED_MP3, LIGHTOFF);
			TimeOutSet(&DispTmr, QUICK_BLINK_INTERVAL);
		}
		return;

	}
#endif	
	
#endif	
	if(gBlink.BlinkFlag)
	{
		if(!IsTimeOut(&DispTmr))
		{
			return;
		}

		if(SLedFlickQuick)
		{
			//DBG(("SLedFlickQuick\n"));
			TimeOutSet(&DispTmr, QUICK_BLINK_INTERVAL);
			SLedQuickoff = !SLedQuickoff;
			Light = SLedQuickoff;
		}
		else if(SLedFlickSlow)
		{
			//DBG(("SLedFlickSlow\n"));
			TimeOutSet(&DispTmr, SLOW_BLINK_INTERVAL);
			SLedSlowoff = !SLedSlowoff;
			Light = SLedSlowoff;
		}
		else
		{
			//DBG(("SLedFlickNormal\n"));
			TimeOutSet(&DispTmr, NORMAL_BLINK_INTERVAL);
			SLedNormaloff = !SLedNormaloff;
			Light = SLedNormaloff;
		}

#ifdef SLED_REP_EN
		if(SLedRepeatFlick)
		{
			SLedRepeatOff = !SLedRepeatOff;
			//DBG(("SLedRepeatOff:%bx\n", SLedRepeatOff));
		}
#endif

		switch(gSys.SystemMode)
		{
#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
			case SYS_MODE_USB:
			case SYS_MODE_SD:
#if defined(AU6210K_ZB_BT007_CSR)||defined(AU6210K_LK_SJ_CSRBT)
				SLedLightOp(LED_POWER, LIGHTOFF);
#else
				SLedLightOp(LED_POWER, Light);
#endif
#ifdef SLED_MP3_EN
				SLedLightOp(LED_MP3, Light);
#endif

#ifdef SLED_USB_EN
				if(gSys.SystemMode == SYS_MODE_USB)
				{
					SLedLightOp(LED_USB, Light);
				}
				else
#endif

#ifdef SLED_SD_EN
				{
					SLedLightOp(LED_SD, Light);
				}
#endif

#ifdef SLED_REP_EN
				SLedLightOp(LED_REPEAT, SLedRepeatOff);
#endif
				break;
#endif
	
#ifdef FUNC_LINEIN_EN
			case SYS_MODE_LINEIN:
#if defined(AU6210K_ZB_BT007_CSR)||defined(AU6210K_LK_SJ_CSRBT)
				SLedLightOp(LED_POWER, LIGHTOFF);
#else
				SLedLightOp(LED_POWER, Light);
#endif
#ifdef SLED_MP3_EN
				SLedLightOp(LED_MP3, Light);
#endif
				
#ifdef SLED_AUX_EN
				SLedLightOp(LED_AUX, Light);
#endif
				break;
#endif
	
#ifdef FUNC_RADIO_EN
			case SYS_MODE_RADIO:
				SLedLightOp(LED_POWER, FALSE);
#ifdef SLED_MP3_EN
				SLedLightOp(LED_MP3, Light);
#endif
				
#ifdef SLED_RAD_EN
				SLedLightOp(LED_RADIO, Light);
#endif
				break;
#endif
	
#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
			case SYS_MODE_AUDIO:
			case SYS_MODE_READER:
			case SYS_MODE_AUDIO_READER:
				SLedLightOp(LED_POWER, Light);
#ifdef SLED_AUD_EN
				SLedLightOp(LED_AUDIO, Light);
#endif
				break;
#endif

#if defined(FUNC_BLUETOOTH_EN)
			case SYS_MODE_BLUETOOTH:
				SLedLightOp(LED_POWER, FALSE);	
				break;
#endif
	
#ifdef FUNC_RTC_EN
			case SYS_MODE_RTC:
	
				break;
#endif

		case SYS_MODE_NONE:
			//SLedLightOp(LED_POWER, Light);
			break;
			
			default:
				break;
		}
	}
	else//if(!gBlink.BlinkFlag)
	{
		switch(gSys.SystemMode)
		{
#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
			case SYS_MODE_USB:
			case SYS_MODE_SD:
				SLedDispPlayState();
				break;
#endif
	
#ifdef FUNC_LINEIN_EN
			case SYS_MODE_LINEIN:
				SLedDispLineIn();
				break;
#endif
	
#ifdef FUNC_RADIO_EN
			case SYS_MODE_RADIO:
				SLedDispRadio();
				break;
#endif
	
#if (defined(FUNC_AUDIO_EN) || defined(FUNC_READER_EN) || defined(FUNC_AUDIO_READER_EN))
			case SYS_MODE_AUDIO:
			case SYS_MODE_READER:
			case SYS_MODE_AUDIO_READER:
				SLedDispAudio();
				break;
#endif

#if defined(FUNC_BLUETOOTH_EN)
			case SYS_MODE_BLUETOOTH:
				SLedLightOp(LED_POWER, FALSE);	
				break;
#endif

#ifdef FUNC_RTC_EN
			case SYS_MODE_RTC:
				SLedDispRTC();
				break;
#endif

			case SYS_MODE_NONE:
				break;
			
			default:
				break;
		}	
	}
}


#endif
