#include "msp430g2553.h"
#include "PCD8544.h"
#include "Nokia_5110_driver.h"
#include "images.h"
#include "string.h"

//GLOBAL VARIABLES
unsigned char i = 0;
volatile signed char screen = 0;
unsigned char current_state = HOME;
volatile unsigned char select = 0;

//FUNCTION PROTOTYPES
void setupClocks(void);
void setupPORT1(void);
void setupPORT2(void);
void setupUSIB(void);
void actions(void);
void home(void);

//MAIN
void main(void)
{
	setupClocks();
	setupPORT1();
	setupPORT2();
	setupUSIB();

	_delay_cycles(500000);
	initLCD();
	clearLCD();
	writeStringToCenter("TAMAGOTCHI", 2);

	_delay_cycles(2000000);
	clearLCD();

	_enable_interrupt();
	writeStringToCenter("Press Select",2);

	while(!select){}

	setAddr(0,0);
	for (i =0; i < 5;i++)
	{
		writeFullscreen(egg);
		_delay_cycles(600000);
		writeFullscreen(egg3);
		_delay_cycles(600000);
	}

	writeFullscreen(egg4);
	_delay_cycles(600000);
	writeFullscreen(egg5);
	_delay_cycles(600000);
	writeFullscreen(egg6);
	_delay_cycles(600000);
	writeFullscreen(egg8);
	_delay_cycles(1000000);

	clearLCD();
	writeStringToCenter("It's a cat!", 2);

	_delay_cycles(1000000);

	setAddr(0, 0);
	clearLCD();

	while(1)
	{
		switch(current_state)
		{
		case HOME:
			home();
			break;
		case ACTIONS:
			actions();
			break;
		case FOOD:
			break;
		case GAMES:
			break;
		case TOILET:
			break;
		case MEDS:
			break;
		}


	}
}

//FUNCTION DECLARATIONS
void setupClocks(void)
{
	WDTCTL = WDTPW + WDTHOLD; // disable WDT
	BCSCTL1 = CALBC1_1MHZ; // 1MHz clock
	DCOCTL = CALDCO_1MHZ;
}

void setupPORT1(void)
{
	P1OUT |= LCD5110_SCE_PIN + LCD5110_DC_PIN;
	P1DIR |= LCD5110_SCE_PIN + LCD5110_DC_PIN;
}

void setupPORT2(void)
{
	P2OUT |= BIT0 + BIT1 + BIT2;
	P2REN |= BIT0 + BIT1 + BIT2;
	P2IE  |= BIT0 + BIT1 + BIT2;
	P2IES |= BIT0 + BIT1 + BIT2;
	P2IFG &= 0x00;
}

void setupUSIB(void)
{
	// setup USIB
	P1SEL |= LCD5110_SCLK_PIN + LCD5110_DN_PIN;
	P1SEL2 |= LCD5110_SCLK_PIN + LCD5110_DN_PIN;

	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2; // SMCLK
	UCB0BR0 |= 0x01; // 1:1
	UCB0BR1 = 0;
	UCB0CTL1 &= ~UCSWRST; // clear SW
}

void home(void)
{
	unsigned char c = 0;
	//Cycle through animations for now
	for (c = 0; c<2 ;c++)
	{
		writeFullscreen(cat);
		_delay_cycles(400000);
		writeFullscreen(cat2);
		_delay_cycles(400000);
		writeFullscreen(cat3);
		_delay_cycles(400000);
		writeFullscreen(cat4);
		_delay_cycles(400000);
	}

	current_state = ACTIONS;

}

void actions(void)
{
	switch (screen)
	{
	case 0:
		writeFullscreen(food);
		break;
	case 1:
		writeFullscreen(games);
		break;
	case 2:
		writeFullscreen(bin);
		break;
	case 3:
		writeFullscreen(meds);
		break;
	default:
		clearLCD();
		writeStringToLCD("Oops");
		break;
	}
}
//INTERRUPTS

#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
{
	_disable_interrupt();
	if (P2IFG & BIT0)
	{
		screen++; //moves screen to the next screen
		if (screen > 3) //reset to the first screen
		{
			screen = 0;
		}
	}

	if (P2IFG & BIT1)
	{
		select = 1; //If middle button is pressed change the selection variable

	}

	if (P2IFG & BIT2)
	{
		screen--; //moves screen to the previous screen
		if (screen < 0) //reset to the last screen
		{
			screen = 3;
		}
	}
	P2IFG &= ~(BIT0 + BIT1 + BIT2);
	_delay_cycles(1000);
	_enable_interrupt();
}
