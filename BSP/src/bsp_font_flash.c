/**
 ******************************************************************************
 * @file    bsp_xxx.c
 * @author  STMicroelectronics
 * @version V1.0
 * @date    2013-xx-xx
 * @brief   spi font flash �ײ�Ӧ�ú���bsp
 ******************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:
 * ��̳
 * �Ա�
 *
 ******************************************************************************
 */

#include "bsp_font_flash.h"
#include "includes.h"
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/**
  * @brief  FONT_SPI_FLASH��ʼ��
  * @param  ��
  * @retval ��
  */
void FONT_SPI_FLASH_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ��SPIʱ�� */
    FONT_FLASH_SPI_APBxClock_FUN ( FONT_FLASH_SPI_CLK , ENABLE );

    /* ʹ��SPI������ص�ʱ�� */

    /* ����SPI�� CS���ţ���ͨIO���� */
    GPIO_InitStructure.GPIO_Pin = FONT_FLASH_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(FONT_FLASH_SPI_CS_PORT, &GPIO_InitStructure);

    /* ����SPI�� SCK����*/
    GPIO_InitStructure.GPIO_Pin = FONT_FLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(FONT_FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

    /* ����SPI�� MISO����*/
    GPIO_InitStructure.GPIO_Pin = FONT_FLASH_SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(FONT_FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

    /* ����SPI�� MOSI����*/
    GPIO_InitStructure.GPIO_Pin = FONT_FLASH_SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(FONT_FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

    /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
    FONT_SPI_FLASH_CS_HIGH();

    /* SPI ģʽ���� */
    // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(FONT_FLASH_SPIx , &SPI_InitStructure);

    /* ʹ�� SPI  */
    SPI_Cmd(FONT_FLASH_SPIx , ENABLE);

}





/**
 * @brief  ��ȡFLASH����
 * @param   pBuffer���洢�������ݵ�ָ��
 * @param   ReadAddr����ȡ��ַ
 * @param   NumByteToRead����ȡ���ݳ���
 * @retval ��
 */
void FONT_SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
    /* ѡ��FLASH: CS�͵�ƽ */
    FONT_SPI_FLASH_CS_LOW();

    /* ���� �� ָ�� */
    FONT_SPI_FLASH_SendByte(JLX128_FastReadData);
    /* ���� �� ��ַ��λ */
    FONT_SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* ���� �� ��ַ��λ */
    FONT_SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* ���� �� ��ַ��λ */
    FONT_SPI_FLASH_SendByte(ReadAddr & 0xFF);

    FONT_SPI_FLASH_SendByte(Dummy_Byte);

    /* ��ȡ���� */
    while (NumByteToRead--) /* while there is data to be read */
    {
        /* ��ȡһ���ֽ�*/
        *pBuffer = FONT_SPI_FLASH_SendByte(Dummy_Byte);
        /* ָ����һ���ֽڻ����� */
        pBuffer++;
    }

    /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
    FONT_SPI_FLASH_CS_HIGH();
}

/**
 * @brief  ʹ��SPI��ȡһ���ֽڵ�����
 * @param  ��
 * @retval ���ؽ��յ�������
 */
u8 FONT_SPI_FLASH_ReadByte(void)
{
    return (FONT_SPI_FLASH_SendByte(Dummy_Byte));
}

/**
 * @brief  ʹ��SPI����һ���ֽڵ�����
 * @param  byte��Ҫ���͵�����
 * @retval ���ؽ��յ�������
 */
u8 FONT_SPI_FLASH_SendByte(u8 byte)
{
    SPITimeout = SPIT_FLAG_TIMEOUT;
    /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
    while (SPI_I2S_GetFlagStatus(FONT_FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
    {
        if ((--SPITimeout) == 0) 
        {
            SPI_TIMEOUT_UserCallback(0);
            break;
        }
    }

    /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
    SPI_I2S_SendData(FONT_FLASH_SPIx , byte);

    //SPITimeout = SPIT_FLAG_TIMEOUT;
    /* �ȴ����ջ������ǿգ�RXNE�¼� */
    //while (SPI_I2S_GetFlagStatus(FONT_FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
    //{
    //    if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
    //}

    /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
    //return SPI_I2S_ReceiveData(FONT_FLASH_SPIx );
    return 0;
}


/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* �ȴ���ʱ��Ĵ���,���������Ϣ */
    FONT_FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
    return 0;
}

#if 0
/**
 * @brief  ��LCD��ָ��λ����ʾһ���ַ���
 * @param  x:ˮƽ��ʼλ��(0~128);y��ֱ��ʼλ��(0~8);text:ָ����ʾ�ַ������׵�ַ;isTurn:�Ƿ���
 * @retval ��
 */
void display_GB2312_string(u8 x, u8 y, unsigned char *text, unsigned char isTurn)
{
    u8 i = 0;
    u8 fontbuf[32] = {0};
    u32 fontaddr = 0;
    while ((text[i] > 0x00) && i < 16)
    {
        if ( ( ( text[i] >= 0xb0 ) && ( text[i] <= 0xf7 ) ) && ( text[i+1] >= 0xa1 ) )
        {
            /*������壨GB2312�������ھ���Ѷ�ֿ� IC �еĵ�ַ�����¹�ʽ�����㣺*/
            /*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32 + BaseAdd;BaseAdd=0*/
            fontaddr = ( ( text[i] - 0xb0 ) * 94 + ( text[i + 1] - 0xa1 ) + 846 ) * 32;
            //get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );      /*ȡ 32 ���ֽڵ����ݣ��浽"fontbuf[32]"*/
            FONT_SPI_FLASH_BufferRead(fontbuf, fontaddr, sizeof (fontbuf) );        /*ȡ 32 ���ֽڵ����ݣ��浽"fontbuf[32]"*/
            //display_graphic_16x16(y,x,fontbuf);                                   /*��ʾ���ֵ� LCD �ϣ�y Ϊҳ��ַ��x Ϊ�е�ַ��fontbuf[]Ϊ����*/
            OLED_xShowCN(x, y, fontbuf, isTurn);
            i += 2;
            x += 16;
        }
        else if ( ( ( text[i] >= 0xa1 ) && ( text[i] <= 0xa3 ) ) && ( text[i+1] >= 0xa1 ) )
        {
            /*������壨GB2312��15x16 ����ַ��ھ���Ѷ�ֿ� IC �еĵ�ַ�����¹�ʽ�����㣺*/
            /*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
            fontaddr = (( text[i]- 0xa1 ) * 94 + ( text[i + 1] - 0xa1)) * 32;
            FONT_SPI_FLASH_BufferRead(fontbuf, fontaddr, sizeof (fontbuf) );        /*ȡ 32 ���ֽڵ����ݣ��浽"fontbuf[32]"*/
            OLED_xShowCN(x, y, fontbuf, isTurn);
            i += 2;
            x += 16;
        }
        else if ((text[i]>=0x20) &&(text[i]<=0x7e))
        {
            unsigned char fontbuf[16];
            fontaddr = (( text[i] - 0x20) * 16) + 0x3cf80;             //0x3b7c0
            FONT_SPI_FLASH_BufferRead( fontbuf, fontaddr, 16 );        /*ȡ 16 ���ֽڵ����ݣ��浽"fontbuf[32]"*/
            OLED_xShowEN(x, y, fontbuf , isTurn);
            i += 1;
            x += 8;
        }
        else
            i++;
    }
}

/**
 * @brief  ��LCD��ָ��λ����ʾһ������ַ�
 * @param  x:ˮƽ��ʼλ��(0~128);y��ֱ��ʼλ��(0~8);uchar:��ʾ���ַ�;isTurn:�Ƿ���
 * @retval ��
 */

/******************************************************************/
void display_GB2312_char(u8 x, u8 y, unsigned char uchar, unsigned char isTurn)
{
    u32 fontaddr = 0;
    unsigned char fontbuf[16];
    fontaddr = (( uchar - 0x20) * 16) + 0x3cf80;             //0x3b7c0
    FONT_SPI_FLASH_BufferRead( fontbuf, fontaddr, 16 );        /*ȡ 16 ���ֽڵ����ݣ��浽"fontbuf[32]"*/
    OLED_xShowEN(x, y, fontbuf , isTurn);
}
#endif


/*********************************************END OF FILE**********************/
