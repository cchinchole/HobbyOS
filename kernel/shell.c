#include  <kernel/shell.h>
#include  <drivers/vga.h>
#include  <drivers/pit.h>
#include  <libc/string.h>
#include  <libc/stdio.h>
#include  <libc/stddef.h>



struct Command {
        char* name;
        char* syntax; //Example -type= -example=
        void (*cbk_ptr)(int, char**);
};
struct Command echo;
struct Command timer;
struct Command clear;
struct Command halt;
struct Command testint;


const int commandSize = 5;
struct Command commands[5];

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

bool firstRun = 0;

void echoCallback(int args, char* string[])
{
    for(int y = 0; y < args; y++)
        kprintf("%s ",string[y]);
}

void timerCallback(int args, char* string[])
{
    switch(args)
    {
        case 1:
            if(strcmp(string[0], "elapsed")==0)
                kprintf("Elapsed tick: %d", get_elapsed_tick());
            else if(strcmp(string[0], "tick")==0)
                kprintf("Current tick: %d\n", give_tick());
            else if(strcmp(string[0], "disable")==0){
                kprintf("Disabling timer\n");
                uninstall_timer();
            }
            else
                kprintf("syntax error1");
            break;
        case 2:
            if(strcmp(string[0], "enable")==0){
                int a = atooi(string[1]);
                if(a == 0)
                {
                  kprintf("Enabling timer\n");
                  install_inf_timer(false, 10000);
                }
                else
                {
                    install_tick_timer(10000, a);
                }
            }
            else
                kprintf("syntax error2");
            break;
        default:
            kprintf("syntax error3");
            break;
    }
}

void haltCallback(int args, char* string[])
{
    kprintf("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
}

void testintCallback(int args, char* string[])
{
    asm volatile("int $0xb");
}

void clearScreenCallback(int args, char* string[])
{
    clear_screen();
}




void buildCommands()
{
echo.name = "echo";
echo.syntax = "echo <string>";
echo.cbk_ptr = echoCallback;

timer.name = "timer";
timer.syntax = "timer <enable, disable, tick, elasped> <duration; 0 for inf>";
timer.cbk_ptr = timerCallback;

clear.name = "clear";
clear.syntax = "timer <enable, disable, tick, elasped> <duration; 0 for inf>";
clear.cbk_ptr = clearScreenCallback;

halt.name = "halt";
halt.syntax = "timer <enable, disable, tick, elasped> <duration; 0 for inf>";
halt.cbk_ptr = haltCallback;

testint.name = "testint";
testint.syntax = "timer <enable, disable, tick, elasped> <duration; 0 for inf>";
testint.cbk_ptr = testintCallback;


commands[0] = echo;
commands[1] = timer;
commands[2] = clear;
commands[3] = halt;
commands[4] = testint;
}


int countSpaces(char string[])
{
    int spaces = 0;
    for(int i = 0; i < strlen(string) ; i++)
    {
        if(string[i] == ' ')
            spaces++;
    }

    return spaces;
}

int user_input(char input[]) {
     put_char('\n');
     int spaces = countSpaces(input);
     char* end = &input[strlen(input)];
     split(input);
     char* cmd = input;
     if(firstRun == 0)
    {
        firstRun = 1;
        buildCommands();
    }

    int i = 0;
    int foundCM = 0;
    for(i = 0; i < commandSize; i++)
    {
        if(strcmp(commands[i].name, cmd)==0)
        {
        char* total_operans[spaces];

        
        total_operans[0] = iter(cmd, end);

        int x = 0;
        for(x = 1; x < spaces; x++){
            split(total_operans[x-1]);
            total_operans[x]= iter(total_operans[x-1], end);
        }
        (*commands[i].cbk_ptr)(spaces, total_operans);

            foundCM = 1;
            break;
        }
    }

    if(foundCM == 0)
    {
        klog(LOG_ERROR, "Unrecognized command: ", false);
        kprintf("%s", cmd);
    }

    kprintf("\n> ");
    return foundCM;

    /*

    else if(strcmp(cmd, "divide") == 0) {
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
    }*/
}
