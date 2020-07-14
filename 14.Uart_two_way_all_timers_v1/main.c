#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

void USART2_Init();
int USART2_write(int ch);
int USART2_read(void);
int fgetc(FILE *f);
int delayMs(int delay);
void Led_on();
void pwm();
void pwm_reversa();


// volver a repetir todo el codigo pero agrengando que se prendan los leds!
// analizarlo paso por paso 

// add pointers with structures 
int main(void){
	int n ;
	char str[100];
	USART2_Init();
	
	printf("Hello from the other side \r\n ");
	fprintf(stdout,"test for stdout \r\n");
	fprintf(stderr,"test for stderr \r\n");
	
	
	while(1){
		printf("Select an option \n");
		scanf("%d",&n);
		
		switch(n){
		  case 1:
				printf("led toggle is enable \n");
			  Led_on();	
        break;			
		  case 2:
				printf("PWM is on : \n");
		    pwm();
			  break;
			case 3:
				printf("PWM reverse is on : \n"); 
			  pwm_reversa();
			  break;
		}
	}	
}




void USART2_Init(){
	
	//1.- Enable RCC
	//RCC->AHB1ENR |= (1<<0); // Enable GPIOA  
	//RCC->APB1ENR |= (1<<17); // Enable USART2
	
	RCC-> AHB1ENR |= 1; // GPIOA
	RCC->APB1ENR |= 0x20000; // Enable USART2
	
	//2.- Configure PA3 as Usart
	 GPIOA-> AFR[0] |= 0x7700; // elegimos usart2
	 /*GPIOA->MODER |= 0x0080; */
	 GPIOA->MODER |= 0x00A0 ; // pone 2 pines en alternate function 
	 
	 // USART Conf
	USART2->BRR = 0x683; // 115200 baud 16 mHz
	USART2->CR1 = 0x000C; // Enable Rx,Tx
	USART2->CR1 |= 0x2000; // Enable Usart 
	
}


int USART2_write(int ch){
		//Wait for Tx buffer empty 
	  while(!(USART2-> SR & 0x0080)){} // habilita TX
			USART2->DR = (ch&0xFF);
			return ch;
}


int USART2_read(void){
	 while(!(USART2->SR & 0x0020)){} // habilita RX
		 return USART2->DR;
}



int delayMs(int delay){
	int i ;
	for(;delay>0;delay--){
		for(i=0;i<3195;i++){}
	}
}


void Led_on(){
	RCC->AHB1ENR |= 1;
	GPIOA->MODER |= 0x400;
	
	while(1){
		GPIOA->ODR|= 0x20;
		delayMs(20);
		GPIOA->ODR &= ~0x20;
		delayMs(100);
	}
}



void pwm(){
	int i ;
	
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


void pwm_reversa(){
	int i ;
	
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
	
	for(int i = 8888;i>0;i--){
		TIM2->CCR1 = i;
	}
	
	TIM2->CR1 = 1 ; // Enable the timer
}




// C interface
struct __FILE{int handle;};
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr = {2};



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
