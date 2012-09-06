#include <stdio.h>
#include <string.h>

#include "cread_line.h"
#include "command.h"

int readline_test(void)
{
    static char lastcommand[CB_SIZE] = {0};
    int len;

    while (1)
    {
        len = readline(PROMPT, lastcommand);
        if (len > 0)
        {
            //printf("len: %d\n", len);
            if (len > CB_SIZE)
            {
                printf("command line too large.\n");
                break;
            }
            //strcpy(lastcommand, console_buffer);
            //printf("[echo]: %s\n", lastcommand);
        }

        else if (len == 0)
        {
            //printf("nothing input.\n");
            // do nothing
        }

        if (len == -1)
        {
            puts("<INTERRUPT>");
        }
        else
        {
            run_command(lastcommand);
        }
     
    }
    return 0;
}


int main(void)
{
    readline_test();

    return 0;
}
