#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int check_arg(int argc, char **argv);

int main(int argc, char **argv)
{
    if(check_arg(argc, argv)==-1)
    {
        printf("rip\n");
    }
    else
    {
        printf("nice\n");
    }

}

int check_arg(int argc, char **argv)
{
    int opt = -1;
    while ((opt = getopt(argc, argv, "i:t:u:s:p:n:x:bdh")) != -1) { //saves string in optarg
        switch(opt) {
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