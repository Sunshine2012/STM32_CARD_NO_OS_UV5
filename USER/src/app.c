/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
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
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "includes.h"
//#include "WAV_C_xiexie.h"
//#include "WAV_C_anjianquka.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

CPU_TS             ts_start;       //ʱ�������
CPU_TS             ts_end;



/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

OS_TCB   AppTaskStartTCB;                                //������ƿ�

OS_TCB   AppTaskTmrTCB;

OS_TCB   AppTaskOledTCB;                // OLED����������ƿ�

OS_TCB   AppTaskCanFrameTCB;            // ��������������ƿ�

OS_TCB   AppTaskUartFrameTCB;           // ��ȡ��������������ƿ�

OS_TCB   AppTaskKeyScanTCB;             // ����ɨ��

OS_Q queue_uart;     // ��Ϣ����

OS_Q queue_can;      // ��Ϣ����


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk [ APP_TASK_START_STK_SIZE ];       //�����ջ

static  CPU_STK  AppTaskTmrStk [ APP_TASK_TMR_STK_SIZE ];

static  CPU_STK  AppTaskOLEDStk [ APP_TASK_OLED_STK_SIZE ];

static  CPU_STK  AppTaskCanFrameStk [ APP_TASK_CAN_FRAME_STK_SIZE ];

static  CPU_STK  AppTaskUartFrameStk [ APP_TASK_UART_FRAME_STK_SIZE ];

static  CPU_STK  AppTaskKeyScanStk [ APP_TASK_KEY_SCAN_STK_SIZE ];
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);                       //����������

static  void  AppTaskTmr  ( void * p_arg );

static  void  AppTaskOLED  ( void * p_arg );

static void  AppTaskCanFrame ( void * p_arg );

static void  AppTaskUartFrame ( void * p_arg );

static void  AppTaskKeyScan ( void * p_arg );

extern unsigned char BMP1[];

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;

    OSInit(&err);                                                           //��ʼ�� uC/OS-III

    /* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) AppTaskStart,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&AppTaskStartStk[0],                         //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������

    OSStart(&err);                                                          //�����������������uC/OS-III���ƣ�
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

    BSP_Init();                                                 //�弶��ʼ��
    CPU_Init();                                                 //��ʼ�� CPU �����ʱ��������ж�ʱ���������������

    cpu_clk_freq = BSP_CPU_ClkFreq();                           //��ȡ CPU �ں�ʱ��Ƶ�ʣ�SysTick ����ʱ�ӣ�
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        //�����û��趨��ʱ�ӽ���Ƶ�ʼ��� SysTick ��ʱ���ļ���ֵ
    OS_CPU_SysTickInit(cnts);                                   //���� SysTick ��ʼ�����������ö�ʱ������ֵ��������ʱ��

    //Mem_Init();                                                 //��ʼ���ڴ������������ڴ�غ��ڴ�ر�

#if OS_CFG_STAT_TASK_EN > 0u                                    //���ʹ�ܣ�Ĭ��ʹ�ܣ���ͳ������
    OSStatTaskCPUUsageInit(&err);                               //����û��Ӧ������ֻ�п�����������ʱ CPU �ģ����
#endif                                                          //���������� OS_Stat_IdleCtrMax ��ֵ��Ϊ������� CPU
                                                                //ʹ����ʹ�ã���
    CPU_IntDisMeasMaxCurReset();                                //��λ�����㣩��ǰ�����ж�ʱ��

    /* ������Ϣ���� queue */
    OSQCreate   ((OS_Q       *)&queue_uart,         //ָ����Ϣ���е�ָ��
                (CPU_CHAR    *)"uart Frame",        //���е�����
                (OS_MSG_QTY   )100,                 //���ɴ����Ϣ����Ŀ
                (OS_ERR      *)&err);               //���ش�������

    /* ������Ϣ���� queue */
    OSQCreate   ((OS_Q       *)&queue_can,          //ָ����Ϣ���е�ָ��
                (CPU_CHAR    *)"can Frame",         //���е�����
                (OS_MSG_QTY   )100,                 //���ɴ����Ϣ����Ŀ
                (OS_ERR      *)&err);               //���ش�������

    /* ���� AppTaskTmr ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskTmrTCB,                              //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Tmr",                              //��������
                 (OS_TASK_PTR ) AppTaskTmr,                                 //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_TMR_PRIO,                          //��������ȼ�
                 (CPU_STK    *)&AppTaskTmrStk[0],                           //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE / 10,                 //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE,                      //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������

    /* ���� AppTaskOLED ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskOledTCB,                             //������ƿ��ַ
                 (CPU_CHAR   *)"OLED_APP",                                  //��������
                 (OS_TASK_PTR ) AppTaskOLED,                                //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_OLED_PRIO,                         //��������ȼ�
                 (CPU_STK    *)&AppTaskOLEDStk[0],                          //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_OLED_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_OLED_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    /* ���� AppTaskCanFRAME ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskCanFrameTCB,                         //������ƿ��ַ
                 (CPU_CHAR   *)"READ_CAN_FRAME_APP",                        //��������
                 (OS_TASK_PTR ) AppTaskCanFrame,                            //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_CAN_FRAME_PRIO,                    //��������ȼ�
                 (CPU_STK    *)&AppTaskCanFrameStk[0],                      //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_CAN_FRAME_STK_SIZE / 10,           //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_CAN_FRAME_STK_SIZE,                //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    /* ���� AppTaskUartFRAME ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskUartFrameTCB,                        //������ƿ��ַ
                 (CPU_CHAR   *)"READ_UART_FRAME_APP",                       //��������
                 (OS_TASK_PTR ) AppTaskUartFrame,                           //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_UART_FRAME_PRIO,                   //��������ȼ�
                 (CPU_STK    *)&AppTaskUartFrameStk[0],                     //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_UART_FRAME_STK_SIZE / 10,          //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_UART_FRAME_STK_SIZE,               //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    /* ���� AppTaskKeyScan ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskKeyScanTCB,                          //������ƿ��ַ
                 (CPU_CHAR   *)"Key_Scan",                                  //��������
                 (OS_TASK_PTR ) AppTaskKeyScan,                             //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_KEY_SCAN_PRIO,                     //��������ȼ�
                 (CPU_STK    *)&AppTaskKeyScanStk[0],                       //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_KEY_SCAN_STK_SIZE / 10,            //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_KEY_SCAN_STK_SIZE,                 //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    OSTaskDel ( & AppTaskStartTCB, & err );                     //ɾ����ʼ������������������
}


/*
*********************************************************************************************************
*                                          TMR TASK
*********************************************************************************************************
*/
void TmrCallback (OS_TMR *p_tmr, void *p_arg) //�����ʱ��MyTmr�Ļص�����
{

    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                             //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                             //�����ж�ʱ����ֵ��ԭ��
    CPU_INT32U       cpu_clk_freq = 0;

    g_tCardMechineStatusFrame.RSCTL = (g_ucSerNum++ % 10) + '0';

    cpu_clk_freq = BSP_CPU_ClkFreq();                   //��ȡCPUʱ�ӣ�ʱ������Ը�ʱ�Ӽ���

    //macLED1_TOGGLE ();

    ts_end = OS_TS_GET() - ts_start;     //��ȡ��ʱ���ʱ�������CPUʱ�ӽ��м�����һ������ֵ��
                                         //�������㶨ʱʱ�䡣
    OS_CRITICAL_ENTER();                 //�����ٽ�Σ���ϣ����������⵽�ж�

    //printf ( "\r\n��ʱ2s��ͨ��ʱ�����ö�ʱ %07d us���� %04d ms��\r\n",
    //                    ts_end / ( cpu_clk_freq / 1000000 ),     //����ʱʱ������� us
    //                    ts_end / ( cpu_clk_freq / 1000 ) );      //����ʱʱ������� ms

    printf ( "%s\n", ( char * ) &g_tCardMechineStatusFrame );
    OS_CRITICAL_EXIT();

    ts_start = OS_TS_GET();                            //��ȡ��ʱǰʱ���
}


static  void  AppTaskTmr ( void * p_arg )
{
    OS_ERR      err = 0;
    OS_TMR      my_tmr = {0};   //���������ʱ������

    (void)p_arg;

    /* ���������ʱ�� */
    OSTmrCreate ((OS_TMR              *)&my_tmr,           //�����ʱ������
               (CPU_CHAR            *)"MySoftTimer",       //���������ʱ��
               (OS_TICK              )20,                  //��ʱ����ʼֵ����10Hzʱ�����㣬��Ϊ1s
               (OS_TICK              )20,                  //��ʱ����������ֵ����10Hzʱ�����㣬��Ϊ1s
               (OS_OPT               )OS_OPT_TMR_PERIODIC, //�����Զ�ʱ
               (OS_TMR_CALLBACK_PTR  )TmrCallback,         //�ص�����
               (void                *)"Timer Over!",       //����ʵ�θ��ص�����
               (OS_ERR              *)err);                //���ش�������

    /* ���������ʱ�� */
    OSTmrStart ((OS_TMR   *)&my_tmr, //�����ʱ������
               (OS_ERR   *)err);    //���ش�������

    ts_start = OS_TS_GET();                       //��ȡ��ʱǰʱ���

    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��

        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err ); //��������������

    }
}

static  void AppTaskOLED ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                 //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                 //�����ж�ʱ����ֵ��ԭ.
    char *pcMsg = NULL;
    OS_MSG_SIZE    msg_size = 0;
    OS_ERR         err = 0;
    CPU_TS         ts = 0;
    unsigned char key = KEY_NUL;

    doShowStatusMenu(DLG_STATUS, 5, NULL);      // ��ʾ�˵�,��Ҫ����ʾ����
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��

        if (g_ucIsUpdateMenu || g_ucKeyValues == KEY_QUIT)
        {
            g_ucIsUpdateMenu = 0;
            g_ucKeyValues = KEY_NUL;
            doShowStatusMenu(DLG_STATUS, 5, NULL);      // ��ʾ�˵�,��Ҫ����ʾ���к�
        }

        if (g_ucaFaultCode[0] != 0 || g_ucaFaultCode[1] != 0 || g_ucaFaultCode[2] != 0 || g_ucaFaultCode[3] != 0)
        {
            doShowFaultCode (DLG_CLEAR_LCD, 5, NULL);
        }

        key = g_ucKeyValues;
        g_ucKeyValues = KEY_NUL;
        if ( key == KEY_ENTRY )
        {
            doShowMainMenu (DLG_MAIN, 0, NULL);       // ��������״̬,����,ֱ���˳�
            g_ucIsUpdateMenu = 1;
        }

        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );
    }
}



void  AppTaskCanFrame ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                     //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                     //�����ж�ʱ����ֵ��ԭ.
    OS_ERR      err = 0;
    CPU_TS      ts  = 0;
    OS_MSG_SIZE    msg_size = 0;
    CPU_INT08U * pMsg = NULL;
    CanRxMsg *ptRxMessage = NULL;                   // can���ݽ��ջ���
    CanRxMsg tRxMessage = {0};                      // can���ݻ���
    OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );                          //��������������
    //myCANTransmit(&gt_TxMessage, g_ucUpWorkingID, 0, 1, 0, 0, 0, NO_FAIL);   // ���ù���̬
    //myCANTransmit(&gt_TxMessage, g_ucUpBackingID, 0, 1, 0, 0, 0, NO_FAIL);   // ���ñ���̬
    //myCANTransmit(&gt_TxMessage, g_ucDownWorkingID, 0, 1, 0, 0, 0, NO_FAIL);   // ���ù���̬
    //myCANTransmit(&gt_TxMessage, g_ucDownBackingID, 0, 1, 0, 0, 0, NO_FAIL);   // ���ñ���̬

    myCANTransmit(&gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
    myCANTransmit(&gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
    myCANTransmit(&gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
    myCANTransmit(&gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬

    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��

        /* ������Ϣ���� queue ����Ϣ */
        ptRxMessage =   OSQPend ((OS_Q        *)&queue_can,            //��Ϣ����ָ��
                                (OS_TICK       )5,                     //�ȴ�ʱ��
                                (OS_OPT        )OS_OPT_PEND_BLOCKING,  //���û�л�ȡ���ź����Ͳ��ȴ�
                                (OS_MSG_SIZE  *)&msg_size,             //��ȡ��Ϣ���ֽڴ�С
                                (CPU_TS       *)&ts,                   //��ȡ������ʱ��ʱ���
                                (OS_ERR       *)&err);                 //���ش���
        if (ptRxMessage)
        {
            tRxMessage = *(CanRxMsg *)ptRxMessage;
            analyzeCANFrame(tRxMessage);
        }
        OSTimeDly ( 1, OS_OPT_TIME_DLY, & err ); //��������������
    }
}

void  AppTaskUartFrame ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                     //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                     //�����ж�ʱ����ֵ��ԭ.
    OS_ERR      err = 0;
    CPU_TS      ts  = 0;
    OS_MSG_SIZE    msg_size = 0;
    CPU_INT08U * pMsg = NULL;
    CPU_INT08U ucaMsg[30] = "";

    OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );                 //�ȴ�1S
    OS_CRITICAL_ENTER();                                        // �����ٽ�Σ���ϣ����������⵽�ж�
    DEBUG_printf ("%s\n","���,��ӭʹ����Ϊ���Ӱ忨ϵͳ");

    printf("%s\n", (char *)&g_tCardMechinePowerOnFrame);          // �ϵ���Ϣ

    OS_CRITICAL_EXIT();

    while (DEF_TRUE)
    {                                                           //�����壬ͨ��д��һ����ѭ��
        pMsg = OSQPend ((OS_Q         *)&queue_uart,            //��Ϣ����ָ��
                        (OS_TICK       )5,                      //�ȴ�ʱ��
                        (OS_OPT        )OS_OPT_PEND_BLOCKING,   //���û�л�ȡ���ź����Ͳ��ȴ�
                        (OS_MSG_SIZE  *)&msg_size,              //��ȡ��Ϣ���ֽڴ�С
                        (CPU_TS       *)&ts,                    //��ȡ������ʱ��ʱ���
                        (OS_ERR       *)&err);                  //���ش���
        if (pMsg)
        {
            strcpy(ucaMsg, pMsg);
            analyzeUartFrame(ucaMsg, msg_size);
        }
        OSTimeDly ( 1, OS_OPT_TIME_DLY, & err );                //��������������
    }
}


void  AppTaskKeyScan ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                     //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                     //�����ж�ʱ����ֵ��ԭ.
    u8 uKey = 0xff;      // ����ֵ
    OS_ERR      err;
    OS_MSG_SIZE    msg_size;

    while (DEF_TRUE)
    {                                                   //�����壬ͨ��д��һ����ѭ��
        matrix_update_key();                            // ɨ�谴��
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );       //��������������
    }
}
