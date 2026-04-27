#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "gpio.h"
#include <util/delay.h>

volatile timer_callback_t timer0_cb = 0;

/******************************************************************************
 * @name tim8_mode_cfg
 *
 * @brief configure the timer mode
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
static void tim8_mode_cfg(struct timer8_config_t timer_cfg)
{
   /* disable timer during config */
   TIM0->TCCRnB = 0;

   TIM0->TCCRnA &= ~((1 << WGM01) | (1 << WGM00));
   TIM0->TCCRnB &=  ~(1 << WGM02);

   switch(timer_cfg.mode)
   {
      case TIM8_MODE_NORMAL:
         break;

      case TIM8_MODE_PHASE_CORRECT_PWM:
         TIM0->TCCRnA |= (1 << WGM00);

         if ((timer_cfg.ocr_a != 0) || (timer_cfg.ocr_b != 0))
         {
            TIM0->TCCRnB |= (1 << WGM02);
         }
         break;

      case TIM8_MODE_CTC:
         TIM0->TCCRnA |= (1 << WGM01);
         break;

      case TIM8_MODE_FAST_PWM:
         TIM0->TCCRnA |= ((1 << WGM01) | (1 << WGM00));

         if ((timer_cfg.ocr_a != 0) || (timer_cfg.ocr_b != 0))
         {
            TIM0->TCCRnB |= (1 << WGM02);
         }
         break;

      default:
         break;
   }
}

/******************************************************************************
 * @name tim8_clk_cfg
 *
 * @brief select clock source and pre-scalar for the timer.
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
static void tim8_clk_cfg(struct timer8_config_t timer_cfg)
{
   TIM0->TCCRnB &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));

   switch(timer_cfg.prescaler)
   {
      case TIM_CLK_OFF:
         break;
      case TIM_CLK_1:
         TIM0->TCCRnB |= (1 << CS00);
         break;
      case TIM_CLK_8:
         TIM0->TCCRnB |= (1 << CS01);
         break;
      case TIM_CLK_64:
         TIM0->TCCRnB |= ((1 << CS00) | (1 << CS01));
         break;
      case TIM_CLK_256:
         TIM0->TCCRnB |= (1 << CS02);
         break;
      case TIM_CLK_1024:
         TIM0->TCCRnB |= ((1 << CS02) | (1 << CS00));
         break;
      case TIM_CLK_EXT_FALLING:
         TIM0->TCCRnB |= ((1 << CS02) | (1 << CS01));
         break;
      case TIM_CLK_EXT_RISING:
         TIM0->TCCRnB |= ((1 << CS02) | (1 << CS01) | (1 << CS00));
         break;
      default:
         break;
   }
}

/******************************************************************************
 * @name timer_0_init
 *
 * @brief initialize timer 0
 *
 * @param  timer_cfg
 * @param  callback_func
 *
 * @return none
 *
 ******************************************************************************/
void timer_0_init(struct timer8_config_t timer_cfg,
                  timer_callback_t       callback_func)
{
   /* disable timer during config */
   TIM0->TCCRnB = 0;

   tim8_mode_cfg(timer_cfg);

   TIM0->OCRnA = timer_cfg.ocr_a;
   TIM0->OCRnB = timer_cfg.ocr_b;

   TIMSK0 = 0;
   TIMSK0 |= (((timer_cfg.enable_compb_irq << OCIE0B)) | (timer_cfg.enable_compa_irq << OCIE0A) | (timer_cfg.enable_ovf_irq << TOIE0));

   timer0_cb = callback_func;

   tim8_clk_cfg(timer_cfg);
}

ISR(TIMER0_COMPA_vect)
{
   if (timer0_cb)
   {
      timer0_cb();
   }
}

/* TODO: allow for 3 unique callback functions to be bound in */
ISR(TIMER0_COMPB_vect)
{
   if (timer0_cb)
   {
      timer0_cb();
   }
}

ISR(TIMER0_OVF_vect)
{
   if (timer0_cb)
   {
      timer0_cb();
   }
}