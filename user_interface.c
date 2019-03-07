#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

int user_interface()
{
    char command[15];
    while(1)
    {
        fgets(command, 15, stdin);
        if (strcasecmp (command, "streams\n") == 0)
        {
            printf("streams: %s\n", command);
        }
        else  if (strcasecmp (command, "status\n") == 0)
        {
            printf("status: %s\n", command);
        }
        else if (strcasecmp (command, "display on\n") == 0)
        {
            printf("display on: %s\n", command);
        }
        else if (strcasecmp (command, "display off\n") == 0)
        {
            printf("display off: %s\n", command);
        }
        else if (strcasecmp (command, "format ascii\n") == 0)
        {
            printf("format ascii: %s\n", command);
        }
        else if (strcasecmp (command, "format hex\n") == 0)
        {
            printf("format hex: %s\n", command);
        }
        else if (strcasecmp (command, "debug on\n") == 0)
        {
            printf("debug on: %s\n", command);
        }
        else if (strcasecmp (command, "debug off\n") == 0)
        {
            printf("debug off: %s\n", command);
        }
        else if (strcasecmp (command, "tree\n") == 0)
        {
            printf("tree: %s\n", command);
        }
        else if (strcasecmp (command, "exit\n") == 0)
        {
            printf("exit: %s\n", command);
            return 1;
        }
        else
        {
            printf("ERROR: Not known command.\n");
            return -1;
        }

    }
}