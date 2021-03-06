#include "bsp_matrix_key.h"
#include "includes.h"

/**************矩阵键盘.c文件*****************************/
u8 matrix_key[4][4];
volatile u8 g_ucKeyValues = KEY_NUL;      // 当前按键值,全局
volatile u8 g_ucKeyContinue = 0;           // 连续按键的标志
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

void matrixKeyboardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );    // 打开时钟,并且打开管脚复用
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);           // 关闭JTAG和SW模式,管脚复用

    /* 键盘行扫描输出线 输出高电平 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* 键盘列扫描输入线 键被按时输入高电平 放开输入低电平 */
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

void matrixKeyboardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD , ENABLE );    // 打开时钟,并且打开管脚复用
    GPIO_PinRemapConfig (GPIO_Remap_SWJ_Disable , ENABLE);           // 关闭JTAG和SW模式,管脚复用

    /* 键盘行扫描输出线 输出高电平 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* 键盘列扫描输入线 键被按时输入高电平 放开输入低电平 */
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

void matrixKeyboardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );    // 打开时钟,并且打开管脚复用
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);           // 关闭JTAG和SW模式,管脚复用

    /* 键盘行扫描输出线 输出高电平 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* 键盘列扫描输入线 键被按时输入高电平 放开输入低电平 */
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

void matrixKeyboardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE );    // 打开时钟
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);           // 关闭JTAG和SW模式,管脚复用

    /* 键盘行扫描输出线 输出高电平 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 键盘列扫描输入线 键被按时输入高电平 放开输入低电平 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    for(i = 0; i < 4; i++)
    {
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }

}

#endif



u8 matrixUpdateKey(void)
{
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char ucTime = 0;
    if (g_ucKeyValues != KEY_NUL)
    {
        return 0;
    }
    for(i = 0; i < 4; i++)             //i是输出口，依次置低电平
    {
        GPIO_ResetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
        for(j = 0; j < 2; j++)            //j是输入口，当键按下时导通被置为低电平
        {
            if(!GPIO_ReadInputDataBit(matrix_key_input[j].GPIO_x, matrix_key_input[j].GPIO_pin))
            {
                delayMs(1);
                while(!GPIO_ReadInputDataBit(matrix_key_input[j].GPIO_x, matrix_key_input[j].GPIO_pin))
                {
                    delayMs(10);
                    if (g_ucKeyContinue == 1)        // 以下是超时处理
                    {
                        if (ucTime++ == 20)         // 如果是连续按键,20ms退出,加上进程延时5ms,共205ms发送一次按键
                        {
                            ucTime = 0;
                            g_ucKeyContinue = 0;     // 连续模式按键,松开之后直接退出
                            break;
                        }
                    }
                    else
                    {
                        if (ucTime++ == 200)
                        {
                            // 长按,或者一直被拉低的情况,认为是错误情况,认为没有按键按下
                            ucTime = 0;
                            break;
                        }
                    }
                }
                GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
                g_ucKeyValues = (i + 1) * 10 + (j + 1);
                g_siKeyTime = 1000;    // 10秒没有按键,且有故障的情况下,则显示故障
                return 0;
            }
        }
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }
    return 1;
}
