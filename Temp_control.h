/*
 * Temp_control.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Lenovo
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define NUM_OF_TEMP_MEASURES 10
typedef unsigned char uint8 ;
#ifndef TEMP_CONTROL_H_
#define TEMP_CONTROL_H_
void temp_ctrl_initialize()
{
	timer0_init();
    timer1_init();
	timer2_init();
	adc_init();
	temp_arr_init();

}
void adc_init(void)
{
	DDRA &=~(1<<7) ;
	// AREF = AVcc
	ADMUX = (1<<REFS0);

	// ADC Enable and prescaler of 128

	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}


uint8 adc_read(void)
{
	// select the corresponding channel 0~7
	uint8 ch = 7;
	ADMUX = (ADMUX & 0xF8)|ch;

	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);

	// wait for conversion to complete
	// ADSC becomes '0' again
	while(ADCSRA & (1<<ADSC));

	return (ADC);
}
ISR(INT0_vect) //int0 " up button " instructions declaration
 {
		if (SetTemp <= 70 ) // check if set temp is <= 70
		{
			SetTemp = SetTemp + 5 ;
		}

		// increase set temp by 5 degrees

		ovf = 0 ; // reset timer
 }

ISR(INT1_vect) //int1 " down Button " instructions declaration
 {

	if ( SetTemp >= 40 ) // check if set temp is >= 40
	{
		SetTemp = SetTemp - 5 ;
	}
		// decrease set temp by 5 degrees
		ovf = 0 ;	// reset timer

 }

ISR(INT2_vect) //int2 " on/ff " instructions declaration
 {
		state = 0 ;
 }

ISR(TIMER0_COMP_vect)
{
	counter_timer0++;
	if(counter_timer0 %6 == 0)
	{

		actual_temp = adc_read();
	    add_new_temp(temp_arr , actual_temp);
		avg_temp = calc_avg_temp(temp_arr);
	}

}

ISR(TIMER1_COMPA_vect)
{
	counter_timer1++;
		if(counter_timer1 %4 == 0)  // toggle led while heating mode
		{
			PORTB ^=(1<<3);
		}
}

ISR(TIMER2_COMP_vect)
{
	if(ovf < 19 ) // 19 overflow simulates 5 seconds on 1M clk with 1024 prescaler
		{ ovf ++ ; }

	else
	{
		end =1 ;
		ovf =0;
	}
}



uint8 calc_avg_temp(uint8 * arr){
	unsigned int sum = 0;
	unsigned int avg;
	uint8 i ;

	for( i=0; i<NUM_OF_TEMP_MEASURES; i++){
		sum = sum + arr[i];
	}
	avg =  sum / NUM_OF_TEMP_MEASURES;

	return (uint8)avg;
}

void temp_arr_init(void)
 {
	 unsigned char first_temp = adc_read();
	 unsigned char i ;

	 for (i = 0; i<NUM_OF_TEMP_MEASURES; i++)
	 {
		 temp_arr[i]= first_temp;
	 }

	 avg_temp = calc_avg_temp(temp_arr);
	 arr_counter = 0;
 }

void add_new_temp(uint8 * arr , uint8 new_val){
	 arr[arr_counter] = new_val;
	 arr_counter ++;

	 if (arr_counter == 9)
	 {
		 arr_counter = 0;
	 }
 }


void TempSetMod (uint8* Temp)
{
	SREG |=(1<<7); // enable global interrupt
	TIMSK |=(1<<6); // enable timer2 overflow interrupt



	end=0;
	ovf=0;
			while( (!(end)) && state )
			{

				right= (*Temp) % 10;
				left= (*Temp) / 10;
				PORTA|=(left<<0);
				PORTC|=(right<<0);
				PORTA |=(1<<4);
				_delay_ms(500);
				PORTA &=(~(1<<4));
				_delay_ms(500);

			}

	TIMSK &=(~(1<<6)); // disable timer2 overflow intrupt
	GICR &=(~(1<<6));   // disable module int 0
	GICR &=(~(1<<7));   // disable module int 1
}

void Operation(void)
{
	if ( CrrTemp <= (SetTemp-5) ) // Check if temp is lower Set temp
	{
		PORTD &=(~(1<<1)); // shut fan
		PORTD |= (1<<0);  // turn heat on
		TIMSK |= (1<<2); // enable timer1

	}

	else if (CrrTemp >=(SetTemp +5)) // Check if temp is higher than Set temp
	{
		PORTD &=(~(1<<0)); //shut heat
		PORTD |= (1<<1); // turn fan on
		TIMSK &= (~(1<<2)); // disable timer1
		PORTB |= (1<<3); // turn led on
	}

	else
	{
		TIMSK &= (~(1<<2)); // disable timer1
		PORTD &=(~(1<<1));  // shut fan
		PORTD &=(~(1<<0)); //shut heat
		PORTB &=(~(1<<3)); // shut led

	}


}

void timer0_init(void)
{
	//choose normal ovf mode
	TCCR0 &= (~(1<<6)) ;
	TCCR0 &= (~(1<<3)) ;
	TCCR0 &= (~(1<<5)) ;
	TCCR0 &= (~(1<<4)) ;

	//set prescaler to 64
	TCCR0 |= (1<<0) ;
	TCCR0 |= (1<<1) ;
	TCCR0 &= (~(1<<2)) ;

    SREG |= (1<<7); // enable global interrupt
    TIMSK |= (1<<0); // enable overflow interrupt

}

void timer1_init(void)
{
	// normal ovf mode
	TCCR1A &= (~(1<<4)) ;
	TCCR1A &= (~(1<<5)) ;
	TCCR1A &= (~(1<<6)) ;
	TCCR1A &= (~(1<<7)) ;
	TCCR1A &= (~(1<<0)) ;
	TCCR1A &= (~(1<<1)) ;
	TCCR1B &= (~(1<<3)) ;
	TCCR1B &= (~(1<<4)) ;

	// prescaler 1024
	TCCR1B |=(1<<0);
	TCCR1B &= (~(1<<1));
	TCCR1B |= (1<<2);

}

void timer2_init(void)
{
	// normal mode
		TCCR2 &=(~(1<<3)) ;
		TCCR2 &=(~(1<<6));

		//oc2 disconnected
		TCCR2 &=(~(1<<4));
		TCCR2 &=(~(1<<5));

		// prescaler 1024
		TCCR2 |= (1<<0);
		TCCR2 |= (1<<1);
		TCCR2 |= (1<<2);
}




#endif /* TEMP_CONTROL_H_ */
