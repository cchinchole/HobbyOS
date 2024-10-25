#include <kernel/io.h>
#include <kernel/interrupts/isr.h>
#include <drivers/keyboard.h>
#include <drivers/pit.h>
#include <drivers/vga.h>
#include <kernel/shell.h>
#include <libc/stdio.h>
#include <libc/string.h>

#define MAX_ROWS 25
#define MAX_COLS 80
#define KEYBOARD_DATA 0x60
#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SC_MAX 57

int cmdCounter = 0;
int curIndex = 0;
static char lastCommand[256][MAX_COLS];
static char key_buffer[MAX_ROWS][MAX_COLS];
char line[25];

char scancodes[] = {
    0,
    0, // esc
    '1',  '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
    8,    // backspace
    '\t', // tab
    'q',  'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
    '\n', // enter
    0,    // ctrl
    'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, // left shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,   // right shift
    '*', // numpad
    0,   // alt
    ' ',
    0,                                               // caps lock
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0, // f1 - f10
    0,                                               // num lock
    0,                                               // scroll lock
    0,                                               // home
    0,
    0,   // page up
    '-', // numpad
    0,    0,
    0,   //(r)
    '+', // numpad
    0,   // end
    0,
    0,                                          // page down
    0,                                          // ins
    0,                                          // del
    0,                                          // sys rq
    0,    0,   0,                               // f11, f12
    0,    0,   0,   0,                          // f13 - 15
    0,    0,   0,   0,   0,   0,   0,   0,   0, // f16 - f24
    0,    0,   0,   0,   0,   0,   0,   0};

char scancodes_caps[] = {
    0,
    0, // ESC
    '1',  '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
    8,    // BACKSPACE
    '\t', // TAB
    'Q',  'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[',  ']',
    '\n', // ENTER
    0,    // CTRL
    'A',  'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
    0, // LEFT SHIFT
    '|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',
    0,   // right shift
    '*', // numpad
    0,   // alt
    ' ',
    0,                                               // caps lock
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0, // f1 - f10
    0,                                               // num lock
    0,                                               // scroll lock
    0,                                               // home
    0,
    0,   // page up
    '-', // numpad
    0,    0,
    0,   //(r)
    '+', // numpad
    0,   // end
    0,
    0,                                          // page down
    0,                                          // ins
    0,                                          // del
    0,                                          // sys rq
    0,    0,   0,                               // f11, f12
    0,    0,   0,   0,                          // f13 - 15
    0,    0,   0,   0,   0,   0,   0,   0,   0, // f16 - f24
    0,    0,   0,   0,   0,   0,   0,   0};

char scancodes_shifted[] = {
    0,
    0, // ESC
    '!',  '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    8,    // BACKSPACE
    '\t', // TAB
    'Q',  'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
    '\n', // ENTER
    0,    // CTRL
    'A',  'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, // LEFT SHIFT
    '|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0,   // RIGHT SHIFT
    '*', // NUMPAD
    0,   // ALT
    ' ',
    0,                                               // CAPS LOCK
    0,    0,   0,   0,   0,   0,   0,   0,   0,   0, // F1 - F10
    0,                                               // NUM LOCK
    0,                                               // SCROLL LOCK
    0,                                               // HOME
    0,
    0,   // PAGE UP
    '-', // NUMPAD
    0,    0,
    0,   //(R)
    '+', // NUMPAD
    0,   // END
    0,
    0,                                          // PAGE DOWN
    0,                                          // INS
    0,                                          // DEL
    0,                                          // SYS RQ
    0,    0,   0,                               // F11, F12
    0,    0,   0,   0,                          // F13 - 15
    0,    0,   0,   0,   0,   0,   0,   0,   0, // F16 - F24
    0,    0,   0,   0,   0,   0,   0,   0};

typedef struct keystate {
  int escaped, shift, ctrl, alt, caps;
};

void backspace(char s[]) {
  int len = strlen(s);
  s[len - 1] = '\0';
}
struct keystate kbd_state = {0, 0, 0, 0, 0};

void enterCommand(char key_buff[]) {
  strcpy(lastCommand[cmdCounter], key_buff);
  cmdCounter++;
  curIndex = cmdCounter;
}

void keyboard_callback(registers_t *r) {
  uint8_t scancode = pbin(KEYBOARD_DATA);

  switch (scancode) {
  case 0x36:
  case 0x2a:
    kbd_state.shift = 1;
    break;

  case 0x36 + 0x80:
  case 0x2a + 0x80:
    kbd_state.shift = 0;
    break;

  case 0x1D:
    kbd_state.ctrl = 1;
    break;

  case 0x1D + 0x80:
    kbd_state.ctrl = 0;
    break;

  case 0x38:
    kbd_state.alt = 1;
    break;

  case 0x38 + 0x80:
    kbd_state.alt = 0;
    break;
  case 0x3a:
    kbd_state.caps = !kbd_state.caps;
    break;
  case 0x48:
    /* Going upward */
    curIndex--;
    if (curIndex < 0)
      curIndex = 0;

    for (int i = 0; i < MAX_COLS; i++) {
      key_buffer[get_offset_row(get_cursor_offset())][i] = '\0';
    }

    for (int i = 2; i < MAX_COLS; i++) {
      put_char_at(' ', get_offset_row(get_cursor_offset()), i, WHITE_ON_BLACK);
    }
    set_cursor_offset(get_offset(get_offset_row(get_cursor_offset()) - 1, 2));
    memcpy(key_buffer[get_offset_row(get_cursor_offset())],
           lastCommand[curIndex], strlen(lastCommand[curIndex]));
    kprintf("%s", lastCommand[curIndex]);
    break;
  case 0x50:
    /* Going downward */
    curIndex++;

    /* Check if we hit the command counter limit, if so then we will leave the option blank */
    if (curIndex > cmdCounter) {
      curIndex = cmdCounter - 1;
      for (int i = 0; i < MAX_COLS; i++) {
        key_buffer[get_offset_row(get_cursor_offset())][i] = '\0';
      }
      for (int i = 2; i < MAX_COLS; i++) {
        put_char_at('\0', get_offset_row(get_cursor_offset()), i,
                    WHITE_ON_BLACK);
      }

      set_cursor_offset(get_offset(get_offset_row(get_cursor_offset()) - 1, 2));
      break;
    } else {
      /* Here we have not hit the limit */
      for (int i = 0; i < MAX_COLS; i++) {
        key_buffer[get_offset_row(get_cursor_offset())][i] = '\0';
      }
      for (int i = 2; i < MAX_COLS; i++) {
        put_char_at('\0', get_offset_row(get_cursor_offset()), i,
                    WHITE_ON_BLACK);
      }

      set_cursor_offset(get_offset(get_offset_row(get_cursor_offset()) - 1, 2));
      memcpy(key_buffer[get_offset_row(get_cursor_offset())],
             lastCommand[curIndex], strlen(lastCommand[curIndex]));
      kprintf("%s", lastCommand[curIndex]);
    }
    break;
    case 0x4B:
        /* TODO: This fix works but its pretty messy, deleting a character in the middle of a string will still leave a space there instead of concatting the string. */
        set_cursor_offset( get_cursor_offset() -1 );
        break;
  case BACKSPACE:
    /* TODO: wtf, just change this to check if we will backspace against the offset of ' >' not check characters like this */
    if (get_char_at(get_offset_row(get_cursor_offset()),
                    get_offset_col(get_cursor_offset()) - 1) != '>' &&
        get_char_at(get_offset_row(get_cursor_offset()) - 1, MAX_COLS - 1) !=
            '\0') {
      backspace(key_buffer[get_offset_row(get_cursor_offset())]);
      put_char('\b');
    }

    break;
  case ENTER:
    char tmphold[256];
    int curOff = get_offset_row(get_cursor_offset());
    strcpy(tmphold, key_buffer[curOff]);
    if (user_input(key_buffer[get_offset_row(get_cursor_offset())]))
      enterCommand(tmphold);
    key_buffer[get_offset_row(get_cursor_offset())][0] = '\0';
    key_buffer[get_offset_row(get_cursor_offset())][1] = '>';
    break;
  case 0x3E:
    put_char('>');
    append(key_buffer[get_offset_row(get_cursor_offset())], '>');
    key_buffer[get_offset_row(get_cursor_offset())][0] = '\0';
    break;

  default:
    if (scancode < 0x80) {
      if (kbd_state.shift == 0 && kbd_state.caps == 0) {
        append(key_buffer[get_offset_row(get_cursor_offset())],
               scancodes[(int)scancode]);
        put_char(scancodes[(int)scancode]);
      } else if (kbd_state.caps == 1) {
        append(key_buffer[get_offset_row(get_cursor_offset())],
               scancodes_caps[(int)scancode]);
        put_char(scancodes_caps[(int)scancode]);
      } else if (kbd_state.shift == 1) {
        append(key_buffer[get_offset_row(get_cursor_offset())],
               scancodes_shifted[(int)scancode]);
        put_char(scancodes_shifted[(int)scancode]);
      }
    }
    break;
  }

  REG_PARAM(r);
}

void install_keyboard() {
  klog(LOG_INFO, "Initializing Keyboard", 1);
  if (register_interrupt_handler(IRQ1, keyboard_callback))
    klog(LOG_SUCCESS, "Keyboard callback setup!", 1);
  else
    klog(LOG_ERROR, "Keyboard callback failed to setup!", 1);
}
