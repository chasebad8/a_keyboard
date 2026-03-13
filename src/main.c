/* main.c - bare-metal blink on Arduino Micro */
#include "gpio.h"

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
   struct gpio_cfg_s gpio_cfg = { .direction = GPIO_OUTPUT };

   gpio_init(&PORTB, 0, gpio_cfg);

   /* PB6 is onboard LED */
   //DDRB |= (1 << PB0);
   DDRD |= (1 << PD5); /* Set PD5 as output */

   while (1)
   {
      gpio_write(&PORTB, 0, !gpio_read(&PORTB, 0));
      _delay_ms(100);
      gpio_write(&PORTB, 0, !gpio_read(&PORTB, 0));

      //PORTB ^= (1 << PB0); /* Toggle LED */
      PORTD ^= (1 << PD5);
      _delay_ms(500);
   }

   return 0;
}