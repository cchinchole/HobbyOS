#include  <drivers/keyboard.h>
#include  <drivers/vga.h>
#include  <drivers/pit.h>
#include  <libc/stdio.h>
#include  <libc/string.h>
#include  <arch/i686/io.h>
#include  <arch/i686/isr.h>
#include  <kernel/shell.h>

#define KEYBOARD_DATA 0x60
#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SC_MAX 57

static char key_buffer[256];



char scancodes[]={
	0,
	0,	//esc
	'1','2','3','4','5','6','7','8','9','0','-','=',
	8,	//backspace
	'\t', //tab
	'q','w','e','r','t','y','u','i','o','p','[',']',
	'\n', //enter
	0, //ctrl
	'a','s','d','f','g','h','j','k','l',';','\'','`',
	0, //left shift
	'\\',
	'z','x','c','v','b','n','m',',','.','/',
	0, //right shift
	'*', //numpad
	0, //alt
	' ',
	0, //caps lock
	0,0,0,0,0,0,0,0,0,0, //f1 - f10
	0, //num lock
	0, //scroll lock
	0, //home
	0,
	0, //page up
	'-', //numpad
	0, 0,
	0, //(r)
	'+', //numpad
	0, //end
	0,
	0, //page down
	0, //ins
	0, //del
	0, //sys rq
	0,
	0,0, //f11, f12
	0,
	0,0,0, //f13 - 15
	0,0,0,0,0,0,0,0,0, //f16 - f24
	0,0,0,0,0,0,0,0
};

char scancodes_shifted[]={
	0,
	0,	//ESC
	'!','@','#','$','%','^','&','*','(',')','_','+',
	8,	//BACKSPACE
	'\t', //TAB
	'Q','W','E','R','T','Y','U','I','O','P','{','}',
	'\n', //ENTER
	0, //CTRL
	'A','S','D','F','G','H','J','K','L',':','"','~',
	0, //LEFT SHIFT
	'|',
	'Z','X','C','V','B','N','M','<','>','?',
	0, //RIGHT SHIFT
	'*', //NUMPAD
	0, //ALT
	' ',
	0, //CAPS LOCK
	0,0,0,0,0,0,0,0,0,0, //F1 - F10
	0, //NUM LOCK
	0, //SCROLL LOCK
	0, //HOME
	0,
	0, //PAGE UP
	'-', //NUMPAD
	0, 0,
	0, //(R)
	'+', //NUMPAD
	0, //END
	0,
	0, //PAGE DOWN
	0, //INS
	0, //DEL
	0, //SYS RQ
	0,
	0,0, //F11, F12
	0,
	0,0,0, //F13 - 15
	0,0,0,0,0,0,0,0,0, //F16 - F24
	0,0,0,0,0,0,0,0
};

typedef struct keystate {
    int escaped, shift, ctrl, alt, caps;
};


void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}
struct keystate kbd_state = {0,0,0,0,0};

void keyboard_callback(registers_t* r)
{
    uint8_t scancode = pbin(KEYBOARD_DATA);

   switch(scancode)
   {
        case 0x36:
        case 0x2a:
            kbd_state.shift = 1;
            break;
            
        case 0x36+0x80:
        case 0x2a+0x80:
            kbd_state.shift = 0;
            break;
            
        case 0x1D:
            kbd_state.ctrl = 1;
            break;
            
        case 0x1D+0x80:
            kbd_state.ctrl = 0;
            break;
            
        case 0x38:
            kbd_state.alt = 1;
            break;
            
        case 0x38+0x80:
            kbd_state.alt = 0;
            break;
        case BACKSPACE:  
            backspace(key_buffer);
            put_char('\b');
        break;
        case ENTER:
            put_char('\n');
            user_input(key_buffer);
            key_buffer[0] = '\0';
        break;
        case 0x3E:
            put_char('>');
            append(key_buffer, '>');
            key_buffer[0] = '\0';
        break;
            
        default:
            if(scancode < 0x80){
                if(kbd_state.shift == 0)
                {
                    append(key_buffer, scancodes[(int)scancode]);
                    put_char(scancodes[(int)scancode]);
                }
                else
                {
                    append(key_buffer, scancodes_shifted[(int)scancode]);
                    put_char(scancodes_shifted[(int)scancode]);
                }
            }
        break;
   }


    REG_PARAM(r);
}
   

void install_keyboard()
{
    klog(LOG_INFO, "Initializing Keyboard", 1);
    if(register_interrupt_handler(IRQ1, keyboard_callback))
		klog(LOG_SUCCESS, "Keyboard callback setup!", 1);
	else
		klog(LOG_ERROR, "Keyboard callback failed to setup!", 1);
	
}