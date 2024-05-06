/*
 * Author: @github.com/annadostoevskaya
 * Filename: main.c
 * Created: 29 Apr 2024 04:08:08 PM
 * Last Update: 06 May 2024 11:39:45 AM
 *
 * Description: <EMPTY>
 */

#ifdef __clang_analyzer__
#define __interrupt(...)
#define __naked(...)
#endif

#include <stdint.h>
#include <stdio.h>

#ifdef __SDCC_stm8
/* stm8s/io.h */
#define _SFR_(addr) (*(volatile uint8_t *)(0x5000 + (addr)))

/* PORT D */
#define PD_ODR _SFR_(0x0F)
#define PD_DDR _SFR_(0x11)
#define PD_CR1 _SFR_(0x12)

/* CLOCK */
#define CLK_CKDIVR _SFR_(0xC6)

/* UART1 */
#define UART_SR   _SFR_(0x230)
#define UART_TXE  7
#define UART_TC   6
#define UART_RXNE 5

#define UART_DR   _SFR_(0x231)
#define UART_BRR1 _SFR_(0x232)
#define UART_BRR2 _SFR_(0x233)

#define UART_CR1 _SFR_(0x234)

#define UART_CR2 _SFR_(0x235)
#define UART_TEN 3
#define UART_REN 2

#define UART_CR3 _SFR_(0x236)
#define UART_CR4 _SFR_(0x237)
#define UART_CR5 _SFR_(0x238)

#define UART_GTR  _SFR_(0x239)
#define UART_PSCR _SFR_(0x23A)

/* TIM4 */
#define TIM4_CR1     _SFR_(0x340)
#define TIM4_CR1_CEN 0
#define TIM4_IER     _SFR_(0x343)
#define TIM4_IER_UIE 0
#define TIM4_SR      _SFR_(0x344)
#define TIM4_UIF     0
#define TIM4_PSCR    _SFR_(0x347)
#define TIM4_ARR     _SFR_(0x348)

#define TIM4_ISR 23

#endif

/* stm8s/delay.h */
static inline void delay_ms(uint16_t ms) {
  for (uint32_t i = 0; i < ((F_CPU / 18000UL) * ms); i += 1) {
    __asm__("nop");
  }
}

/* stm8s/uart.h */
void uart_init() {
  UART_BRR2 = 0x00;
  UART_BRR1 = 0x0D;
  UART_CR2 = (1 << UART_TEN) | (1 << UART_REN);
}

void uart_write(uint8_t data) {
  UART_DR = data;
  while (!(UART_SR & (1 << UART_TC)))
    ;
}

uint8_t uart_read() {
  while (!(UART_SR & (1 << UART_RXNE)))
    ;
  return UART_DR;
}

int putchar(int c) {
  uart_write(c);
  return 0;
}

#define P_LED 0x04

void timer_isr(void) __interrupt(TIM4_ISR) {
  PD_ODR ^= (1 << P_LED);
  TIM4_SR &= ~(1 << TIM4_UIF);
}

extern uint8_t *_interrupt_vect;
extern void *_sdcc_program_startup;

int main() {
  __asm__("rim");

  PD_DDR |= (1 << P_LED);
  PD_CR1 |= (1 << P_LED);

  TIM4_PSCR |= 0x07;

  TIM4_ARR = 77; // 10ms

  TIM4_IER |= (1 << TIM4_IER_UIE);
  TIM4_CR1 |= (1 << TIM4_CR1_CEN);

  uart_init();

  for (;;) {
    printf("ivt: %p\n", _interrupt_vect);
    printf("_sdcc_program_startup: %p\n", _sdcc_program_startup);
    printf("sizeof pointer: %d\n", sizeof(_interrupt_vect));
    delay_ms(5000);
  }

  return 0;
}
