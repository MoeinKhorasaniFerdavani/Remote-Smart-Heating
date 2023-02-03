/*
 * RemoteSmartHeating.c
 *
 * Created: 2/3/2023 9:56:31 AM
 * Author : Khorasani
 */ 


#define F_CPU 8000000UL			/* Define CPU Frequency e.g. here 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include Delay header file */

#define LCD_Dir  DDRB			/* Define LCD data port direction */
#define LCD_Port PORTB			/* Define LCD data port */
#define RS PB0				/* Define Register Select pin */
#define EN PB1 				/* Define Enable signal pin *///#include "LCD_16x2_H.h"
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

char target = 30;
char current_tempreture = 20;
double alpha = 0.4;
double callibration_factor = 4.2;
char controller_value = 50;

void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	LCD_Port &= ~ (1<<RS);		/* RS=0, command reg. */
	LCD_Port |= (1<<EN);		/* Enable pulse */
	_delay_us(100);
	LCD_Port &= ~ (1<<EN);

	_delay_us(20000);

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(100);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(200);
}

void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
	LCD_Port |= (1<<RS);		/* RS=1, data reg. */
	LCD_Port|= (1<<EN);
	_delay_us(1000);
	LCD_Port &= ~ (1<<EN);

	_delay_us(20000);

	LCD_Port = (LCD_Port & 0x0F) | (data << 4); /* sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(100);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(200);
}

void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Dir = 0xFF;			/* Make LCD port direction as o/p */
	_delay_ms(2000);			/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);              /* Display on cursor off*/
	LCD_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);              /* Clear display screen*/
	_delay_ms(200);
}

void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}

void LCD_Clear()
{
	_delay_ms(2000);
	LCD_Command (0x01);		/* Clear display */
	_delay_ms(2000);
	LCD_Command (0x80);		/* Cursor at home position */
}

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
	LCD_Init();			/* Initialization of LCD*/
	
	LCD_String("Tempreture");/* Write string on 1st line of LCD*/
	_delay_ms(2000);
	LCD_Command(0xC0);/* Go to 2nd line*/
	//itoa(current_tempreture,String);
	//LCD_String("Hello World");	/* Write string on 2nd line*/
	while(1);
}





//int main()
//{
	//char String[5];
	//int value;
//
	////ADC_Init();
	//LCD_Init();
	//
	////LCD_Init();			/* Initialization of LCD */
	////LCD_String("ADC value");	/* Write string on 1st line of LCD */
//
	////while(1)
	////{
	//
		////LCD_Command(0xc4);	/* LCD16x2 cursor position */
		////getTempreture();	/* Read ADC channel 0 */
		////updateControllerValue(current_tempreture,target);
		//
		////LCD_Clear();
		////itoa(current_tempreture,String,10);	/* Integer to string conversion */ 
		//LCD_String("Hwllo");//show current tempreture
		////itoa(controller_value,String,10);	/* Integer to string conversion */
		////LCD_String_xy(1,1,String);//show current value 
		////DELAY NEEDED FOR 5 MIN 
		////UART_TxChar(controller_value)
		////LCD_String(String);						
		////LCD_String("  ");			
		//while(1);
	////{
	//return 0;
//}