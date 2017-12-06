#ifndef __PORT_H
#define __PORT_H

#include "stm32f10x.h"


///////////////////////////////////////////////////////////////
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08

//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //���
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //����

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //���
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //����

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //���
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //����

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //���
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //����

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //���
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //���
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //���
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����



#define FI1 PAin(5)      // PA5
#define FI2 PAin(6)      // PA6
#define FI3 PAin(7)      // PA7
#define DI1 PBin(0)      // PB0
#define DI2 PBin(1)      // PB1
#define DI3 PBin(10)     // PB10
#define DI4 PBin(11)     // PB11
#define Bit1 PCin(11)    // PC11
#define Bit2 PCin(12)    // PC12
#define DK1 PCout(8)     // PC8
#define DK2 PCout(9)     // PC9
#define FK1 PBout(12)    // PB12
#define FK2 PBout(13)    // PB13

#define KEY0 PAin(0)     // PA0
#define KEY1 PCin(1)     // PC1
#define KEY2 PCin(2)     // PC2
#define KEY3 PCin(3)     // PC3

#define YK PBin(14)     // PB14
#define QK PBin(15)     // PB15

//LED�˿ڶ���
#define LED0 PCout(6)// PC6
#define LED1 PCout(7)// PC7


extern u8 RunStep;
extern u8 KaOn;
extern u8 Order;
extern u8 Ka_state;
void PORT_Init(void);    //IO�ڳ�ʼ��
void K_run(void);    	 //�Ϳ����к���
u8 KEY_Scan(void);       //��ɨ�躯��
void KEY_deal(u8 temp);  //����������

#endif
