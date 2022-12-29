/************************************************
	������USB����λ��ͨ������ļ�
	���ߣ�С��
	Ӣ������overlong
************************************************/


/************************************************
	ͷ�ļ�
************************************************/
#include "shell_control.h"



/************************************************
	ȫ�ֱ���
************************************************/
char* tab_arg[4] ={0};



/************************************************
	����
************************************************/
void aaa(void)
{
	printf("\r\n��ʾ��ǰ���������ID");
}

/*******************************************************************************
������look_time
���ܣ��鿴ϵͳʱ��
���������
	arg���������ڴ�
�����������
���أ���
*******************************************************************************/
static void look_time(void * arg)
{
	RCC_ClocksTypeDef  get_rcc_clock; 
	RCC_GetClocksFreq(&get_rcc_clock);
	uint32_t data[] = {get_rcc_clock.SYSCLK_Frequency ,get_rcc_clock.HCLK_Frequency ,get_rcc_clock.PCLK1_Frequency ,
										 get_rcc_clock.PCLK2_Frequency  ,get_rcc_clock.ADCCLK_Frequency};
	printf("\r\n\tSYSCLK:%uHz" 	,data[0]);
	printf("\r\n\tHCLK:%uHz" 		,data[1]);
	printf("\r\n\tPCLK1:%uHz" 	,data[2]);
	printf("\r\n\tPCLK2:%uHz" 	,data[3]);
	printf("\r\n\tADCCLK:%uHz" 	,data[4]);
	
	printk("\r\n\r\nSTM32F1 >");
}


/*******************************************************************************
������shell_control_explain
���ܣ���������ID����help�н��������
���������
	ID�������ID����32λ��
�����������
���أ���
*******************************************************************************/
void shell_control_explain(unsigned int* ID)
{
	uint32_t tab[] = {0x8CC6D608 ,0x8D4C533C ,
										0x8d598e99 ,0x8DC87250 ,
										0x8E09AF53 ,0x9152F754 ,
										0xA08971F3 ,0xCD1A7A1A ,
										0xD9BDA819 ,0xDA1061B3 };
	char* str[] = 	 {"\r\n����ָ�" 																												,"\r\n�鿴GPIOx����״̬��[����] [GPIOx(GPIOA~G)]",
										"\r\n�鿴���������ID��" 																								,"\r\n�鿴оƬ��ϵͳʱ�Ӳ�����",
										"\r\n�鿴����ʱ��״̬��" 																								,"\r\ndebug������Ϣ��",
										"\r\n�鿴�����Ѿ�ע���˵����" 																				,"\r\n�������Ÿߵ͵�ƽ��[����] [GPIOx(GPIOA~G)] [GPIO_Pin(0~15)] [1/0]",
										"\r\n�鿴shell�汾��" 																									,"\r\n�鿴����汾��"};
	for(int i = 0 ;i < 10 ;i++){
		if(*ID == tab[i]){
			printf("%s" ,str[i]);
		}
	}
}



/*******************************************************************************
������shell_version
���ܣ���ʾ��ǰ����汾
���������
	arg���������ڴ�
�����������
���أ���
*******************************************************************************/
static void software_version(void * arg)
{
	printk("\r\n\t%s\r\n",software_VERSION);
	printk("\r\n\r\nSTM32F1 >");
}



/*******************************************************************************
������check_peripheral
���ܣ��鿴����ʱ��״̬
���������
	arg���������ڴ�
�����������
���أ���
*******************************************************************************/
static void check_peripheral(void * arg)
{
	uint16_t temporary = 0;
	uint16_t a = 0;
	uint8_t array[][array_len] = {"AFIOEN" ,"IOPAEN" ,"IOPBEN" ,"IOPCEN" ,"IOPDEN" ,"IOPEEN" ,"ADC1EN" ,"ADC2EN" ,"TIM1EN" ,"SPI1EN" ,"USART1EN"};
	uint16_t APB2 = (uint16_t)(RCC->APB2ENR & 0x00007FFF);
	
	a = APB2;
	APB2 =  (a & 0x0001) | ((a & 0x007c) >> 1) | ((a & 0x1E00) >> 3) | ((a & 0x4000) >> 4);
	for(int i = 0 ;i < 11 ;i++){
		temporary = (APB2 >> i) & 0x0001;
		printf("\r\n\t%s:%d" ,array[i] ,temporary);
	}
	printk("\r\n\r\nSTM32F1 >");
}


/*******************************************************************************
������check_GPIO
���ܣ��鿴GPIO״̬
���������
	arg���������ڴ�
�����������
���أ���
*******************************************************************************/
static void check_GPIO(void * arg)
{
	char* argv[2] = {0};
	uint16_t ReadValue = 0;
	int data_falg = NO;
	int argc = cmdline_strtok((char*)arg,argv,2);
	if(strcmp(argv[1] ,"GPIOA") == 0){
		ReadValue = GPIO_ReadOutputData(GPIOA);
		data_falg = EN;
	}
	else if(strcmp(argv[1] ,"GPIOB") == 0){
		ReadValue = GPIO_ReadOutputData(GPIOB);
		data_falg = EN;
	}
	else if(strcmp(argv[1] ,"GPIOC") == 0){
		ReadValue = GPIO_ReadOutputData(GPIOC);
		data_falg = EN;
	}
	else if(strcmp(argv[1] ,"GPIOD") == 0){
		ReadValue = GPIO_ReadOutputData(GPIOD);
		data_falg = EN;
	}
	else
		data_falg = NO;
	if(data_falg == NO){
		printf("\r\n\tError");
	}
	else{
		uint16_t temporary = 0;
		for(int i = 0 ;i < 16 ;i++){
			temporary = (ReadValue >> i) & 0x0001;
			printf("\r\n\tP%c%d:%d" ,*(argv[1]+4) ,i ,temporary);
		}
	}
	data_falg = NO;
	printk("\r\n\r\nSTM32F1 >");
}







/*******************************************************************************
������set_GPIO
���ܣ��޸�GPIO
���������
	arg���������ڴ�
�����������
���أ���
*******************************************************************************/
static void set_GPIO(void * arg)
{
	//���巽��ʹ�õı����Լ�����
	char* argv[4] = {0};
//	int argc = cmdline_strtok((char*)arg,argv,4);
	for(int i = 0 ;i < 4 ;i++){
		argv[i] = tab_arg[i];
	}
	uint8_t GPIOPIN = 0;
	const uint16_t GPIO_PIN_arrat[] = {GPIO_Pin_0 ,GPIO_Pin_1 ,GPIO_Pin_2 ,GPIO_Pin_3 ,GPIO_Pin_4 ,GPIO_Pin_5 ,
															 GPIO_Pin_6 ,GPIO_Pin_7 ,GPIO_Pin_8 ,GPIO_Pin_9 ,GPIO_Pin_10,GPIO_Pin_11,
															 GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15};
	const char* GPIO_PIN_nmb[16] = {"0" ,"1" ,"2" ,"3" ,"4" ,"5" ,"6" ,"7" ,"8" ,"9" ,
															 "10","11","12","13","14","15"};
	const char* GPIO_str_array[7] = {"GPIOA" ,"GPIOB" ,"GPIOC" ,"GPIOD" ,"GPIOE" ,"GPIOF" ,"GPIOG"};
	GPIO_TypeDef * GPIOx[7] = {GPIOA ,GPIOB ,GPIOC ,GPIOD ,GPIOE ,GPIOF ,GPIOG};
	int data_falg = NO;		//��ȷ�����־λ
	
	//�ж������Ƿ�Ϲ�
	if(strcmp(argv[1] ,"GPIOA") == 0 || strcmp(argv[1] ,"GPIOB") == 0 || strcmp(argv[1] ,"GPIOC") || strcmp(argv[1] ,"GPIOD") == 0){
		for(char i = 0 ;i < 16 ;i++){
			if(strcmp(argv[2] ,GPIO_PIN_nmb[i]) == 0){
				if(*argv[3] == '0' || *argv[3] == '1')
					data_falg = EN;
				GPIOPIN = i;
			}
		}
	}
	
	//�Ϲ��ʼ����GPIO����
	if(data_falg == EN){
		data_falg = NO;
		for(int i = 0 ;i < 7 ;i++){
			if(strcmp(argv[1] ,GPIO_str_array[i]) == 0){
				if(*argv[3] == '1'){
					GPIO_SetBits(GPIOx[i] ,GPIO_PIN_arrat[GPIOPIN]);
					data_falg = EN;
					if((GPIO_ReadOutputData(GPIOx[i]) & (0x0001 << GPIOPIN)) == 0){//�ж������Ƿ���ȷ
						data_falg = NO;
					}
				}
				else{
					GPIO_ResetBits(GPIOx[i] ,GPIO_PIN_arrat[GPIOPIN]);
					data_falg = EN;
					if((GPIO_ReadOutputData(GPIOx[i]) & (0x0001 << GPIOPIN)) == 1){//�ж������Ƿ���ȷ
						data_falg = NO;
					}
				}
			}
		}
		
		//�ж��Ƿ����óɹ�
		if(data_falg == EN)
			printf("\r\n\t�ɹ��޸�%s_Pin%sΪ%s" ,argv[1] ,argv[2] ,argv[3]);
		else
			printf("\r\n\tError");
	}
	else
		printf("\r\n\tError");
	data_falg = NO;
	printk("\r\n\r\nSTM32F1 >");
}


/*******************************************************************************
������really_set_GPIO
���ܣ�ѯ���Ƿ�ȷ���޸�GPIO
���������
	arg���������ڴ�
�����������
���أ���
*******************************************************************************/
static void really_set_GPIO(void * arg)
{
	char* argv[4] = {0};
	int argc = cmdline_strtok((char*)arg,argv,4);
	printf("\r\n�Ƿ�ȷ���޸�%s_Pin%sΪ%s" ,argv[1] ,argv[2] ,argv[3]);
	for(int i = 0 ;i < 4 ;i++){
		tab_arg[i] = argv[i];
	}
	shell_confirm_GPIO(&shellx ,"",set_GPIO);
}




/*******************************************************************************
������shell_conteol_register
���ܣ�ע������
�����������
�����������
���أ���
*******************************************************************************/
void shell_conteol_register(void)
{
	shell_register_command("version_software" 			,software_version);
	shell_register_command("check_sysclock" 				,look_time);
	shell_register_command("check_gpio" 						,check_GPIO);
	shell_register_command("check_peripheral" 			,check_peripheral);
	shell_register_command("set_gpio" 							,really_set_GPIO);
	
}

