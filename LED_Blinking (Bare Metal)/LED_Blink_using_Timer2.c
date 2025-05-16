//Base Address
#define RCC_BASE 0x40021000
#define GPIOC_BASE 0x40011000
#define TIM2_BASE 0x40000000

// Peripheral registers
#define RCC_APB2ENR   (*(volatile unsigned int* ) (RCC_BASE+0x18))
#define GPIOC_CRH     (*(volatile unsigned int*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR     (*(volatile unsigned int*)(GPIOC_BASE + 0x0C))
#define RCC_APB1ENR   (*(volatile unsigned int* ) (RCC_BASE+0x1C))
#define TIM2_CR1      (*(volatile unsigned int*)(TIM2_BASE + 0x00))
#define TIM2_SR       (*(volatile unsigned int*)(TIM2_BASE + 0x10))
#define TIM2_PSC      (*(volatile unsigned int*)(TIM2_BASE + 0x28))
#define TIM2_ARR      (*(volatile unsigned int*)(TIM2_BASE + 0x2C))


int main(void){
	// Enable GPIOC clock
	RCC_APB2ENR |=0x10; // IOPCEN
	// Set PC13 as push-pull output
	GPIOC_CRH &=0xFF0FFFFF; // Clear CNF13[1:0] and MODE13[1:0]
	GPIOC_CRH |=0x00200000; // MODE13 = 0b10 (Output 2 MHz), CNF13 = 0b00 (Push-pull)

	// Enable TIM2 clock
	RCC_APB1ENR |=0x1;
	// Set up TIM2 for 1-second interval
	TIM2_PSC=7999; // 8 MHz / (7999 + 1) = 1 kHz
	TIM2_ARR=999; // 1000 ticks = 1 sec

	// Start the timer
	TIM2_CR1 |=0x1; // CEN = 1

	while(1){
		if(TIM2_SR & (1U<<0)){ // UIF (Update interrupt flag)
			TIM2_SR &= ~(1U<<0); // Clear UIF
			GPIOC_ODR ^=0x2000; // Toggle PC13
		}
	}

}
