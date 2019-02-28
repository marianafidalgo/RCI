#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT "59000"

int main(void)
{
	struct addrinfo hints, *res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[128];

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_DGRAM; //UDP socket
	hints.ai_flags= AI_NUMERICSERV;

	n= getaddrinfo("tejo.tecnico.ulisboa.pt",PORT,&hints,&res);
	if(n!=0)/*error*/
		exit(1);

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1)/*error*/
		exit(1);

	strcpy(buffer, "WHOISROOT abc:1.1.1.1:1 2.2.2.3:2\n");
	n= sendto(fd,buffer,strlen(buffer),0,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/
		exit(1);

	addrlen=sizeof(addr);
	nread=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
	if(nread ==-1)/*error*/
		exit(1);
	write(1,"echo: ",6);
	write(1,buffer,n);


	//freeaddrinfo(res);
	close(fd);
	exit(0);
}