#include "keyboard_matrix.h"

struct keyboard_matrix_s
{
   uint16_t raw[MATRIX_ROWS];
   uint16_t report[MATRIX_ROWS];
   uint16_t debounce[MATRIX_ROWS][MATRIX_COLUMNS];
};

static struct keyboard_matrix_s matrix;

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
   struct gpio_cfg_s gpio_cfg = { .direction = GPIO_OUTPUT,
                                  .pup       = GPIO_PDOWN};

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
 * @name key_matrix_read_raw
 *
 * @brief read the raw input from the column GPIO pin and add it to the raw
 *        array.
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
static void key_matrix_read_raw(uint16_t row, uint16_t col)
{
   uint8_t pin_state = gpio_read(matrix_col_pin_mapping[col].port, matrix_col_pin_mapping[col].pin);

   if(pin_state == GPIO_LOW)
   {
      matrix.raw[row] |= (1 << col);
   }
   else
   {
      matrix.raw[row] &= ~(1 << col);
   }
}

/******************************************************************************
 * @name key_matrix_debounce_and_update_report
 *
 * @brief a simple debouncing algorithm implemented for every key.
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
static void key_matrix_debounce_and_update_report(uint16_t row, uint16_t col)
{
   if(((matrix.raw[row] >> col) & 1) != ((matrix.report[row] >> col) & 1))
   {
      if((matrix.debounce[row][col]++) >= MAX_DEBOUNCE)
      {
         if(((matrix.raw[row] >> col) & 1) == 0)
         {
            gpio_write(GPIOB, PB0, LED_LOW);
            matrix.report[row] &= ~(1 << col);
         }
         else
         {
            gpio_write(GPIOB, PB0, LED_HIGH);
            matrix.report[row] |= (1 << col);
         }

         matrix.debounce[row][col] = 0;
      }
   }
}

/******************************************************************************
 * @name  key_matrix_scan
 *
 * @brief the key matrix operates by driving 1 row low at a time. By default all
 *        rows are asserted. After driving the row low, we can now check each
 *        column in the row to see if any of them are also now low. Since our
 *        columns were pull up inputs, if pressed, they will now ground to 0
 *        for that row. After scanning the raw GPIO value we handle debounding
 *        and then create a report to send to USB host.
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
void key_matrix_scan(void)
{
   for (uint8_t row = 0; row < MATRIX_ROWS; row++)
   {
      matrix.raw[row] = 0;
      gpio_write(matrix_row_pin_mapping[row].port, matrix_row_pin_mapping[row].pin, GPIO_LOW);

      for (uint8_t col = 0; col < MATRIX_COLUMNS; col++)
      {
         key_matrix_read_raw(row, col);
         key_matrix_debounce_and_update_report(row, col);
      }

      gpio_write(matrix_row_pin_mapping[row].port, matrix_row_pin_mapping[row].pin, GPIO_HIGH);
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
void key_matrix_report(USB_KeyboardReport_Data_t *keyboard_report)
{
   uint8_t keys_pressed = 0;

   for (uint8_t row = 0; row < MATRIX_ROWS; row++)
   {
      for (uint8_t col = 0; col < MATRIX_COLUMNS; col++)
      {
         if ((matrix.report[row] & (1 << col)) != 0)
         {
            /* USB can only handle 6 simutaneous key presses in one report */
            if (keys_pressed < MAX_KEY_PRESSES)
            {
               keyboard_report->KeyCode[keys_pressed++] = matrix_to_key_map[row][col];
            }
         }
      }
   }
}