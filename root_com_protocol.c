#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

int root_communication_protocol(char *out, char *input, int interface, int hex)
{
    char command[15];
    char Tport[15];
    char Uport[15];
    char StreamID[64];
    char StreamNAME[64];
    char StreamADDR[15];
    char StreamPORT[10];
    char Ipaddr[15];


    printf("input: %s", input);
    strcpy(command, input);
    strtok(command, " \n");
    printf("command %s\n", command);
    printf("inputss: %s\n", input);
    if (strcmp (command, "WHOISROOT") == 0) //who is root
    {
        //WHOISROOT tres:1.1.1.1:59000 193.136.138.142:59000
        sscanf (input, "%s %[^:]:%[^:]:%s %[^:]:%s\n", command, StreamNAME, StreamID, StreamPORT, Ipaddr, Uport);
        //error
        udpc(out, input, interface, hex);

    }
    else  if (strcmp (command, "REMOVE") == 0)
    {
        sscanf (input, "%s %[^:]:%[^:]:%s \n", command, StreamNAME, StreamID, StreamPORT);
        udpc(out, input, interface, hex);
    }
    else if (strcasecmp (command, "DUMP") == 0)
    {
        udpc(out, input, interface, hex);
    }
    else
    {
        printf("ERROR: Not known command\n");
        return -1;
    }

}