#include "defs.h"

int udpc_RS (char *out, char *command, char *rsaddr)
{
	struct addrinfo hints, *res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	size_t i = 0;
	char buffer[1024], hexa[1024];   //1024???

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_DGRAM; //UDP socket
	hints.ai_flags= AI_NUMERICSERV;

	n = getaddrinfo(rsaddr,"59000",&hints,&res);
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
 		for (i = 0; i < strlen(buffer); i++)
		{
			sprintf(&hexa[i*2],"%02x", buffer[i]);
  		}
		strcpy(buffer, hexa);
	}

	if(display == 1)
		write(1,buffer,nread);

	freeaddrinfo(res);

	strncpy(out, buffer, nread);
}

int udpc_POP (char *ipADDR, char *uPORT, char *tPORT)
{
	struct addrinfo hints, *res;
	int fd, addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[1024]="", command[1024]="";

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_DGRAM; //UDP socket
	hints.ai_flags= AI_NUMERICSERV | AI_NUMERICHOST;

	n = getaddrinfo(ipADDR, uPORT, &hints,&res);
	if(n!=0) //error
		exit(1);

	fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1) //error
		exit(1);

	n = sendto(fd,"POPREQ\n",7,0,res->ai_addr,res->ai_addrlen);
	if(n==-1) //error
		exit(1);

	addrlen=sizeof(addr);
	nread=recvfrom(fd,buffer,1024,0,(struct sockaddr*)&addr,&addrlen);
	if(nread ==-1)/*error*/
		exit(1);

	sscanf (buffer, "%s %s %[^:]:%s\n", command, streamID, ipADDR, tPORT);

	freeaddrinfo(res);
	close(fd);
}

int udpc_PA (char *ipADDR, char *uPORT, char *tPORT)
{
	struct addrinfo hints, *res;
	int fd, addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[1024]="", PA[1024]="";

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_DGRAM; //UDP socket
	hints.ai_flags= AI_NUMERICSERV | AI_NUMERICHOST;

	n = getaddrinfo(ipADDR, uPORT, &hints,&res);
	if(n!=0) //error
		exit(1);

	fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1) //error
		exit(1);

	strcpy(PA, ipADDR);
	strcat(PA, ":");
	strcat(PA, tPORT);
	strcat(PA, "\n");

	n = sendto(fd,PA,25,0,res->ai_addr,res->ai_addrlen);
	if(n==-1) //error
		exit(1);

/*	addrlen=sizeof(addr);
	nread=recvfrom(fd,buffer,1024,0,(struct sockaddr*)&addr,&addrlen);
	if(nread ==-1)
		exit(1);

	sscanf (buffer, "%s %s %[^:]:%s\n", command, streamID, ipADDR, tPORT);*/

	freeaddrinfo(res);
	close(fd);
}

