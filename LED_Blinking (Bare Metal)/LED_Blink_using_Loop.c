// Base Addresses
#define RCC_BASE     0x40021000
#define GPIOC_BASE   0x40011000

#define delay        50000

// Peripheral registers
#define RCC_APB2ENR  (*(volatile unsigned int*)(RCC_BASE + 0x18))
#define GPIOC_CRH    (*(volatile unsigned int*)(GPIOC_BASE + 0x04))
#define GPIOC_ODR    (*(volatile unsigned int*)(GPIOC_BASE + 0x0C))
#define GPIOC_BSRR   (*(volatile unsigned int*)(GPIOC_BASE + 0x10))

int main(void) {
    // Enable clock for GPIOC (IOPCEN = bit 4)
    RCC_APB2ENR |= (1 << 4);
    
    // Configure PC13 as General Purpose Output Push-Pull
    // Clear CNF13[1:0] and MODE13[1:0] (bits 23:20)
    GPIOC_CRH &= ~(0xF << 20);
    // Set MODE13 = 0b10 (Output mode, max speed 2 MHz), CNF13 = 0b00 (Push-pull)
    GPIOC_CRH |= (0x2 << 20);
    
    while (1) {
        // Reset PC13 (set bit 29 to reset pin 13)
        GPIOC_BSRR |= (1U << 13);  // Set PC13 low (LED on for Blue Pill)
        for (int i = 0; i < delay; i++);  // crude delay

        // Set PC13 (bit 13 + 16 = bit 29)
        GPIOC_BSRR |= (1U << (29));  // Set PC13 high (LED off)
        for (int i = 0; i < delay; i++);  // crude delay
    }
}