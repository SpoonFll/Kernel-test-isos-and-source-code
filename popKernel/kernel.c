
#define VGA_ADDRESS 0xB8000

#ifndef KEYBOARD_H
#define KEYBOARD_H
#define KEYBOARD_PORT 0x60

#define KEY_A 0x1E
#define KEY_B 0x30
#define KEY_C 0x2E
#define KEY_D 0x20
#define KEY_E 0x12
#define KEY_F 0x21
#define KEY_G 0x22
#define KEY_H 0x23
#define KEY_I 0x17
#define KEY_J 0x24
#define KEY_K 0x25
#define KEY_L 0x26
#define KEY_M 0x32
#define KEY_N 0x31
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_Q 0x10
#define KEY_R 0x13
#define KEY_S 0x1F
#define KEY_T 0x14
#define KEY_U 0x16
#define KEY_V 0x2F
#define KEY_W 0x11
#define KEY_X 0x2D
#define KEY_Y 0x15
#define KEY_Z 0x2C
#define KEY_ENTER 0x1C
#define KEY_1 0x02
#define KEY_2 0x03
#define KEY_3 0x04
#define KEY_4 0x05
#define KEY_5 0x06
#define KEY_6 0x07
#define KEY_7 0x08
#define KEY_8 0x09
#define KEY_9 0x0A
#define KEY_0 0x0B
#define KEY_MINUS 0x0C
#define KEY_EQUAL 0x0D
#define KEY_SQUARE_OPEN_BRACKET 0x1A
#define KEY_SQUARE_CLOSE_BRACKET 0x1B
#define KEY_SEMICOLON 0x27
#define KEY_BACKSLASH 0x2B
#define KEY_COMMA 0x33
#define KEY_DOT 0x34
#define KEY_FORESLASH 0x35
#define KEY_F1 0x3B
#define KEY_F2 0x3C
#define KEY_F3 0x3D
#define KEY_F4 0x3E
#define KEY_F5 0x3F
#define KEY_F6 0x40
#define KEY_F7 0x41
#define KEY_F8 0x42
#define KEY_F9 0x43
#define KEY_F10 0x44
#define KEY_F11 0x85
#define KEY_F12 0x86
#define KEY_BACKSPACE 0x0E
#define KEY_DELETE 0x53
#define KEY_DOWN 0x50
#define KEY_END 0x4F
#define KEY_ESC 0x01
#define KEY_HOME 0x47
#define KEY_INSERT 0x52
#define KEY_KEYPAD_5 0x4C
#define KEY_KEYPAD_MUL 0x37
#define KEY_KEYPAD_Minus 0x4A
#define KEY_KEYPAD_PLUS 0x4E
#define KEY_KEYPAD_DIV 0x35
#define KEY_LEFT 0x4B
#define KEY_PAGE_DOWN 0x51
#define KEY_PAGE_UP 0x49
#define KEY_PRINT_SCREEN 0x37
#define KEY_RIGHT 0x4D
#define KEY_SPACE 0x39
#define KEY_TAB 0x0F
#define KEY_UP 0x48

#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE_COLOR 15

#endif

unsigned short *terminal_buffer;

unsigned int vga_index;
unsigned int lineNumber;

char get_ascii_char(int ascii)
{
	switch(ascii)
	{
		case KEY_A:
		     return 'a';
		     break;
		case KEY_B:
		     return 'b';
		     break;
		case KEY_C:
		     return 'c';
		     break;
		case KEY_D:
		     return 'd';
		     break;
		case KEY_E:
		     return 'e';
		     break;
		case KEY_F:
		     return 'f';
		     break;	
		case KEY_G:
		     return 'g';
		     break;
		case KEY_H:
		     return 'h';
		     break;
		case KEY_I:
		     return 'i';
		     break;
		case KEY_J:
		     return 'j';
		     break;
		case KEY_K:
		     return 'k';
		     break;
		case KEY_L:
		     return 'l';
		     break;
		case KEY_M:
		     return 'm';
		     break;
		case KEY_N:
		     return 'n';
		     break;
		case KEY_O:
		     return 'o';
		     break;
		case KEY_P:
		     return 'p';
		     break;
		case KEY_Q:
		     return 'q';
		     break;
		case KEY_R:
		     return 'r';
		     break;
		case KEY_S:
		     return 's';
		     break;
		case KEY_T:
		     return 't';
		     break;
		case KEY_U:
		     return 'u';
		     break;	
		case KEY_V:
		     return 'v';
		     break;
		case KEY_W:
		     return 'w';
		     break;
		case KEY_X:
		     return 'x';
		     break;
		case KEY_Y:
		     return 'y';
		     break;
		case KEY_Z:
		     return 'z';
		     break;
		return (char)0;

	}
}
void print_string(char *str,unsigned char color)
{
	int index = 0;
	while (str[index])
	{
		terminal_buffer[vga_index] = (unsigned short)str[index]|(unsigned short)color<<8;
		index++;
		vga_index++;
	}
}
void print_character(char c, unsigned char color)
{
	terminal_buffer[vga_index] = (unsigned short)c|(unsigned short)color<<8;
	vga_index++;
}
void next_line(void)
{
	lineNumber+=1;
	vga_index=80*lineNumber;
}

int keyboardInput(int port)
{
	int ret;
	asm volatile("in %1, %0" : "=a"(ret): "d"(port));
}
void keyboardOutput(int port, int data)
{
	asm volatile("out %0, %1":"=a"(data):"d"(port));
}
char get_input_code()
{
	char ch =0;
	while((ch = keyboardInput(KEYBOARD_PORT)) != 0)
	{
		if(ch > 0)
			return ch;
	}
	return ch;
}
void wait_for_io(int timer)
{
	while(1)
	{
	asm volatile("nop");
	timer--;
	if(timer<=0)
		break;
	}
}
void test_input(void)
{
	char ch = 0;
	char keycode = 0;
	do
	{
		keycode = get_input_code();
		if(keycode == KEY_ENTER)
		{	
			next_line();
		}
		else
		{
			ch = get_ascii_char(keycode);
			

			print_character(ch,GREEN);
		}
		wait_for_io(0x03FFFFFF);
	}while(ch>-1);

}
void clear_screen(void)
{
	int index = 0;
	while(index<80*25*20)
	{
		terminal_buffer[index]= (unsigned short)' '|(unsigned short)BLACK<<8;
		index+=1;
	}
}
void main(void)
{
	terminal_buffer = (unsigned short *) VGA_ADDRESS;
	lineNumber = 0;
	vga_index=0;
	clear_screen();
	print_string("test kernel start",YELLOW);
	next_line();
	print_string("welcome to Jackernel V 1.0",GREEN);
	next_line();
	print_string("test keyboard read",WHITE_COLOR);
	test_input();
	next_line();
	print_string("test kernel end",RED);
//	clear_screen();
	return;
}
