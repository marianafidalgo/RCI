#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int check_arg(int argc, char **argv);
int user_interface();

int main(int argc, char **argv)
{
    if(check_arg(argc, argv)==-1)
    {
            return -1;
    }
    else
    {
        printf("proceed\n");
    }
      if(user_interface()==-1)
    {
        printf("wrong command \n");
    }
    else
    {
        printf("the end\n");
    }
}

int check_arg(int argc, char **argv)
{
    int opt = -1;
    while ((opt = getopt(argc, argv, "i:t:u:s:p:n:x:bdh")) != -1) { //saves string in optarg
        switch(opt)
        {
            case 'i':
            //optarg = ipadrr
            printf("teste -i:%s\n", optarg);
                break;
            case 't':
            //optarg = tport
            printf("teste -t:%s\n", optarg);
                break;
            case 'u':
            //optarg = uport
            printf("teste -u:%s\n", optarg);
                break;
            case 's':
            //optarg = rsaddr:rsport
            printf("teste -s:%s\n", optarg);
                break;
            case 'p':
            //optarg = tcpsessions
            printf("teste -p:%s\n", optarg);
                break;
            case 'n':
            //optarg = bestpops
            printf("teste -n:%s\n", optarg);
                break;
            case 'x':
            //optarg = tsecs
            printf("teste -x:%s\n", optarg);
                break;
            case 'b':
            //doesn't show stream data
            printf("teste -b:%s\n", optarg);
                break;
            case 'd':
            //detailed info
            printf("teste -d:%s\n", optarg);
                break;
            case 'h':
            //command line sinopsis
            printf("teste -h:%s\n", optarg);
                break;
            default:
                return -1;
        }
    }
    return 1;
}

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