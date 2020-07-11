#include "stm32f4xx.h"                  // Device header

int main(void){
	 RCC-> AHB1ENR |= 4; // ENABLE PORT C
	 RCC-> AHB1ENR |= 1; // ENABLE PORT A
	
	 GPIOA -> MODER |= 0x400; //0100 0000 0000
	
	while(1){
			if(GPIOC->IDR & 0x2000){ // 0010 0000 0000 0000  if pc13 high si cumple con el registro
				 GPIOA->BSRR = 0x002; // parte high del registro
			}
			else {
				GPIOA->BSRR = 0x20;  // parte low del registro
			}
		
	}
	
}
