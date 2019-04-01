#include "defs.h"

int tcps_init(char *PORT)
{
	struct addrinfo hints, *res;
	int newfd, fd,addrlen,n,nread;
	struct sockaddr_in addr;


	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_STREAM; //TCP socket
	hints.ai_flags= AI_PASSIVE|AI_NUMERICSERV;

	n= getaddrinfo(NULL,PORT,&hints,&res);
	if(n!=0 && debug == 1)
	{
		printf("ERROR: getaddr tcps_init\n");
		exit(1);
	}

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1 && debug == 1)
	{
		printf("ERROR: socket tcps_init\n");
		exit(1);
	}

	n=bind(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1 && debug == 1)
	{
		printf("ERROR: bind tcps_init\n");
		exit(1);
	}

	n=listen(fd,128);
	if(n == -1&& debug == 1)
	{
		printf("ERROR: listen tcps_init\n");
		exit(1);
	}

	freeaddrinfo(res);

	return fd;
}

int tcps_WE (int fdDOWN)
{
	struct addrinfo hints, *res;
	int fd, addrlen,n,nread;
	struct sockaddr_in addr;
	char welcome[25], buffer[128],receive[128], filho[21], redirect[21];
	char *tok;

	if((fd=accept(fdDOWN,(struct sockaddr*)&addr,&addrlen))==-1)
	{
		printf("Nop :'( \n");
		return -1;
	}
	else if(tcpsessions < 1)
	{
		printf("no session available\n");
		//redirect
		strcpy(redirect, "RE ");
		strcat(redirect, Filho.IP);
		strcat(redirect, ":");
		strcat(redirect, Filho.PORT);
		strcat(redirect, "\n");
		n = write(fd, redirect, strlen(redirect));
		printf("%s no session \n", redirect);
		if(n==-1 && debug == 1)
		{
			printf("ERROR: write tcps_WE\n");
			exit(1);
		}
		return 0;
	}
	else
	{
		printf("tcpsess %d\n", tcpsessions);
		state = 1;
		strcpy(welcome, "WE ");
		strcat(welcome, streamID);
		strcat(welcome, "\n");

		/*envia welcome*/
		n = write(fd, welcome, strlen(welcome));
		if(n==-1 && debug == 1)
		{
			printf("ERROR: writewe tcps_WE\n");
			exit(1);
		}

		Filho.fd[pos] = fd;
		printf("fdddd %d\n ",Filho.fd[pos] );

		return fd;
	}
}

int tcps_Receive (int fd, char *out)
{
	int n, pa = 0;
	char recebe[128]="", buffer[128]="", mess[25]="", id[25]="", bp[25]="", ap[2]="";

	n = read(fd, buffer, 128);
	if(n==-1)
	{
		printf("ERROR: writewe tcps_WE\n");
		exit(1);
	}
	printf("filho saiu %d\n", n);
	if(n == 0)
	{
		Filho.fd[pos-1]=-1;
		printf("filho delete\n"); //apagar dos bestpops???
		tcpsessions++;
		return -1;
	}
	printf("Servidor recebe %s\n", recebe);

	strcpy(recebe, buffer);
	char *token = strtok(recebe, " ");

	printf("token %s\n", recebe);

	if (strcmp("NP", token) == 0)
	{
		sscanf(buffer, "%s %[^:]:%s\n", mess, Filho.IP, Filho.PORT);

		printf("guarda filho %s %s\n", Filho.IP, Filho.PORT);
		if (tcpsessions > 0)
			tcpsessions--;
		pos++;
		return fd;
	}
	else if (strcmp("PR", token) == 0)
	{
		if(strcmp(out, "URROOT") == 0)
		{
			printf("prrr %s \n", buffer);
			sscanf(buffer, "%s %s %s %s\n", mess, id, bp, ap);
			pa = atoi(ap);
			if (bestpops >= pa)
			{
				printf("guarda bp\n");
				for(int i = pops; i < pops+pa; i++)
					strcpy(BP[i], bp);

				bestpops=bestpops-pa;
				pops = pa;
			}
			else if (bestpops != 0)
			{
				printf("guarda else bp\n");
				for(int i = pops; i < pops+pa; i++)
					strcpy(BP[i], bp);
				bestpops=0;
			}
		}
		else
		{
			n = write(fdUP, buffer, strlen(buffer));
			if(n==-1 && debug == 1)
			{
				printf("ERROR: writewe tcps_WE\n");
				exit(1);
			}
		}

	}
}

int tcps_POPQUERY (int fdD)
{
	int n;
	char popquery[128], popreply[128], bp[3], num[128], qi[6];

	sprintf(bp, "%d", bestpops);
	sprintf(qi, "%02x", queryid);
	strcpy(popquery, "PQ ");
	strcat(popquery, qi);
	strcat(popquery, " ");
	strcat(popquery, bp);
	strcat(popquery, "\n");

	if(queryid < 65536)
		queryid++;
	else
		queryid = 0;

	/*envia popquery*/
	n = write(fdD, popquery, strlen(popquery));

	if(n==-1 && debug == 1)
	{
		printf("ERROR: write tcps_POPQUERY\n");
		exit(1);
	}
}
