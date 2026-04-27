#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

typedef void (*timer_callback_t)(void);

typedef enum
{
    TIM8_MODE_NORMAL,
    TIM8_MODE_CTC,
    TIM8_MODE_FAST_PWM,
    TIM8_MODE_PHASE_CORRECT_PWM

} tim8_mode_t;

typedef enum
{
    TIM_CLK_OFF = 0,
    TIM_CLK_1,
    TIM_CLK_8,
    TIM_CLK_64,
    TIM_CLK_256,
    TIM_CLK_1024,
    TIM_CLK_EXT_FALLING,
    TIM_CLK_EXT_RISING

} tim_prescaler_t;

struct timer8_config_t
{
    tim8_mode_t     mode;
    tim_prescaler_t prescaler;

    uint8_t ocr_a;
    uint8_t ocr_b;

    bool enable_ovf_irq;
    bool enable_compa_irq;
    bool enable_compb_irq;

};

typedef enum
{
    TIM16_MODE_NORMAL,
    TIM16_MODE_CTC_OCRnA,
    TIM16_MODE_CTC_ICRn,
    TIM16_MODE_FAST_PWM_OCRnA,
    TIM16_MODE_FAST_PWM_ICRn,
    TIM16_MODE_PHASE_CORRECT_PWM_ICRn

} tim16_mode_t;

struct timer16_config_t
{
    tim16_mode_t    mode;
    tim_prescaler_t prescaler;

    uint16_t top;
    uint16_t ocr_a;
    uint16_t ocr_b;
    uint16_t ocr_c;

    bool enable_ovf_irq;
    bool enable_compa_irq;
    bool enable_compb_irq;
    bool enable_compc_irq;

};

struct timer_8_bit_cfg_regs_s
{
   volatile uint8_t TCCRnA;
   volatile uint8_t TCCRnB;
   volatile uint8_t TCNTn;
   volatile uint8_t OCRnA;
   volatile uint8_t OCRnB;
};

struct timer_16_bit_cfg_regs_s
{
   volatile uint8_t TCCRnA;
   volatile uint8_t TCCRnB;
   volatile uint8_t TCCRnC;
   volatile uint8_t res;
   volatile uint8_t TCNTnL;
   volatile uint8_t TCNTnH;
   volatile uint8_t ICRnL;
   volatile uint8_t ICRnH;
   volatile uint8_t OCRnAL;
   volatile uint8_t OCRnAH;
   volatile uint8_t OCRnBL;
   volatile uint8_t OCRnBH;
   volatile uint8_t OCRnCL;
   volatile uint8_t OCRnCH;
};

#define TIM0 ((struct timer_8_bit_cfg_regs_s *)&TCCR0A)
#define TIM1 ((struct timer_16_bit_cfg_regs_s *)&TCCR1A)
#define TIM3 ((struct timer_16_bit_cfg_regs_s *)&TCCR3A)

/******************************************************************************
 * @name timer_0_init
 *
 * @brief initialize timer 0
 *
 * @param  timer_cfg
 *
 * @return none
 *
 ******************************************************************************/
void timer_0_init(struct timer8_config_t timer_cfg);

/******************************************************************************
 * @name timer_0_bind_compa_irq_cb
 *
 * @brief bind in comp a irq callback function
 *
 * @param  cb_func
 *
 * @return none
 *
 ******************************************************************************/
void timer_0_bind_compa_irq_cb(timer_callback_t cb_func);

/******************************************************************************
 * @name timer_0_bind_compb_irq_cb
 *
 * @brief bind in comp b irq callback function
 *
 * @param  cb_func
 *
 * @return none
 *
 ******************************************************************************/
void timer_0_bind_compb_irq_cb(timer_callback_t cb_func);

/******************************************************************************
 * @name timer_0_bind_ovf_irq_cb
 *
 * @brief bind in overflow irq callback function
 *
 * @param  cb_func
 *
 * @return none
 *
 ******************************************************************************/
void timer_0_bind_ovf_irq_cb(timer_callback_t cb_func);

#endif