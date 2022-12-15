/*
 * EEPROM.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Lenovo
 */
#include <avr/io.h>

#define clr_bit(reg,bit) reg&=~(1<<bit)
#define tog_bit(reg,bit) reg^=(1<<bit)
#define read_bit(reg,bit) reg&(1<<bit)
#define is_bit_set(reg,bit) reg&(1<<bit)>>bit
#define is_bit_clr(reg,bit) !(reg&(1<<bit)>>bit)
#define set_bit(reg,bit) reg|=(1<<bit)
typedef unsigned char uint8 ;
#include "Temp_control.h"
#ifndef EEPROM_H_
#define EEPROM_H_
unsigned char EEPROM_read (const unsigned short addr)
{

	EEARL = (char)addr;
	EEARH = (char)(addr>>8);

	// Start Reading
	set_bit(EECR,EERE);

	// Return Data
	return EEDR;
}

void EEPROM_write(const unsigned short addr, uint8 data){
	uint8 INT_EN=0;

	// Setup Address Register
	EEARL = (char)addr;
	EEARH = (char)(addr>>8);

	// Put Data in Data Register
	EEDR = data;

	// Disable Interrupts to work fine
	if(((SREG&(1<<7))>>7)==1){
		INT_EN = 1;
		clr_bit(SREG,7);
	}

	// Start Writing
	set_bit(EECR,EEMWE);
	set_bit(EECR,EEWE);
	while(((EECR&(1<<EEWE))>>EEWE)==1);

	// Back
	if (INT_EN==1)
	{
		set_bit(SREG,7);
	}

}



#endif /* EEPROM_H_ */
