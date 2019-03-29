#include "initvar.h"


int udpc_RS(char *out, char *command, char *rsaddr);
int adesao ();
int check_arg(int argc, char **argv);
int tcpc_init(char *ip, char *port);
int tcpc_new(char *ip, char *port);
int tcpc_Receive(int fdUP);
int tcps_init(char *PORT);
int tcps_WE (int fdDOWN, struct ofilho Filho[]);
int tcps_POPQUERY (int fdD);
int udps_init(char *ipaddr, char *uport);
int udps_SA (char *streamID, char *ipaddr, char *tport, int fd);
int user_interface(char *out, char *command);
int udpc_POP (char *ipADDR, char *tPORT, char *uPORT);
int udpc_PA (char *ipADDR, char *uPORT, char *tPORT);


int main(int argc, char **argv)
{
    struct timeval * t1;
    struct timeval  t2;
    int d = 0, maxfd = 0, socketstate = 0, fdsSA = -1;
    fd_set fd_socket;
    char input[128];
    char out[128], command[128];

    d = check_arg(argc, argv);

    struct ofilho Filho[tcpsessions];
    Filho[0].fd = -1;

    printf("size %d\n", tcpsessions);

    printf("size %lu\n", sizeof(Filho));

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

    printf("proceed\n");

    t1 = NULL;
    t2.tv_usec = 0;
    t2.tv_sec = 5;
    t1 = &t2;

    while(1)
    {
        //inicializar máscara
        FD_ZERO(&fd_socket);

        FD_SET(0,&fd_socket);
        maxfd = 0;

        if(fdDOWN != -1)
        {
            FD_SET(fdDOWN , &fd_socket);
            if(maxfd < fdDOWN)
                maxfd = fdDOWN;
        }
        if(fdUP != -1)
        {
            FD_SET(fdUP , &fd_socket);
            if(maxfd < fdUP)
                maxfd = fdUP;
        }
        if(fdSA != -1)
        {
            FD_SET(fdSA, &fd_socket);
            if(maxfd < fdSA)
                maxfd = fdSA;
        }

        socketstate = select(maxfd+1, &fd_socket, (fd_set*) NULL, (fd_set*) NULL, (struct timeval *) &t2);

        if(socketstate < 0)
            printf("Erro socketstate\n"); //erro
            // De 5 em 5 segundos o socketstate é igual a 0 por causa do timeout
        else if(socketstate == 0)
        {
            t1 = NULL;
            t2.tv_usec = 0;
            t2.tv_sec = 5;
            t1 = &t2;
            //pop_query
            printf("filho %d\n", Filho[0].fd);
            if(Filho[0].fd != 0 && Filho[0].fd != -1)
                tcps_POPQUERY(Filho[0].fd);
        }
        if(socketstate > 0)
        {
            if(FD_ISSET(0, &fd_socket))
            {
                printf("escreveu no teclado");
                fgets(input, 128, stdin);
                user_interface(out, input);
            }
            else if(fdUP != -1 && FD_ISSET(fdUP, &fd_socket))
            {
                printf("receive up\n");
                //adesao();
                tcpc_Receive(fdUP);
            }
            else if(fdDOWN != -1 && FD_ISSET(fdDOWN, &fd_socket))
            {
                printf("receive down\n");
                tcps_WE(fdDOWN, Filho);
            }
            else if(fdSA != -1 && FD_ISSET(fdSA, &fd_socket))
            {
                printf("receive SA\n");
                udps_SA(streamID, ipaddr, tport, fdSA); //SA send to PQ
            }
        }
    }
}

int check_arg(int argc, char **argv)
{

    char input[128];
    char out [128];
    int opt = -1;
    char arg[64];
    char *array[3];
    int fd = -1, i = 0;

    for(i = 1; i < argc; i++)
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
                sscanf (optarg, "%[^:]:%s \n", rsaddr, rsport);
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
        udpc_RS(out, input, rsaddr);
        exit(0);
    }
    else
    {
        adesao();
    }
    return 1;
}

int user_interface( char *out , char *command)
{
    char remove[128]="";

    printf("command: %s", command);
    if (strcasecmp (command, "streams\n") == 0)
    {
        strcpy(command, "DUMP\n");
        udpc_RS(out, command, streamADDR);
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
        //abandonar a arvore
        strcpy(remove, "REMOVE ");
        strcat(remove, streamID);
        strcat(remove, "\n");
        udpc_RS(out, remove, streamADDR);
        exit(0);
    }
    else
    {
        printf("sai interface\n");
        return -1;
    }
    return 1;
}
int adesao ()
{
    char input[128];
    char command[128];
    char out [128];
    char output[128];
    char ipADDR[15], uPORT[15], tPORT[15];
    int flag = -1;


    strcpy(input, "WHOISROOT ");
    strcat(input, streamID);
    strcat(input, " ");
    strcat(input, ipaddr);
    strcat(input, ":");
    strcat(input, uport);
    strcat(input, "\n");

    udpc_RS(out, input , rsaddr);

    strcpy(output, out);
    char *token = strtok(output, " ");

    if(strcmp(token, "URROOT")==0)
    {
        sscanf (out, "%s %[^:]:%[^:]:%s \n", command, streamNAME, streamADDR, streamPORT);
        tcpc_init(streamADDR, streamPORT); //data
        fdDOWN = tcps_init(tport);
        fdSA = udps_init(ipaddr, uport);
    }
    else if( strcmp(token, "ROOTIS")==0)
    {
        sscanf (out, "%s %[^:]:%[^:]:%s %[^:]:%s \n", command, streamNAME, streamADDR, streamPORT, ipADDR, uPORT);
        printf("addr %s, port %s\n", ipADDR, uPORT);
        udpc_POP(ipADDR, uPORT, tPORT); //send PQ to SA
        while(flag == -1)
        {
            printf("flag %d\n", flag);
            printf(" ipADDR %s uPORT %s\n", ipADDR, tPORT);
            printf("POP ipADDR %s tPORT %s\n", ipADDR, tPORT);
            flag = tcpc_new(ipADDR, tPORT);

            printf("flag %d\n", flag);
        }
        printf("fduppppp %d\n", fdUP);
        fdDOWN = tcps_init(tport);

    }
}