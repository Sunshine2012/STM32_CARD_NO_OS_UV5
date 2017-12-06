#ifndef APP_VOICE_H
#define APP_VOICE_H

#include "stm32f10x.h"

#define VOICE_EN						  0					//
#define DEVICE_USE_UART2_EN		0					// ������ز���USART2

#define AUDIO_TYPE_MAX_NUM		0x40//20					// ��������������
#define AUDIO_PLAY_MAX_LEN		10					// һ��������󳤶ȣ��Ե�λ��������Ϊ��λ

#define WAV_TYPE_DATA_SHIFT		0x2b				// WAV��ʽ�ļ�������ƫ����
#define AUDIO_PLAY_SIGNLE		1					// ���β���
#define AUDIO_PLAY_CIRCURT		2					// ѭ������
#define AUDIO_PLAY_PARA_ERR		0xff				// �������Ų����������
#define AUDIO_PLAY_SET_OK		1					// �����������óɹ�
#define AUDIO_CALL_MAX_NUMS		100					// ͬʱ��¼������Ϣ����




typedef struct {
	const unsigned char *Audio[AUDIO_TYPE_MAX_NUM];	// �������׵�ַ
	const unsigned int AudioLen[AUDIO_TYPE_MAX_NUM];// �����γ���
}AUDIO_TYPE;


typedef struct {
	uint8_t		playType;							// ��������
	uint8_t 	audioLen;							// �����������೤��
	uint8_t  	audioIndex[AUDIO_PLAY_MAX_LEN];		// ��������������
}AUDIO_PLAY;

















void VoicePlayDeal(void);

#endif
