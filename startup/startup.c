#include <stdint.h>

/* grab our symbols that were defined in the linker script */
extern uint16_t _data_start;
extern uint16_t _data_end;
extern uint16_t _data_load;
extern uint16_t _bss_start;
extern uint16_t _bss_end;
extern uint16_t _stack_start;

/* declare main() */
int main(void);
void reset_handler(void);

/* create an array of function pointers called vectors that will be placed in the .vectors code section by the linker.
   these are all read only, the function pointer cannot be changed.
   check the interrupt vector table in the datasheet to know what the other interrupts are for */
__attribute__((section(".vectors")))
void (* const vectors_arr[])(void) =
{
    reset_handler,  /* function to execute on POR */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

/******************************************************************************
 * @name reset_handler
 *
 * @brief reset handler function
 *
 * On a restart restart_handler is the first function that is called from the
 * interrupt vector
 *
 * @param  none
 *
 * @return none
 *
 ******************************************************************************/
    void reset_handler(void) {
    uint16_t *src, *dst;

    /* Initialize .data section: copy from flash to SRAM */
    src = &_data_load;
    dst = &_data_start;
    while (dst < &_data_end) {
        *dst++ = *src++;
    }

    /* Zero out .bss section */
    dst = &_bss_start;
    while (dst < &_bss_end) {
        *dst++ = 0;
    }

    /* Initialize stack pointer (SPH: high byte, SPL: low byte) */
    uint16_t stack_top = (uint16_t)&_stack_start;
    asm volatile (
        "movw r28, %0\n\t"  // move 16-bit value to Y register (r28:r29)
        "out __SP_L__, r28\n\t"
        "out __SP_H__, r29\n\t"
        :
        : "r"(stack_top)
        : "r28","r29"
    );

    /* Jump to main */
    main();

    /* If main returns, loop forever */
    while (1) {}
    }

