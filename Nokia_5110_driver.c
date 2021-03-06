#include "Nokia_5110_driver.h"

void writeStringToLCD(const char *string) {
	while(*string) {
		writeCharToLCD(*string++);
	}
}

void writeStringToCenter(const char *string, char bank) {
	char length = strlen(string);
	setAddr(42 - ((length*6)/2),bank);

	writeStringToLCD(string);
}

void writeFullscreen(const unsigned char image [])
{
	unsigned short i = 0;
	for (i = 0; i < PCD8544_MAXBYTES; i++){
		writeToLCD(LCD5110_DATA, image[i]);
	}
}

void writeSprite(const unsigned char image[], unsigned char length)
{
	unsigned char i;
	for (i =0; i < length; i++)
	{
		writeToLCD(LCD5110_DATA, image[i]);
	}
	writeToLCD(LCD5110_DATA, 0);

}

void writeCharToLCD(char c) {
	unsigned char i;
	for(i = 0; i < 5; i++) {
		writeToLCD(LCD5110_DATA, font[c - 0x20][i]);
	}
	writeToLCD(LCD5110_DATA, 0);
}

void writeBlockToLCD(char *byte, unsigned char length) {
	unsigned char c = 0;
	while(c < length) {
		writeToLCD(LCD5110_DATA, *byte++);
		c++;
	}
}

void writeGraphicToLCD(char *byte, unsigned char transform) {
	int c = 0;
	char block[8];

	if(transform & FLIP_V) {
		SPI_LSB_FIRST;
	}

	if(transform & ROTATE) {
		c = 1;
		while(c != 0) {
			(*byte & 0x01) ? (block[7] |= c) : (block[7] &= ~c);
			(*byte & 0x02) ? (block[6] |= c) : (block[6] &= ~c);
			(*byte & 0x04) ? (block[5] |= c) : (block[5] &= ~c);
			(*byte & 0x08) ? (block[4] |= c) : (block[4] &= ~c);
			(*byte & 0x10) ? (block[3] |= c) : (block[3] &= ~c);
			(*byte & 0x20) ? (block[2] |= c) : (block[2] &= ~c);
			(*byte & 0x40) ? (block[1] |= c) : (block[1] &= ~c);
			(*byte & 0x80) ? (block[0] |= c) : (block[0] &= ~c);
			*byte++;
			c <<= 1;
		}
	}

	else {
		while(c < 8) {
			block[c++] = *byte++;
		}
	}

	if(transform & FLIP_H) {
		c = 7;
		while(c > -1) {
			writeToLCD(LCD5110_DATA, block[c--]);
		}
	}

	else {
		c = 0;
		while(c < 8) {
			writeToLCD(LCD5110_DATA, block[c++]);
		}
	}
	SPI_MSB_FIRST;
}

void writeToLCD(unsigned char dataCommand, unsigned char data) {
	LCD5110_SELECT;
	if(dataCommand) {
		LCD5110_SET_DATA;
	} else {
		LCD5110_SET_COMMAND;
	}
	UCB0TXBUF = data;
	while(!(IFG2 & UCB0TXIFG))
		;
	LCD5110_DESELECT;
}

void clearLCD() {
	setAddr(0, 0);
	int c = 0;
	while(c < PCD8544_MAXBYTES) {
		writeToLCD(LCD5110_DATA, 0);
		c++;
	}
	setAddr(0, 0);
}

void clearBank(unsigned char bank) {
	setAddr(0, bank);
	int c = 0;
	while(c < PCD8544_HPIXELS) {
		writeToLCD(LCD5110_DATA, 0);
		c++;
	}
	setAddr(0, bank);
}

void setAddr(unsigned char xAddr, unsigned char yAddr) {
	writeToLCD(LCD5110_COMMAND, PCD8544_SETXADDR | xAddr);
	writeToLCD(LCD5110_COMMAND, PCD8544_SETYADDR | yAddr);
}

void initLCD() {
	writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	writeToLCD(LCD5110_COMMAND, PCD8544_SETVOP | 0x3F);
	writeToLCD(LCD5110_COMMAND, PCD8544_SETTEMP | 0x02);
	writeToLCD(LCD5110_COMMAND, PCD8544_SETBIAS | 0x03);
	writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET);
	writeToLCD(LCD5110_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}
