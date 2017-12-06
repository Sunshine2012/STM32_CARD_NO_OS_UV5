#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"
#include <stdio.h>

//#define  sFLASH_ID                    0xEF3015    //AT26XX16
//#define  sFLASH_ID                    0xEF4015    //W25Q16
//#define  sFLASH_ID                    0XEF4018    //W25Q128
//#define  sFLASH_ID                      0XEF4017    //W25Q64
#define  sFLASH_ID                      0x1F0400    //AT26F004

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/*�����-��ͷ*******************************/

#define AT26XX_FastReadData             0x0B
#define AT26XX_ReadData                 0x03

#define AT26XX_BlockErase4K             0x20
#define AT26XX_BlockErase32K            0x52
#define AT26XX_BlockErase64K            0xD8
#define AT26XX_ChipEraseFirst           0x60
#define AT26XX_ChipEraseSecend          0xc7

#define AT26XX_ByteProgram              0x02
#define AT26XX_SequentialByteProgram    0xAF

#define AT26XX_WriteEnable              0x06
#define AT26XX_WriteDisable             0x04
#define AT26XX_ProtectSector            0x36
#define AT26XX_UnprotectSector          0x39
#define AT26XX_ReadSectorProtection     0x3c

#define AT26XX_ReadStatusReg            0x05
#define AT26XX_WriteStatusReg           0x01

#define AT26XX_DeviceID                 0x9F
#define AT26XX_PowerDown                0xB9
#define AT26XX_ReleasePowerDown	        0xAB

/* WIP(busy)��־��FLASH�ڲ�����д�� */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
/*�����-��β*******************************/


/*SPI�ӿڶ���-��ͷ****************************/
#define      FLASH_SPIx                         SPI1
#define      FLASH_SPI_APBxClock_FUN            RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CLK                      RCC_APB2Periph_SPI1

//CS(NSS)���� Ƭѡѡ��ͨGPIO����
#define      FLASH_SPI_CS_APBxClock_FUN         RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CS_CLK                   RCC_APB2Periph_GPIOA
#define      FLASH_SPI_CS_PORT                  GPIOA
#define      FLASH_SPI_CS_PIN                   GPIO_Pin_1

//SCK����
#define      FLASH_SPI_SCK_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define      FLASH_SPI_SCK_CLK                  RCC_APB2Periph_GPIOA
#define      FLASH_SPI_SCK_PORT                 GPIOA
#define      FLASH_SPI_SCK_PIN                  GPIO_Pin_5
//MISO����
#define      FLASH_SPI_MISO_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MISO_CLK                 RCC_APB2Periph_GPIOA
#define      FLASH_SPI_MISO_PORT                GPIOA
#define      FLASH_SPI_MISO_PIN                 GPIO_Pin_6
//MOSI����
#define      FLASH_SPI_MOSI_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MOSI_CLK                 RCC_APB2Periph_GPIOA
#define      FLASH_SPI_MOSI_PORT                GPIOA
#define      FLASH_SPI_MOSI_PIN                 GPIO_Pin_7

#define  		SPI_FLASH_CS_LOW()     						GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define  		SPI_FLASH_CS_HIGH()    						GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )

/*SPI�ӿڶ���-��β****************************/

/*�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*��Ϣ���*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-SPI-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-SPI-FLASH-DEBUG->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                            if(FLASH_DEBUG_ON)\
                                            printf("<<-SPI-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                            }while(0)

void SPI_FLASH_init(void);
void SPI_FLASH_UnprotectSector(u32 SectorAddr);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_ChipErase(void);
void SPI_FLASH_ByteWrite(u32 WriteAddr, u8 ch);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteDisnable(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);


#endif /* __SPI_FLASH_H */

