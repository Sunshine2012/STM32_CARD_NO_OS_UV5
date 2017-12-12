/************************************************************
  * ��Ȩ����(C)2014
  * ��Ŀ����:
  * �ļ�����:       __QUEUE_H
  * ����ƽ̨:       STM32F103VC
  * ���뻷��:       Keil 4.72 for Arm
  * ��������:       c
  * ����ժҪ:
  *----------------------------------------------------------
  * ��    ��:       .���
  * ��������:
  * �� �� ��:       v1.0
  * ��    ��:
  *----------------------------------------------------------
  * �� �� ��:
  * �޸�����:
  * ��    ��:
************************************************************/


#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "includes.h"
// ���ն��л��������С
#define RX_DATA_BUFF_SIZE       50

// �������ֽ�ת���ɰ���
#define MAKE_HALF_WORD(a, b)	(((b & 0xff) << 8) | (a&0xff))
// ��ȡshort���͵����ݵĵ��ֽ�
#define LOWBYTE(a)		(a & 0xff)
// ��ȡshort���͵����ݵĸ��ֽ�
#define HIGHBYTE(a)		((a & 0xff00) >> 8)
// ����һ���ֽ��е�bλ�Ƿ�Ϊ1
#define TESTBIT(a, b)   ((a >> b) & 0x01)
// ��λ����a�е�bλ
#define SETBIT(a, b)	(a = (a | (1 << b)))
// ��������a�е�bλ
#define CLRBIT(a, b)	(a = (a & (~(1 << b))))

// ���ն���:���տ�������
typedef struct CanQueue
{
    CanRxMsg news[RX_DATA_BUFF_SIZE];
    u32 top;
    u32 bottom;
    u32 empty;        //��Ϊ���ʾΪ��
    u32 full;         //��Ϊ���ʾ��
}CanQueue;

// ���ն���:������λ������
typedef struct UartQueue
{
    char news[RX_DATA_BUFF_SIZE][50];
    u32 top;
    u32 bottom;
    u32 empty;        //��Ϊ���ʾΪ��
    u32 full;         //��Ϊ���ʾ��
}UartQueue;

u8 canInitQueue (CanQueue * p_tQueue);
u8 canInQueue (CanQueue * const p_tQueue, CanRxMsg * const p_tNewNode);
u8 canOutQueue (CanQueue * const p_tQueue, CanRxMsg * p_tReturnNode);

u8 uartInitQueue (UartQueue * p_tQueue);
u8 uartInQueue (UartQueue * const p_tQueue, u8 * const p_ucaNewNode);
u8 uartOutQueue (UartQueue * const p_tQueue, u8 * p_ucaReturnNode);


#endif
