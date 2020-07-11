#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <stdio.h>


// Freq_buzzer
#define C	16
#define D 18
#define E 20



// C interface
struct __FILE{int handle;};
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr = {2};

void Cambio_Freq();
int fgetc(FILE *f);
int fputc(int c, FILE *f);
void PWM();
void Usart2_init();
void delayMs();
int USART2_write(int ch);
int USART2_read(void);




int main(void){
	// hacer un menu y controlarlo con uart?
	
	
	
}





int fgetc(FILE *f){
	 int c;
	 c= USART2_read();
	  
	 if(c=='\r'){
			USART2_write(c);
			c='\n';
		}
	 USART2_write(c);
	return c;	
}

int fputc(int c, FILE *f){
	 return USART2_write(c);
}



void Cambio_Freq(){
	
	RCC->AHB1ENR |= 1;
	GPIOA->MODER |= 0x400;; // 0100 0000 0000 PA5
	
	//Setup timmer
	RCC->APB1ENR |= 1;
	TIM2->PSC= 48000-1 ; // 300 Ms
	TIM2->ARR= 33333-1;	//16 000 000 divided by 48000 = 1 hz , se hace en partes este proceso or 1 mhz?
	TIM2->CNT = 0 ; // Clear timer
	TIM2->CR1 = 1; // Enable tim2
	
	while(1){
		// WAIT FLAG
	while((!TIM2->SR & 1)){}
		TIM2->SR &=~1;
		GPIOA->ODR ^=0x20; // output pin 5
		}
}


void PWM(){
	int i = 0 ;
	
	// Enable clock GPIOA  P5 , cambiar de pin
	RCC->AHB1ENR |= 0x01;
	GPIOA->AFR[0] |= 0x00100000;  // pag 179 tim3?
	GPIOA->MODER |= 0x00000800;
	
	//Timer Setup 
	RCC->APB1ENR |= 0x01;
	TIM2->PSC = 10-1; // ? 
	TIM2->ARR = 26667-1; // valor default para contar , maximum value that the timer can count before overflow
	TIM2->CNT = 0; // counter in 0 
	TIM2->CCMR1 = 0x0060; // Enable pwm mode
	TIM2->CCER = 1 ; // Enable PWM chanel 1 
	//TIM2->CCR1 = 8889 - 1; // Pulse with 1/3 period 266661-1 / 8889 = 33% aprox
	
	for(int i = 0;i<8888;i++){
		TIM2->CCR1 = i;
	}
	
	TIM2->CR1 = 1 ; // Enable the timer
}


void Usart2_init(){
		RCC->AHB1ENR |= (1<<0); 
	  RCC->APB1ENR |= (1<<17); // USART RCC
	
	  //GPIO USART1 TX,RX , PA3 AS USART
	  GPIOA->AFR[0] |= 0x7700; //
    GPIOA->MODER |= 0x00A0;  //  pone 2 pines en alternate function 
	  
	  // Setting usar
    USART2->BRR = 0x683;
	  USART2->CR1 = 0x000C; // Enable Rx,Tx
	  USART2->CR1 |= 0x2000; // Enable Usart 	  

}
		

void delayMs(int delay){
	int i;
	
	for(;delay;delay--){
		for(i=0;i<3195;i++){
		}
	}
}


int USART2_write(int ch){
  while(!(USART2->SR & 0x0080)){}
	USART2->DR = (ch&0xFF); // ff = 1111 1111 , para filtrar valores 		
	return ch;
}



int USART2_read(void){
	  while(!(USART2->SR & 0x0020)){} // status de 1 solo bit RX , para que solo se filtren estos bits 
		return USART2->DR;
}




