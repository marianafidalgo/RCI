#include "defs.h"

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;

    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds)
        ;
}

int tcps_init(char *PORT)
{
	struct addrinfo hints, *res;
	int newfd, fd,addrlen,n,nread;
	struct sockaddr_in addr;


	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_STREAM; //TCP socket
	hints.ai_flags= AI_PASSIVE|AI_NUMERICSERV;

	n= getaddrinfo(NULL,PORT,&hints,&res);
	if(n!=0 && debug == 1)
	{
		printf("ERROR: getaddr tcps_init\n");
		exit(1);
	}

	fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd==-1 && debug == 1)
	{
		printf("ERROR: socket tcps_init\n");
		exit(1);
	}

	n=bind(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1 && debug == 1)
	{
		printf("ERROR: bind tcps_init\n");
		exit(1);
	}

	n=listen(fd,128);
	if(n == -1&& debug == 1)
	{
		printf("ERROR: listen tcps_init\n");
		exit(1);
	}

	freeaddrinfo(res);

	return fd;
}

int tcps_WE (int fdDOWN, char *out)
{
	struct addrinfo hints, *res;
	int fd, addrlen,n,nread;
	struct sockaddr_in addr;
	char welcome[25], buffer[128],receive[128], filho[21], redirect[21];
	char *tok;

	if((fd=accept(fdDOWN,(struct sockaddr*)&addr,&addrlen))==-1)
	{
		printf("Nop :'( \n");
		return -1;
	}
	else if(tcpsessions < 1)
	{
		printf("no session available\n");
		//redirect
		strcpy(redirect, "RE ");
		strcat(redirect, Filho.IP[0]);
		strcat(redirect, ":");
		strcat(redirect, Filho.PORT[0]);
		strcat(redirect, "\n");
		n = write(fd, redirect, strlen(redirect));
		printf("%s no session \n", redirect);
		if(n==-1 && debug == 1)
		{
			printf("ERROR: write tcps_WE\n");
			exit(1);
		}
		return 0;
	}
	else
	{
		printf("tcpsess %d\n", tcpsessions);
		strcpy(welcome, "WE ");
		strcat(welcome, streamID);
		strcat(welcome, "\n");

		/*envia welcome*/
		n = write(fd, welcome, strlen(welcome));
		if(n==-1 && debug == 1)
		{
			printf("ERROR: writewe tcps_WE\n");
			exit(1);
		}
		printf("out %s\n", out);
		if(strcmp(out, "URROOT") == 0)
		{
			delay(2);
			n = write(fd, "SF\n", 3);
			if(n==-1 && debug == 1)
			{
				printf("ERROR: writewe tcps_WE\n");
				exit(1);
			}
		}
		else
		{
			printf("FLOWING %d\n",flowing );
			if(flowing == 1)
			{
				delay(2);
				n = write(fd, "SF\n", 3);
				if(n==-1 && debug == 1)
				{
					printf("ERROR: writewe tcps_WE\n");
					exit(1);
				}
			}
			else
			{
				delay(2);
				n = write(fd, "BS\n", 3);
				if(n==-1 && debug == 1)
				{
					printf("ERROR: writewe tcps_WE\n");
					exit(1);
				}
			}
		}
		Filho.fd[pos] = fd;
		printf("fdddd %d\n ",Filho.fd[pos] );

		return fd;
	}
	freeaddrinfo(res);
}

int tcps_Receive (int fd, char *out)
{
	int n, pa = 0, j = 0, t = 0, PAfalta = 0;
	char recebe[128]="", buffer[128]="", mess[25]="", id[25]="", bp[25]="", ap[2]="", flow[25]="", newPA[25]="";
	char **array;

	array = (char **)malloc(10 * sizeof(char *));
    for (int i=0; i<10; i++)
    {
        array[i] = (char *)malloc(25 * sizeof(char));
        strcpy(array[i]," ");
    }

	printf("Servidor recebe %s\n", recebe);

	n = read(fd, buffer, 128);
	if(n==-1)
	{
		printf("ERROR: writewe tcps_Receive\n");
		exit(1);
	}
	if(n == 0)
	{
		printf("filho delete\n");
		for(int i = 0; i < counter; i++)
        {
			if(fd == Filho.fd[i])
			{
				strcpy(flow, Filho.IP[i]);
				strcat(flow, ":");
				strcat(flow, Filho.PORT[i]);
				strcpy(newPA, ipaddr);
				strcat(newPA, ":");
				strcat(newPA, tport);
				Filho.fd[i]=-1;
				flowing = 0;
				close(fd);
				for(int j = i; j < counter-1; j++)
				{
					strcpy(Filho.IP[j], Filho.IP[j+1]);
					strcpy(Filho.PORT[j], Filho.PORT[j+1]);
					Filho.fd[j] = Filho.fd[j+1];
				}

				for(int i = 0; i < counterbp; i++)
        		{
					if(strcmp(BP[i], flow) == 0)
					{
						strcpy(BP[i], newPA);
						printf("filho mudar no bp %d\n", i);
					}

					else
					{
						printf("else filho mudar no bp %s\n", newPA);
						if(counterbp > 1)
						{
							for(int j = counterbp-2; j >= 0; j--)
								strcpy(BP[j+1], BP[j]);
						}
						strcpy(BP[0], newPA);
					}
				}
			}
			else
			{
				n = write(Filho.fd[i], "BS\n", 3);
				if(n==-1 && debug == 1)
				{
					printf("ERROR: writewe tcps_Receive\n");
					exit(1);
				}
			}
		}
		tcpsessions++;
		pos--;
		return -1;
	}

	strcpy(recebe, buffer);
	char *token = strtok(recebe, " ");

	printf("token %s\n", recebe);

	if (strcmp("NP", token) == 0)
	{
		sscanf(buffer, "%s %[^:]:%s\n", mess, Filho.IP[pos], Filho.PORT[pos]);

		printf("guarda filho %s %s\n", Filho.IP[pos], Filho.PORT[pos]);
		if (tcpsessions > 0)
			tcpsessions--;
		pos++;
		flowing = 1;
		return fd;
	}
	else if (strcmp("PR", token) == 0)
	{
		if(strcmp(out, "URROOT") == 0)
		{
			printf("prrr %s \n", buffer);
			sscanf(buffer, "%s %s %s %s\n", mess, id, bp, ap);
			pa = atoi(ap);
			PAfalta = pa;
			printf("bestpops %d\n", bestpops);
			printf("pa  %d\n", pa);
			if (bestpops >= pa)
			{
				printf("guarda bp %d\n", pops);
				for(int m = 0; m < pa; m++)
				{
					j = 0;
					for(int i = pops; i < counterbp; i++)
					{
						printf("i j %d %d\n", i, j);
						printf("aquiii BPi %s\n", BP[j]);
						printf("bppp %s\n", bp);
						if((strcmp(BP[j], bp) == 0) && PAfalta == 0)
						{
							printf("iagua1\n");
							break;
						}
						else
						{
							printf("escreve bp\n");
							strcpy(BP[i], bp);
							bestpops--;
							pops++;
							PAfalta--;
							break;
						}
						j++;
					}
				}
			}
			else if (bestpops != 0)
			{
				printf("guarda else bp\n");
				for(int i = pops; i < bestpops; i++)
				{
					if((strcmp(BP[i], bp)!=0) || PAfalta > 0)
					{
						strcpy(BP[i], bp);
						PAfalta--;
					}
				}
				bestpops=0;
			}
		}
		else
		{
			n = write(fdUP, buffer, strlen(buffer));
			if(n==-1 && debug == 1)
			{
				printf("ERROR: writewe tcps_WE\n");
				exit(1);
			}
		}

	}
	else if (strcmp("TR", token) == 0)
	{
		if(strcmp(out, "URROOT") == 0)
		{
			//guarda
			printf("guarda tree reply\n");
			char *tok = strtok(buffer, " ");
            t = 0;
	        while(tok != NULL)
	        {
		        array[t++] = tok;
		        tok = strtok(NULL, " \n");
	        }
			treecounter--;
			strcat(tree, array[1]);
			strcat(tree, "(");
			strcat(tree, array[2]);
			while(strcmp(array[t], " ") != 0)
			{
				strcat(tree, " ");
				strcat(tree,array[t]);
				treecounter++;
				t++;
			}
			strcat(tree, ")");
			strcat(tree, "\n");
		}
		else
		{
			printf("envia tree reply");
			n = write(fdUP, buffer, strlen(buffer));
			if(n==-1 && debug == 1)
			{
				printf("ERROR: writewe tcps_WE\n");
				exit(1);
			}
		}
		if(treecounter == 0)
        	printf("FINAL TREEEEEEEEEEEEEEEEEEEEEEEE\n%s", tree);
	}
}

int tcps_POPQUERY (int fdD)
{
	int n;
	char popquery[128], popreply[128], bp[3], num[128], qi[6];

	sprintf(bp, "%d", bestpops);
	sprintf(qi, "%04x", queryid);
	strcpy(popquery, "PQ ");
	strcat(popquery, qi);
	strcat(popquery, " ");
	strcat(popquery, bp);
	strcat(popquery, "\n");

	if(queryid < 65536)
		queryid++;
	else
		queryid = 0;

	/*envia popquery*/
	n = write(fdD, popquery, strlen(popquery));
	if(n==-1 && debug == 1)
	{
		printf("ERROR: write tcps_POPQUERY\n");
		exit(1);
	}
}

int tcps_TREEQUERY (int fdD, char * ip, char *port)
{
	int n;
	char treequery[128];

	strcpy(treequery, "TQ ");
	strcat(treequery, ip);
	strcat(treequery, ":");
	strcat(treequery, port);
	strcat(treequery, "\n");

	/*envia treequery*/
	n = write(fdD, treequery, strlen(treequery));
	if(n==-1 && debug == 1)
	{
		printf("ERROR: write tcps_POPQUERY\n");
		exit(1);
	}
}

