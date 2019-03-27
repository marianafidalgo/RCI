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
	if(n!=0)/*error*/
		exit(1);

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1)/*error*/
		exit(1);

	n=bind(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/
		exit(1);

	if(listen(fd,5)==-1)/*error*/
		exit(1);

	freeaddrinfo(res);

	return fd;
}

int tcps_WE (int fdDOWN, char *filhodireto)
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
		strcat(redirect, &filhodireto[0]);
		strcat(redirect, "\n");
		n = write(fd, redirect, strlen(redirect));
		if(n==-1)/*error*/
			exit(1);
		return -1;
	}

	else
	{
		printf("tcpsess %d\n", tcpsessions);
		printf ("fd We %d\n", fd);

		strcpy(welcome, "WE ");
		strcat(welcome, streamID);
		strcat(welcome, "\n");

		/*envia welcome*/
		n = write(fd, welcome, strlen(welcome));
		if(n==-1)/*error*/
			exit(1);

		/*fica à espera de receber newpop*/
		n = read(fd, newpop, 25);

		if(n==-1)/*error*/
			exit(1);

		newpop[25]='\0';
		printf("new pop %s\n", newpop);

		//guarda filho
        sscanf(newpop, "%s %s\n", welcome, filho);
		strcpy(&filhodireto[tcpsessions-1], filho);
		tcpsessions--;

		//guarda fd
	}

	return fd;
}