#include <includes.h>
#include "bsp_general_time.h"

uint32_t g_time = 0;  // ms ��ʱ����
uint32_t g_timeMsg = 0; // ms ��ʱ����
uint32_t g_timePressKeyDelay = 0; // ms ���������ȴ�ʱ��,���鿨ʧ��֮��,ʹ��,Ȼ���ٴ��ϰ���״̬

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void  GENERAL_TIM2_IRQHandler (void)
{
    // 2�ж�һ��
    if ( TIM_GetITStatus( GENERAL_TIM2, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(GENERAL_TIM2 , TIM_FLAG_Update);       // ���ж�
        g_tCardMechineStatusFrame.CARD_MECHINE1.cardNum[0] = g_uiaInitCardCount[1] / 100 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.cardNum[1] = g_uiaInitCardCount[1] / 10 % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.cardNum[2] = g_uiaInitCardCount[1] % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.antHasCard = g_ucaCardIsReady[0] + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.status = g_ucaHasBadCard[0] ? '2' : '0';

        g_tCardMechineStatusFrame.CARD_MECHINE2.cardNum[0] = g_uiaInitCardCount[2] / 100 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.cardNum[1] = g_uiaInitCardCount[2] / 10 % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.cardNum[2] = g_uiaInitCardCount[2] % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.antHasCard = g_ucaCardIsReady[1] + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.status = g_ucaHasBadCard[1] ? '2' : '0';

        g_tCardMechineStatusFrame.CARD_MECHINE3.cardNum[0] = g_uiaInitCardCount[3] / 100 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.cardNum[1] = g_uiaInitCardCount[3] / 10 % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.cardNum[2] = g_uiaInitCardCount[3] % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.antHasCard = g_ucaCardIsReady[2] + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.status = g_ucaHasBadCard[1] ? '2' : '0';

        g_tCardMechineStatusFrame.CARD_MECHINE4.cardNum[0] = g_uiaInitCardCount[4] / 100 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.cardNum[1] = g_uiaInitCardCount[4] / 10 % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.cardNum[2] = g_uiaInitCardCount[4] % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.antHasCard = g_ucaCardIsReady[3] + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.status = g_ucaHasBadCard[1] ? '2' : '0';

        g_tCardMechineStatusFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
        g_tCardMechineStatusFrame.UP_SPIT_IS_OK = g_ucUpWorkingID + '0';
        g_tCardMechineStatusFrame.DOWN_SPIT_IS_OK = g_ucDownWorkingID + '0';

        //if (timeMsg == 2)    // 2���ϱ�һ��ϵͳ��Ϣ
        {
            //timeMsg = 0;
            printf ( "%s\r\n", ( char * ) &g_tCardMechineStatusFrame );
        }
    }
}
/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void  GENERAL_TIM3_IRQHandler (void)
{
    // 10ms�ж�һ��
    if ( TIM_GetITStatus( GENERAL_TIM3, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(GENERAL_TIM3 , TIM_FLAG_Update);       // ���ж�
        /*
        if (g_time == 0 && g_ucKeyValues == KEY_NUL)
        {
            g_time = 3000;
            g_ucKeyValues = KEY_QUIT;                                   // 30���ӽ���Ļ�ص�����ʾ
            LCD_BAK_SET;
            //TIM_ITConfig(GENERAL_TIM3,TIM_IT_Update,DISABLE);         // �ر��ж�
        }
        */
        /*
        if (g_timePressKeyDelay == 0 && g_ucRepeatKeyMechine != 0)
        {
            if (g_ucaCardIsReady[g_ucRepeatKeyMechine - 1] == 1 )           // ������
            {
                g_ucaCardIsReady[g_ucRepeatKeyMechine - 1] = 0;
                antSwitch( g_ucRepeatKeyMechine );                          //  �л�����
                g_tCardKeyPressFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                g_tCardKeyPressFrame.CARD_MECHINE = g_ucRepeatKeyMechine <= 2 ? '1' : '2';
                g_tCardKeyPressFrame.MECHINE_ID = g_ucRepeatKeyMechine + '0';
                printf ( "%s\n", ( char * ) &g_tCardKeyPressFrame );
                g_ucRepeatKeyMechine = 0;
            }
            else if (g_ucaCardIsReady[g_ucRepeatKeyMechine - 1] == 0)          // ��δ����
            {
                g_timePressKeyDelay = 500;
            }
        }
        g_time--;
        g_timePressKeyDelay--;
        */
    }
}

// �ж����ȼ�����
static void GENERAL_TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ ;
    // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *  TIM_CounterMode          TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef;
// *-----------------------------------------------------------------------------
// */
static void GENERAL_TIM2_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK, ENABLE);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM2_Period;
    // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM2_Prescaler;
    // ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(GENERAL_TIM2, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM2, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(GENERAL_TIM2,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM2, ENABLE);
}

// �ж����ȼ�����
static void GENERAL_TIM3_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM3_IRQ ;
    // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *  TIM_CounterMode          TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef;
// *-----------------------------------------------------------------------------
// */
static void GENERAL_TIM3_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    GENERAL_TIM3_APBxClock_FUN(GENERAL_TIM3_CLK, ENABLE);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM3_Period;
    // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM3_Prescaler;
    // ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM3, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(GENERAL_TIM3,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM3, ENABLE);
}

void generalTIM2Init(void)
{
    GENERAL_TIM2_NVIC_Config();
    GENERAL_TIM2_Mode_Config();
}
void generalTIM3Init(void)
{
    GENERAL_TIM3_NVIC_Config();
    GENERAL_TIM3_Mode_Config();
}

/*********************************************END OF FILE**********************/
