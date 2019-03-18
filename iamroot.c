#include "funcs.h"


#define TPORT "58000"
#define UPORT "58000"
#define RSADDR "193.136.138.142"
#define RSPORT "59000"
#define TPORT "58000"
#define TCPSESSIONS 1
#define BESTPOPS 1
#define TSECS 5


char streamID[64]= "";
char streamNAME[64];
char streamADDR[15];
char streamPORT[10];

char ipaddr[15];
char sarg[15];
unsigned int tcpsessions = TCPSESSIONS;
unsigned int bestpops = BESTPOPS;
unsigned int tsecs = TSECS;
char tport[15]=TPORT;
char uport[15]=UPORT;

int display = 1;
int debug = 0;
int hex = 0;


int main(int argc, char **argv)
{

    int d =0;
    FILE *fp = NULL;
    char input[128];
    char out[128], command[128];

    d = check_arg(argc, argv);
    printf("d: %d\n", d);
    if(d == -1)
    {
            printf("Error: Check your args\n");
            return -1;
    }
    else if(d == 0)
    {
            printf("Exit\n");
            exit(0);
    }
    else
    {
        printf("proceed\n");
    }

    /*open file and write args*/

    fp = fopen(streamID,"w"); /*name id!! é o que queremos encontra*/

    if (fp == NULL)
    {
        printf("File was not open sucessfully !!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "streamID = %s\n", streamID);
    fprintf(fp, "streamNAME = %s\n", streamNAME);
    fprintf(fp, "streamADDR = %s\n", streamADDR);
    fprintf(fp, "streamPORT = %s\n", streamPORT);
    fprintf(fp, "ipaddr = %s\n", ipaddr);
    fprintf(fp, "tcpsessions = %d\n", tcpsessions);
    fprintf(fp, "bestpops = %d\n", bestpops);
    fprintf(fp, "tsecs = %d\n", tsecs);
    fprintf(fp, "tport = %s\n", tport);
    fprintf(fp, "uport = %s\n", uport);

    fclose(fp);

    while(1)
    {

        fgets(input, 128, stdin);

        printf("main input: %s\n", input);
        if( user_interface(out, input)==-1)
        {
            root_communication_protocol(out, input, display, hex);
            char *token = strtok(out, " ");
            printf("tok %s\n", token);

            if(strcmp(token, "URROOT") == 0)
            {
                sscanf (out, "%s %[^:]:%[^:]:%s \n", command, streamNAME, streamADDR, streamPORT);
                tcpc(command, streamNAME, streamADDR, streamPORT);
                /*refresh*/
            }
            else if( strcmp(token, "ROOTIS") == 0)
            {
                sscanf (out, "%s %[^:]:%[^:]:%s %[^:]:%s\n", command, streamNAME, streamADDR, streamPORT, ipaddr, uport);
                tcpc(command, streamNAME, streamADDR, streamPORT);
            }
        }
    }

}

int check_arg(int argc, char **argv)
{
    int opt = -1;
    char arg[64];
    char *array[3];
    char input[128];
    char command[128];
    char out [128];
    char output[128];

    for(int i = 1; i < argc; i++)
    {
        strcpy(arg,argv[i]);
        if(arg[0] != '-')
        {
            strcpy(streamID, argv[i]);
            strcpy(arg, streamID);

            char *tok = strtok(arg, ":");
            i = 0;
	        while(tok != NULL)
	        {
                if( i > 2)
                {
                    return -1;
                }
		        array[i++] = tok;
		        tok = strtok(NULL, ":\n");
	        }

            strcpy(streamNAME, array[0]);
            strcpy(streamADDR, array[1]);
            strcpy(streamPORT, array[2]);

            break;
        }
        else
        {
            i++;
        }
    }

    while ((opt = getopt(argc, argv, "i:t:u:s:p:n:x:bdh")) != -1) { //saves string in optarg
        switch(opt)
        {
            case 'i':
                strcpy(ipaddr, optarg);
                break;
            case 't':
                strcpy(tport, optarg);
                break;
            case 'u':
                strcpy(uport, optarg);
                break;
            case 's':
                strcpy(sarg, optarg);
                break;
            case 'p':
                tcpsessions = atoi(optarg);
                break;
            case 'n':
                bestpops = atoi(optarg);
                break;
            case 'x':
                tsecs = atoi(optarg);
                break;
            case 'b':
            //doesn't show stream data
                display = 0;
                break;
            case 'd':
            //detailed info
                debug = 1;
                break;
            case 'h':
            //command line sinopsis
                printf("Usage: ./iamroot [<streamID>] [-i <ipaddr>] [-t <tport>] [-u <uport>]\n[-s <rsaddr>[:<rsport>]]\n[-p <tcpsessions>]\n[-n <bestpops>] [-x <tsecs>]\n[-b] [-d] [-h]\n");
                printf("Defaul: -t 58000 -u 58000 -s 193.136.138.142:59000 -p 1 -n 1 -x 5\n");
                exit(0);
            default:
                return -1;
        }
    }

    if(strcmp(streamID, "") == 0)
    {
        strcpy(input, "DUMP\n");
        udpc(out, input, display, hex);
        exit(0);
    }
    else
    {
        strcpy(input, "WHOISROOT ");
        strcat(input, streamID);
        strcat(input, " ");
        strcat(input, ipaddr);
        strcat(input, ":");
        strcat(input, uport);
        strcat(input, "\n");

        udpc(out, input , display, hex);
        char *token = strtok(out, " ");
        printf("tok %s\n", token);

        if(strcmp(token, "URROOT")==0)
        {
            sscanf (out, "%s %[^:]:%[^:]:%s \n", command, streamNAME, streamADDR, streamPORT);
            tcpc(command, streamNAME, streamADDR, streamPORT);
           /* tcps(tport);
            udps(uport);*/
        }
        else if( strcmp(token, "ROOTIS")==0)
        {
            sscanf (out, "%s %[^:]:%[^:]:%s %[^:]:%s\n", command, streamNAME, streamADDR, streamPORT, ipaddr, uport);
        }
    }

    return 1;
}

int user_interface( char *out , char *command)
{
    printf("command: %s", command);
    if (strcasecmp (command, "streams\n") == 0)
    {
        strcpy(command, "DUMP\n");
        udpc(out, command, display, hex);
    }
    else  if (strcasecmp (command, "status\n") == 0)
    {
        //
    }
    else if (strcasecmp (command, "display on\n") == 0)
    {
        display = 1;
    }
    else if (strcasecmp (command, "display off\n") == 0)
    {
        display = 0;
    }
    else if (strcasecmp (command, "format ascii\n") == 0)
    {
        hex = 0;
    }
    else if (strcasecmp (command, "format hex\n") == 0)
    {
        hex = 1;
    }
    else if (strcasecmp (command, "debug on\n") == 0)
    {
        debug = 1;
    }
    else if (strcasecmp (command, "debug off\n") == 0)
    {
        debug = 0;
    }
    else if (strcasecmp (command, "tree\n") == 0)
    {
        //
    }
    else if (strcasecmp (command, "exit\n") == 0)
    {
        printf("exit: %s\n", command);
        //abandonar a arvore
        exit(0);
    }
    else
    {
        printf("sai interface\n");
        return -1;
    }
    return 1;
}