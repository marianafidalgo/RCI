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

	n= getaddrinfo(ip,port,&hints,&res);
	if(n!=0)/*error*/
		exit(1);

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1)/*error*/
		exit(1);

	n= connect(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1)
	/*error*/
		exit(1);

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
	if(n!=0)/*error*/
		exit(1);

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1)/*error*/
		exit(1);

	printf("new fd %d\n", fd);

	n= connect(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/
		exit(1);

	n= read(fd, buffer, sizeof(buffer)-1);
	if(n==-1)/*error*/
		exit(1);

	buffer[n]='\0';
	printf("welcome %s\n", buffer);

	strcpy(check, "WE ");
	strcat(check, streamID);
	strcat(check, "\n");

	if (strcmp(buffer, check) == 0)
	{
		printf("tcpsess %d", tcpsessions);
		strcpy(newpop, "NP ");
		strcat(newpop, ip);
		strcat(newpop, ":");
		strcat(newpop, port);
		strcat(newpop, "\n");
		n = write(fd, newpop, strlen(newpop));
		if(n==-1)/*error*/
			exit(1);
	}
	else
	{
		sscanf(buffer, "%s %[^:]:%s\n", newpop, ip, port);
		close(fd);
		freeaddrinfo(res);
		return -1;
	}

	freeaddrinfo(res);
	return fd;
}