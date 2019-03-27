#include "defs.h"

int udps_init(char *ipaddr, char *uport)
{
	struct addrinfo hints,*res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[128] = " ";
	char resp[128] = " ";

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_DGRAM; //UDP socket
	hints.ai_flags= AI_PASSIVE|AI_NUMERICSERV;

	n= getaddrinfo(NULL,uport,&hints,&res);
	if(n!=0)/*error*/
		exit(1);

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1)/*error*/
		exit(1);

	n=bind(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/
		exit(1);

	freeaddrinfo(res);
	return fd;
}

int udps_SA (char *streamID, char *ipaddr, char *tport, int fdSA, char *fdDOWNsessions)
{
	struct addrinfo hints,*res;
	int fd, addrlen,n,nread, d;
	struct sockaddr_in addr;
	char buffer[128] = "";
	char resp[128] = "";

	addrlen=sizeof(addr);
	nread=recvfrom(fdSA,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
	if(nread ==-1)/*error*/
		exit(1);

	if(strcmp("POPREQ\n", buffer) == 0)
	{
		/*for (d = 0; d <= 10; d++)
		 {
        	if(fdDOWNsessions[d] != '\0')
				break;
		 }

		sscanf(&fdDOWNsessions[d], "%[^:]:%s \n", ipaddr, tport);*/
		strcpy(resp, "POPRESP ");
		strcat(resp, " ");
		strcat(resp, streamID);
		strcat(resp, " ");
		strcat(resp, ipaddr);
		strcat(resp, ":");
		strcat(resp, tport);

		n=sendto(fdSA,resp,strlen(resp),0,(struct sockaddr*)&addr,addrlen);
		if(n==-1)/*error*/
			exit(1);
	}
	//else guardar fd em array e ver quantas estao free. alguem faz connect mas o array já tem fd preenchido!
	/*else
	{
		printf("PA %s", buffer);
		strcpy(&fdDOWNsessions[pos], buffer);
		pos++;
	}*/


}