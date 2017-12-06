/**
  ******************************************************************************
  * @file    OLED_I2C.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   128*64�����OLED��ʾ�������ļ�����������SD1306����IICͨ�ŷ�ʽ��ʾ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:
  * ��̳
  * �Ա�

    * Function List:
    *    1. void I2C_Configuration(void) -- ����CPU��Ӳ��I2C
    * 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- ��Ĵ�����ַдһ��byte������
    * 3. void WriteCmd(unsigned char I2C_Command) -- д����
    * 4. void WriteDat(unsigned char I2C_Data) -- д����
    * 5. void OLED_Init(void) -- OLED����ʼ��
    * 6. void OLED_SetPos(unsigned char x, unsigned char y) -- ������ʼ������
    * 7. void OLED_Fill(unsigned char fill_Data) -- ȫ�����
    * 8. void OLED_CLS(void) -- ����
    * 9. void OLED_ON(void) -- ����
    * 10. void OLED_OFF(void) -- ˯��
    * 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize,unsigned char isTurn) -- ��ʾ�ַ���(�����С��6*8��8*16����)
    * 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N,unsigned char isTurn) -- ��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
    * 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMPͼƬ
    *
  *
  ******************************************************************************
  */



#include <includes.h>
#include "bsp_LCD_I2C.h"
#include "delay.h"
#include "codetab.h"

/*��Ϣ���*/
#define IIC_DEBUG_ON         1

#define IIC_INFO(fmt,arg...)           printf("<<-IIC-FLASH-INFO->> "fmt"\n",##arg)
#define IIC_ERROR(fmt,arg...)          printf("<<-IIC-FLASH-ERROR->> "fmt"\n",##arg)
#define IIC_DEBUG(fmt,arg...)          do{\
                                            if(IIC_DEBUG_ON)\
                                            printf("<<-IIC-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                            }while(0)


 /**
  * @brief  I2C_Configuration����ʼ��Ӳ��IIC����
  * @param  ��
  * @retval ��
  */
void I2C_Configuration(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    /* ���� RESET ���� */
    GPIO_InitStructure.GPIO_Pin = macOLED_RESET_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( macOLED_RESET_GPIO_PORT, & GPIO_InitStructure );

    /*STM32F103RET6оƬ��Ӳ��I2C: PB6 -- SCL; PB7 -- SDA */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C���뿪©���
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_DeInit(I2C1);//ʹ��I2C1
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;//������I2C��ַ,���д��
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

    I2C_Cmd(I2C1, ENABLE);
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}


 /**
  * @brief  I2C_WriteByte����OLED�Ĵ�����ַдһ��byte������
  * @param  addr���Ĵ�����ַ
    *                    data��Ҫд�������
  * @retval ��
  */
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                 //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                 //�����ж�ʱ����ֵ��ԭ.
    OS_CRITICAL_ENTER();         // �����ٽ�Σ���ϣ����������⵽�ж�
    start_flag();
    transfer (OLED_ADDRESS);
    transfer (addr);
    transfer (data);
    stop_flag();
    OS_CRITICAL_EXIT();
}


 /**
  * @brief  WriteCmd����OLEDд������
  * @param  I2C_Command���������
  * @retval ��
  */
void WriteCmd(unsigned char I2C_Command)//д����
{
    I2C_WriteByte(0x00, I2C_Command);
}


 /**
  * @brief  WriteDat����OLEDд������
  * @param  I2C_Data������
  * @retval ��
  */
void WriteDat(unsigned char I2C_Data)//д����
{
    I2C_WriteByte(0x40, I2C_Data);
}


 /**
  * @brief  OLED_Init����ʼ��OLED
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
    macOLED_RESET_ON();
    macOLED_RESET_OFF();

    WriteCmd(0x3b); //
    WriteCmd(0x03); //

    WriteCmd(0x38); //
    WriteCmd(0x05); //
    WriteCmd(0x0c); //

    WriteCmd(0x39); //
    WriteCmd(0x08); //
    WriteCmd(0x10); //
    WriteCmd(0xff); //
}


 /**
  * @brief  OLED_SetPos�����ù��
  * @param  x,���xλ��
    *                    y�����yλ��
  * @retval ��
  */
void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{
    WriteCmd(0x38);
    WriteCmd(0x40 + y);
    x = x + 4;
    WriteCmd(( x & 0x0f ) | 0xe0 );
    WriteCmd((( x & 0xf0 ) >> 4 )| 0xf0 );

}

 /**
  * @brief  OLED_Fill�����������Ļ
  * @param  fill_Data:Ҫ��������
    * @retval ��
  */
void OLED_Fill(unsigned char fill_Data)//ȫ�����
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        WriteCmd(0x40+m);        //page0-page1
        WriteCmd(0xe0 | 0x03);        //low column start address
        WriteCmd(0xf0);        //high column start address
        for(n=0;n<129;n++)
            {
                WriteDat(fill_Data);
            }
    }
}

 /**
  * @brief  OLED_CLS������
  * @param  ��
    * @retval ��
  */
void OLED_CLS(void)//����
{
    OLED_Fill(0x00);
}


 /**
  * @brief  OLED_ON����OLED�������л���
  * @param  ��
    * @retval ��
  */
void OLED_ON(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X14);  //������ɱ�
    WriteCmd(0XAF);  //OLED����
}


 /**
  * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  * @param  ��
    * @retval ��
  */
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X10);  //�رյ�ɱ�
    WriteCmd(0XAE);  //OLED����
}

 /**
  * @brief  OLED_ShowStr����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * @param  x,y : ��ʼ������(x:0~127, y:0~7);
    *                    ch[] :- Ҫ��ʾ���ַ���;
    *                    TextSize : �ַ���С(1:6*8 ; 2:8*16)
    * @retval ��
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
    OLED_Fill(0);
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<6;i++)
                    WriteDat(F6x8[c][i]);
                x += 6;
                j++;
            }
        }break;
        case 2:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i]);
                OLED_SetPos(x,y+1);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i+8]);
                x += 8;
                j++;
            }
        }break;
    }
}

 /**
  * @brief  OLED_ShowCN����ʾcodetab.h�еĺ���,16*16����
  * @param  x,y: ��ʼ������(x:0~127, y:0~7);
    *                    N:������codetab.h�е�����
    * @retval ��
  */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N, unsigned char isTurn)
{
    unsigned char wm=0;
    unsigned int  adder=32*N;
    OLED_SetPos(x , y);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x,y + 1);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
}

 /**
  * @brief  OLED_xShowCN���ַ���
  * @param  x,y: ��ʼ������(x:0~127, y:0~7);
    *
    * @retval ��
  */
void OLED_xShowCN(unsigned char x, unsigned char y, unsigned char * pFontbuf ,unsigned char isTurn)
{
    unsigned char wm=0;
    OLED_SetPos( x , y );
    // �����ʾ
    for( wm = 0; wm < 16; wm++ )
    {
        WriteDat( 0 );
    }
    OLED_SetPos( x,y + 1 );
    for(wm = 0; wm < 16; wm++)
    {
        WriteDat( 0 );
    }

    OLED_SetPos( x , y );
    for( wm = 0; wm < 16; wm++ )
    {
        WriteDat( !isTurn ? *pFontbuf++ : ~(*pFontbuf++));
    }
    OLED_SetPos( x,y + 1 );
    for(wm = 0; wm < 16; wm++)
    {
        WriteDat( !isTurn ? *pFontbuf++ : ~(*pFontbuf++));
    }
}

 /**
  * @brief  OLED_xShowEN���ַ���
  * @param  x,y: ��ʼ������(x:0~127, y:0~7);
    *
    * @retval ��
  */
void OLED_xShowEN(unsigned char x, unsigned char y, unsigned char * pFontbuf, unsigned char isTurn)
{
    unsigned char wm=0;
    OLED_SetPos( x , y );
    // �����ʾ
    for( wm = 0; wm < 8; wm++ )
    {
        WriteDat( 0 );
    }
    OLED_SetPos( x,y + 1 );
    for(wm = 0; wm < 8; wm++)
    {
        WriteDat( 0 );
    }

    OLED_SetPos( x , y );
    for( wm = 0; wm < 8; wm++ )
    {
        WriteDat( !isTurn ? *pFontbuf++ : ~(*pFontbuf++));
    }
    OLED_SetPos( x,y + 1 );
    for(wm = 0; wm < 8; wm++)
    {
        WriteDat( !isTurn ? *pFontbuf++ : ~(*pFontbuf++));
    }
}

 /**
  * @brief  OLED_DrawBMP����ʾBMPλͼ
  * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~7);
    *                    x1,y1 : ���Խ���(������)������(x1:1~128,y1:1~8)
    * @retval ��
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

    if(y1%8==0)
        y = y1/8;
    else
        y = y1/8 + 1;
    for(y=y0;y<y1;y++)
    {
        OLED_SetPos(x0,y);
        for(x=x0;x<x1;x++)
            {
                WriteDat(BMP[j++]);
            }
    }
}

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t IIC_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* �ȴ���ʱ��Ĵ���,���������Ϣ */
    IIC_ERROR("IIC �ȴ���ʱ!errorCode = %d",errorCode);
    return 0;
}

void delay ( int i )
{
    int j, k;

    for ( j = 0; j < i; j++ )
    {
        for ( k = 0; k < 990; k++ )
        {
            ;
        }
    }
}


void LCD_GPIO_Config ( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure I2C1 pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init ( GPIOB, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init ( GPIOB, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init ( GPIOB, &GPIO_InitStructure );
    LCD_BKLIGHT_HIGH ();
    LCD_RESET_LOW ();
    delay ( 1000 );
    LCD_RESET_HIGH ();
    delay ( 1000 );
    initial_lcd ();
    LCD_BKLIGHT_LOW ();
    //clear_screen ();                                //clear all dots
}


//--------------wait a switch,jump out if P2.0 get a signal"0"------------------
void transfer ( int data1 )
{
    int i;

    for ( i = 0; i < 8; i++ )
    {
        LCD_SCLK_LOW ();                            //scl=0;
        LCD_SCLK_LOW ();                            //scl=0;

        if ( data1 & 0x80 )
        {
            LCD_SDA_HIGH ();                        //sda=1;
        }
        else
        {
            LCD_SDA_LOW ();                         //sda=0;
        }

        LCD_SCLK_LOW ();                            //scl=0;
        LCD_SCLK_HIGH ();                           //scl=1;
        LCD_SCLK_HIGH ();                           //scl=1;
        LCD_SCLK_LOW ();                            //scl=0;
        data1 = data1 << 1;
    }

    LCD_SDA_LOW ();                                 //sda=0;
    LCD_SDA_LOW ();                                 //sda=0;
    LCD_SCLK_HIGH ();                               //scl=1;
    LCD_SCLK_HIGH ();                               //scl=1;
    LCD_SCLK_LOW ();                                //scl=0;
    LCD_SCLK_LOW ();                                //scl=0;
}


void start_flag ()
{
    LCD_SCLK_HIGH ();                               //scl=1;
    LCD_SCLK_HIGH ();                               //scl=1;
    LCD_SCLK_HIGH ();                               //scl=1;
    LCD_SCLK_HIGH ();                               //scl=1;
    LCD_SDA_HIGH ();                                //sda=1;
    LCD_SDA_HIGH ();                                //sda=1;
    LCD_SDA_HIGH ();                                //sda=1;
    LCD_SDA_HIGH ();                                //sda=1;
    LCD_SDA_LOW ();                                 //sda=0;
    LCD_SDA_LOW ();                                 //sda=0;
    LCD_SDA_LOW ();                                 //sda=0;
    LCD_SDA_LOW ();                                 //sda=0;
}


void stop_flag ()
{
    LCD_SCLK_HIGH ();                               //scl=1;
    LCD_SCLK_HIGH ();                               //scl=1;
    LCD_SDA_LOW ();                                 //sda=0;
    LCD_SDA_LOW ();                                 //sda=0;
    LCD_SDA_HIGH ();                                //sda=1;
    LCD_SDA_HIGH ();                                //sda=1;
}

void initial_lcd ()
{
    start_flag ();                                  //��ʼ��־
    transfer ( 0x7e );                              //ѡ��SLAVE ADDRESS
    transfer ( 0x00 );                              //��ʾ���´�����ֽ���ָ��*/
    transfer ( 0x3b );                              //�������ã�MX=1,MY=1,PD=0,H1=1,H0=1,
    transfer ( 0x03 );                              //�����λ*/
    transfer ( 0x38 );                              //�������ã�MX=1,MY=1,PD=0,H1=0,H0=0,
    transfer ( 0x05 );                              //����VLCD�ķ�Χ��0X05��ʾ�ߵ�ѹ 0X04��ʾ�͵�ѹ
    transfer ( 0x0c );                              //����ʾ
    transfer ( 0x39 );                              //�������ã�MX=1,MY=1,PD=0,H1=0,H0=1,
    transfer ( 0x08 );                              //��ʾ���ã�DO=0,V=0,(Top/bottom row mode set data order)
    transfer ( 0x10 );                              //BIAS����Ϊ��0x10:1/11,0x11:1/10,0x12:1/9
    transfer ( 0xff );                              //�Աȶ����ã������0x80�������0xff����ֵԽ��ԱȶȾ�Խ��
    stop_flag ();                                    //������־
}

