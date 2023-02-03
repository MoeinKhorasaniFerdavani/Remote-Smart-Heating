/*
 * RemoteSmartHeating.c
 *
 * Created: 2/3/2023 9:56:31 AM
 * Author : Khorasani
 */ 


#define F_CPU 8000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
//#include "LCD_16x2_H.h"
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

char target = 30;
char current_tempreture = 20;
double alpha = 0.4;
double callibration_factor = 4.2;
char controller_value = 50;

void updateControllerValue(current_tempreture,target)
{
	char delta = (char)(alpha * (target - current_tempreture));
	controller_value += delta;
}


void ADC_Init()
{
	DDRA=0x0;			/* Make ADC port as input */
	ADCSRA = 0x87;			/* Enable ADC, fr/128  */
	ADMUX = 0x60;			/* Vref: Avcc, ADC channel: 0  and use left adjustment*/
	
}

void getTempreture()
{
	

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */
	
	_delay_us(10);
	
	current_tempreture = (char)(callibration_factor * ADCH);/* Return digital value*/
}


void UART_init(long USART_BAUDRATE)
{
	UCSRB |=  (1 << TXEN);	/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit char size */
	UBRRL = BAUD_PRESCALE;			/* Load lower 8-bits of the baud rate */
	UBRRH = (BAUD_PRESCALE >> 8);		/* Load upper 8-bits*/
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
	UDR = ch ;
}

int main()
{
	char String[5];
	int value;

	ADC_Init();
	//LCD_Init();			/* Initialization of LCD */
	//LCD_String("ADC value");	/* Write string on 1st line of LCD */

	while(1)
	{
	
		//LCD_Command(0xc4);	/* LCD16x2 cursor position */
		getTempreture();	/* Read ADC channel 0 */
		updateControllerValue(current_tempreture,target);
		itoa(current_tempreture,String,10);	/* Integer to string conversion */ 
		//UART_TxChar(controller_value)
		//LCD_String(String);						
		//LCD_String("  ");			
	}
	return 0;
}