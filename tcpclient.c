#include "defs.h"

int tcpc_init (char *ip, char *port)
{
	struct addrinfo hints, *res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[128]= " ";
	char streamid [128]= " ";

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_STREAM; //TCP socket
	hints.ai_flags= AI_NUMERICSERV | AI_NUMERICHOST;
	printf("ip %s port %s\n", ip, port);

	n= getaddrinfo(ip,port,&hints,&res);
	if(n!=0 && debug == 1)
	{
		printf("ERROR: getaddr tcpc_init\n");
		exit(1);
	}

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1 && debug == 1)
	{
		printf("ERROR: socket tcpc_init\n");
		exit(1);
	}

	n = connect(fd,res->ai_addr,res->ai_addrlen);
	printf("n %d\n", n);
	if(n==-1 && debug == 1)
	{
		printf("fd %d \n", fd);
		printf("ERROR: connect tcpc_init\n");
		exit(1);
	}

	//ouvir da fonte!!!

	return fd;
}

int tcpc_new(char *ip, char *port)
{
	struct addrinfo hints, *res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[128] = "", newpop[128] ="", check[128] = "";

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_STREAM; //TCP socket
	hints.ai_flags= AI_NUMERICSERV;

	n= getaddrinfo(ip,port,&hints,&res);
	if(n!=0 && debug == 1)/*error*/
	{
		printf("ERROR: getaddr tcpcnew\n");
		exit(1);
	}

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1 && debug == 1)
	{
		printf("ERROR: sokect tcpcnew\n");
		exit(1);
	}

	printf("new fd %d\n", fd);

	fdUP = fd;

	n= connect(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1 && debug == 1)
	{
		printf("ERROR: connect tcpcnew\n");
		exit(1);
	}

	n= read(fd, buffer, sizeof(buffer));
	if(n==-1 && debug == 1)
	{
		printf("ERROR: read tcpcnew\n");
		exit(1);
	}

	buffer[n]='\0';
	printf("welcome %s\n", buffer);

	strcpy(check, "WE ");
	strcat(check, streamID);
	strcat(check, "\n");

	if (strcmp(buffer, check) == 0)
	{
		printf("tcpsess %d", tcpsessions);
		strcpy(newpop, "NP ");
		strcat(newpop, ipaddr);
		strcat(newpop, ":");
		strcat(newpop, tport);
		strcat(newpop, "\n");
		n = write(fd, newpop, strlen(newpop));
		if(n==-1 && debug == 1)
		{
			printf("ERROR: write tcpcnew\n");
			exit(1);
		}

	}
	else
	{
		printf("buffer oasld %s\n", buffer);
		sscanf(buffer, "%s %[^:]:%s\n", newpop, ip, port);
		close(fd);
		freeaddrinfo(res);
		return -1;
	}

	freeaddrinfo(res);
	return fd;
}

int tcpc_Receive(int fdUP)
{
	int n;
	char buffer[128] = "", popreply[128] ="", PQ[3]="", qi[6]="", bp[3]="", ts[3]="";

	printf("fdUP %d\n", fdUP);

	n= read(fdUP, buffer, sizeof(buffer));
	if(n==-1 && debug == 1)
	{
		//adesão ??
		printf("ERROR: read tcpc_Receive adesao??\n");
		exit(1);
	}

	printf("hey\n");
	strcpy(popreply, buffer);
	printf("i just met you\n");
	char *token = strtok(popreply, " ");
	printf("and this is crazy %s\n", popreply);

	if (strcmp("PQ", token) == 0)
	{
		printf("so here's my number \n");
		printf("PQQQQ %s", buffer);
		sscanf(buffer, "%s %s %s\n", PQ, qi, bp);
		sprintf(ts, "%d", tcpsessions);

		strcpy(popreply, "PR ");
		strcat(popreply, qi);
		strcat(popreply, " ");
		strcat(popreply, ipaddr);
		strcat(popreply, ":");
		strcat(popreply, tport);
		strcat(popreply, " ");
		strcat(popreply, ts);
		strcat(popreply, "\n");

		n = write(fdUP, popreply, strlen(popreply));
		if(n==-1 && debug == 1)
		{
			printf("ERROR: write tcpcnew\n");
			exit(1);
		}

	}
	/*else
	{
		printf("buffer oasld %s\n", buffer);
		sscanf(buffer, "%s %[^:]:%s\n", newpop, ip, port);
		close(fd);
		freeaddrinfo(res);
		return -1;
	}*/

}