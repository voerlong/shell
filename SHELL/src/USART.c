#include "USART.h"

char data[DATA_ARRAY_LENGTH] = {0};
uint8_t count = 0;


/**********************************************************
** ������: Usart_Configuration
** ��������: ����1����, ��������ʱ�ӣ�GPIO����
** �������: ��
** �������: ��
***********************************************************/
void Usart_Configuration(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);	//��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1,ENABLE);//�򿪴���1��ʱ��
	
	NVIC_InitTypeDef NVIC_Initstrue;									//�������жϣ�����Ҫ����жϳ�ʼ���ṹ��
	USART_InitTypeDef USART_InitStruct;								//���ڳ�ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;									//GPIO��ʼ��ָ��
	
	//Rx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;				
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStruct);								//��ʼ������
	
	//Tx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStruct);								//��ʼ������
	
	
	USART_InitStruct.USART_BaudRate = 230400;																		//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ�����ƣ��رգ�
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;									//�������պͷ���
	USART_InitStruct.USART_Parity = USART_Parity_No;														//û��У��λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;													//һ��ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;										//ÿ�η��ͽ������ݳ���
 
	USART_Init(USART1,&USART_InitStruct);																			//��ʼ������
	USART_Cmd(USART1,ENABLE);																									//ʹ�ܴ���
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);															//USART_IT_RXNE:���������ж�
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);															//USART_IT_RXNE:���������ж�
	
	NVIC_Initstrue.NVIC_IRQChannel = USART1_IRQn;					//������stm32f10x.hͷ�ļ���
	NVIC_Initstrue.NVIC_IRQChannelCmd = ENABLE;						//ʹ���ж�
	NVIC_Initstrue.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�
	NVIC_Initstrue.NVIC_IRQChannelSubPriority = 1;				//��Ӧ���ȼ�
	NVIC_Init(&NVIC_Initstrue);
}


/*******************************************************************************
������USARTSendArray
���ܣ���������
���������
	USARTx�����ڣ�x��ʾ���ںţ�
	array������
�����������
���أ���
*******************************************************************************/
void USARTSendArray(USART_TypeDef* USARTx,const char * strbuf,unsigned short len)
{
	uint16_t i=0;
	while(i <= len)
	{
		USART_SendData(USARTx, *(strbuf + i) );
		while((USART1->SR&0X40)==0);
		i++;
	}
}



/*******************************************************************************
������puts
���ܣ���������
���������
	USARTx�����ڣ�x��ʾ���ںţ�
	array������
�����������
���أ���
*******************************************************************************/
void USART1_puts(const char * strbuf,unsigned short len)
{
	USARTSendArray(USART1 ,strbuf ,len);
}


/*******************************************************************************
������USART1_IRQHandler
���ܣ�����1�ж�
�����������
�����������ȫ�ֱ�����data
���أ���
*******************************************************************************/
void USART1_IRQHandler(void) //���� 1 �жϷ������ 
{ 
	//�ж� USARTx ���ж����� USART_IT �Ƿ������
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		data[count] = USART_ReceiveData(USART1);												//���ڽ��ն�ȡ��������	
		count++;
		USART_ClearFlag(USART1,USART_IT_RXNE); 								//���ͨ�������һ������жϱ�־λ�ĺ����� 
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
		shell_input(&shellx ,data ,count);
		count = 0;
		USART1->SR;				//ǿ�����SR�Ĵ���
		USART1->DR;				//ǿ�����DR�Ĵ���
	}
}
	





