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

	n = getaddrinfo(NULL,uport,&hints,&res);
	if(n != 0 && debug == 1)
	{
		printf("ERROR: getaddr udps_init\n");
		exit(1);
	}


	fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd == -1 && debug == 1)
	{
		printf("ERROR: socket udps_init\n");
		exit(1);
	}

	n = bind(fd,res->ai_addr,res->ai_addrlen);
	if(n == -1 && debug == 1)
	{
		printf("ERROR: bind udps_init\n");
		exit(1);
	}

	freeaddrinfo(res);
	return fd;
}

int udps_SA (char *streamID, char *ipaddr, char *tport, int fdSA, char *fdDOWNsessions)
{
	struct addrinfo hints,*res;
	int fd, addrlen,n,nread, d;
	struct sockaddr_in addr;
	char buffer[128] = "", resp[128] = "", ip[25] = "", port[25] = "";

	addrlen = sizeof(addr);
	nread = recvfrom(fdSA,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
	if(nread == -1 && debug == 1)
	{
		printf("ERROR: recvfrom udps_SA\n");
		exit(1);
	}
	//printf("vou dar bestpop %s, %s\n", BP[0], idbp);

	if(strcmp("POPREQ\n", buffer) == 0)
	{
		if(debug == 1)
			printf("Vou dar ipADDR %s tPORT %s\n", ipaddr, tport);
		/*if((strcmp(BP[1], " ") != 0) && tcpsessions == 0)
		{
			sscanf(BP[1], "%[^:]:%s", ip, port);
			printf("vou dar bestpop %s, %d\n", BP[0], pops);
			if(pops > 1)
			{
				for(int i = 1; i < pops; i++)
				{
					printf("pops %d\n", pops);
					printf("%s %s\n", BP[i], BP[i+1]);
					strcpy(BP[i], BP[i+1]);
					printf("%s %s\n", BP[i], BP[i+1]);
					printf("estou a shiftaaaaar BP[0] %s\n", BP[0]);
				}
				strcpy(BP[pops], " ");
			}
			else
			{
				strcpy(BP[1], " ");
				printf("estou a shiftar BP[0] %s\n", BP[0]);
			}
			pops--;
		}
		else
		{*/
			strcpy(ip, ipaddr);
			strcpy(port, tport);
		//}

		strcpy(resp, "POPRESP ");
		strcat(resp, " ");
		strcat(resp, streamID);
		strcat(resp, " ");
		strcat(resp, ip);
		strcat(resp, ":");
		strcat(resp, port);


		n = sendto(fdSA,resp,strlen(resp),0,(struct sockaddr*)&addr,addrlen);
		if(n == -1 && debug == 1)
		{
			printf("ERROR: recvfrom udps_SA\n");
			exit(1);
		}
	}
	//else guardar fd em array e ver quantas estao free. alguem faz connect mas o array já tem fd preenchido!

}