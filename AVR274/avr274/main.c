/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief 
 *      This is an example for testing the One-wire Software UART
 *
 *      Data will be received and sent back when a return character is received
 *      or the data array is full.
 *
 * \par Application note:
 *      AVR274: Single-wire Software UART
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Name:  $
 * $Revision: 1222 $
 * $RCSfile: main.c,v $
 * $Date: 2006-11-14 17:32:38 +0100 (ti, 14 nov 2006) $  \n
 ******************************************************************************/

#include "single_wire_UART.h"
#include <ioavr.h>
#include <inavr.h>

#define RECEIVED_DATA_SIZE	8

void Print_String(char* str);

/*! \brief  Sends an array of data.
 *
 *  This function sends an array of data to the
 *  UART. If TX buffer is full, the function will
 *	busy wait until there is room in the buffer.
 *  The array must end with a string terminator ('\n').
 */
void Print_String(char* str)
{
  while( *str != '\0' )
  {
    if( !READ_FLAG(SW_UART_status, SW_UART_TX_BUFFER_FULL) )
    {
      SW_UART_Transmit((uint8_t) *str++);
    }
    if( READ_FLAG(SW_UART_status, SW_UART_FRAME_ERROR) )
    {
      PORTB = 0xF0;
      for(;;){}
    }
  }
}

/*! \brief  Main loop.
 *
 *  This contains a while loop receiving data until
 *  the received_data array is full or a return
 *  character is received. It will then send back the
 *  received data and repeat the loop.
 */
int main( void )
{
  __enable_interrupt();   //!< Enable global interrupts.
  SW_UART_Enable();
  DDRB = 0xFF;
  PORTB = 0xFF;
  uint8_t received_data[RECEIVED_DATA_SIZE];
  uint8_t i;
  for(i=0;i<RECEIVED_DATA_SIZE;i++)
  {
    received_data[i] = 0; 
  }
  i=0;

  //Print_String("Test Program\n\r");
  for(;;)
  {
    if( READ_FLAG(SW_UART_status, SW_UART_RX_BUFFER_OVERFLOW)) 			
    {
      PORTB = 0x0F;
      for(;;){}
    }
    if( READ_FLAG(SW_UART_status, SW_UART_FRAME_ERROR) )
    {
      PORTB = 0xF0;
      for(;;){}
    }
    while( (!READ_FLAG(SW_UART_status, SW_UART_RX_BUFFER_FULL)) ){} //!< Wait until receive buffer is full.
      
    received_data[i] = SW_UART_Receive();
    i++;
    if( (i >= RECEIVED_DATA_SIZE-1) || (received_data[i-1] == 0x0D) ) //!< Stop receiving if buffer is full or a return character is received.
    {
      received_data[i] = '\0';
      Print_String(((char *) received_data));    //!< Send back received data.
      i = 0;
    }	
  }
}
