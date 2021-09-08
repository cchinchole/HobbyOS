#include  <kernel/shell.h>
#include  <drivers/vga.h>
#include  <drivers/pit.h>
#include  <libc/string.h>
#include  <libc/stdio.h>

void split(char* tmp){
	int len = strlen(tmp);
	
	int i;
	for(i = 0; i < len; i++)
		if((tmp[i] == '\t') || (tmp[i] == ' ') || (tmp[i] == '\n'))
			tmp[i] = 0;
}

char* iter(char* str, char* end){
	str += strlen(str);
	while(str < end){
		str++;
		if(*str != 0)
			return str;
	}

	return NULL;
}

int atooi(char* str) 
{ 
    // Initialize result 
    int res = 0; 
  
    // Iterate through all characters 
    // of input string and update result 
    for (int i = 0; str[i] 
                    != '\0'; 
         ++i) 
        res = res * 10 + str[i] - '0'; 
  
    // return result. 
    return res; 
} 

void user_input(char input[]) {
     char* end = &input[strlen(input)];
     split(input);
     char* cmd = input;

    if (strcmp(cmd, "HLT") == 0) {
        kprintf("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    else if(strcmp(cmd, "ENTIMER") == 0) {
        kprintf("Enabling timer\n");
        install_timer(10000);
    }
    else if(strcmp(cmd, "HLTIMER") == 0) {
        kprintf("Disabling timer\n");
        uninstall_timer();
    }
    else if(strcmp(cmd, "TESTINT") == 0) {
        char* operans = iter(cmd, end);
		if(operans == NULL){
			kprintf("\nSpecify an interrupt number");
        }
        else
        {
            //int operand = atooi(operans);
            //asm("int $0x" : : "a" (operand));
        }

        asm volatile("int $0x2");
    }
    else if(strcmp(cmd, "DIVIDE") == 0) {
        char* operans = iter(cmd, end);
		if(operans == NULL)
			kprintf("\nSpecify two numbers");
        else{
            int num1 = atooi(operans);
            split(operans);
            char* opertwo = iter(operans, end);
    		    if(opertwo == NULL){
                    kprintf("\nNO");
                }
                else
                 {
                        int num2 = atooi(opertwo);
                        kprintf("Result: %d", num1/num2);
                }
            }
    }
    else if(strcmp(cmd, "CLEAR") == 0) {
        clear_screen();
    }
    else {
        kprintf("Unrecognized command: %s", cmd);
    }
    kprintf("\n> ");
}
