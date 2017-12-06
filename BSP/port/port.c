#include "includes.h"
#include "port.h"

u8 Task=0;				 //����
u8 Order=0;				 //���0���Զ����У�1����������
u8 KaOn=1;				 //�û�����1���ÿ���0������
u8 Ka_state=0;			 //������������״̬
u8 RunStep;				 //�������в���

OS_ERR      err;

void NVIC0_IRQHandler(void)
{
    //ȷ���Ƿ������EXTI Line�ж�
    if (EXTI_GetITStatus(NVIC0_INT_EXTI_LINE) != RESET)
    {
        // LED1 ȡ��
        // macLED1_TOGGLE();
        DK1=1;                              //����λ��1���������ɲ��
        DK2=1;

        // ����Ҫ��ʱ

        DK1=0;                              //����λ��1���������ͣ
        DK2=0;
        Ka_state=1;
        //����жϱ�־λ
        EXTI_ClearITPendingBit(NVIC0_INT_EXTI_LINE);
    }
}

void NVIC1_IRQHandler(void)
{
    //ȷ���Ƿ������EXTI Line�ж�
    if (EXTI_GetITStatus(NVIC1_INT_EXTI_LINE) != RESET)
    {
        // LED2 ȡ��
        // macLED2_TOGGLE();
        //����жϱ�־λ
        EXTI_ClearITPendingBit(NVIC1_INT_EXTI_LINE);
    }
}

void NVIC2_IRQHandler(void)
{
    //ȷ���Ƿ������EXTI Line�ж�
    if (EXTI_GetITStatus(NVIC2_INT_EXTI_LINE) != RESET)
    {
        // LED3 ȡ��
        // macLED1_TOGGLE();
        //����жϱ�־λ
        EXTI_ClearITPendingBit(NVIC2_INT_EXTI_LINE);
    }
}

void NVIC3_IRQHandler(void)
{
    //ȷ���Ƿ������EXTI Line�ж�
    if (EXTI_GetITStatus(NVIC3_INT_EXTI_LINE) != RESET)
    {
        // LED4 ȡ��
        // macLED2_TOGGLE();
        //����жϱ�־λ
        EXTI_ClearITPendingBit(NVIC3_INT_EXTI_LINE);
    }
}



void PORT_Init(void)
{
    RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
    RCC->APB2ENR|=1<<3;     //ʹ��PORTBʱ��
    RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��

    GPIOA->CRL&=0X0000FFF0; //PA0,4,5,6,7���ó�����
    GPIOA->CRL|=0X88880008; //PA0,4,5,6,7���ó�������
    GPIOB->CRL&=0XFFFFFF00; //PB0,1���ó�����
    GPIOB->CRL|=0X00000088; //PB0,1���ó�������
    GPIOB->CRH&=0X00FF00FF; //PB10,11,14,15���ó�����
    GPIOB->CRH|=0X88008800; //PB10,11,14,15���ó�������
    GPIOC->CRH&=0X00000FFF; //PC11,12,13,14,15���ó�����
    GPIOC->CRH|=0X88888000; //PC11,12,13,14,15���ó�������
    GPIOC->CRL&=0XFFFF0000; //PC0,1,2,3���ó�����
    GPIOC->CRL|=0X00008888; //PC0,1,2,3���ó�������
    GPIOB->CRH&=0X0000FFFF; //PB12,PB13Ϊ���, PB14,PB15���ó�����
    GPIOB->CRH|=0X88110000; //PB12,PB13Ϊ�������, PB14,PB15���ó�������
    GPIOC->CRH&=0XFFFFFF00; //PC8,9���ó����
    GPIOC->CRH|=0X00000011; //PC8,9Ϊ�������

    GPIOA->ODR|=0X000000F1;	//PA0,4,5,6,7����
    GPIOB->ODR|=0X0000CC03;	//PB0,1,10,11,14,15����
    GPIOC->ODR|=0X0000F80F;	//PC0,1,2,3,11,12,13,14,15����

    DK1=0;
    DK2=0;
    FK1=0;
    FK2=0;

    // Ex_NVIC_Config(GPIO_A,5,FTIR); //�½��ش���
    // Ex_NVIC_Config(GPIO_A,6,FTIR); //�½��ش���
    // Ex_NVIC_Config(GPIO_A,7,FTIR); //�½��ش���
    // Ex_NVIC_Config(GPIO_B,0,FTIR); //�½��ش���
    // Ex_NVIC_Config(GPIO_B,1,FTIR); //�½��ش���
    // Ex_NVIC_Config(GPIO_B,10,FTIR); //�½��ش���
    // Ex_NVIC_Config(GPIO_B,11,FTIR); //�½��ش���

    // MY_NVIC_Init(0,1,EXTI0_IRQChannel,0);        //��ռ0�������ȼ�1����0
    // MY_NVIC_Init(0,2,EXTI1_IRQChannel,0);        //��ռ0�������ȼ�2����0
    // MY_NVIC_Init(0,3,EXTI9_5_IRQChannel,0);      //��ռ0�������ȼ�3����0
    // MY_NVIC_Init(0,4,EXTI15_10_IRQChannel,0);    //��ռ0�������ȼ�4����0
}

#if 0
//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
    DK1=1;                              //����λ��1���������ɲ��
    DK2=1;
    OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    DK1=0;                              //����λ��1���������ͣ
    DK2=0;
    Ka_state=1;
    EXTI->PR=1<<0;                      //���LINE0�ϵ��жϱ�־λ
}
//
//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{
    DK1=1;                              //����λ��1���������ɲ��
    DK2=1;
    OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    DK1=0;                              //����λ��2���������ͣ
    DK2=0;
    Ka_state=2;
    EXTI->PR=1<<1;                      //���LINE1�ϵ��жϱ�־λ
}
//
//�ⲿ�ж�10,11�������
void EXTI15_10_IRQHandler(void)
{
    if (DI3==0)                         //����λ��3���������ͣ
    {
        DK1=1;                          //����λ��1���������ɲ��
        DK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        DK1=0;
        DK2=0;
        Ka_state=3;
        EXTI->PR=1<<10;                 //���LINE10�ϵ��жϱ�־λ
    }
    else if (DI4==0)                    //����λ��4���������ͣ
    {
        DK1=1;                          //����λ��1���������ɲ��
        DK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        DK1=0;
        DK2=0;
        Ka_state=4;
        EXTI->PR=1<<11;                 //���LINE11�ϵ��жϱ�־λ
    }
}
//
//�ⲿ�ж�5,6,7�������
void EXTI9_5_IRQHandler(void)
{
    if (FI1==0)                         //�ÿ���ת��λ�����������ת��λ
    {
        FK1=1;                          //����λ��1���������ɲ��
        FK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        FK1=0;
        FK2=1;
        Ka_state=5;
        EXTI->PR=1<<5;                  //���LINE10�ϵ��жϱ�־λ
    }
    else if (FI2==0)                    //���������0λ
    {
        FK1=1;                          //����λ��1���������ɲ��
        FK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        FK1=0;
        FK2=0;
        Ka_state=6;
        EXTI->PR=1<<6;                  //���LINE11�ϵ��жϱ�־λ
    }
    else if (FI3==0)                    //������ת��λ�����������ת��λ
    {
        FK1=1;                          //����λ��1���������ɲ��
        FK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        FK1=1;
        FK2=0;
        Ka_state=7;
        EXTI->PR=1<<7;                  //���LINE11�ϵ��жϱ�־λ
    }
}
#endif

//��һ�������ж�
void Next_Step1(void)                   //��һ�������������ж�
{
    if (Order==0||Order==2)
    {
        RunStep=2;                      //����ڶ���ȡ��
        Ka_state=0;
    }
}
//
void Next_Step2(void)                   //�ڶ���ȡ���������ж�
{
    if (Order==0&&QK==1)                //�����Զ�����ʱ��ȡ�꿨�ٽ��������
    {
        RunStep=3;
        Ka_state=0;
    }
}
//
void Next_Step3(void)                   //�����������������ж�
{
    if (Order==0)
    {
        RunStep=4;
        Ka_state=0;
    }
}
//
void Next_Step4(void)                   //���Ĳ��������ж�
{
    if (Order==0&&YK==0)                //�����Զ�����ʱ���������п��ٷ��ص�һ��
    {
        RunStep=1;                      //���ص�һ��
        Ka_state=0;
    }
}
//
void K_Take(void)
{
    u8 k;
    for (k=0; k<=1; k++)                //ȡ����ʱ
    {
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    }
}
//
u8 kc=0;
void K_Onoff(void)
{
    u8 k;
    for (k=0; k<=1; k++)                //�п���ʱ
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    if (++kc>=10)                       //�û�������
    {
        KaOn=0;
        kc=0;
    }
    else
        KaOn=1;
}
//
//�Ϳ����к���
void K_run(void)
{
    switch (RunStep)
    {
    case 1:
        if (KaOn&&!Ka_state)            //��һ��
        {
            FK1=1;                      //���������кÿ�
            FK2=0;
            Ka_state=8;                 //���������ת��Ϊ״̬9
        }
        else if (!KaOn&&!Ka_state)      //���������л���
        {
            FK1=0;
            FK2=1;
            Ka_state=9;                 //���������ת��Ϊ״̬8
        }
        if (Ka_state==6)                //״̬6�����������0λ
        {
            Next_Step1();
        }
        break;
    case 2:
        if (!Ka_state)                  //�ڶ���
        {
            K_Take();                   //ȡ���ж�
            Next_Step2();
        }
        break;
    case 3:
        if (!Ka_state)                  //������
        {
            DK1=0;
            DK2=1;
            Ka_state=10;                //�����������
        }
        if (Ka_state==1||Ka_state==2||Ka_state==3||Ka_state==4)
        {
            OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
            Next_Step3();               //���������λ������һ��
        }
        break;
    case 4:
        if (!Ka_state)                  //���Ĳ�
        {
            K_Onoff();                  //�û����ж�
            Next_Step4();
        }
        break;
    }
}
//
//����ɨ�躯��
//���ذ���ֵ
//0��û���κΰ�������
//1��2,3,4,5����KEY����
u8 KEY_Scan(void)
{
    static u8 key_up=1;                       //�������ɿ���־
    if (key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
    {
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );                         //ȥ����
        key_up=0;
        if (KEY0==0)
        {
            return 1;
        }
        else if (KEY1==0)
        {
            return 2;
        }
        else if (KEY2==0)
        {
            return 3;
        }
        else if (KEY3==0)
        {
            return 4;
        }
    }
    else if (KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1)
    {
        key_up=1;
    }
    return 0;                                    // �ް�������
}
//
//����������
void KEY_deal(u8 step)
{
    switch (step)
    {
    case 1:
        if (LED1==0||LED0==0)           //�������������л�
        {
            RunStep=0;
            LED1=1;         //��LED
            LED0=1;
            DK1=0;
            DK2=0;
            FK1=0;
            FK2=0;
        }
        else
        {
            RunStep=1;      //�ӵ�һ����ʼ����ѭ������
            Ka_state=0;
            LED1=0;         //��LED1
            LED0=1;
            Order=0;
        }
        break;
    case 2:
        RunStep=1;          //�������ÿ�
        LED1=1;
        LED0=0;         //��LED0
        Ka_state=0;
        KaOn=1;
        Order=1;
        break;
    case 4:
        RunStep=3;          //������һ�ſ�
        LED1=1;
        LED0=0;         //��LED0
        Ka_state=0;
        Order=1;
        break;
    case 3:
        RunStep=1;          //����������
        LED1=1;
        LED0=0;         //��LED0
        Ka_state=0;
        KaOn=0;
        Order=1;
        break;
    }
}
