/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F103
*                                        Evaluation Board
*
* Filename      : main.h
* Version       :
* Programmer(s) :
*********************************************************************************************************
*/

#ifndef  __Main_H__
#define  __Main_H__




/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include <includes.h>

/*
*********************************************************************************************************
*                                          GPIO PIN DEFINITIONS
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

extern u8 g_ucConnectMode;       // 1为联机模式,其他为单机测试模式
extern u8 g_ucIsSetting;         // 如果有人在操作界面的时候,新来的报警都处于后台显示,当处理完成之后
extern u8 g_ucIsUpdateMenu;      // 更新显示
extern u8 g_ucCurDlg;            // 当前显示的菜单号
extern u8 g_ucHighLightRow;      // 当前显示的菜单需要高亮的行
extern u8 g_ucCurID;             // 当前通信设备的号,在设置设备号时候使用
extern u8 g_ucIsNewWarningCode;  // 有新的报警,再次更新界面,在同一时候,有多个未处理的报警
extern u8 g_ucUpWorkingID;       // 上工位工作卡机号
extern u8 g_ucUpBackingID;       // 上工位备用卡机号
extern u8 g_ucDownWorkingID;     // 下工位工作卡机号
extern u8 g_ucDownBackingID;     // 下工位备用卡机号
extern u8 g_ucCurOutCardId;      // 当前出卡的卡机号
extern u8 g_ucLockPressKey;      // 按键锁定
extern u8 g_ucaCardIsReady[4];   // 卡就绪
extern u8 g_ucaFaultCode[4];     // 卡机是否有未处理的故障
extern u8 g_ucaDeviceStatus[4];  // 两个卡机处于待机(Standby)状态下,按键按下,主机收到按键信息发卡,只要有卡没有被取走,即使收到按键信息,也不再发卡.
extern u8 g_ucaMechineExist[4];  // 卡机是否存在并通信正常
extern u8 g_ucaHasBadCard[4];    // 有坏卡
extern u8 g_ucaMasterStandbyStatus[4];  // 卡机的主备机状态
extern u8 g_ucaStatus[4];        // 卡机的工作状态

extern u8 g_ucP_RsctlFrame;      // 收到一帧正确的数据
extern u8 g_ucIsUpdateMsgFlag;   // 向PC上报卡机消息标志位
extern u8 g_ucIsCycleMsgFlag;    // 向卡机发送定时查询消息标志位


extern u8 g_ucRepeatKeyMechine;  // 如果连续出现两张坏卡,则记录即将发卡的卡机,等待500ms之后,再次检测卡机是否就绪并上报按键值
extern u8 g_ucBadCardCount;      // 如果连续出现4张坏卡,则记录即将发卡的卡机,则不再发卡

extern u8 g_ucKeyPressCount;     // 在2秒钟连续按键6次,则重启设备

extern Dlg           g_dlg[];
extern Dlg g_dlg_fault_code[];

extern CanQueue  g_tCanRxQueue;        // CAN接收卡机数据队列
extern UartQueue g_tUARTRxQueue;       // UART接收PC机数据队列
extern CanRxMsg  g_tCanRxMsg;          // CAN数据出队元素
extern u8 g_ucaUartRxMsg[50];          // UART数据出队元素


extern u32 g_uiSerNum;      // 帧序号,全局,卡机与主机之间的帧序号
extern u32 g_uiSerNumPC;    // 帧序号,全局,PC与主机之间的帧序号
extern u32 g_uiCurNum;      // 作为当前帧号的比对,如果帧序号不对,则代表数据丢失
/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/

//#define __DEBUG__

#ifdef __DEBUG__
#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...) do{}while(0)
#endif

#ifdef __DEBUG__
#define DEBUG_printf(format,...) printf(format,##__VA_ARGS__)
#else
#define DEBUG_printf(format,...) do{}while(0)
#endif


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                     PERIPHERAL POWER/CLOCK SERVICES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                              LED SERVICES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              STATUS INPUTS
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif




