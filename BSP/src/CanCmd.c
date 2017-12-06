/************************************************************
  * ��Ȩ����(C)2014, ���ݺ���ͨ�ż��Źɷ����޹�˾.
  * ��Ŀ����:       XX�豸�Կ����
  * �ļ�����:       CanCmd.c
  * ����ƽ̨:       STM32F107VC
  * ���뻷��:       Keil 4.72 for Arm
  * ��������:       c
  * ����ժҪ:  
  *----------------------------------------------------------
  * ��    ��:       .���
  * ��������:       20140507
  * �� �� ��:       v1.0
  * ��    ��:
  *---------------------------------------------------------- 
  * �� �� ��:  
  * �޸�����:
  * ��    ��:
************************************************************/



#include "CanCmd.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_can.h"
#include "stdlib.h"
#include "View.h"
#include "stdio.h"
#include "stdlib.h"

// ���ػ�״̬
u8 g_PowerState = 0;

// GPS״̬
u8 g_GPSState = 0;

// �ߵ�״̬
u8 g_IMUState = 0;

// ϵͳ����ģʽ1
u8 g_WorkMode1 = 0;

// ϵͳ����ģʽ2
u8 g_WorkMode2 = 0;

// ϵͳ����״̬
u8 g_WorkState = 0;

// ϵͳ�����־
u8 g_FlagErr = 0;

// �ź�ǿ��
u8 g_Strength = 0;


// ���н��յ�����Ϣ�Ļ���
CanRxMsg RxMessage  = {0};

// ���յ�����Ϊ0x14�Ķ���:���Ǹ�״̬
RxQueue g_tRxQueue_0x14;
// ���յ�����Ϊ0x36�Ķ���:��λ��,������
RxQueue g_tRxQueue_0x36;
// ���յ�����Ϊ0x49�Ķ���
// RxQueue g_tRxQueue_0x49;
// ���յ�����Ϊ0x1b�Ķ���:���Ǿ���
RxQueue g_tRxQueue_0x1b;
// ���յ�����Ϊ0x1f�Ķ���:GPS������γ��
RxQueue g_tRxQueue_0x1f;

// ���յ�����Ϊ0x1f�Ķ���:GPS������γ��
RxQueue g_tRxQueue_0x49;


//RxQueue aa = 
//{
//    .top = 1
//};


/**********************************************************************
  * ��������:   char * StrRev (char * str)
  * ��������:   �ַ�����ת
  * �������:   Ҫ��ת���ַ���
  * �������:   str
  * �� �� ֵ:   char * str
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   
  * ����˵��:   
************************************************************************/
char * StrRev (char * str)
{
    char *p1 = NULL;
    char *p2 = NULL;
    
    if (!str || !*str)
    {
        return str;
    }
    
    for (p1 = str, p2 = str + strlen (str) - 1; p2 > p1; ++p1,--p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

/**********************************************************************
  * ��������:   char * ftoa (int n, char *s, int b)
  * ��������:   ����������ת����ASCII
  * �������:   int n:�Ѿ�ת��Ϊ���͵ĸ�����(100��), char *s:Ҫ�������ݵ�����, int b:ת����Ŀ�����
  * �������:   char *s
  * �� �� ֵ:   char *s
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   
  * ����˵��:   
************************************************************************/

char * ftoa (int n, char *s, int b)
{
    static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i = 0,sign = 0;
    int temp = 0;
    if ((sign = n) < 0)
    {    
        n = -n;
    }
    temp = n;
    do
    {
        // ������������Ǳ��Ŵ���100����,���Լ�������2λС�� 
        if (2 == i)
        {
            s[i++] = '.';
        }
        s[i++] = digits[n % b];
        
    }while((n /= b) > 0);
    // �����ʱС��10,����Ϊ����0,��С����,һ��ǰ��0
    if (temp < b)
    {
        s[i++] = '0';
        s[i++] = '.';
        s[i++] = '0';
    }
    // �����ʱ����С��100,����10,����Ϊ����С����,��һ��ǰ��0
    if (temp >= b && temp < (b * b))
    {
        s[i++] = '.';
        s[i++] = '0';
    }
    
    // �������С��0,��Ҳ��Ϊ��ǰ�����'-'
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
    
    return StrRev(s);
}

// �������ݽ���,�������Զ����е�,Ҳ������ȡ֮��ֱ�ӽ��н���
u8 RxDataCmdPro (CanRxMsg tRxTemp)
{
    static unsigned short int siAzi = 0; 
    static unsigned short int siEle = 0;
    static short int siSatLongitude = 0;    // ���Ǿ���
    static short int siGPS_Longitude = 0;   // GPS����
    static short int siGPS_Latitude = 0;    // GPSγ��
    static short int siStrength = 0;        // �ź�ǿ��    
    
    // ����ת����ASCII���ַ�������,��СΪ128
    char cAtemp[128] = " ";
    switch (tRxTemp.Data[0])
    {
        case (0x14):
            siStrength = MAKE_HALF_WORD (tRxTemp.Data[1],tRxTemp.Data[2]);
            
            // �ź�ǿ�����Ϊ100,������20ȡ��
            g_Strength = siStrength / 20;
        
            // PowerState = 1  Ϊ����״̬
            g_PowerState = (TESTBIT (tRxTemp.Data[3], 3)) & 0x01;
            if ((g_PowerState == 0) && (0x01 == g_tSvv[2].select[0]))// ѡ���˿���, �������,ϵͳû������,��ʾ����,����Ѱ��
            {
                strcpy ((char *)StatusPara[5],(char *)StatusPara[7]);
            }
            else 
            {
                strcpy ((char *)StatusPara[5],(char *)ShutDown[(~g_PowerState & 0x01) + 1]);
            }
            
            g_FlagErr = tRxTemp.Data[3];
            g_WorkMode1 = tRxTemp.Data[5] & 0x07;
            g_WorkMode2 = tRxTemp.Data[4] & 0x03;
           // g_WorkState = 

            g_GPSState = tRxTemp.Data[3] >> 2 & 0x01;
            
            break;
        case (0x1b):// ���Ǿ���
            siSatLongitude = MAKE_HALF_WORD(tRxTemp.Data[1],tRxTemp.Data[2]);// 
            // ���Ѿ��Ŵ�100����float����ת����10���Ƶ��ַ���
            ftoa (siSatLongitude, cAtemp, 10);
            // 
            strcpy ((char *)StatusPara[6],cAtemp);
            break;
        case (0x1f):// GPS����
            // ����
            siGPS_Longitude = MAKE_HALF_WORD(tRxTemp.Data[1],tRxTemp.Data[2]);
            // γ��
            siGPS_Latitude = MAKE_HALF_WORD(tRxTemp.Data[3],tRxTemp.Data[4]);
            // ת��������GPS ��γ��
            if (siGPS_Longitude > -18000 && siGPS_Longitude < 18000)
            {
                ftoa (siGPS_Longitude, cAtemp, 10);
                strcpy ((char *)StatusPara[3],cAtemp);
            }
            if (siGPS_Latitude > 0 && siGPS_Latitude < 9000)
            {
                ftoa (siGPS_Latitude, cAtemp, 10);
                strcpy ((char *)StatusPara[4],cAtemp);
            }
            break;
        case (0x36):

            // ��λ��
            siAzi = MAKE_HALF_WORD(tRxTemp.Data[1],tRxTemp.Data[2]);
            // ������
            siEle = MAKE_HALF_WORD(tRxTemp.Data[3],tRxTemp.Data[4]);
            if (siEle > 32768)
            {
                siEle -= 65536;
            }
            // ���Ѿ��Ŵ�100����float����ת����10���Ƶ��ַ�������������״̬
            ftoa (siAzi, cAtemp, 10);
            strcpy ((char *)StatusPara[1],cAtemp);
            // ���Ѿ��Ŵ�100����float����ת����10���Ƶ��ַ���
            ftoa (siEle, cAtemp, 10);
            strcpy ((char *)StatusPara[2],cAtemp);
            
            // ���յ��ķ�λ���븩���Ǹ��µ��ֶ����Ʋ˵� 
//            if (g_tSvv[2].select[0] != 3)
//            {
//                int temp = atoi(cAtemp);
//            }
                
            
            break;
#if 0 
            case (0x49):
            // ��λ��
            siAzi = MAKE_HALF_WORD(tRxTemp.Data[1],tRxTemp.Data[2]);
            // ������
            siEle = MAKE_HALF_WORD(tRxTemp.Data[3],tRxTemp.Data[4]);
            if (siEle > 32768)
            {
                siEle -= 65536;
            }
            // ���Ѿ��Ŵ�100����float����ת����10���Ƶ��ַ�������������״̬
            ftoa (siAzi, cAtemp, 10);
            strcpy ((char *)StatusPara[1],cAtemp);
            // ���Ѿ��Ŵ�100����float����ת����10���Ƶ��ַ���
            ftoa (siEle, cAtemp, 10);
            strcpy ((char *)StatusPara[2],cAtemp);
            
            // ���յ��ķ�λ���븩���Ǹ��µ��ֶ����Ʋ˵� 
//            if (g_tSvv[2].select[0] != 3)
//            {
//                int temp = atoi(cAtemp);
//            }
            
            break;
#endif 
        default :
            break;
        
    }
    return 0;
    
}


/**********************************************************************
  * ��������:   u8 InitQueue (RxQueue * tRxQueue)
  * ��������:   ��ʼ������
  * �������:   RxQueue * tRxQueue: ����ָ��
  * �������:   
  * �� �� ֵ:   �ɹ�Ϊ0
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   20140512
  * ����˵��:   ��
************************************************************************/
u8 InitQueue (RxQueue * tRxQueue)
{
    memset (tRxQueue, 0, sizeof (RxQueue));
	tRxQueue->empty = 1;    // ����Ϊ�� 
	tRxQueue->full = 0;     // ����Ϊ�� 
	tRxQueue->top = 0;      // ���е��±�ָ��
	tRxQueue->bottom = 0;   // ���е�Ŀ��ָ��
    return 0;
}


/**********************************************************************
  * ��������:   u8 InQueue (RxQueue * const p_tQueue, CanRxMsg * const p_tNewNode)
  * ��������:   ���������,�������õ�Ϊѭ������,���д�СΪRX_DATA_BUFF_SIZE,�����ݴ�����,����ͷ������ɾ��
  * �������:   RxQueue * tRxQueue: ����ָ��, CanRxMsg * const p_tNewNode:��������е�����ָ��
  * �������:   
  * �� �� ֵ:   �ɹ�Ϊ0, ���ɹ�Ϊ-1����-2
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   20140512
  * ����˵��:   ��
************************************************************************/
u8 InQueue (RxQueue * const p_tQueue, CanRxMsg * const p_tNewNode)
{
    int i = 0;
    // ���е�ַ��Ч
    if (!p_tQueue)
        return -1;
    // ��������ݵ�ַ��Ч
    if (!p_tNewNode)
        return -1;    
    // ����������һ��Ԫ�ؽ��бȽ�,��������������ȫһ��,�����ݲ������,ֱ�ӷ���,���������ظ�����
    if (1 != p_tQueue->empty)
    {
        for (i = 0; i < 8; i++)
        {
            // ����������,������Ƚ�,ֱ�����һ������
            if (p_tQueue->news[p_tQueue->top - 1].Data[i] == p_tNewNode->Data[i])
            {
                continue;
            }
            else 
            {
                break;
            }
        }
    }
    // ���Ƚϵ����һ�����ݻ������,��ֱ�ӷ���
    if (8 == i)
    {
        return -2;
    }
    if (((p_tQueue->top + 1) % RX_DATA_BUFF_SIZE) == p_tQueue->bottom)   
    {
        // ѭ������
       p_tQueue->bottom =  (p_tQueue->bottom + 1) % RX_DATA_BUFF_SIZE; 
        // ������
        p_tQueue->full = 1;
    }
    // ���ݽ������
    p_tQueue->news[p_tQueue->top] = *p_tNewNode;
    p_tQueue->top = (p_tQueue->top + 1) % RX_DATA_BUFF_SIZE;
    p_tQueue->empty = 0;
    return 0;
}


/**********************************************************************
  * ��������:   u8 OutQueue (RxQueue * const p_tQueue, CanRxMsg * p_tReturnNode)
  * ��������:   ������
  * �������:   RxQueue * const p_tQueue:�����׵�ַ, CanRxMsg * p_tReturnNode:������Ԫ�ر����ַ
  * �������:   ������Ԫ��
  * �� �� ֵ:   �ɹ�Ϊ0
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   
  * ����˵��:   
************************************************************************/
u8 OutQueue (RxQueue * const p_tQueue, CanRxMsg * p_tReturnNode)
{
    if (!p_tQueue)
        return -1;  
    if (!p_tReturnNode)
        return -1;
    if (1 == p_tQueue->empty)
        return -1;
    // �ն���
    if ((p_tQueue->bottom % RX_DATA_BUFF_SIZE) == p_tQueue->top)
    {
        p_tQueue->empty = 1;
        return -1;
    }    
    // ���ݳ�����
    *p_tReturnNode = p_tQueue->news[p_tQueue->bottom];
    p_tQueue->bottom = (p_tQueue->bottom + 1) % RX_DATA_BUFF_SIZE; 
    p_tQueue->full = 0;
    return 0;
}


/**********************************************************************
  * ��������:   void CANTransmit (CanTxMsg * pTxd)
  * ��������:   ��������
  * �������:   CanTxMsg * pTxd:����������ݵĵ�ַ
  * �������:   ��
  * �� �� ֵ:   ��
  * �� �� ��:   ���
  * ����ʱ��:   20140511
  * �޸�ʱ��:   
  * ����˵��:   
************************************************************************/
void CANTransmit (CanTxMsg * pTxd)
{  
    u32 i = 0;
    u8 TransmitMailbox;
    // ���������ǵ����ݶ��Ǳ�׼֡,����������ͳһ���ó�����
    pTxd->IDE = CAN_ID_STD;
    pTxd->RTR = CAN_RTR_DATA;

    TransmitMailbox=CAN_Transmit(CAN1,pTxd);
    
    // �ȴ����ݷ������
    i = 0;
    while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i != 0xFF))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending(CAN1,CAN_FIFO0) < 1) && (i != 0xFF))
    {
        i++;
    }
    
    
}



/**********************************************************************
  * ��������:   u8 RptSatPara (StructViewValue *gp_tSvv)
  * ��������:   ���Ҫ���͵�����
  * �������:   StructViewValue *gp_tSvv:Ҫ�������ݵĽṹ
  * �������:   
  * �� �� ֵ:   �ɹ�Ϊ0
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:
  * ����˵��:
************************************************************************/
u8 RptSatPara()
{
    
    return 0;
}

/**********************************************************************
  * ��������:   u8 ViewValueSendSelect (StructViewValue *gp_tSvv)
  * ��������:   ���Ҫ���͵�����
  * �������:   StructViewValue *gp_tSvv:Ҫ�������ݵĽṹ
  * �������:   
  * �� �� ֵ:   �ɹ�Ϊ0
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:
  * ����˵��:
************************************************************************/
u8 ViewValueSendSelect (StructViewValue *gp_tSvv)
{
    int i = 0;
    int j = 0;
    int temp = 0;
    int  iTtemp = 0;
    char cAtemp[20] = " ";
    CanTxMsg TxMessage;
    memset (&TxMessage, 0, sizeof (CanRxMsg));
    
    switch (g_RealView.ID)
    {
        case VIEW_ID_SATSELECT:                 // ����ѡ��
            switch (g_RealView.Cursor)
            {
                case 1:
                    // ��ѡ�������ǵ����Ƹ��Ƶ���ؽ���
                    strcpy (StatusPara[0], g_tSvv[0].para[0][g_tSvv[0].select[0]]);
                    break;
                case 2:
                    // ѡ�����Ǿ���
                    gp_tSvv->select[0] == 0x01 ? (cAtemp [0] = '+') : (cAtemp [0] = '-');
                    // ���ƾ��ȵĺ�6λ
                    for (i = 1; i <= 6; i++)
                    {
                        cAtemp [i] = gp_tSvv->select[i] - 1 + '0';
                    }
                    // ����С�����������
                    cAtemp[4] = '.';     
                    cAtemp[7] = '\0';
                    
                    // ������ת���ɸ����Ͳ��Ŵ�100��Ϊ����
                    iTtemp = atof (cAtemp) * 100;
                    // ������180��-180֮��
                    if (iTtemp > -18000 && iTtemp < 18000)
                    {                            
                        ftoa (iTtemp, cAtemp, 10);
                        // ���ĺõľ��ȸ��Ƹ�״̬��ʾ����
                        strcpy ((char *)StatusPara[6],cAtemp);

                        TxMessage.StdId = CAN_TX_STD_ID;
                        TxMessage.DLC = SEND_SIZE_8;
                        TxMessage.Data[0] = ACU_STA_CMD; 
                        //TxMessage.Data[0] = ACU_PARAMETER; 
                        TxMessage.Data[1] = LOWBYTE(iTtemp);
                        TxMessage.Data[2] = HIGHBYTE(iTtemp);
                        CANTransmit (&TxMessage);
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                    break;
                case 3:
                    
                    // �����Ǵ�������Ϳ�(��)������
                    TxMessage.StdId = CAN_TX_STD_ID;
                    TxMessage.DLC = SEND_SIZE_2;
                    // ���ѡ��ػ�(��ShutDown[2]),����0x00,����0x01
                    TxMessage.Data[0] = ACU_SHUTDOWN_CMD;
                    (gp_tSvv->select[0] == 2) ?  (TxMessage.Data[1] = ACU_TURN_DOWN) : \
                                                (TxMessage.Data[1] = ACU_TURN_ON);
                    // �������ο�������
                    while (j < 3)
                    {
                        CANTransmit (&TxMessage);
                        j++;
                    }  
                    
                    // ���ѡ��Ĳ����ֶ�,�������޸�Ԥ�÷�λ��,Ԥ�ø�����
                    if (3 == gp_tSvv->select[0])
                    {
                        g_tAngleSet[0].isvalue = 1;
                        g_tAngleSet[1].isvalue = 1; 
                        
                        TxMessage.StdId = CAN_TX_STD_ID;
                        TxMessage.DLC = SEND_SIZE_4;
                        TxMessage.Data[0] = ACU_HAND_MOVEMENT; 
                        TxMessage.Data[1] = ACU_HAND;   // �ֶ�
                        CANTransmit (&TxMessage);        
                        
                    }
                    else
                    {
                        g_tAngleSet[0].isvalue = 0;
                        g_tAngleSet[1].isvalue = 0; 

                        TxMessage.StdId = CAN_TX_STD_ID;
                        TxMessage.DLC = SEND_SIZE_4;
                        TxMessage.Data[0] = ACU_HAND_MOVEMENT; 
                        TxMessage.Data[1] = ACU_AUTO;   // �Զ�
                        CANTransmit (&TxMessage);
                    }
                    
                    return 0;
                    break;
                default :
                    break;
            }
            break;
            
        case VIEW_ID_MANUAL_CONTROL:            // �ֶ�����
             switch (g_RealView.Cursor)
                {
                    case 1:
                        // ���Ʒ�λ�ǵ�6λ
                        for (i = 0; i <= 6; i++)
                        {
                            cAtemp [i] = gp_tSvv->select[i] - 1 + '0';
                        }
                        // ����С�����������
                        cAtemp[3] = '.';     
                        cAtemp[6] = '\0';
                        
                        // ����λ��ת���ɸ����Ͳ��Ŵ�100��Ϊ����
                        iTtemp = atof (cAtemp) * 100;
                        // ��λ����0��360֮��
                        if (iTtemp > 0 && iTtemp < 36000)
                        {                            
                            ftoa (iTtemp, cAtemp, 10);
                            // ���ĺõľ��ȸ��Ƹ�״̬��ʾ����
                            strcpy ((char *)StatusPara[1],cAtemp);

                            TxMessage.StdId = CAN_TX_STD_ID;
                            TxMessage.DLC = SEND_SIZE_8;
                            TxMessage.Data[0] = ACU_HAND_MOVEMENT; 
                            TxMessage.Data[1] = ACU_AZI;
                            TxMessage.Data[2] = LOWBYTE(500/10);
                            TxMessage.Data[3] = HIGHBYTE(500/10);
                            TxMessage.Data[4] = CMD_0x55;
                            
                            CANTransmit (&TxMessage);
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                        break;
                    case 2:
                        // ���Ƹ����ǵ�6λ
                        for (i = 0; i <= 6; i++)
                        {
                            cAtemp [i] = gp_tSvv->select[i] - 1 + '0';
                        }
                        // ����С�����������
                        cAtemp[3] = '.';     
                        cAtemp[6] = '\0';
                        
                        // ����λ��ת���ɸ����Ͳ��Ŵ�100��Ϊ����
                        iTtemp = atof (cAtemp) * 100;
                        // ��������23��79��֮��
                        if (iTtemp > 2300 && iTtemp < 7900)
                        {                            
                            ftoa (iTtemp, cAtemp, 10);
                            // ���ĺõľ��ȸ��Ƹ�״̬��ʾ����
                            strcpy ((char *)StatusPara[2],cAtemp);

                            TxMessage.StdId = CAN_TX_STD_ID;
                            TxMessage.DLC = SEND_SIZE_8;
                            TxMessage.Data[0] = ACU_HAND_MOVEMENT; 
                            TxMessage.Data[1] = ACU_ELE;
                            TxMessage.Data[2] = LOWBYTE(100/10);
                            TxMessage.Data[3] = HIGHBYTE(100/10);
                            TxMessage.Data[4] = CMD_0x55;
                            
                            CANTransmit (&TxMessage);
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                        break;
                    default :
                        break;
                }
            break;
            
        default:
        
            break;
    
    }
	
    return 0;
}


/**********************************************************************
  * ��������:   void CANTransmitTest (void)
  * ��������:   ���Ͳ��Ժ���
  * �������:   ��
  * �������:   ��
  * �� �� ֵ:   ��
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   
  * ����˵��:   
************************************************************************/
void CANTransmitTest (void)
{  
    u32 i = 0;
    u8 TransmitMailbox;
    CanTxMsg TxMessage;
    // ����
    memset (&TxMessage, 0, sizeof (CanRxMsg));
    /* transmit */
    TxMessage.StdId = 0x6060 >> 5;
    TxMessage.ExtId = 0x00;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;;
    TxMessage.DLC = SEND_SIZE_8;
    TxMessage.Data[0] = 0x14;
    TxMessage.Data[1] = 0x00;
    TxMessage.Data[2] = 0x00;
    TxMessage.Data[3] = 0x81;
    TxMessage.Data[4] = 0x00;
    TxMessage.Data[5] = 0x00;
    TxMessage.Data[6] = 0xaa;
    TxMessage.Data[7] = 0xf0;
    TransmitMailbox = CAN_Transmit (CAN1,&TxMessage);
    i = 0;
    while((CAN_TransmitStatus (CAN1,TransmitMailbox) != CANTXOK) && (i != 0xFF))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending (CAN1,CAN_FIFO0) < 1) && (i != 0xFF))
    {
        i++;
    }
}


#if 0
/*******************************************************************************
* Function Name  : CAN_Polling
* Description    : Configures the CAN and transmit and receive by polling
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
*******************************************************************************/
TestStatus CAN_Polling(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    /* CAN register init */
    CAN_DeInit(CAN1);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    
    //������ CANʱ�� = 36MHz / 5 = 7.2MHz, CAN������ = 7.2MHz / (1 + 6 + 5) = 500Kb/s
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Prescaler = 6;
    CAN_Init(CAN1, &CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}
#endif


/**********************************************************************
* Function Name  : CAN_Interrupt
* Description    : Configures the CAN and transmit and receive by interruption
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
***********************************************************************/
TestStatus CAN_Interrupt(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CanTxMsg TxMessage;
    u32 i = 0;

    /* CAN register init */
    CAN_DeInit(CAN1);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

    // ������ CANʱ�� = 36MHz / 6 = 6MHz, CAN������ = 6MHz / (1 + 6 + 5) = 500Kb/s
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Prescaler = 6;  

    CAN_Init(CAN1,&CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    /* CAN FIFO0 message pending interrupt enable */ 
    CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);

    return (TestStatus)0;
}


/**********************************************************************
  * ��������:   void NVIC_Configuration(void)
  * ��������:   CAN����,ϵͳ�жϹ������� 
  * �������:   ��
  * �������:   ��
  * �� �� ֵ:   ��
  * �� �� ��:   ���
  * ����ʱ��:   20140509
  * �޸�ʱ��:   
  * ����˵��:   
************************************************************************/
void NVIC_Configuration(void)
{ 
	NVIC_InitTypeDef NVIC_InitStructure;

  	/* Configure the NVIC Preemption Priority Bits */  
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  //NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* enabling interrupt */
    // CAN�ж�����
    NVIC_InitStructure.NVIC_IRQChannel = 20;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}


/**********************************************************************
  * ��������:   void GPIO_Configuration(void)
  * ��������:   ����CAN�ڵ�GPIO����,LEDָʾ�Ĺ���
  * �������:   ��
  * �������:   ��
  * �� �� ֵ:   ��
  * �� �� ��:   ���
  * ����ʱ��:   20140509
  * �޸�ʱ��:   
  * ����˵��:   
************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  	/* Configure PD.02, PD.03, PD.04 and PD.07 as Output push-pull */
  	// For STM3210B-LK1 use PD.02 -PC.07
  	// GPD.2ΪCANоƬ��ʹ������,�͵�ƽ��Ч
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    
  	/* Configure CAN pin: RX */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  	/* Configure CAN pin: TX */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap2_CAN, ENABLE);	   //��Ӱ��CAN IO�ŵ� PD0��PD1
   
    GPIO_SetBits (GPIOE, GPIO_Pin_0);// �ر��ĸ�LED
    GPIO_SetBits (GPIOE, GPIO_Pin_1);
    GPIO_SetBits (GPIOE, GPIO_Pin_2);
    GPIO_SetBits (GPIOE, GPIO_Pin_3);
}


/**********************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler    void CAN1_RX0_IRQHandler(void)
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.�жϷ����ӳ���
* Input          : None
* Output         : None
* Return         : None
************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{
    // �رս����ж�
    CAN_ITConfig (CAN1,CAN_IT_FMP0, DISABLE);
    memset (&RxMessage, 0, sizeof (RxMessage));
    
    CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
    
    switch (RxMessage.Data[0])
    {
        case (0x14):    // ϵͳ״̬
            InQueue (&g_tRxQueue_0x14, &RxMessage);
            break;
        case (0x1b):    // ���ǲ�������
            InQueue (&g_tRxQueue_0x1b, &RxMessage);
            break; 
        case (0x1f):    // GPS����
            InQueue (&g_tRxQueue_0x1f, &RxMessage);
            break; 
        case (0x22):    // �������
            PEout(2) = 0;
            break;
        case (0x36):    // ���սǶȲ���
            InQueue (&g_tRxQueue_0x36, &RxMessage);
            break;
        
        case (0x49):
            InQueue (&g_tRxQueue_0x49, &RxMessage);
            break;
        default :
            break;
         
    }

    // ���ж�
    CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);
}


/**********************************************************************
  * ��������:   void CanCMD(void)
  * ��������:   CANͨ�ź���
  * �������:   ��
  * �������:   ��
  * �� �� ֵ:   ��
  * �� �� ��:   ���
  * ����ʱ��:   20140508
  * �޸�ʱ��:   
  * ����˵��:   
************************************************************************/
void CanCMD(void)
{
    int i = 0;
    CanRxMsg tMegTemp;
	// CAN��GPIO����
    GPIO_Configuration ();
    // CAN�ж�����
	NVIC_Configuration();
	// CAN������,����������,���ж�,���ȴ������ж�
    CAN_Interrupt ();   
	// ��ʼ�����ն���
    InitQueue(&g_tRxQueue_0x14);
    InitQueue(&g_tRxQueue_0x1b);
    InitQueue(&g_tRxQueue_0x1f);
    InitQueue(&g_tRxQueue_0x36);
    InitQueue(&g_tRxQueue_0x49);
    
    // ��������CAN������2msһ֡�����ʴ���,20ms���յ�10֡����,���뵽��������,20ms���һ�ν��ն���,������
	while (1)
    {
        //CANTransmitTest ();
        if (!OutQueue (&g_tRxQueue_0x14, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        if (!OutQueue (&g_tRxQueue_0x1b, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        if (!OutQueue (&g_tRxQueue_0x1f, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        if (!OutQueue (&g_tRxQueue_0x36, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        if (!OutQueue (&g_tRxQueue_0x49, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        OSTimeDly (20);
    }
}