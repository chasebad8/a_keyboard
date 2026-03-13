/* atmega32u4.ld - a minimal linker script to initialize the required sections of memory for a C program
   This MCU utilizes the Harvard architecture where code and data are stored in different memories.
   Therefore code will go into flash and data will go into SRAM */

/* think of this like memory mapped IO. Instead of mmap, we are physically deciding where we want
   our code to reside. */
MEMORY
{
   /* MCU flash memory 0x0000 - 0x7FFF */
   FLASH (rx)  : ORIGIN = 0x0000, LENGTH = 0x7FFF
   /* MCU SRAM memory 0x0100 - 0xA00 */
   /* TODO: This worked too? RAM   (rwx) : ORIGIN = 0x800100, LENGTH = 0x900 */
   RAM   (rwx) : ORIGIN = 0x100, LENGTH = 0x900
}

/* the sections command tells the linker how to combine the input files into an output ELF and where segments belong in memory. */
SECTIONS
{
   /* the vector table that is at flash address 0x0000 */
   .vectors :
   {
      /* KEEP keyword tells the linker to preserve this section even if it looks like it is not used */
      KEEP(*(.vectors))
   } > FLASH

   /* program code and read-only data are in the .text section */
   .text :
   {
      /* the compiler will mark C code to go to the .text section */
      *(.text*)
      /* the compiler will mark constants to go to the .rodata section */
      *(.rodata*)
   } > FLASH

   /* AT = address type or the load address where data is stored.
      We can use that address and the data_start and end variables to copy our data from flash to SRAM */
   .data :
   {
      _data_start = .;
      *(.data*)
      _data_end = .;
   } > RAM AT > FLASH

    /* Location of .data in flash */
    _data_load = LOADADDR(.data);

   /* the .bss section is for 0 initialized data. We don't actually need to pull anything from flash here but we
      do need to initialize it all to 0 in SRAM. */
   .bss :
   {
      _bss_start = .;
      *(.bss*)
      _bss_end = .;
   } > RAM

    /* Stack at the top of SRAM, essentially 0x100 + 2.5K */
    _stack_start = ORIGIN(RAM) + LENGTH(RAM);
}