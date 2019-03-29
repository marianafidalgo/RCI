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

	if(listen(fd,5)==-1 && debug == 1)
	{
		printf("ERROR: listen tcps_init\n");
		exit(1);
	}

	freeaddrinfo(res);

	return fd;
}

int tcps_WE (int fdDOWN, struct ofilho Filho[])
{
	struct addrinfo hints, *res;
	int fd, addrlen,n,nread;
	struct sockaddr_in addr;
	char welcome[25], newpop[25], filho[21], redirect[21];
	char *tok;

	if((fd=accept(fdDOWN,(struct sockaddr*)&addr,&addrlen))==-1)/*error*/
	{
		printf("Nop\n");
		exit(1);
	}
	else if(tcpsessions < 1)
	{
		printf("no session available\n");
		//redirect
		strcpy(redirect, "RE ");
		strcat(redirect, Filho[0].IP);
		strcat(redirect, ":");
		strcat(redirect, Filho[0].PORT);
		strcat(redirect, "\n");
		n = write(fd, redirect, strlen(redirect));
		if(n==-1 && debug == 1)
		{
			printf("ERROR: write tcps_WE\n");
			exit(1);
		}
		return -1;
	}
	else
	{
		printf("tcpsess %d\n", tcpsessions);
		printf ("fd We %d\n", fdDOWN);

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

		/*fica à espera de receber newpop*/
		n = read(fd, newpop, 25);
		if(n==-1&& debug == 1)
		{
			printf("ERROR: read tcps_WE\n");
			exit(1);
		}

		Filho[pos].fd = fd;
		printf("fdddd %d\n ",Filho[pos].fd );


		newpop[25]='\0';
		printf("new pop %s\n", newpop);

		// separar o que se chama filho em IP e port e guardas essas duas coisas.
		// guardar o fd
		//guarda filho

        sscanf(newpop, "%s %[^:]:%s\n", welcome, Filho[pos].IP, Filho[pos].PORT);

		printf("adeus %s %s\n", Filho[pos].IP, Filho[pos].PORT);
		tcpsessions--;
		pos++;

	}

	return fd;
}

int tcps_POPQUERY (int fdD)
{
	int n;
	char popquery[128], popreply[128], bp[3], qi[6];

	sprintf(bp, "%d", bestpops);
	sprintf(qi, "%d", queryid);
	strcpy(popquery, "PQ ");
	strcat(popquery, qi);
	strcat(popquery, " ");
	strcat(popquery, bp);
	strcat(popquery, "\n");

	if(queryid < 65536)
		queryid++;
	else
		queryid = 0;

	/*envia welcome*/
	n = write(fdD, popquery, strlen(popquery));
	if(n==-1 && debug == 1)
	{
		printf("ERROR: write tcps_POPQUERY\n");
		exit(1);
	}
	/*fica à espera de receber reply*/
	n = read(fdD, popreply, 25);
	printf("PRRRR %s", popreply);
	if(n==-1 && debug == 1)
	{
		printf("ERROR: read tcps_POP\n");
		exit(1);
	}

}