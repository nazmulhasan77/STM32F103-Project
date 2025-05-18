# STM32F103 LED Blink (Bare Metal)

This project demonstrates how to blink an LED connected to **PC13** using **Timer 2 (TIM2)** on the STM32F103 microcontroller. It uses **direct register access (bare-metal programming)** without HAL or SPL.

## 📌 Project Overview

- **Microcontroller:** STM32F103C8T6 ("Blue Pill")
- **LED Pin:** PC13
- **Timer Used:** TIM2
- **System Clock:** 8 MHz
- **Blink Rate:** 1 Hz (LED toggles every 1 second)

## 🔧 How It Works

1. Enables clocks for GPIOC and TIM2.
2. Configures PC13 as a push-pull output.
3. Sets TIM2 with:
   - Prescaler = 7999 → timer ticks every 1 ms
   - Auto-reload = 999 → timer overflows every 1000 ms (1 second)
4. In the main loop:
   - Waits for the timer's update event flag (UIF).
   - Clears the UIF flag.
   - Toggles the LED on PC13.
   - 
## 📄 Using HAL Library
```c
 while (1)
  {

	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	  HAL_Delay(1000);
  }
```

## 📄 Using Loop Delay
```c
    while (1) {

		//GPIOC_ODR ^=0x2000; // Toggle PC13 another Method
		
        // Reset PC13 (set bit 29 to reset pin 13)
        GPIOC_BSRR |= (1U << 13);  // Set PC13 low (LED on for Blue Pill)
        for (int i = 0; i < delay; i++);  // crude delay

        // Set PC13 (bit 13 + 16 = bit 29)
        GPIOC_BSRR |= (1U << (29));  // Set PC13 high (LED off)
        for (int i = 0; i < delay; i++);  // crude delay
    }

```
## 📄 LED_Blink_using_Timer2

```c
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

```

## 💻 Requirements

- STM32F103C8T6 or compatible microcontroller
- STM32CubeIDE or Makefile-based toolchain
- ST-Link or USB-to-Serial programmer
- LED on PC13 (optional on-board LED)

## 🚀 Flashing Instructions

### Using STM32CubeIDE

1. Create a new STM32 project for STM32F103C8Tx.
2. Replace `main.c` with the code above.
3. Build and flash using the IDE.


## 🙌 Acknowledgments

- STMicroelectronics
- ARM Cortex-M documentation
- Embedded bare-metal programming community
