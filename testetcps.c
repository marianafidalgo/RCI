#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> //depois tirar

int main(void)
{
	struct addrinfo hints, *res;
	int newfd, fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[128] = " ", welcome[128]= " " , name[128]= " ", ip[128]= " ", port[128]= " ", newpop[128]= " ";

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_STREAM; //TCP socket
	hints.ai_flags= AI_PASSIVE|AI_NUMERICSERV;

	n= getaddrinfo(NULL,"58008",&hints,&res);
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

	if((newfd=accept(fd,(struct sockaddr*)&addr,&addrlen))==-1)/*error*/
		exit(1);

	n=read(newfd,buffer,128);
	if(n==-1)/*error*/
		exit(1);

    sscanf (buffer, "%[^:]:%[^:]:%s \n", name, ip, port);

    strcat(newpop, "NP ");
	strcat(newpop, ip);
	strcat(newpop, ":");
	strcat(newpop, port);
	strcat(newpop, "\n");
	printf("%s\n", newpop);

	write(1, buffer, n);

	n = write(newfd,welcome,n);
	if(n==-1)/*error*/
		exit(1);

	freeaddrinfo(res);
	close(fd);
	exit(0);
}