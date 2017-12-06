#include "includes.h"
#include "menu.h"


Dlg g_dlg[] =           {
                        {DLG_CLEAR_LCD,      "                ", "                ", "                ", "                "},

                        {DLG_LOGO,           "    ****����    ", " www.*****.com  ", "   ****������   ", "   �汾: V1.0   "},
                        {DLG_STATUS,         "1.����:         ", "2.����:         ", "3.����:         ", "4.����:         "},
                        {DLG_MAIN,           "1.����״̬      ", "2.��������      ", "3.��������      ", "4.��������      "},
                        {DLG_CARD_COUNT_SET, "1 �Ż�����      ", "2 �Ż�����      ", "3 �Ż�����      ", "4 �Ż�����      "},
                        {DLG_CARD_ID,        "   ���ÿ���ID   ", "��������        ", "������:         ", "ͨ��ID��:       "},
                        {DLG_WORKING_SET,    "  ������������  ", "1:����    2:����", "3:����    4:����", "                "},

                        {DLG_CARD_MAIN,      "����״̬:1�ſ���", "         2�ſ���", "         3�ſ���", "         4�ſ���"},
                        {DLG_STATUS_ONE,     " 1�ų���        ", " 2�ų���        ", " 3�ų���        ", " 4�ų���        "},
                        {DLG_STATUS_TWO,     " 1�ų�����      ", " 2�ų�����      ", " 3�ų�����      ", " 4�ų�����      "},

                        {DLG_DEBUG_MAIN,     "    �ſ�������  ", "1:��������      ", "2:��������      ", "                "},
                        {DLG_DEBUG_ONE,      "��: ��һ�źÿ�   ", "��: ��һ�Ż���   ", "��: ��һ�ſ�     ", "��: ѭ������     "},
                        {DLG_DEBUG_TWO,      "��: ����������   ", "��: ����������   ", "��: ����������   ", "��: ����������   "},

                        {DLG_FAULT_CODE,     "    �ſ�������  ", "������:         ", "˵��:           ", "                "},

                     };
Dlg g_dlg_fault_code[] = {
                        {DLG_CLEAR_LCD,      "                ", "                ", "                ", "                "},

                        {FAULT_CODE01,       "    �ſ�������  ", "������:(01H)    ", "˵��: ��ʼ������", "������ܻ�λ    "},
                        {FAULT_CODE02,       "    �ſ�������  ", "������:(02H)    ", "˵��: ��ʼ������", "�����ת���ܻ�λ"},
                        {FAULT_CODE03,       "    �ſ�������  ", "������:(03H)    ", "˵��: ��ʼ������", "�����ת���ܻ�λ"},
                        {FAULT_CODE04,       "    �ſ�������  ", "������:(04H)    ", "˵��: ����ʱ����", "�����ǰ��ת    "},
                        {FAULT_CODE05,       "    �ſ�������  ", "������:(05H)    ", "˵��: ����ʱ����", "�������ת    "},
                        {FAULT_CODE06,       "    �ſ�������  ", "������:(06H)    ", "˵��: ����ʱ����", "���ǰ����ת    "},
                        {FAULT_CODE07,       "    �ſ�������  ", "������:(07H)    ", "˵��: ����ʱ����", "���ǰ���س̶�ת"},
                        {FAULT_CODE08,       "    �ſ�������  ", "������:(08H)    ", "˵��: ����ʱ����", "���������ת    "},
                        {FAULT_CODE09,       "    �ſ�������  ", "������:(09H)    ", "˵��: ����ʱ����", "��������س̶�ת"},
                        {FAULT_CODE0A,       "    �ſ�������  ", "������:(0AH)    ", "˵��: ����ʱ����", "�����ת        "},
                        {FAULT_CODE0B,       "    �ſ�������  ", "������:(0BH)    ", "˵��: ����ʱ����", "�����ת        "},
                        {FAULT_CODE0C,       "    �ſ�������  ", "������:(0CH)    ", "˵��: ͨѶ����  ", "                "},
                        {FAULT_CODE0D,       "    �ſ�������  ", "������:(δ֪)   ", "                ", "                "},
                        {FAULT_CODE0E,       "    �ſ�������  ", "������:(δ֪)   ", "                ", "                "},
                        {FAULT_CODE0F,       "    �ſ�������  ", "������:(δ֪)   ", "                ", "                "},
                        {FAULT_CODE10,       "    �ſ�������  ", "˵��: �������޿�", "��װ��IC��      ", "                "},
                        {FAULT_CODE11,       "    �ſ�������  ", "������:(11H)    ", "˵��:CAN���߹���", "                "},

                        {FAULT_CODE11+1,     "    �ſ�������  ", "������:(δ֪)   ", "                ", "                "},
                     };

// �ҵ���ʾ�Ĳ˵�ID,���������������е�����
u8 check_menu(u8 ch)
{
    u8 i = 0;
    for (i = 0; i < (sizeof (g_dlg) / sizeof (g_dlg[0])); i++)
    {
        if(g_dlg[i].ID == ch)
        {
            return i;
        }
    }
    return 0;
}

// ��ʾ״̬�˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusMenu (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i;
    u8 dlgId = check_menu(dlg_id);
    u8 master1[] = "����";
    u8 master2[] = "����";
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    if(g_ucUpWorkingID == 1 && g_ucUpBackingID == 2)
    {
        for (i = 0; i < 4; i++)
        {
            g_dlg[dlgId].MsgRow[0][i + 2] = master1[i];
            g_dlg[dlgId].MsgRow[1][i + 2] = master2[i];
        }
    }
    else if (g_ucUpWorkingID == 2 && g_ucUpBackingID == 1)
    {
        for (i = 0; i < 4; i++)
        {
            g_dlg[dlgId].MsgRow[0][i + 2] = master2[i];
            g_dlg[dlgId].MsgRow[1][i + 2] = master1[i];
        }

    }
    if (g_ucDownWorkingID == 3 && g_ucDownBackingID == 4)
    {

        for (i = 0; i < 4; i++)
        {
            g_dlg[dlgId].MsgRow[2][i + 2] = master1[i];
            g_dlg[dlgId].MsgRow[3][i + 2] = master2[i];
        }

    }
    else if (g_ucDownWorkingID == 4 && g_ucDownBackingID == 3)
    {
        for (i = 0; i < 4; i++)
        {
            g_dlg[dlgId].MsgRow[2][i + 2] = master2[i];
            g_dlg[dlgId].MsgRow[3][i + 2] = master1[i];
        }

    }

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);

}

// ��ʾ�˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowMainMenu (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);
    do
    {
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                        doShowStatusOne (DLG_STATUS_ONE, 5, NULL);
                        break;
                    case 1:
                        doShowCardCountSet (DLG_CARD_COUNT_SET, 0, NULL);
                        break;
                    case 2:
                        doShowWorkingSet (DLG_WORKING_SET, 1, NULL);
                        break;
                    case 3:
                        doShowDebugMain (DLG_DEBUG_MAIN, 0, NULL);
                        break;
                    default:
                        key = KEY_NUL;
                        break;
                }
                break;
            case KEY_UP:
                if (0 < isNotRow)
                {
                    isNotRow--;
                    isTurnShow(0,isNotRow);
                    key = KEY_NUL;
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_DOWN:
                if (3 > isNotRow)
                {
                    isNotRow++;
                    isTurnShow(0,isNotRow);
                    key = KEY_NUL;
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_CANCEL:    // �˳�
                return;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,�˳�,��������ֵ
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
            isTurnShow(0,isNotRow);
        }
        g_ucKeyValues = KEY_NUL;
    }while (0);
}

// ���ÿ���������,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowCardCountSet (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 isSetMode = 0;   // ���������ģʽ/
    u8 i = 0;
    u8 j = 0;
    u8 seek = 0;        // ָʾ��ǰ���ڵ�����λ��
    u8 temp = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 num = 1;      // ������
    u16 id = 0x7810 | num;                              // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                       // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                                // CANͨ�ŵ�ID���ֽ�
    u8 cardCountShowSeek = 11;                          // �������ַ��Ӵ����еĵڼ�����ʾ,Ĭ�ϴ�11���ֽڿ�ʼ��ʾ
    u16 usaCardCount[4] = {1000,1001,1002,1003};        // ���п�����ʼ������������
    u8 str_aCardCount[4][6] = {0};                      // ���п�����ʼ��������������ʾ�ַ���
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    for (i = 0; i < 4; i++)
    {
        usaCardCount[i] = g_uiaInitCardCount[i + 1];    // ���Ƴ�ʼ�������������棬�Թ��޸�
    }
    for (i = 0; i < 4; i++)
    {
        sprintf(str_aCardCount[i],"%04d",usaCardCount[i]);      // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
    }

    for (i = 0; i < 4; i++)     // ���Ƴ�ʼ���������ַ����鵽��ʾ����
    {
        for (j = 0; j < 4; j++)
        {
            g_dlg[dlgId].MsgRow[i][j + cardCountShowSeek] = str_aCardCount[i][j];
        }
    }
    for (i = 0; i < 4; i++) // ��ʾ
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);
    g_ucKeyValues = KEY_NUL;
    do
    {
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        isSetMode = 1;      // ��������ģʽ,��ȡ�����˳����ã��ٴΰ�ȡ�������˻���һ���˵�
                        displayGB2312String (0, isNotRow * 2, g_dlg[dlgId].MsgRow[isNotRow], 0);
                        sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);      // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����

                        displayGB2312StringLen (cardCountShowSeek, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0, 4); // ��ʾ4�ֽ�

                        isTurnShow(cardCountShowSeek / 2,isNotRow);
                        key = KEY_NUL;      // ���ٸ��½�����
                        g_ucIsUpdateMenu = 0;
                        break;
                    default:
                        break;
                }
                break;
            case KEY_UP:
                if (isSetMode)
                {
                    switch(seek)
                    {
                        case 0:
                            temp = usaCardCount[isNotRow] / 1000;
                            usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 1000 : usaCardCount[isNotRow];
                            break;
                        case 1:
                            temp = usaCardCount[isNotRow] % 1000 / 100;
                            usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 100 : usaCardCount[isNotRow];
                            break;
                        case 2:
                            temp = usaCardCount[isNotRow] % 100 / 10;
                            usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 10 : usaCardCount[isNotRow];
                            break;
                        case 3:
                            temp = usaCardCount[isNotRow] % 10;
                            usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 1 : usaCardCount[isNotRow];
                            break;
                    }
                    sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);        // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
                    for (i = 0; i < 4; i++)
                    {
                        displayGB2312String (cardCountShowSeek + i, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                    isTurnShow(0,isNotRow);
                    key = KEY_NUL;      // ���ٸ��½�����
                    g_ucIsUpdateMenu = 0;
                }
                else            // ���������ģʽ,���������ֵ
                {
                    if (0 < isNotRow)
                    {
                        isNotRow--;
                        isTurnShow(0,isNotRow);
                        key = KEY_NUL;
                    }
                    else
                    {
                        key = KEY_NUL;
                    }
                }
                break;
            case KEY_DOWN:
                if (isSetMode)
                {
                    switch(seek)
                    {
                        case 0:
                            temp = usaCardCount[isNotRow] / 1000;
                            usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 1000 : usaCardCount[isNotRow];
                            break;
                        case 1:
                            temp = usaCardCount[isNotRow] % 1000 / 100;
                            usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 100 : usaCardCount[isNotRow];
                            break;
                        case 2:
                            temp = usaCardCount[isNotRow] % 100 / 10;
                            usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 10 : usaCardCount[isNotRow];
                            break;
                        case 3:
                            temp = usaCardCount[isNotRow] % 10;
                            usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 1 : usaCardCount[isNotRow];
                            break;
                    }
                    sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);        // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
                    for (i = 0; i < 4; i++)
                    {
                        displayGB2312String (cardCountShowSeek + i, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                    isTurnShow(0,isNotRow);
                    key = KEY_NUL;      // ���ٸ��½�����
                    g_ucIsUpdateMenu = 0;
                }
                else            // ���������ģʽ,���������ֵ
                {
                    if (isNotRow < 3)
                    {
                        isNotRow++;
                        isTurnShow(0,isNotRow);
                        key = KEY_NUL;
                    }
                    else
                    {
                        key = KEY_NUL;
                    }
                }

                break;
            case KEY_LEFT:
                if (isSetMode && seek > 0)
                {
                    seek--;
                    for (i = 0; i < 4; i++)
                    {
                        displayGB2312String (cardCountShowSeek + i, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                    isTurnShow(0,isNotRow);
                }
                key = KEY_NUL;
                g_ucIsUpdateMenu = 0;
                break;
            case KEY_RIGHT:
                if (isSetMode && seek < 3)
                {
                    seek++;
                    for (i = 0; i < 4; i++)
                    {
                        displayGB2312String (cardCountShowSeek + i, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                    isTurnShow(0,isNotRow);
                }
                key = KEY_NUL;      // ���ٸ��½�����
                g_ucIsUpdateMenu = 0;
                break;
            case KEY_OK:
                isSetMode = 0;      // �˳�����ģʽ
                seek = 0;                   // ���㵱ǰ���ڵ�����λ��
                g_uiaInitCardCount[isNotRow + 1] = usaCardCount[isNotRow];
                for (i = 0; i < 4; i++)     // �����޸Ĺ��Ŀ���������ʾ����
                {
                    for (j = 0; j < 4; j++)
                    {
                        g_dlg[dlgId].MsgRow[i][j + cardCountShowSeek] = str_aCardCount[i][j];
                    }
                }
                break;
            case KEY_CANCEL:
                if (!isSetMode)
                {
                    return;                     // ����Ƿ�����ģʽ,��ֱ���˳�
                }
                else
                {
                    isSetMode = 0;              // �˳�����ģʽ
                    seek = 0;                   // ���㵱ǰ���ڵ�����λ��
                }
                break;
            default:
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
            isTurnShow(0,isNotRow);
        }
        g_ucKeyValues = KEY_NUL;
    }while (0);
}



// ״̬�˵�1,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusOne (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 num = *(u8 *)p_parm;      // ������
    u8 str_num[5] = {0};
    u8 str_id[5] = {0};
    u16 id = 0x7810 | num;       // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    //LCD12864_Clear();
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);
    do
    {
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_RIGHT:
                doShowStatusTwo (DLG_STATUS_TWO, 5, NULL);
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;

            for (i = 0; i < 4; i++)
            {
                displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
    }while (0);
}


// ״̬�˵�2,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusTwo (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 num = *(u8 *)p_parm;      // ������
    u8 str_num[5] = {0};
    u8 str_id[5] = {0};
    u16 id = 0x7810 | num;       // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    //LCD12864_Clear();
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);
    do
    {
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_LEFT:
                return;
                break;
            case KEY_QUIT:
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;

            for (i = 0; i < 4; i++)
            {
                displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
    }while (0);
}



// ID���ò˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowWorkingSet (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 num = 1;      // ������
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u8 master1[] = "1:����    2:����";
    u8 master2[] = "1:����    2:����";
    u8 master3[] = "3:����    4:����";
    u8 master4[] = "3:����    4:����";
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    //u16 id = 0x7811;      // CANͨ�ŵ�ID

    if(g_ucUpWorkingID == 1 && g_ucUpBackingID == 2)
    {
        for (i = 0; i < 16; i++)
        {
            g_dlg[dlgId].MsgRow[1][i] = master1[i];
        }
    }
    else if (g_ucUpWorkingID == 2 && g_ucUpBackingID == 1)
    {
        for (i = 0; i < 16; i++)
        {
            g_dlg[dlgId].MsgRow[1][i] = master2[i];
        }
    }
    if (g_ucDownWorkingID == 3 && g_ucDownBackingID == 4)
    {

        for (i = 0; i < 16; i++)
        {
            g_dlg[dlgId].MsgRow[2][i] = master3[i];
        }
    }
    else if (g_ucDownWorkingID == 4 && g_ucDownBackingID == 3)
    {
        for (i = 0; i < 16; i++)
        {
            g_dlg[dlgId].MsgRow[2][i] = master4[i];
        }
    }
    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);
    do
    {
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                key = KEY_NUL;
                break;
            case KEY_UP:
                if (1 < isNotRow)
                {
                    isNotRow--;
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_DOWN:
                if (2 > isNotRow)
                {
                    isNotRow++;
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_LEFT:
                switch (isNotRow)
                {
                    case 1:
                        for (i = 0; i < 16; i++)
                        {
                            g_dlg[dlgId].MsgRow[isNotRow][i] = master1[i];
                        }
                        g_ucUpWorkingID = 1;
                        g_ucUpBackingID = 2;
                        break;
                    case 2:
                        for (i = 0; i < 16; i++)
                        {
                            g_dlg[dlgId].MsgRow[isNotRow][i] = master3[i];
                        }
                        g_ucDownWorkingID = 3;
                        g_ucDownBackingID = 4;
                        break;
                    default:
                        break;
                }
                break;
            case KEY_RIGHT:
                switch (isNotRow)
                {
                    case 1:
                        for (i = 0; i < 16; i++)
                        {
                            g_dlg[dlgId].MsgRow[isNotRow][i] = master2[i];
                        }
                        g_ucUpWorkingID = 2;
                        g_ucUpBackingID = 1;
                        break;
                    case 2:
                        for (i = 0; i < 16; i++)
                        {
                            g_dlg[dlgId].MsgRow[isNotRow][i] = master4[i];
                        }
                        g_ucDownWorkingID = 4;
                        g_ucDownBackingID = 3;
                    default:
                        break;

                }
                break;
            case KEY_OK:
                for (i = 0; i < 4; i++) // ÿ������������,��λ״̬
                {
                    g_ucaDeviceIsSTBY[i] = 1;
                }
                switch (isNotRow)
                {
                    case 1:
                        myCANTransmit(gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
                        myCANTransmit(gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
                        isNotRow++;
                        break;
                    case 2:
                        myCANTransmit(gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
                        myCANTransmit(gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
                        isNotRow--;
                        break;
                    default:
                        break;
                }
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                break;

        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
            isTurnShow(0,isNotRow);
        }
        g_ucKeyValues = KEY_NUL;
    }while (0);
}

// ���Բ˵���ѡ��,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugMain (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 num = 1;      // ������
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u16 id = 0x7810 | num;       // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    sprintf(str_num,"0%d",num);
    str_num [3] = 0;
    for (i = 0; i < 2; i++)
    {
        g_dlg[dlgId].MsgRow[0][i] = str_num[i];
    }
    //LCD12864_Clear();
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);
    do
    {
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                        key = KEY_NUL;
                        break;
                    case 1:
                        doShowDebugOne (DLG_DEBUG_ONE, 5, &num);
                        break;
                    case 2:
                        doShowDebugTwo (DLG_DEBUG_TWO, 5, &num);
                        break;
                    default:
                        break;
                }
                break;
            case KEY_UP:
                if (0 < isNotRow)
                {
                    isNotRow--;
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_DOWN:
                if (isNotRow < 2)
                {
                    isNotRow++;
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_LEFT:
                if (isNotRow == 0 && num > 1 )
                {
                    num--;
                    //id--;
                    sprintf(str_num, "0%d", num);
                    str_num [3] = 0;
                    for (i = 0; i < 2; i++)
                    {
                        g_dlg[dlgId].MsgRow[0][i] = str_num[i];
                    }
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_RIGHT:
                if (isNotRow == 0 && num < 4)
                {
                    num++;
                    //id++;
                    sprintf(str_num, "0%d", num);
                    str_num [3] = 0;
                    for (i = 0; i < 2; i++)
                    {
                        g_dlg[dlgId].MsgRow[0][i] = str_num[i];
                    }
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_OK:
                key = KEY_NUL;
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
            isTurnShow(0,isNotRow);
        }
        g_ucKeyValues = KEY_NUL;
    }while (0);
}



// ���Բ˵�(����),�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugOne (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 num = *(u8 *)p_parm;      // ������
    u8 str_num[5] = {0};
    u8 str_id[5] = {0};
    u16 id = 0x7810 | num;       // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    //LCD12864_Clear();
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);
    do
    {
        for (i = 0; i < 4; i++)
        {
            g_ucaDeviceIsSTBY[i] = 1;
        }
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_UP:
                isNotRow = 0;
                myCANTransmit(gt_TxMessage, num, num, SPIT_ONE_OK_CARD, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_DOWN:
                isNotRow = 1;
                myCANTransmit(gt_TxMessage, num, num, SPIT_ONE_BAD_CARD, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_LEFT:
                isNotRow = 2;
                myCANTransmit(gt_TxMessage, num, num, HOOK_ONE_CARD, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_RIGHT:
                isNotRow = 3;
                myCANTransmit(gt_TxMessage, num, num, CYCLE_SPIT_ONE_CARD, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_QUIT:
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            /*
            for (i = 0; i < 4; i++)
            {
                displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
            */
            isTurnShow(0,isNotRow);
        }
        g_ucKeyValues = KEY_NUL;
    }while (0);
}

// ���Բ˵�(����),�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugTwo (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 num = *(u8 *)p_parm;      // ������
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u16 id = 0x7810 | num;      // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    g_ucKeyContinu = 1;             // ���뵥��ģʽ
    //LCD12864_Clear();
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);
    do
    {
        for (i = 0; i < 4; i++)
        {
            g_ucaDeviceIsSTBY[i] = 1;
        }
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_UP:
                isNotRow = 0;
                myCANTransmit(gt_TxMessage, num, num, SPIT_MOTOR_POSITIVE_STEP, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_DOWN:
                isNotRow = 1;
                myCANTransmit(gt_TxMessage, num, num, SPIT_MOTOR_NEGATIVE_STEP, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_LEFT:
                isNotRow = 2;
                myCANTransmit(gt_TxMessage, num, num, HOOK_MOTOR_POSITIVE_STEP, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_RIGHT:
                isNotRow = 3;
                myCANTransmit(gt_TxMessage, num, num, HOOK_MOTOR_NEGATIVE_STEP, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_QUIT:
                break;
            case KEY_CANCEL:
                g_ucKeyContinu = 0;             // �˳�����ģʽ
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            g_ucKeyContinu = 0;             // �˳�����ģʽ
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            isTurnShow(0,isNotRow);
        }
        g_ucKeyValues = KEY_NUL;
        g_ucKeyContinu = 1;             // �ɿ�����֮��,��������������ģʽ
    }while (0);
}


// ��ʾ������,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowFaultCode (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 j = 0;
    u8 n = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 num = 1;      // ������
    u8 str_num[5] = {0};
    u16 id = 0x7810 | num;       // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    u8 faultCodeIndex = 0;
    u8 faultCode = 0;
    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < (sizeof (g_dlg_fault_code) / sizeof (g_dlg_fault_code[0])); j++)
        {
            if (g_ucaFaultCode[i] != 0 && g_ucaFaultCode[i] == g_dlg_fault_code[j].ID) // ���ҹ������Ӧ����ʾ����
            {
                faultCode = g_ucaFaultCode[i];   // ���µ�ǰ��δ����Ĺ��ϺͿ�����
                faultCodeIndex = i;
                //g_ucaFaultCode[i] = 0;
                num = i + 1;        // ��ס���ĸ������й���,���������
                sprintf(str_num,"%02d",num);
                for (n = 0; n < 2; n++)
                {
                    g_dlg_fault_code[j].MsgRow[0][n + 1] = str_num[n];
                }
                for (n = 0; n < 4; n++)
                {
                    displayGB2312String (0, n * 2, g_dlg_fault_code[j].MsgRow[n], 0);     // ��ʾ���Ͻ���
                }
                goto while_label;
            }
        }
    }

while_label:
    do
    {
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:     // �ڹ�������ʾ����,���˵������Խ������ý���,�Ҳ��������ֵ
                g_ucaFaultCode[faultCodeIndex] = faultCode; // ���浱ǰδ����Ĺ���,�����´��ٴδ���
                return;
                break;
            case KEY_CANCEL:
                g_ucaFaultCode[faultCodeIndex] = 0;
                myCANTransmit(gt_TxMessage, num, NO_FAIL, CLEAR_FAULT_CODE, CLEAR_FAULT, NO_FAIL, NO_FAIL, faultCode);
                for (i = 0; i < 4; i++)     // ���Ͻ��֮��,�����־,����ȴ�״̬,�ȴ������ķ�������
                {
                    g_ucaDeviceIsSTBY[i] = 1;
                }
                g_ucIsUpdateMenu = 1;
                g_ucCurDlg = DLG_MAIN;
                return;
                break;
            default:
                break;
        }

        g_ucKeyValues = KEY_NUL;
    }while(0);
}
