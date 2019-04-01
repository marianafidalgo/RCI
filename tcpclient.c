#include "defs.h"

int adesao ();

int tcpc_init (char *ip, char *port)
{
	struct addrinfo hints, *res;
	int fd,addrlen,n,nread;
	struct sockaddr_in addr;
	char buffer[128]= " ", data[128]="";

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
	if(n==-1 && debug == 1)
	{
		printf("fd %d \n", fd);
		printf("ERROR: connect tcpc_init\n");
		exit(1);
	}

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
		printf("ERROR: getaddr tcpc_new\n");
		exit(1);
	}

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1 && debug == 1)
	{
		printf("ERROR: sokect tcpc_new\n");
		exit(1);
	}

	n= connect(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1 && debug == 1)
	{
		printf("ERROR: connect tcpc_new\n");
		exit(1);
	}

	n= read(fd, buffer, sizeof(buffer));
	if(n==-1 && debug == 1)
	{
		printf("ERROR: read tcpc_new\n");
		exit(1);
	}
	fdUP = fd;

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
			printf("ERROR: write tcpc_new\n");
			exit(1);
		}

	}
	else
	{
		printf("redirect recebido %s\n", buffer);
		sscanf(buffer, "%s %[^:]:%s\n", newpop, ip, port);
		close(fd);
		freeaddrinfo(res);
		return -1;
	}

	freeaddrinfo(res);
	return fd;
}

int tcpc_Receive( char *out, int counter)
{
	int n;
	char buffer[128] = "", receive[128]="", popreply[128] ="", PQ[3]="", qi[6]="", bp[3]="", ts[3]="";

	n = read(fdUP, buffer, sizeof(buffer));


	if(n==-1 && debug == 1)
	{
		//adesão ??
		printf("ERROR: read tcpc_Receive adesao??\n");
		exit(1);
	}
	if(n == 0)
	{
		adesao();
		return 0; //ver
	}

	if(strcmp("URROOT", out) == 0)
	{
		sprintf(qi, "%02x", n);
		strcpy(receive, "DA ");
		strcat(receive, qi);
		strcat(receive, "\n");
		strcat(receive, buffer);

        for(int i = 0; i < counter; i++)
        {
			if(Filho.fd[i] != -1)
			{
				n = write(Filho.fd[i], receive, strlen(receive));
				if(n==-1 && debug == 1)
				{
					printf("ERROR: write tcpcnew\n");
					exit(1);
				}
			}
		}
	}
	else
	{
		strcpy(receive, buffer);
		char *token = strtok(receive, " ");
		printf("token %s\n", token);

		if (strcmp("DA", token) == 0)
		{
			for(int i = 0; i < counter; i++)
        	{
				if(Filho.fd[i] != -1 && Filho.fd[i] != 0)
				{
					n = write(Filho.fd[i], buffer, strlen(buffer));
					if(n==-1 && debug == 1)
					{
						printf("ERROR: write tcpcnew\n");
						exit(1);
					}
				}
			}
		}
		if (strcmp("PQ", token) == 0)
		{
			if(tcpsessions > 0)
			{
				printf("recebe pop query %s\n", buffer);
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
			else
			{
				for(int i = 0; i < counter; i++)
				{
					if(Filho.fd[i] != -1 && Filho.fd[i] != 0)
					{
						printf("vou enviar PQ filho\n");
						n = write(Filho.fd[i], buffer, strlen(buffer));
						if(n==-1 && debug == 1)
						{
							printf("ERROR: write tcpcnew\n");
							exit(1);
						}
					}
				}
			}
		}
	}
}