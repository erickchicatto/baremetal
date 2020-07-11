#include "stm32f4xx.h"                  // Device 

void delayMs(int delay);

int main(void){


	RCC->AHB1ENR |= 1; // OR operation , only with the position 0 in register
	GPIOA->MODER |= 0x400; // 0000 0000 0000 0000 0000 0100 0000 0000 
		
	while(1){
		GPIOA->ODR |= 0x20; // 0b 0010 0000 , 
	  delayMs(100);
		GPIOA->ODR &= ~0x20;
		delayMs(100);
	}
	
}


// Una forma de escribir los delays
void delayMs(int delay){
	int i;
	for(;delay>0;delay--){
		for(i=0;i<3195;i++){
		}
	}
}
