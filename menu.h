typedef const MenuStruct
{
	const char *icon;
	unsigned char num_menupoints;
	unsigned char left;
	unsigned char right;
	unsigned char enter;
	void (*fp) (void);
} MenuType;
