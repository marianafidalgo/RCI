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

	n = getaddrinfo(NULL,PORT,&hints,&res);
	if(n!=0 && debug == 1)
	{
		printf("ERROR: getaddr tcps_init\n");
		exit(1);
	}

	fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(fd == -1 && debug == 1)
	{
		printf("ERROR: socket tcps_init\n");
		exit(1);
	}

	n = bind(fd,res->ai_addr,res->ai_addrlen);
	if(n == -1 && debug == 1)
	{
		printf("ERROR: bind tcps_init %s\n", strerror(errno));
		exit(1);
	}

	n = listen(fd,128);
	if(n == -1 && debug == 1)
	{
		printf("ERROR: listen tcps_init\n");
		exit(1);
	}

	freeaddrinfo(res);

	return fd;
}

int tcps_WE (int fdDOWN, char *out)
{
	//struct addrinfo hints, *res;
	int fd, addrlen,n,nread;
	struct sockaddr_in addr;
	char welcome[25], buffer[128],receive[128], filho[21], redirect[21];
	char *tok;

	if((fd = accept(fdDOWN,(struct sockaddr*)&addr,&addrlen)) == -1)
	{
		if(debug == 1)
			printf("ERROR: Did not accept tcps_WE\n");
		return -1;
	}
	else if(tcpsessions < 1)
	{
		if(debug == 1)
			printf("no session available\n");
		//redirect
		strcpy(redirect, "RE ");
		strcat(redirect, Filho.IP[0]);
		strcat(redirect, ":");
		strcat(redirect, Filho.PORT[0]);
		strcat(redirect, "\n");
		n = write(fd, redirect, strlen(redirect));
		if(n == -1 && debug == 1)
		{
			printf("ERROR: write tcps_WE\n");
			exit(1);
		}
		return 0;
	}
	else
	{
		strcpy(welcome, "WE ");
		strcat(welcome, streamID);
		strcat(welcome, "\n");

		/*envia welcome*/
		n = write(fd, welcome, strlen(welcome));
		if(n == -1 && debug == 1)
		{
			printf("ERROR: writewe tcps_WE\n");
			exit(1);
		}
		if(strcmp(out, "URROOT") == 0)
		{
			delay(2);
			n = write(fd, "SF\n", 3);
			if(n == -1 && debug == 1)
			{
				printf("ERROR: writewe tcps_WE\n");
				exit(1);
			}
		}
		else
		{
			if(flowing == 1)
			{
				delay(2);
				n = write(fd, "SF\n", 3);
				if(n == -1 && debug == 1)
				{
					printf("ERROR: writewe tcps_WE\n");
					exit(1);
				}
			}
			else
			{
				delay(2);
				n = write(fd, "BS\n", 3);
				if(n == -1 && debug == 1)
				{
					printf("ERROR: writewe tcps_WE\n");
					exit(1);
				}
			}
		}
		Filho.fd[pos] = fd;

		return fd;
	}
	//freeaddrinfo(res);
}

int tcps_Receive (int fd, char *out)
{
	int n, pa = 0, j = 0, t = 0, p = 0, PAfalta = 0, flag = 0;
	char recebe[128] = "", buffer[128] = "", popquery[128]="", mess[25] = "", id[25] = "", bp[25] = "", ap[2] = "", flow[25] = "", newPA[25] = "", hexad[2048] = "", reply[128] = "";
	char **array, **arrayPQ;

	array = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        array[i] = (char *)malloc(25 * sizeof(char));
        strcpy(array[i],"\0");
    }
	/*arrayPQ = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        arrayPQ[i] = (char *)malloc(25 * sizeof(char));
        strcpy(arrayPQ[i],"\0");
    }*/

	n = read(fd, buffer, 128);
	if(n == -1)
	{
		printf("ERROR: writewe tcps_Receive\n");
		exit(1);
	}
	if(n == 0)
	{
		if(debug == 1)
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

				/*for(int i = 0; i < counterbp; i++)
        		{
					if(strcmp(BP[i], flow) == 0)
					{
						strcpy(BP[i], newPA);
					}
					else
					{
						if(counterbp > 1)
						{
							for(int j = counterbp-2; j >= 0; j--)
								strcpy(BP[j+1], BP[j]);
						}
						if (i == (counterbp - 1))
							strcpy(BP[0], newPA);
					}
				}*/

			}
			else if(Filho.fd[i] != -1 && Filho.fd[i] != 0)
			{
				n = write(Filho.fd[i], "BS\n", 3);
				if(n == -1 && debug == 1)
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

	if(debug == 1)
		printf("token %s\n", recebe);

	if (strcmp("NP", token) == 0)
	{
		sscanf(buffer, "%s %[^:]:%s\n", mess, Filho.IP[pos], Filho.PORT[pos]);

		if(debug == 1)
			printf("guarda filho %s %s\n", Filho.IP[pos], Filho.PORT[pos]);
		if (tcpsessions > 0)
			tcpsessions--;
		pos++;
		flowing = 1;
		return fd;
	}
	/*else if (strcmp("PR", token) == 0)
	{
		sscanf(buffer, "%s %s %s %s\n", mess, idbp, bp, ap);
		printf("sscanf : %s %s %s %s\n", mess, idbp, bp, ap);
		pa = atoi(ap);
		int bestp = atoi(bp);
		int qid = atoi(id);
		strcpy(popquery, buffer);

		char *tok = strtok(popquery, " ");
		p = 0;
		while(tok != NULL)
		{
			arrayPQ[p] = tok;
			tok = strtok(NULL, "\n");
			p++;
			printf("HJKAHERKJAH %s\n", arrayPQ[p]);
		}

		if(strcmp(out, "URROOT") == 0)
		{
			printf("prrr %s", buffer);
			if((strcmp(BP[0], idbp)) != 0)
			{
				printf("strcmp(BP[0], idbp)\n");
				BP[0] = idbp; //acho que isto é burro, checka!!
				//limpar vetor para novo id
				for(int i = 1; i < ((counterbp * counter) + 1); i++)
					strcpy(BP[i], "\0");
				//preencher com pontos de acesso recebidos
				for(int i = 1; i < pa+1; i++)
				{
					printf("preencher novo id com pa %s %s\n", bp, ap);
					strcpy(BP[i], bp);
					pops = pa;
					printf("paaaaa %d pops %d \n", pa, pops);
				}
			}
			else
			{
				for(int i = 1; i < ((counterbp * counter) + 1); i++)
				{
					if((strcmp(BP[i], "\0") == 0));
					{
						for(int j = i; j < i+pa; j++)
						{
							strcpy(BP[j], bp);
							pops = j;
							printf("pa %d pooooops %d \n", pa, pops);
						}
						break;
					}
				}
			}
		}
		else
		{
			for(int i = 0; i < 128; i+=2)
			{
				if(BPcheck[i] == qid)
				{
					j = i;
					break;
				}
			}
			if(pa < BPcheck[j+1])
			{
				printf("mmmmmmmmmmmmmm %s \n", buffer);
				n = write(fdUP, buffer, strlen(buffer));
				if(n == -1 && debug == 1)
				{
					printf("ERROR: writewe tcps_WE\n");
					exit(1);
				}
				BPcheck[j+1] = BPcheck[j+1] - pa;
			}
			else if(pa = BPcheck[j+1])
			{
				printf("pa=\n");
				n = write(fdUP, buffer, strlen(buffer));
				if(n == -1 && debug == 1)
				{
					printf("ERROR: writewe tcps_WE\n");
					exit(1);
				}
				for(int i = j; i < 126; i++)
				{
					BPcheck[i] = BPcheck[i+2];
				}
			}
			else if(pa > BPcheck[j+1])
			{
				strcpy(reply, "PR ");
				strcat(reply, id);
				strcat(reply, " ");
				strcat(reply, ipaddr);
				strcat(reply, ":");
				strcat(reply, tport);
				strcat(reply, " ");
				strcat(reply, bp);
				strcat(reply, "\n");
				printf("replyyyy %s \n", reply);
				n = write(fdUP, reply, strlen(reply));
				if(n == -1 && debug == 1)
				{
					printf("ERROR: writewe tcps_WE\n");
					exit(1);
				}
				for(int i = j; i < 126; i++)
				{
					BPcheck[i] = BPcheck[i+2];
				}
			}
		}

	}*/
	else if (strcmp("TR", token) == 0)
	{
		if(strcmp(out, "URROOT") == 0)
		{

			char *tok = strtok(buffer, " ");
            t = 0;
	        while(tok != NULL)
	        {
		        array[t] = tok;
		        tok = strtok(NULL, " \n");
				t++;
	        }
			treecounter--;
			strcat(tree, array[1]);
			strcat(tree, "(");
			strcat(tree, array[2]);
			t = 3;
			while(strcmp(array[t], "\0") != 0)
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
			n = write(fdUP, buffer, strlen(buffer));
			if(n == -1 && debug == 1)
			{
				printf("ERROR: writewe tcps_WE\n");
				exit(1);
			}
		}
		if(treecounter == 0 && (strcmp(out, "URROOT") == 0) && hex == 0 )
        	printf("%s \n", tree);
		else if(treecounter == 0 && (strcmp(out, "URROOT") == 0) && hex == 1 )
		{
			for (int i = 0; i < strlen(tree); i++)
			{
				sprintf(&hexad[i*4],"%04x\n", tree[i]);
			}
			printf("%s\n", hexad);
		}
	}
}

/*int tcps_POPQUERY (int fdD)
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


	n = write(fdD, popquery, strlen(popquery));
	if(n == -1 && debug == 1)
	{
		printf("ERROR: write tcps_POPQUERY\n");
		exit(1);
	}
}*/

int tcps_TREEQUERY (int fdD, char * ip, char *port)
{
	int n;
	char treequery[128];

	strcpy(treequery, "TQ ");
	strcat(treequery, ip);
	strcat(treequery, ":");
	strcat(treequery, port);
	strcat(treequery, "\n");

	//envia treequery

	n = write(fdD, treequery, strlen(treequery));
	if(n == -1 && debug == 1)
	{
		printf("ERROR: write tcps_POPQUERY\n");
		exit(1);
	}
}

