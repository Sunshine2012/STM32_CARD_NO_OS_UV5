/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:
  * ��̳    :
  * �Ա�    :
  *
  ******************************************************************************
  */

#include "includes.h"
#include "bsp_exti.h"

/**
 * @brief  ����Ƕ�������жϿ�����NVIC
 * @param  ��
 * @retval ��
 */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����NVICΪ���ȼ���1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* �����ж�Դ1 */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC0_INT_EXTI_IRQ;
    /* ������ռ���ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /* ���������ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /* ʹ���ж�ͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* �����ж�Դ2������ʹ������������� */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC1_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);    
    
    /* �����ж�Դ3������ʹ������������� */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC2_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
    
    /* �����ж�Դ4������ʹ������������� */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC3_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  ���� IOΪEXTI�жϿڣ��������ж����ȼ�
 * @param  ��
 * @retval ��
 */
void EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*��������GPIO�ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd(NVIC0_INT_GPIO_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(NVIC1_INT_GPIO_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(NVIC2_INT_GPIO_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(NVIC3_INT_GPIO_CLK,ENABLE);

    /* ���� NVIC �ж�*/
    NVIC_Configuration();

    /*--------------------------NVIC0����-----------------------------*/
    /* ѡ���ж��õ���GPIO */
    GPIO_InitStructure.GPIO_Pin = NVIC0_INT_GPIO_PIN;
    /* ����Ϊ�������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NVIC0_INT_GPIO_PORT, &GPIO_InitStructure);

    /* ѡ��EXTI���ź�Դ */
    GPIO_EXTILineConfig(NVIC0_INT_EXTI_PORTSOURCE, NVIC0_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = NVIC0_INT_EXTI_LINE;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �½����ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------NVIC1����-----------------------------*/
    /* ѡ���ж��õ���GPIO */
    GPIO_InitStructure.GPIO_Pin = NVIC1_INT_GPIO_PIN;
    /* ����Ϊ�������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NVIC1_INT_GPIO_PORT, &GPIO_InitStructure);

    /* ѡ��EXTI���ź�Դ */
    GPIO_EXTILineConfig(NVIC1_INT_EXTI_PORTSOURCE, NVIC1_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = NVIC1_INT_EXTI_LINE;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �½����ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    
    /*--------------------------NVIC2����-----------------------------*/
    /* ѡ���ж��õ���GPIO */
    GPIO_InitStructure.GPIO_Pin = NVIC2_INT_GPIO_PIN;
    /* ����Ϊ�������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NVIC2_INT_GPIO_PORT, &GPIO_InitStructure);

    /* ѡ��EXTI���ź�Դ */
    GPIO_EXTILineConfig(NVIC2_INT_EXTI_PORTSOURCE, NVIC2_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = NVIC2_INT_EXTI_LINE;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �½����ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    
    /*--------------------------NVIC3����-----------------------------*/
    /* ѡ���ж��õ���GPIO */
    GPIO_InitStructure.GPIO_Pin = NVIC3_INT_GPIO_PIN;
    /* ����Ϊ�������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NVIC3_INT_GPIO_PORT, &GPIO_InitStructure);

    /* ѡ��EXTI���ź�Դ */
    GPIO_EXTILineConfig(NVIC3_INT_EXTI_PORTSOURCE, NVIC3_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = NVIC3_INT_EXTI_LINE;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �½����ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    
}
/*********************************************END OF FILE**********************/



