#include "avr/io.h"
#include "avr/interrupt.h"

int state=0;

ISR(TIM0_COMPA_vect)
{
	switch (!(PINB&&(1<<PINB1|1<<PINB0))){
	//forward pressed
		case PINB0:
			switch (state){
				case 0:
				PORTB= (0<<PB5) | (0<<PB4) | (0<<PB3) | (1<<PB2);
				state++;
				break;
				case 1:
				PORTB= (0<<PB5) | (0<<PB4) | (1<<PB3) | (1<<PB2);
				state++;
				break;				
				case 2:
				PORTB= (0<<PB5) | (0<<PB4) | (1<<PB3) | (0<<PB2);
				state++;
				break;				
				case 3:
				PORTB= (0<<PB5) | (1<<PB4) | (1<<PB3) | (0<<PB2);
				state++;
				break;				
				case 4:
				PORTB= (0<<PB5) | (1<<PB4) | (0<<PB3) | (0<<PB2);
				state++;
				break;				
				case 5:
				PORTB= (1<<PB5) | (1<<PB4) | (0<<PB3) | (0<<PB2);
				state++;
				break;				
				case 6:
				PORTB= (1<<PB5) | (0<<PB4) | (0<<PB3) | (0<<PB2);
				state++;
				break;				
				case 7:
				PORTB= (1<<PB5) | (0<<PB4) | (0<<PB3) | (1<<PB2);
				state=0;
				break;
			}
		break;
	//reverse pressed
		case PINB1:	
			switch (state){
				case 0:
				PORTB= (1<<PB5) | (0<<PB4) | (0<<PB3) | (0<<PB2);
				state++;
				break;				
				case 1:
				PORTB= (1<<PB5) | (1<<PB4) | (0<<PB3) | (0<<PB2);
				state++;
				break;				
				case 2:
				PORTB= (0<<PB5) | (1<<PB4) | (0<<PB3) | (0<<PB2);
				state++;
				break;				
				case 3:
				PORTB= (0<<PB5) | (1<<PB4) | (1<<PB3) | (0<<PB2);
				state++;
				break;				
				case 4:
				PORTB= (0<<PB5) | (0<<PB4) | (1<<PB3) | (0<<PB2);
				state++;
				break;				
				case 5:
				PORTB= (0<<PB5) | (0<<PB4) | (1<<PB3) | (1<<PB2);
				state++;
				break;				
				case 6:
				PORTB= (0<<PB5) | (0<<PB4) | (0<<PB3) | (1<<PB2);
				state++;
				break;				
				case 7:
				PORTB= (1<<PB5) | (0<<PB4) | (0<<PB3) | (1<<PB2);
				state=0;
				break;
			}		
		break;	

		default:
		break;	
	}

}

int main (void){

//setup io pins
PORTB= (0<<PB5) | (0<<PB4) | (0<<PB3) | (0<<PB2) |(1<<PB1) | (1<<PB0);
DDRB= (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (0<<DDB1) | (0<<DDB0);

//setup timer and interrupt
OCR0A=0xA; 
TCCR0A|=(1<<COM0A1) | (0<<COM0A0);
TCCR0B|=(0<<CS02) | (0<<CS01) | (1<<CS00);
TIMSK0|=(1<<OCIE0A);

sei(); // Enable Global Interrupts

}
