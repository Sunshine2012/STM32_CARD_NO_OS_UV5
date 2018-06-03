/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI

  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "includes.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

#if 0
// �����жϷ�����
// �ѽ��յ������ݴ���һ�����黺�������棬�����յ��ĵ�ֵ����">"ʱ����ֵ����
void macUSART1_IRQHandler( void )
{
    if ( USART_GetITStatus( macUSART1, USART_IT_RXNE ) != RESET )
    {
        g_rx_buf[g_num] = USART_ReceiveData( macUSART1 );

        if ( g_rx_buf[g_num] == FRAME_START )
        {
            memset( g_rx_buf, 0, sizeof( g_rx_buf ) );
            g_rx_buf[0] = FRAME_START;
            g_num = 0;
            g_num++;
        }
        else if ( g_rx_buf[g_num] == FRAME_END ) // �����յ���ֵ����'>'ʱ������һ֡����
        {

            if(POSITIVE_ACK == g_rx_buf[2])      // �������Ӧ��֡,��ѹջ
            {
            /*
                if ( g_rx_buf[1] == g_uiCurNum ) //��ȡ����Ϣ����ж�,������
                {
                    g_uiCurNum = 0;
                    g_siCardTakeMsgTime = 0;// �����ط�
                    g_siStatusOverTimeS = 0;
                    g_ucaDeviceStatus[g_ucCurOutCardId - 1] = 0;
                }
            */
                g_num = 0;
                g_rx_buf[g_num] = 0;
            }
            else if (NAGATIVE_ACK == g_rx_buf[2])
            {

                g_num = 0;
                g_rx_buf[g_num] = 0;
            }
            else
            {

                // ��ֹ���ڽ����ж�
                //USART_ITConfig(macUSART1, USART_IT_RXNE, DISABLE);
                g_tP_RsctlFrame.RSCTL = g_rx_buf[1];
                //printf ("%s\n",(char *)&g_tP_RsctlFrame);   //������Ӧ��֡
                // ʹ�ܴ��ڽ����ж�
                //USART_ITConfig(macUSART1, USART_IT_RXNE, ENABLE);
                g_ucP_RsctlFrame = 1;
                g_rx_buf[g_num + 1] = 0;    // ������β��ʶ��
                /* ������Ϣ����Ϣ���� queue */
                uartInQueue( &g_tUARTRxQueue, g_rx_buf ); // �����Ǿ���,���Բ�����������
            }
        }

        // ��ֵ����ʱ�������������һ��
        else
        {

            g_num++;

            if ( g_num > 50 ) //һ֡�������50�ֽ�,��������
            {

                // ��ֹ���ڽ����ж�
                //USART_ITConfig(macUSART1, USART_IT_RXNE, DISABLE);
                g_tN_RsctlFrame.RSCTL = g_rx_buf[1];
                printf ("%s\n",(char *)&g_tN_RsctlFrame);   //���͸�Ӧ��֡
                // ʹ�ܴ��ڽ����ж�
                //USART_ITConfig(macUSART1, USART_IT_RXNE, ENABLE);

                g_num = 0;
                g_rx_buf[g_num] = 0;
            }
        }
    }
}
#endif

// �����жϷ�����
// �ѽ��յ������ݴ���һ�����黺�������棬�����յ��ĵ�ֵ����">"ʱ����ֵ����
void macUSART1_IRQHandler( void )
{
    static u16 i = 0;
    if ( USART_GetITStatus( macUSART1, USART_IT_IDLE ) != RESET )
    {
        i = USART1->SR;
        i = USART1->DR;
        USART_ClearITPendingBit(macUSART1, USART_IT_IDLE);

        g_num = SENDBUFF_SIZE - DMA_GetCurrDataCounter (USART1_RX_DMA_CHANNEL);
        // printf ("%d\r\n",g_num);
        g_ucaRxBuf[g_num] = 0;    // ������β��ʶ��
        // printf ("%s\r\n",g_ucaRxBuf);

        if ( FRAME_START ==  g_ucaRxBuf[0] \
          && FRAME_END == g_ucaRxBuf[g_num - 1] )
        {
            // ���������Ӧ��֡,�������
            if( POSITIVE_ACK == g_ucaRxBuf[2] \
             || NAGATIVE_ACK == g_ucaRxBuf[2] )
            {
                g_num = 0;
                g_ucaRxBuf[0] = 0;
                DMA1_Channel5->CNDTR = SENDBUFF_SIZE;
                return;
            }

            //DMA_ClearITPendingBit(DMA_IT_HT);//�����������жϱ�־
            //DMA_ClearITPendingBit(DMA_IT_TC);
            DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);//�ر�DMA,��ֹ�������������
            // ��ֹ���ڿ����ж�
            //USART_ITConfig(macUSART1, USART_IT_IDLE, DISABLE);
            g_tP_RsctlFrame.RSCTL = g_ucaRxBuf[1];
            g_ucP_RsctlFrame = 1;
            g_ucaRxBuf[g_num + 1] = 0;    // ������β��ʶ��
            /* ������Ϣ����Ϣ���� queue */
            uartInQueue( &g_tUARTRxQueue, g_ucaRxBuf ); // �����Ǿ���,���Բ�����������

            g_num = 0;
            g_ucaRxBuf[0] = 0;
            DMA1_Channel5->CNDTR = SENDBUFF_SIZE;
            //memset( g_ucaRxBuf, 0, sizeof( g_ucaRxBuf ) );
            DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);//����DMA
        }
        else // ���͸�Ӧ��
        {
            //g_tN_RsctlFrame.RSCTL = g_ucaRxBuf[1] + '0';
            printf ("%s\n",(char *)&g_tN_RsctlFrame);   //���͸�Ӧ��֡

            g_num = 0;
            g_ucaRxBuf[0] = 0;
            DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);//�ر�DMA,��ֹ�������������
            DMA1_Channel5->CNDTR = SENDBUFF_SIZE;
            DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);//����DMA
        }
    }
}


#if uart4

// �����жϷ�����
// �ѽ��յ������ݴ���һ�����黺�������棬�����յ��ĵ�ֵ����">"ʱ����ֵ����
void macUSART4_IRQHandler( void )
{
    if ( USART_GetITStatus( macUSART4, USART_IT_RXNE ) != RESET )
    {
        g_rx_buf[g_num] = USART_ReceiveData( macUSART4 );

        if ( g_rx_buf[g_num] == FRAME_START )
        {
            memset( g_rx_buf, 0, sizeof( g_rx_buf ) );
            g_rx_buf[0] = FRAME_START;
            g_num = 0;
            g_num++;
        }
        else if ( g_rx_buf[g_num] == FRAME_END ) // �����յ���ֵ����'>'ʱ������һ֡����
        {
            g_rx_buf[g_num + 1] = 0;    // ������β��ʶ��

            /* ������Ϣ����Ϣ���� queue */
            uartInitQueue( &g_tUARTRxQueue, g_rx_buf ); // �����Ǿ���,���Բ�����������
        }

        // ��ֵ����ʱ�������������һ��
        else
        {
            g_num++;

            if ( g_num > 50 ) //һ֡�������50�ֽ�,��������
            {
                g_num = 0;
            }
        }
    }
}
#endif

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
