/* Doxygen documentation mainpage ********************************************/
/*! \mainpage
 * \section intro Introduction
 * This documents the software for application note AVR274. This is a
 * single-wire software UART.
 *
 * \section compinfo Compilation Info
 * This software was compiled with IAR Embedded Workbench, 4.20A. For other
 * compiliers than IAR modifications are needed.\n\n
 * To make project on IAR EWAVR:\n
 * \li Add the .c files to project (main.c, single_wire_UART.c).
 * \li Change the single_wire_UART.h according to the device you are using and UART settings you would like to use.
 * \li Go to the options menu in Embedded Workbench (alt+F7).
 * \li Under "General/Target", make sure the correct device and memory model is selected.
 * \li Under "General/System", check the "Enable bit definitions in I/O include files" option.
 * \li Under "C/C++ Compiler/Optimization" select maximum speed optimization.
 * \li Under "Linker/Output" set the output format to "ubrof 8" when debugging with AVR Studio and "intel-extended" for release.
 *
 * \section deviceinfo Device Info
 * All devices with a timer compare interrupt and an external interrupt can
 * be used. The example is written for ATmega32. The single_wire_UART.h needs
 * to be changed according to the device and baud rate settings.
 *
 * \section codesize Code Size
 * single_wire_UART.c   500 bytes (454 bytes if UART_counter and SW_UART_Status are locked in dedicated register)\n
 * main.c               218 bytes
 *
 * \section contactinfo Contact Info
 * For more info about Atmel AVR visit http://www.atmel.com/products/AVR/ \n
 * For application notes visit
 * http://www.atmel.com/dyn/products/app_notes.asp?family_id=607 \n
 * Support mail: avr@atmel.com
 */
