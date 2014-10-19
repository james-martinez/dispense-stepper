#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int direction=0;
int state=0;

ISR(TIM0_COMPA_vect)
{
if((PINB & (1<<PB0)) == 0 && direction==0){
	//button pressed , forward mode
			switch (state){
				case 0:
				PORTB= (0<<PB4) | (0<<PB3) | (0<<PB2) | (1<<PB1);
				state++;
				break;
				case 1:
				PORTB= (0<<PB4) | (0<<PB3) | (1<<PB2) | (1<<PB1);
				state++;
				break;				
				case 2:
				PORTB= (0<<PB4) | (0<<PB3) | (1<<PB2) | (0<<PB1);
				state++;
				break;				
				case 3:
				PORTB= (0<<PB4) | (1<<PB3) | (1<<PB2) | (0<<PB1);
				state++;
				break;				
				case 4:
				PORTB= (0<<PB4) | (1<<PB3) | (0<<PB2) | (0<<PB1);
				state++;
				break;				
				case 5:
				PORTB= (1<<PB4) | (1<<PB3) | (0<<PB2) | (0<<PB1);
				state++;
				break;				
				case 6:
				PORTB= (1<<PB4) | (0<<PB3) | (0<<PB2) | (0<<PB1);
				state++;
				break;				
				case 7:
				PORTB= (1<<PB4) | (0<<PB3) | (0<<PB2) | (1<<PB1);
				state=0;
				break;
			}
}
	 else if((PINB & (1<<PB0)) == 0 && direction==1){
	//button pressed , reverse mode	
			switch (state){
				case 7:
				PORTB= (0<<PB4) | (0<<PB3) | (0<<PB2) | (1<<PB1);
				state=0;
				break;
				case 6:
				PORTB= (0<<PB4) | (0<<PB3) | (1<<PB2) | (1<<PB1);
				state++;
				break;				
				case 5:
				PORTB= (0<<PB4) | (0<<PB3) | (1<<PB2) | (0<<PB1);
				state++;
				break;				
				case 4:
				PORTB= (0<<PB4) | (1<<PB3) | (1<<PB2) | (0<<PB1);
				state++;
				break;				
				case 3:
				PORTB= (0<<PB4) | (1<<PB3) | (0<<PB2) | (0<<PB1);
				state++;
				break;				
				case 2:
				PORTB= (1<<PB4) | (1<<PB3) | (0<<PB2) | (0<<PB1);
				state++;
				break;				
				case 1:
				PORTB= (1<<PB4) | (0<<PB3) | (0<<PB2) | (0<<PB1);
				state++;
				break;				
				case 0:
				PORTB= (1<<PB4) | (0<<PB3) | (0<<PB2) | (1<<PB1);
				state++;
				break;
			}	  
	 }
}

int main (void){

//setup io pins
PORTB= (0<<PB4) | (0<<PB3) | (0<<PB2) |(0<<PB1) | (1<<PB0);
DDRB=  (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (0<<DDB0);

//setup timer and interrupt
OCR0A|=0xA; 
TCCR0A|=(1<<COM0A1) | (0<<COM0A0);
TCCR0B|=(0<<CS02) | (0<<CS01) | (1<<CS00);
TIMSK0|=(1<<OCIE0A);
   
//check button state at startup for direction.   
if((PINB & (1<<PB0)) == 0){
   direction=1; // reverse mode
   }
   
sei(); // Enable Global Interrupts

while (1){}
}
