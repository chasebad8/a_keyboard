#ifndef KEYBOARD_MATRIX_H
#define KEYBOARD_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include <LUFA/Drivers/USB/USB.h>

#include "gpio.h"

#define MATRIX_ROWS     (5)
#define MATRIX_COLUMNS  (14)
#define MAX_DEBOUNCE    (5)

#define MAX_KEY_PRESSES (6)

struct matrix_port_pin_key_s
{
   volatile struct gpio_port_regs_s *port;
   uint8_t pin;
};

/* the port and pin that each row in the keyboard matrix maps to */
static const struct matrix_port_pin_key_s matrix_row_pin_mapping[MATRIX_ROWS] =
{
   { GPIOD, PD0 },
   { GPIOB, PB4 },
   { GPIOD, PD2 },
   { GPIOD, PD3 },
   { GPIOB, PB2 },
};

/* the port and pin that each column in the keyboard matrix maps to */
static const struct matrix_port_pin_key_s matrix_col_pin_mapping[MATRIX_COLUMNS] =
{
   { GPIOD, PD1 },
   { GPIOB, PB5 },
   { GPIOE, PE6 },
   { GPIOD, PD7 },
   { GPIOC, PC6 },
   { GPIOD, PD4 },
   { GPIOF, PF4 },
   { GPIOF, PF5 },
   { GPIOF, PF6 },
   { GPIOF, PF7 },
   { GPIOB, PB1 },
   { GPIOB, PB3 },
   { GPIOB, PB6 },
};

static const uint8_t matrix_to_key_map[MATRIX_ROWS][MATRIX_COLUMNS] =
{
   {
      HID_KEYBOARD_SC_ESCAPE,
      HID_KEYBOARD_SC_1_AND_EXCLAMATION,
      HID_KEYBOARD_SC_2_AND_AT,
      HID_KEYBOARD_SC_3_AND_HASHMARK,
      HID_KEYBOARD_SC_4_AND_DOLLAR,
      HID_KEYBOARD_SC_5_AND_PERCENTAGE,
      HID_KEYBOARD_SC_6_AND_CARET,
      HID_KEYBOARD_SC_7_AND_AMPERSAND,
      HID_KEYBOARD_SC_8_AND_ASTERISK,
      HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS,
      HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS,
      HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE,
      HID_KEYBOARD_SC_BACKSPACE
   },

   {
      HID_KEYBOARD_SC_TAB,
      HID_KEYBOARD_SC_Q,
      HID_KEYBOARD_SC_W,
      HID_KEYBOARD_SC_E,
      HID_KEYBOARD_SC_R,
      HID_KEYBOARD_SC_T,
      HID_KEYBOARD_SC_Y,
      HID_KEYBOARD_SC_U,
      HID_KEYBOARD_SC_I,
      HID_KEYBOARD_SC_O,
      HID_KEYBOARD_SC_P,
      HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE,
      HID_KEYBOARD_SC_BACKSLASH_AND_PIPE
   },

   {
      HID_KEYBOARD_SC_CAPS_LOCK,
      HID_KEYBOARD_SC_A,
      HID_KEYBOARD_SC_S,
      HID_KEYBOARD_SC_D,
      HID_KEYBOARD_SC_F,
      HID_KEYBOARD_SC_G,
      HID_KEYBOARD_SC_H,
      HID_KEYBOARD_SC_J,
      HID_KEYBOARD_SC_K,
      HID_KEYBOARD_SC_L,
      HID_KEYBOARD_SC_SEMICOLON_AND_COLON,
      HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE,
      HID_KEYBOARD_SC_ENTER
   },

   {
      HID_KEYBOARD_SC_LEFT_SHIFT,
      HID_KEYBOARD_SC_Z,
      HID_KEYBOARD_SC_X,
      HID_KEYBOARD_SC_C,
      HID_KEYBOARD_SC_V,
      HID_KEYBOARD_SC_B,
      HID_KEYBOARD_SC_N,
      HID_KEYBOARD_SC_M,
      HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN,
      HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,
      HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,
      HID_KEYBOARD_SC_EQUAL_AND_PLUS,
      HID_KEYBOARD_SC_RIGHT_SHIFT
   },

   {
      HID_KEYBOARD_SC_LEFT_CONTROL,
      HID_KEYBOARD_SC_LEFT_GUI,
      HID_KEYBOARD_SC_LEFT_ALT,
      HID_KEYBOARD_SC_RESERVED,
      HID_KEYBOARD_SC_RESERVED,
      HID_KEYBOARD_SC_SPACE,
      HID_KEYBOARD_SC_RESERVED,
      HID_KEYBOARD_SC_RESERVED,
      HID_KEYBOARD_SC_LEFT_ARROW,
      HID_KEYBOARD_SC_DOWN_ARROW,
      HID_KEYBOARD_SC_UP_ARROW,
      HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE,
      HID_KEYBOARD_SC_RIGHT_ARROW,
   }
};

/******************************************************************************
 * @name key_matrix_init
 *
 * @brief initialize the keyboard matrix. Initialize all rows as outputs and all
 *        columns as inputs.
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
void key_matrix_init(void);

/******************************************************************************
 * @name key_matrix_scan
 *
 * @brief initialize the keyboard matrix. Initialize all rows as outputs and all
 *        columns as inputs.
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
void key_matrix_scan(void);

/******************************************************************************
 * @name key_matrix_scan
 *
 * @brief initialize the keyboard matrix. Initialize all rows as outputs and all
 *        columns as inputs.
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
void key_matrix_report(USB_KeyboardReport_Data_t *keyboard_report);

#endif