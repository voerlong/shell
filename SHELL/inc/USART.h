#ifndef __USART__H
#define __USART__H

#include "stm32f10x.h"
#include "shell.h"

#define  DATA_ARRAY_LENGTH 512

void USART1_puts(const char * strbuf,unsigned short len);


void Usart_Configuration(void);

extern char data[DATA_ARRAY_LENGTH];

#endif

