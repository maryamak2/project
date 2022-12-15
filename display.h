/*
 * display.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Lenovo
 */
//THIS FILE FOR 7SEGMENT , LED & heating cooling elements control
#include <avr/io.h>
typedef unsigned char uint8 ;
#ifndef DISPLAY_H_
#define DISPLAY_H_
void initialize  ()
{
	DDRB &=(~(1<<2)); //set on/off button as input
		DDRB |=(1<<3); //set " led " as output

		DDRD |=(1<<0); // set heater as output
		DDRD |= (1<<1); // set fan as output
		DDRD &=(~(1<<2)); //set " up " button as input
		DDRD &=(~(1<<3)); //set " down " button as input
		PORTD &=(~(1<<0));  //intialization to port by zero
		PORTD &=(~(1<<1));  //intialization to port by zero


		DDRA|=(1<<0); // tens digit bit output
		DDRA|=(1<<1); // tens digit bit
		DDRA|=(1<<2); // tens digit bit
		DDRA|=(1<<3); // tens digit bit
		DDRA|=(1<<4); // toggling bit
		DDRC|=(1<<0); // unit digit bit
		DDRC|=(1<<1); // unit digit bit
		DDRC|=(1<<2); // unit digit bit
		DDRC|=(1<<3); // unit digit bit
}
void disp (uint8 temp)

{
	PORTA |=(1<<4);
	char left= temp / 10;
	char right= temp % 10;
	PORTA|=left ;
	PORTC|=right ;

}

void closed(void)
{
	PORTA &=(~(1<<4)) ; // shut 7 segment
	SREG &=(~(1<<7)); // disable global interrupt
	PORTD &=(~(1<<1)); // shut fan
	PORTD &=(~(1<<0));//shut heat
	PORTB &=(~(1<<3)); // shut led
}





#endif /* DISPLAY_H_ */
