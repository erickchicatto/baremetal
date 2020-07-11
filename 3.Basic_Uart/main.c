// APIB1 bit 17 : USART2
//USART connected to PA2 

#include "stm32f4xx.h"                  // Device header

void USART2_Init(void);
void USART_write(int ch);
void delayMs(int delay);

int main(void){
	USART2_Init();
	
	while(1){
		USART_write('Y');
		USART_write('i');
		USART_write('\r');
		USART_write('\n');
		delayMs(10);
		}		
}


void USART2_Init(void){
	
	RCC->APB1ENR |= 0x20000; // si se expande en binario es equivalente a expander en bits
	RCC->AHB1ENR |= 1; // para el PINA2 , alternate function que es en donde se encuentra 
	
	GPIOA->AFR[0] = 0x0700; // este valor equivale directo a AF7 0111 , pero en hexadecimal. escribe en todo el puerto
	GPIOA->MODER |= 0x0020; // para activar el PA2 como alternate function
	
	USART2 -> BRR = 0x0683  ;// 9600 , 16Mhz escribe en todo el puerto
	USART2 -> CR1 = 0x0008; // Enable TX
	USART2 -> CR1 |= 0x2000; // Activa el bit 13 y activa el USART
	
}



void USART_write(int ch){
	// wait while TX buffer is empty 
	while(!(USART2->SR & 0x0080)){}
	USART2->DR = (ch & 0xFF);
}



void delayMs(int delay){
	int i ; 
	for(;delay>0;delay--){
		for(i=0;i<3195;i++);
	}
}