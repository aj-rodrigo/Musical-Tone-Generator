/*
 * Lab5 forreal.c
 *
 * Created: 6/18/2021 3:37:10 PM
 * Author : ajrodrigo
 */ 

#include <avr/io.h>


int main(void)
{
    DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);		//Setting rows as output
    DDRB &= ~(1 << DDB0) & ~(1 << DDB1) & ~(1 << DDB2) & ~(1 << DDB3);	//Setting columns as input
    PORTB |= (1 << PORTB0) | (1<<PORTB1) | (1 << PORTB2) | (1<<PORTB3); //Enables pull-up resistor
    PORTD |= (1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);						//Set all rows high
	
	DDRD |= (1 << DDD1);												//Setting data direction for speaker functionality

	TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2B1);				//Set the timer mode to Fast PWM Timer 2 8 bits
	TCCR2B |= (1 << WGM22) | (1 << CS22) | (1 << CS21);					//Set pre-scaler to 256; Timer/Control Register A & B is part of Mode selection
	
	int data[4][4] = {{141, 133, 125, 118},								//Hand calculated OCR2A values by using given formula of frequency and period
					 {111, 105, 99, 93},
					 {88, 83, 78, 74},
					 {70, 66, 62, 58}};
	
    while (1)															//Infinite while loop that checks if keys are pressed and if so, play sound
    {
		for(int i = 4; i < 8; i++)
		{
			PORTD &= ~ (1 << i);										//Set row to low
			for(int j = 0; j < 4; j++)
			{
				if(!(PINB & (1 << j)))									//If button is pushed
				{
					while(!(PINB & (1 << j)))							//
					{
						OCR2A = data[i-4][j];							//Setting our OCR2A value to position in the int array
						OCR2B = OCR2A / 2;								//Duty cycle of 50% remains constant throughout program
						
						while((TIFR2 & (1 << OCF2B)) == 0)				//Wait for OCR2B overflow event
						{
							PORTD |= (1 << 1);							//If true, keep playing sound while button is being pushed
						}
						TIFR2 |= (1 << OCF2B);							//Reset OCR2B overflow flag
						
						while((TIFR2 & (1 << OCF2A)) == 0)				//Wait for OCR2A overflow event
						{
							PORTD &= ~(1 << 1);							//If false, stop playing sound when button is released
						}
						TIFR2 |= (1 << OCF2A);							//Reset OCR2A overflow flag
					}
				}
			}
			PORTD |= (1 << i); //Set row to high
			
		}
    }
}

