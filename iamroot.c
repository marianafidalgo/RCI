
#include "initvar.h"

int check_arg(int argc, char **argv);
int tcpc_init(char *ip, char *port);
int tcps_init(char *command, char *PORT);
int udps_init(char *ipaddr, char *uport);
int udps_SA (char *streamID, char *ipaddr, char *tport, int fd);
int user_interface(char *out, char *command);
int udpc_RS(char *out, char *command, char *rsaddr);
int udpc_POP (char *ipSA, char *portSA, char *out);
int root_communication_protocol(char* out, char *input, char* rsaddr);


int main(int argc, char **argv)
{
    struct timeval * t1;
    struct timeval  t2;
    int d = 0, maxfd = 0, socketstate = 0;
    FILE *fp = NULL;
    fd_set fd_socket;
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

    printf("fdUp = %d\n", fdUP);

    printf("proceed\n");

    /*open file and write args*/

    fp = fopen(streamID,"w"); /*name id!! é o que queremos encontrar*/

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

       /* t1 = NULL;
        t2.tv_usec = 0;
        t2.tv_sec = 5;
        t1 = &t2;*/

        socketstate = select(maxfd+1, &fd_socket, (fd_set*) NULL, (fd_set*) NULL, (struct timeval *) NULL);

        if(socketstate < 0)
            printf("Erro socketstate\n"); //erro
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
                //ver se URROOT se sim fazer read
                /*fica à espera de receber welcome*/
               /* n= write(fdUP, streamid, 128);
                if(n==-1)/*error*/
                   // exit(1);

                /*fica à espera de receber welcome*/
               /* n= read(fd, welcome, 128);
                if(n==-1)/*error*/
                   // exit(1);

                /*escreve o que recebeu no terminal*/
                /*write(1, welcome, n);

                strcpy(check, "WE ");
                strcat(check, streamid);
                    char welcome[128];
                    int n= read(fdUP, welcome, 128);
                    if(n==-1)/*error
                        exit(1);

                    printf("%s", welcome);*/
            }
            else if(fdDOWN != -1 && FD_ISSET(fdDOWN, &fd_socket))
            {
                //printf("receive down\n");
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
    int opt = -1;
    char arg[64];
    char *array[3];
    char input[128];
    char command[128];
    char out [128];
    char output[128];
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
            tcpc_init(streamADDR, streamPORT);
            fdDOWN = tcps_init(command, tport);
            fdSA = udps_init(ipaddr, uport);

        }
        else if( strcmp(token, "ROOTIS")==0)
        {
            sscanf (out, "%s %[^:]:%[^:]:%s %[^:]:%s\n", command, streamNAME, streamADDR, streamPORT, ipaddr, uport);

            udpc_POP(ipaddr, uport, out); //send PQ to SA

            printf("command %s\n", command);

            //fdSA = udps_init(ipaddr, tport, streamPORT, streamID);
            //printf("ponto acesso: %s %s", )
            //fdDOWN = tcpc_init(command, streamNAME, streamADDR, streamPORT, ipaddr, tport);

            /*while(nao adere)
            {
                tcps_close(command, tport);
                fdDOWN = tcpc_init(command, streamNAME, streamADDR, streamPORT, ipaddr, tport);
            }

            //confirmação - Sim
            fdUP = tcpc_init(command, streamNAME, streamADDR, streamPORT, ipaddr, tport);
            //enviar novo PA
            fdUP
            return*/
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
        udpc_RS(out, command, /*display, /*int hex,*/ streamADDR);
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