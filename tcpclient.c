#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int tcpc(char *command, char *streamname, char *streamip, char *streamport, char *ip, char *port)
{
	struct addrinfo hints, *res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[128]= " ";
	char streamid [128]= " ";
	char welcome[128]= " ", newpop[128]= " ", check[128] = " ";

	strcat(streamid, streamname);
	strcat(streamid, ":");
	strcat(streamid, streamip);
	strcat(streamid, ":");
	strcat(streamid, streamport);
	strcat(streamid, "\n");
	printf("streamid %s\n", streamid);

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

	n= connect(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/
		exit(1);

	n= write(fd, streamid, 128);
	if(n==-1)/*error*/
		exit(1);

	/*fica à espera de receber welcome*/
	n= read(fd, welcome, 128);
	if(n==-1)/*error*/
		exit(1);

	/*escreve o que recebeu no terminal*/
	write(1, welcome, n);

	strcpy(check, "WE ");
	strcat(check, streamid);

	if(strcmp(welcome, check) == 0)
	{
		strcat(newpop, "NP ");
		strcat(newpop, ip);
		strcat(newpop, ":");
		strcat(newpop, port);
		strcat(newpop, "\n");

		n= write(fd, newpop, 128);
		if(n==-1)/*error*/
			exit(1);

		freeaddrinfo(res);
		close(fd);
		return 1;
	}

	freeaddrinfo(res);
	close(fd);
	return 0;
}