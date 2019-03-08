#include "funcs.h"

#define PORT "59000"

int udpc(char *command, int interface, int hex)
{
	struct addrinfo hints, *res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[1024], hexa[1024];   //1024???

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

	strcpy(buffer, command);
	n= sendto(fd,buffer,strlen(buffer),0,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/
		exit(1);

	addrlen=sizeof(addr);
	nread=recvfrom(fd,buffer,1024,0,(struct sockaddr*)&addr,&addrlen);
	if(nread ==-1)/*error*/
		exit(1);

	if(hex==1)
	{
 		for (size_t i = 0; i < strlen(buffer); i++)
		{
			sprintf(&hexa[i*2],"%02x", buffer[i]);
  		}
		strcpy(buffer, hexa);
	}

	if(interface == 1)
		write(1,buffer,nread);

	freeaddrinfo(res);
	close(fd);
}