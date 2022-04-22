
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
		case KEY_Q:
		     return 'q';
		     break;
		case KEY_W:
		     return 'w';
		     break;
		case KEY_E:
		     return 'e';
		     break;

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
			char* character = ch;
			print_string(character,GREEN);
		}
		wait_for_io(0x02FFFFFF);
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
