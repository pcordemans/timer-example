// includes low level peripheral definitions
#include "stm32l476xx.h"

//All IRQ handlers are weakly defined by the compiler as C functions
//As C has different function calling conventions then C++, the code block has to be marked as extern C
extern "C" {
    // Interrupt handler is called when the interrupt has been raised
    void TIM7_IRQHandler(){
        // clear TIM 7 interrupt flag
        TIM7->SR = 0;
        
        if(GPIOA->ODR & (1<<5)){
            //GPIOA_BSRR reset pin 5: set bit 21
            GPIOA->BSRR |= 1 << 21;
        }
        else {
            //GPIOA_BSRR set pin 5: set bit 5
            GPIOA->BSRR |= 1 << 5;
        }
    }
}

/**
 * Initializes PA5 as a digital output
 * */
void led_init(){
    //Enable GPIOA peripheral in the AHB2ENR: set bit 0
    RCC->AHB2ENR |= 1;

    // GPIOA_MODER set GP output mode: reset bit 11 & set bit 10
    GPIOA->MODER &= ~(1 << 11);
    GPIOA->MODER |= 1 << 10;
}

void tim7_init(){
    //Enable TIM7 in the APB1ENR1: set bit 5
    RCC->APB1ENR1 |= 1 << 5;

    // Enable the global interrupt for TIM7 in NVIC
    NVIC_EnableIRQ(TIM7_IRQn);

    // Given a clock of 48MHz, set prescaler and count register for a period of 1s
    TIM7->PSC = 1999;
    TIM7->ARR = 23999;

    // Set event generation, re-initializes timer counter and generates an update of the registers
    TIM7->EGR |= 1;

    // Enable interrupt in TIM7
    TIM7->DIER |= 1;
}

void tim7_start(){
    // Start TIM7
    TIM7->CR1 |= 1;
}    

int main()
{    
    
    led_init();

    tim7_init();    
    
    tim7_start();

    while (true)
    {
        //wait here for interrupts
    }    
}
