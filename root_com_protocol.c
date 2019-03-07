#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

int root_communication_protocol()
{
    char command[100];
    char input[100];
    while(1)
    {
        fgets(input, 100, stdin);
        printf("input: %s", input);
       /* sscanf (input, "%s %s %s:%s\n", command, streamID, ipaddr, uport);

        if (strcasecmp (command, "WHOISROOT") == 0) //who is root
        {
            printf("WHO IS ROOT: %s %s:%s\n", streamID, ipaddr, uport);
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
        else
        {
            printf("ERROR: Not known command.\n");
            return -1;
        }*/

    }
}