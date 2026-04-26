/*
Timer/Counter (TCNT0) and Output Compare Registers (OCR0A and OCR0B) are 8 bit registers.
We can program the maximum value to count to with OCR0A reg.

Interrupt signals are sent to the Timer Interrupt Flag Register (TIFR0).
All interrupts are individually masked with the Timer Interrupt Mask Register (TIMSK0).
The clock can be interal, scaled, or external.

TCCR0A: COMA = 0x00 - we don't to hook this up to an output I/O pin so we should just set the mode to be 0.
        COMB = 0x00

TCCR0B clock select -> clk I/O / 1024 = 0x101

TCNT0 - RO, the current counter value for the timer

OCR0A
OCR0B: The output compare register. You write the max value that you want the timer to compare itself against in here.
I think you need to have the TCCR0A bits set into the right mode for this to do anything.

TIMSK0:
- OCIE0B: timer/counter output compare match b interrupt enable.
- OCIE0A:

TIFR0:
OCF0B: set when there is a match.
OCF0A

In this case I think we want interrupt 22 or Address 0x002A to fire and run our interrupt function.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "gpio.h"
#include <util/delay.h>

volatile timer_callback_t timer0_cb = 0;

void configure_timer_0(timer_callback_t cb_func)
{
   timer0_cb = cb_func;

   /* set the clock src to none to stop timer */
   TCCR0B = 0;
   /* set to CTC mode. Once the interrupt fires counter will reset to 0 */
   TCCR0A = (1 << WGM01);
   /* value to count up to */
   OCR0A |= 0xFF;
   /* enable COMPA interrupt */
   TIMSK0 |= (1 << OCIE0A);
   /* 256 pre-scaler */
   TCCR0B |= (1 << CS02);
}

ISR(TIMER0_COMPA_vect)
{
   if (timer0_cb)
   {
      timer0_cb();
   }
}