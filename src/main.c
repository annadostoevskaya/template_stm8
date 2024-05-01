/*
 * Author: @github.com/annadostoevskaya
 * Filename: main.c
 * Created: 29 Apr 2024 04:08:08 PM
 * Last Update: 01 May 2024 1:07:32 AM
 *
 * Description: <EMPTY>
 */

#include <stdint.h>

#ifdef __SDCC_stm8
#define _SFR_(addr) (*(volatile uint8_t *)(0x5000 + (addr)))

/* PORT D */
#define PD_ODR _SFR_(0x0F)
#define PD_DDR _SFR_(0x11)
#define PD_CR1 _SFR_(0x12)

/* CLOCK */
#define CLK_CKDIVR _SFR_(0xC6)

#endif

#define P_LED 4

static inline void delay_ms(uint16_t ms) {
  for (uint32_t i = 0; i < ((F_CPU / 18000UL) * ms); i += 1) {
    __asm__("nop");
  }
}

int main() {
  // NOTE(annad): Disable prescale for High-Speed-Internal RC
  // CLK_CKDIVR &= ~(0x18);

  PD_DDR |= (1 << P_LED);
  PD_CR1 |= (1 << P_LED);

  while (1) {
    /* toggle pin every 250ms */
    PD_ODR ^= (1 << P_LED);
    delay_ms(1000);
  }

  return 0;
}
