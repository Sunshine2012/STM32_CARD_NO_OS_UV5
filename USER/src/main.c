#include <includes.h>

u8 g_ucIsUpdateMenu     = 0;            // ������ʾ
u8 g_ucCurDlg           = 0;            // ��ǰ��ʾ�Ĳ˵�ID
u8 g_ucCurID            = 1;            // ��ǰͨ���豸�ĺ�
u8 g_ucUpWorkingID      = 1;            // �Ϲ�λ����������
u8 g_ucUpBackingID      = 2;            // �Ϲ�λ���ÿ�����
u8 g_ucDownWorkingID    = 3;            // �¹�λ����������
u8 g_ucDownBackingID    = 4;            // �¹�λ���ÿ�����
u8 g_ucaCardIsReady[4]  = {1, 1, 1, 1}; // ������
u8 g_ucaFaultCode[4]    = {0, 0, 0, 0}; // �����Ƿ���δ����Ĺ���
u8 g_ucaDeviceIsSTBY[4] = {1, 1, 1, 1}; // �ϻ��������������ڴ���(Standby)״̬��,��������,�����յ�����������Ϣ,��ʱֻ����������,���ֻ�յ�һ��������Ϣ,��ֱ�ӷ���
u8 g_ucaMechineExist[4] = {0, 0, 0, 0}; // �����Ƿ���ڲ�ͨ������

RxQueue g_tRxQueueCan = {0};        // ����
CanRxMsg g_tRxQueueMsg = {0};       // ����Ԫ��

void bspInit (void)
{
    delayInit();                // ��ʱ����
    LED_Init ();                // ��ʼ�� LED

    USART1_Config ();           // ��ʼ�� USART1
    //USART4_Config ();         // ��ʼ�� USART4


    //DAC_init();
    matrix_keyboard_init ();

    lcdInit();

    initQueue(&g_tRxQueueCan);

    canInit();                  // ��ʼ��CANͨ��
}


void lcdRef()
{
    u8 key = KEY_NUL;

    if (g_ucCurDlg == DLG_MAIN && g_ucIsUpdateMenu || g_ucKeyValues == KEY_QUIT)
    {
        g_ucIsUpdateMenu = 0;
        g_ucKeyValues = KEY_NUL;
        doShowStatusMenu(DLG_STATUS, 5, NULL);      // ��ʾ�˵�,��Ҫ����ʾ���к�
    }

    if (g_ucaFaultCode[0] != 0 || g_ucaFaultCode[1] != 0 || g_ucaFaultCode[2] != 0 || g_ucaFaultCode[3] != 0)
    {
        doShowFaultCode (DLG_CLEAR_LCD, 5, NULL);
    }

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    if ( key == KEY_ENTRY )
    {
        doShowMainMenu (DLG_MAIN, 0, NULL);
        g_ucIsUpdateMenu = 1;
    }
    if (g_ucKeyValues != KEY_NUL)
    {
        switch (g_ucCurDlg)
        {
            case DLG_STATUS:
                doShowStatusMenu(DLG_STATUS, 5, NULL);      // ��ʾ������˵�,��ǰ״̬
                break;
            case DLG_MAIN:
                doShowMainMenu (DLG_MAIN, 0, NULL);         // ��������״̬
                break;
            case DLG_WORKING_SET:

                break;

            case DLG_STATUS_TWO:
                doShowStatusOne (DLG_STATUS_ONE, 5, NULL);
                break;
            case DLG_CARD_COUNT_SET:
                doShowCardCountSet (DLG_CARD_COUNT_SET, 0, NULL);
                break;
            case DLG_DEBUG_MAIN:

                break;
            case DLG_DEBUG_ONE:

                break;
            case DLG_DEBUG_TWO:

                break;
            case DLG_DEBUG_THREE:

                break;

        }
    }
}

int main(void)
{
    u8 ret = 0;

    bspInit();

    myCANTransmit(gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
    myCANTransmit(gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
    myCANTransmit(gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
    myCANTransmit(gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
    //memset( &g_tRxQueueMsg, 0 ,sizeof (g_tRxQueueMsg) );
    doShowStatusMenu(DLG_STATUS, 5, NULL);      // ��ʾ�˵�,��Ҫ����ʾ����
    while (1)
    {

        ret = outQueue( &g_tRxQueueCan ,&g_tRxQueueMsg );
        if ( 0 == ret )
        {
            analyzeCANFrame(g_tRxQueueMsg);
        }
        matrix_update_key();                            // ɨ�谴��
        lcdRef();
    }
}

