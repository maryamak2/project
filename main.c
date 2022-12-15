/*
 * main.c
 *
 *  Created on: Dec 7, 2022
 *      Author: Hazem
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Temp_control.h"
#define set_bit(reg,bit) reg|=(1<<bit)
#define clr_bit(reg,bit) reg&=~(1<<bit)
#define tog_bit(reg,bit) reg^=(1<<bit)
#define read_bit(reg,bit) reg&(1<<bit)
#define is_bit_set(reg,bit) reg&(1<<bit)>>bit
#define is_bit_clr(reg,bit) !(reg&(1<<bit)>>bit)
#include "display.h"
#include "EEPROM.h"
#define NUM_OF_TEMP_MEASURES 10


uint8 SetTemp = 60 ;
uint8 end;
uint8 right ;
uint8 left ;
uint8 state;
uint8 ovf;
uint8 heat;
uint8 CrrTemp;

uint8 temp_arr[NUM_OF_TEMP_MEASURES];
uint8 counter_timer0;
uint8 counter_timer1 ;
uint8 temp_measure_f;
uint8 actual_temp;
uint8 avg_temp;
uint8 arr_counter;

void button_up()
{
	SREG |=(1<<7); // enable global interrupt

								GICR |=(1<<6);   // enable module int 0
								MCUCR |=(1<<1);      // " UP " button  status register set ( data sheet)
								MCUCR &=(~(1<<0));   // " UP " button  status register set ( data sheet)

								GICR |=(1<<7);   // enable module int 1
								MCUCR |=(1<<3);      // " DOWN " button  status register set ( data sheet)
								MCUCR &=(~(1<<2));   // " DOWN " button  status register set ( data sheet)

}

void button_down()
{

	SREG |=(1<<7); // enable global interrupt

	GICR |=(1<<7);   // enable module int 1
	MCUCR |=(1<<3);      // " DOWN " button  status register set ( data sheet)
	MCUCR &=(~(1<<2));   // " DOWN " button  status register set ( data sheet)

	GICR |=(1<<6);   // enable module int 0
	MCUCR |=(1<<1);      // " UP " button  status register set ( data sheet)
	MCUCR &=(~(1<<0));   // " UP " button  status register set ( data sheet)

}


int main ()
{
	initialize();
	temp_ctrl_initialize();
	EEPROM_write(0x0000,SetTemp);

	while(1)
	{



		if (PINB && (1<<2)) //check for on/off button state
		{
			state =1;
			SREG |=(1<<7); // enable global interrupt
			GICR |=(1<<5);   // enable module int 2
			MCUCSR &=(~(1<<6)); // " on/off " button  status register set ( data sheet)

			SetTemp = EEPROM_read(0x0000); // read set temp from eeprom

			while (state)
			{

				if (PIND && (1<<2)) //check for up button state
						{ while( PIND && (1<<2) ) ; // waiting button to be released

						    button_up();
							TempSetMod(&SetTemp); // function to enter temperature setting mode
						}



						if (PIND && (1<<3)) //check for down button state
						{while( PIND && (1<<3) ) {} // waiting button to be released

						     button_down();
							TempSetMod(&SetTemp); // function to enter temperature setting mode*/
						}

						CrrTemp=avg_temp;// function to read registered temperature
						 disp(CrrTemp); //display current temp value

						Operation();// function to compare sensor temp with set temp & decide state

			}

			EEPROM_write(0x0000,SetTemp); 	// send set temp value to EEPROM
			closed(); // function to shut the system condition
		}

	}



	return 0;
	}
