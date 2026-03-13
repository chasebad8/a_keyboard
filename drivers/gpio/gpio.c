#include "gpio.h"
#include <avr/io.h>

/******************************************************************************
 * @name gpio_init
 *
 * @brief initialize a GPIO pin
 *
 * DDxn PORTxn PUD (in MCUCR) I/O    Pull-up Comment
 * 0    0      X              Input  No      Tri-state (Hi-Z)
 * 0    1      0              Input  Yes     Pxn will source current if ext. pulled low (col)
 * 0    1      1              Input  No      Tri-state (Hi-Z)
 * 1    0      X              Output No      Output Low (Sink)
 * 1    1      X              Output No      Output High (Source)
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
void gpio_init(volatile uint8_t *port, uint8_t pin, struct gpio_cfg_s gpio_cfg)
{
   struct gpio_port_regs_s *gpio_regs = (struct gpio_port_regs_s *)port;

   if(gpio_cfg.direction == GPIO_INPUT)
   {
      gpio_regs->DDRx &= ~(1 << pin);

      if(gpio_cfg.phup == GPIO_PDOWN)
      {
         gpio_regs->PORTx &= ~(1 << pin);
      }
      else /* GPIO_PUP */
      {
         gpio_regs->PORTx |= (1 << pin);
   }
   }
   else /* GPIO_OUTPUT */
   {
      gpio_regs->DDRx |= (1 << pin);
   }
}

/******************************************************************************
 * @name gpio_write
 *
 * @brief write to a GPIO pin
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
void gpio_write(volatile uint8_t *port, uint8_t pin, enum gpio_signal_e signal)
{
   if(signal == GPIO_LOW)
   {
      *port &= ~(1 << pin);
   }
   else
   {
      *port |= (1 << pin);
   }
}

/******************************************************************************
 * @name gpio_read
 *
 * @brief read a GPIO pin
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
uint8_t gpio_read(volatile uint8_t *port, uint8_t pin)
{
   return (*port & (1 << pin)) >> pin;
}