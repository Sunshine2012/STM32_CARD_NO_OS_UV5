#include "bsp_matrix_key.h"
#include "includes.h"

/**************�������.c�ļ�*****************************/
unsigned char matrix_key[4][4];
unsigned char g_ucKeyValues = KEY_NUL;      // ��ǰ����ֵ,ȫ��
unsigned char g_ucKeyContinu = 0;           // ���������ı�־
struct io_port
{
    GPIO_TypeDef *GPIO_x;
    unsigned short GPIO_pin;
};

#ifdef OLD_KEY

static struct io_port matrix_key_output[4] =
{
    {GPIOB, GPIO_Pin_4 },   {GPIOA, GPIO_Pin_13},
    {GPIOA, GPIO_Pin_14},   {GPIOA, GPIO_Pin_15}
};
static struct io_port matrix_key_input[4] =
{
    {GPIOB, GPIO_Pin_3}, {GPIOB, GPIO_Pin_2},
    //{GPIOD, GPIO_Pin_6}, {GPIOD, GPIO_Pin_7}
};

void matrix_keyboard_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );    // ��ʱ��,���Ҵ򿪹ܽŸ���
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);           // �ر�JTAG��SWģʽ,�ܽŸ���

    /* ������ɨ������� ����ߵ�ƽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ������ɨ�������� ������ʱ����ߵ�ƽ �ſ�����͵�ƽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    for(i = 0; i < 4; i++)
    {
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }

}

#elif NEW_KEY

static struct io_port matrix_key_output[4] =
{
    {GPIOC, GPIO_Pin_4 },   {GPIOC, GPIO_Pin_5},
    {GPIOB, GPIO_Pin_0},    {GPIOB, GPIO_Pin_1}
};
static struct io_port matrix_key_input[4] =
{
    {GPIOC, GPIO_Pin_12}, {GPIOD, GPIO_Pin_2},
};

void matrix_keyboard_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD , ENABLE );    // ��ʱ��,���Ҵ򿪹ܽŸ���
    GPIO_PinRemapConfig (GPIO_Remap_SWJ_Disable , ENABLE);           // �ر�JTAG��SWģʽ,�ܽŸ���

    /* ������ɨ������� ����ߵ�ƽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ������ɨ�������� ������ʱ����ߵ�ƽ �ſ�����͵�ƽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    for(i = 0; i < 4; i++)
    {
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }

}

#elif NEW_BOARD_NEW_KEY

static struct io_port matrix_key_output[4] =
{
    {GPIOA, GPIO_Pin_13},   {GPIOB, GPIO_Pin_4 },
    {GPIOA, GPIO_Pin_14},   {GPIOA, GPIO_Pin_15}
};
static struct io_port matrix_key_input[4] =
{
    {GPIOB, GPIO_Pin_3}, {GPIOB, GPIO_Pin_2},
    //{GPIOD, GPIO_Pin_6}, {GPIOD, GPIO_Pin_7}
};

void matrix_keyboard_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );    // ��ʱ��,���Ҵ򿪹ܽŸ���
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);           // �ر�JTAG��SWģʽ,�ܽŸ���

    /* ������ɨ������� ����ߵ�ƽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ������ɨ�������� ������ʱ����ߵ�ƽ �ſ�����͵�ƽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    for(i = 0; i < 4; i++)
    {
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }

}

#elif NEW_BOARD_NEW_KEY_JTAG

static struct io_port matrix_key_output[4] =
{
    { GPIOC, GPIO_Pin_8 },   { GPIOC, GPIO_Pin_9 },
    { GPIOC, GPIO_Pin_13 },   { GPIOC, GPIO_Pin_14 },
};
static struct io_port matrix_key_input[4] =
{
    { GPIOC, GPIO_Pin_6 }, { GPIOC, GPIO_Pin_7 },
};

void matrix_keyboard_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE );    // ��ʱ��
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);           // �ر�JTAG��SWģʽ,�ܽŸ���

    /* ������ɨ������� ����ߵ�ƽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* ������ɨ�������� ������ʱ����ߵ�ƽ �ſ�����͵�ƽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    for(i = 0; i < 4; i++)
    {
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }

}

#endif



u8 matrix_update_key(void)
{
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char ucTime = 0;
    static unsigned char isReleaseKey = 1;
    if (g_ucKeyValues != KEY_NUL)
    {
        return 0;
    }
    for(i = 0; i < 4; i++)             //i������ڣ������õ͵�ƽ
    {
        GPIO_ResetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
        for(j = 0; j < 2; j++)            //j������ڣ���������ʱ��ͨ����Ϊ�͵�ƽ
        {
            if(!GPIO_ReadInputDataBit(matrix_key_input[j].GPIO_x, matrix_key_input[j].GPIO_pin))
            {
                delayMs(1);
                while(!GPIO_ReadInputDataBit(matrix_key_input[j].GPIO_x, matrix_key_input[j].GPIO_pin))
                {
                    delayMs(10);
                    if (g_ucKeyContinu == 1)        // �����ǳ�ʱ����
                    {
                        if (ucTime++ == 20)         // �������������,20ms�˳�,���Ͻ�����ʱ10ms,��210ms����һ�ΰ���
                        {
                            ucTime = 0;
                            g_ucKeyContinu = 0;     // ����ģʽ����,�ɿ�֮��ֱ���˳�
                            break;
                        }
                    }
                    else
                    {
                        if (ucTime++ == 200)
                        {
                            // ����,����һֱ�����͵����,��Ϊ�Ǵ������,��Ϊû�а�������
                            ucTime = 0;
                            break;
                        }
                    }
                }
                GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
                g_ucKeyValues = (i + 1) * 10 + (j + 1);
                generalTIMInit();       // ���30��û�а���,���˳�����������ʾ��ǰ��������״̬
                return 0;
            }
        }
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }
    return 1;
}
