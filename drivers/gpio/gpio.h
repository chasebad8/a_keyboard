#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

enum gpio_signal_e
{
   GPIO_LOW = 0,
   GPIO_HIGH
};

enum gpio_direction_e
{
   GPIO_INPUT = 0,
   GPIO_OUTPUT
};

enum gpio_pup_e
{
   GPIO_PDOWN = 0,
   GPIO_PUP
};

struct gpio_cfg_s
{
   enum gpio_direction_e direction;
   enum gpio_pup_e       pup;
};

struct gpio_port_regs_s
{
   volatile uint8_t PORTx;
   volatile uint8_t DDRx;
   volatile uint8_t PINx;
};

#define GPIOB ((struct gpio_port_regs_s *)&PORTB)
#define GPIOC ((struct gpio_port_regs_s *)&PORTC)
#define GPIOD ((struct gpio_port_regs_s *)&PORTD)

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
void gpio_init(volatile struct gpio_port_regs_s *port, uint8_t pin, struct gpio_cfg_s gpio_cfg);

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
void gpio_write(volatile struct gpio_port_regs_s *port, uint8_t pin, enum gpio_signal_e signal);

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
uint8_t gpio_read(volatile struct gpio_port_regs_s *port, uint8_t pin);

#endif //GPIO_H