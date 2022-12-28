#include "USART.h"

char data[DATA_ARRAY_LENGTH] = {0};
uint8_t count = 0;


/**********************************************************
** 函数名: Usart_Configuration
** 功能描述: 串口1配置, 包括串口时钟，GPIO配置
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void Usart_Configuration(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);	//打开GPIOA的时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1,ENABLE);//打开串口1的时钟
	
	NVIC_InitTypeDef NVIC_Initstrue;									//若开启中断，则需要这个中断初始化结构体
	USART_InitTypeDef USART_InitStruct;								//串口初始化结构体
	GPIO_InitTypeDef GPIO_InitStruct;									//GPIO初始化指针
	
	//Rx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;				
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStruct);								//初始化引脚
	
	//Tx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStruct);								//初始化引脚
	
	
	USART_InitStruct.USART_BaudRate = 230400;																		//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件控制（关闭）
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;									//开启接收和发送
	USART_InitStruct.USART_Parity = USART_Parity_No;														//没有校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;													//一个停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;										//每次发送接收数据长度
 
	USART_Init(USART1,&USART_InitStruct);																			//初始化串口
	USART_Cmd(USART1,ENABLE);																									//使能串口
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);															//USART_IT_RXNE:开启接收中断
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);															//USART_IT_RXNE:开启接收中断
	
	NVIC_Initstrue.NVIC_IRQChannel = USART1_IRQn;					//储存在stm32f10x.h头文件中
	NVIC_Initstrue.NVIC_IRQChannelCmd = ENABLE;						//使能中断
	NVIC_Initstrue.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级
	NVIC_Initstrue.NVIC_IRQChannelSubPriority = 1;				//响应优先级
	NVIC_Init(&NVIC_Initstrue);
}


/*******************************************************************************
函数：USARTSendArray
功能：发送数组
输入参数：
	USARTx：串口（x表示串口号）
	array：数组
输出参数：无
返回：无
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
函数：puts
功能：发送数组
输入参数：
	USARTx：串口（x表示串口号）
	array：数组
输出参数：无
返回：无
*******************************************************************************/
void USART1_puts(const char * strbuf,unsigned short len)
{
	USARTSendArray(USART1 ,strbuf ,len);
}


/*******************************************************************************
函数：USART1_IRQHandler
功能：串口1中断
输入参数：无
输出参数：（全局变量）data
返回：无
*******************************************************************************/
void USART1_IRQHandler(void) //串口 1 中断服务程序 
{ 
	//判断 USARTx 的中断类型 USART_IT 是否产生中
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		data[count] = USART_ReceiveData(USART1);												//串口接收读取到的数据	
		count++;
		USART_ClearFlag(USART1,USART_IT_RXNE); 								//最后通常会调用一个清除中断标志位的函数， 
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
		shell_input(&shellx ,data ,count);
		count = 0;
		USART1->SR;				//强制清除SR寄存器
		USART1->DR;				//强制清除DR寄存器
	}
}
	





