#include "SWM166.h"

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_ACMP0_INP,  0);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_ACMP0_INN,  0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_ACMP1_INP,  0);
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_ACMP1_INN,  0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_ACMP2_INP,  0);
	PORT_Init(PORTB, PIN0,  PORTB_PIN0_ACMP2_INN,  0);
//	PORT_Init(PORTA, PIN14, PORTA_PIN14_ACMP3_INP, 0);
//	PORT_Init(PORTB, PIN14, PORTB_PIN14_ACMP3_INN, 0);
	
	SYS->ACMPCR |= (1 << SYS_ACMPCR_CMP0ON_Pos) | 
				   (1 << SYS_ACMPCR_CMP1ON_Pos) |
				   (1 << SYS_ACMPCR_CMP2ON_Pos);

	SYS->ACMPCR2 = (1 << SYS_ACMPCR2_0NVR_Pos) |
				   (1 << SYS_ACMPCR2_1NVR_Pos) |
				   (1 << SYS_ACMPCR2_2NVR_Pos);
	SYS->PGACR  |= (1 << SYS_PGACR_P8K2_Pos);		//ACMP0、ACMP1、ACMP2的P端经8.2K电阻相连，相连点电平替代VREF电平作为它们的N端输入

	while(1==1)
	{
		printf("ACMP0->OUT = %d   ACMP1->OUT = %d   ACMP2->OUT = %d\n", (SYS->ACMPSR & SYS_ACMPSR_CMP0OUT_Msk) ? 1 : 0,
																		(SYS->ACMPSR & SYS_ACMPSR_CMP1OUT_Msk) ? 1 : 0,
																		(SYS->ACMPSR & SYS_ACMPSR_CMP2OUT_Msk) ? 1 : 0);
		
		for(i = 0; i < SystemCoreClock/32; i++) __NOP();
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN0, PORTA_PIN0_UART0_RX, 1);	//GPIOA.0配置为UART0 RXD
	PORT_Init(PORTA, PIN1, PORTA_PIN1_UART0_TX, 0);	//GPIOA.1配置为UART0 TXD
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
