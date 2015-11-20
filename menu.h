#ifndef _MENU_H_
#define _MENU_H_

typedef const MenuStruct
{
	const char *icon;
	unsigned char num_menupoints;
	unsigned char left;
	unsigned char right;
	unsigned char enter;
	void (*fp) (void);
} MenuType;

#endif
