#ifndef __USART4_H
#define	__USART4_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
  * ���ں궨�壬��ͬ�Ĵ��ڹ��ص����ߺ�IO��һ������ֲʱ��Ҫ�޸��⼸����
  */
	
// ����1-USART1
//#define  DEBUG_USARTx                       USART1
//#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
//#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT         GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_9
//#define  DEBUG_USART_RX_GPIO_PORT         GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN          GPIO_Pin_10

//#define  DEBUG_USART_IRQ                  USART1_IRQn
//#define  DEBUG_USART_IRQHandler           USART1_IRQHandler


// ����2-USART2
//#define  DEBUG_USARTx                   USART2
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT         GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_2
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

//#define  DEBUG_USART_IRQ                USART2_IRQn
//#define  DEBUG_USART_IRQHandler         USART2_IRQHandler

//// ����3-USART3
//#define  DEBUG_USARTx                   USART3
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT         GPIOB   
//#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_10
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  DEBUG_USART_IRQ                USART3_IRQn
//#define  DEBUG_USART_IRQHandler         USART3_IRQHandler

////// ����4-UART4
////#define  DEBUG_USARTx                   UART4
////#define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
////#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
////#define  DEBUG_USART_BAUDRATE           115200

////// USART GPIO ���ź궨��
////#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
////#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
////    
////#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
////#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
////#define  DEBUG_USART_RX_GPIO_PORT       GPIOC
////#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

////#define  DEBUG_USART_IRQ                UART4_IRQn
////#define  DEBUG_USART_IRQHandler         UART4_IRQHandler


/**************************USART��������********************************/
#define             macUSART4_BAUD_RATE                       9600

#define             macUSART4                                 UART4
#define             macUSART4_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macUSART4_CLK                             RCC_APB1Periph_UART4
#define             macUSART4_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART4_GPIO_CLK                        RCC_APB2Periph_GPIOC
#define             macUSART4_TX_PORT                         GPIOC
#define             macUSART4_TX_PIN                          GPIO_Pin_10
#define             macUSART4_RX_PORT                         GPIOC
#define             macUSART4_RX_PIN                          GPIO_Pin_11
#define             macUSART4_IRQ                             UART4_IRQn
#define             macUSART4_IRQHandler                      UART4_IRQHandler

// ����5-UART5
//#define  DEBUG_USARTx                   UART5
//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT         GPIOC   
//#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_12
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

//#define  DEBUG_USART_IRQ                UART5_IRQn
//#define  DEBUG_USART_IRQHandler         UART5_IRQHandler


void USART4_Config(void);
void USART4_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void USART4_nSendString( USART_TypeDef * pUSARTx, char *str,int n);
void USART4_SendString( USART_TypeDef * pUSARTx, char *str);
void USART4_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */
