#include "keyboard_matrix.h"

// struct matrix_scan_s
// {
//    uint16_t matrix_raw[MATRIX_ROWS]
//    /* todo: add debouncing */
// }

static uint16_t matrix_raw[MATRIX_ROWS];

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
void key_matrix_init(void)
{
   struct gpio_cfg_s gpio_cfg = {.direction = GPIO_OUTPUT, .pup = GPIO_PDOWN};

   /* initialize the rows as output pins */
   for(uint8_t row = 0; row < MATRIX_ROWS; row++)
   {
      gpio_init(matrix_row_pin_mapping[row].port, matrix_row_pin_mapping[row].pin, gpio_cfg);
   }

   gpio_cfg.direction = GPIO_INPUT;
   gpio_cfg.pup       = GPIO_PUP;

   /* initialize the columns as input pins */
   for(uint8_t col = 0; col < MATRIX_COLUMNS; col++)
   {
      gpio_init(matrix_col_pin_mapping[col].port, matrix_col_pin_mapping[col].pin, gpio_cfg);
   }
}

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
void key_matrix_scan(void)
{
   // 1. SCAN: Read raw state from hardware
   for (uint8_t row = 0; row < MATRIX_ROWS; row++)
   {
      gpio_write(matrix_row_pin_mapping[row].port,
                 matrix_row_pin_mapping[row].pin, GPIO_LOW);

      _delay_us(5);  // Small delay for settling

      matrix_raw[row] = 0;
      for (uint8_t col = 0; col < MATRIX_COLUMNS; col++)
      {
         uint8_t pin_state = gpio_read(matrix_col_pin_mapping[col].port,
                                       matrix_col_pin_mapping[col].pin);
         if (pin_state == GPIO_LOW)
         {
            matrix_raw[row] |= (1 << col);
         }
         else
         {
            matrix_raw[row] &= ~(1 << col);
         }
      }

      gpio_write(matrix_row_pin_mapping[row].port,
                 matrix_row_pin_mapping[row].pin, GPIO_HIGH);
   }
}

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
void key_matrix_report(USB_KeyboardReport_Data_t **keyboard_report)
{
   uint8_t keys_pressed = 0;
   USB_KeyboardReport_Data_t *keyboard = *keyboard_report;

   // 3. CHANGE DETECTION: Find what changed
   for (uint8_t row = 0; row < MATRIX_ROWS; row++)
   {
      for (uint8_t col = 0; col < MATRIX_COLUMNS; col++)
      {
         if((matrix_raw[row] & (1 << col)) == 1)
         {
            keyboard->KeyCode[keys_pressed++] = matrix_to_key_map[row][col];
         }
      }
   }
}