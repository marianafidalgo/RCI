#include "funcs.h"

#define PORT "59000"

int dump(void)
{
	struct addrinfo hints, *res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[128];

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_DGRAM; //UDP socket
	hints.ai_flags= AI_NUMERICSERV;

	n = getaddrinfo("193.136.138.142",PORT,&hints,&res);
	if(n!=0)/*error*/
		exit(1);

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1)/*error*/
		exit(1);

	strcpy(buffer, "DUMP\n");
	n= sendto(fd,buffer,strlen(buffer),0,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/
		exit(1);

	addrlen=sizeof(addr);
	nread=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
	if(nread ==-1)/*error*/
		exit(1);

	//write(1,"echo: ",6);

	write(1,buffer,nread);

	freeaddrinfo(res);
	close(fd);
}