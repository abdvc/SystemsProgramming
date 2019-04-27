/*
 Copyright (c) 1986 Regents of the University of California.
 All rights reserved.  The Berkeley software License Agreement
 specifies the terms and conditions for redistribution.

	@(#)streamread.c	6.2 (Berkeley) 5/8/86
*/
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "node.h"

/*
 * This program creates a socket and then begins an infinite loop. Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 */

float add(char *ptr);
float sub(char *ptr);
float mult(char *ptr);
float divide(char *ptr);
void run(char *ptr);

int main()
{
	int sock, length;
	struct sockaddr_in server;
	int msgsock;
	char buf[1024], buff[1024];
	int rval;
	node n;
	//int i;

	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("opening stream socket");
		exit(1);
	}
	/* Name socket using wildcards */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = 0;
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)))
	{
		perror("binding stream socket");
		exit(1);
	}
	/* Find out assigned port number and print it out */
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *)&server, (socklen_t *)&length))
	{
		perror("getting socket name");
		exit(1);
	}
	printf("Socket has port #%d\n", ntohs(server.sin_port));
	fflush(stdout);

	/* Start accepting connections */
	listen(sock, 15);
	do
	{
		msgsock = accept(sock, 0, 0);
		if (msgsock == -1)
		{
			perror("accept");
			exit(1);
		}

		int f = fork();

		if (f < 0)
		{
			perror("fork");
			exit(1);
		}
		else if (f == 0)
		{
			//child
			close(sock);
			time_t t;
			n.pid = getpid();
			n.ppid = getppid();
			n.starttime = time(&t);
			addNode(&n);
			//printNodes();

			do
			{
				memset(buf, 0, sizeof(buf));
				memset(buff, 0, sizeof(buff));
				if ((rval = read(msgsock, buf, 1024)) < 0)
					perror("reading stream message");
				//i = 0;
				if (rval == 0)
					printf("Ending connection\n");
				else
				{
					printf("-->%s\n", buf);

					char *ptr;

					ptr = strtok(buf, " \n");

					char res[50];
					float f;

					if (strcmp(ptr, "add") == 0)
					{
						f = add(ptr);
						sprintf(res, "Result of add : %.2f\n", f);
						int s = send(msgsock, res, strlen(res), 0);
						if (s < 0)
						{
							perror("send");
							exit(0);
						}
					}
					else if (strcmp(ptr, "run") == 0)
					{
						run(ptr);
						int s = send(msgsock, "", 0, 0);
					}
					else if (strcmp(ptr, "sub") == 0)
					{
						f = sub(ptr);
						sprintf(res, "Result of add : %.2f\n", f);
						int s = send(msgsock, res, strlen(res), 0);
						if (s < 0)
						{
							perror("send");
							exit(0);
						}
					}
					else if (strcmp(ptr, "mult") == 0)
					{
						f = mult(ptr);
						sprintf(res, "Result of add : %.2f\n", f);
						int s = send(msgsock, res, strlen(res), 0);
						if (s < 0)
						{
							perror("send");
							exit(0);
						}
					}
					else if (strcmp(ptr, "div") == 0)
					{
						f = divide(ptr);
						sprintf(res, "Result of add : %.2f\n", f);
						int s = send(msgsock, res, strlen(res), 0);
						if (s < 0)
						{
							perror("send");
							exit(0);
						}
					}
					else if (strcmp(ptr, "list") == 0)
					{
						printNodes();
						int s = send(msgsock, "", 0, 0);
					}
					else if (strcmp(ptr, "kill") == 0)
					{
						ptr = strtok(NULL, " ");
						int pid = atoi(ptr);
						kill(pid, SIGTERM);
						node* temp = findNode(pid);
						time_t t3;
						temp->endtime = time(&t3);
						temp->timeelapsed = temp->endtime - temp->starttime;
						int s = send(msgsock, "Process terminated", 18, 0);
					}
					else if (strcmp(ptr, "exit") == 0)
					{
						write(STDOUT_FILENO, "Client disconnected\n", sizeof("Client disconnected\n"));
						time_t t2;
						n.endtime = time(&t2);
						n.timeelapsed = n.endtime - n.starttime;
						close(msgsock);
						exit(0);
					}
					else
					{
						//buf = "Error: Invalid command : ";
						char buf2[1024];
						strcpy(buf2, "Error: Invalid command : ");
						strcat(buf2, ptr);
						strcat(buf2, "\n");
						write(STDOUT_FILENO, buf2, strlen(buf2));
						memset(buf2, 0, sizeof(buf2));
					}
					memset(res, 0, sizeof(res));
				}
			} while (rval != 0);
		}
		else
		{
			//parent
			close(msgsock);
		}

	} while (true);
	/*
	 * Since this program has an infinite loop, the socket "sock" is
	 * never explicitly closed.  However, all sockets will be closed
	 * automatically when a process is killed or terminates normally.
	 */
}

float add(char *ptr)
{
	float res = 0;
	//result for the calculations

	while (ptr != NULL)
	{
		//if ptr is null it means the entire string has been tokenized
		ptr = strtok(NULL, " ");
		//null passed so the string continues to tokenize
		//delimiter is " "

		if (ptr != NULL)
		{
			float i = atof(ptr);
			//parses from string to int
			res += i;
		}
	}
	return res;
}

float sub(char *ptr)
{
	bool flag = false;
	//flag to check for the first numeric value
	float res = 0;
	//result for the calculations

	while (ptr != NULL)
	{
		ptr = strtok(NULL, " ");
		if (ptr != NULL)
		{
			float i = atof(ptr);
			res -= i;
			if (!flag)
			{
				//on first run res-= i; -> res = 0 - i = -i
				res *= -1;
				//to make first argument positive
				flag = true;
			}
		}
	}
	return res;
}

float mult(char *ptr)
{
	float res = 1;
	//result for the calculations
	// res is set to 1 so that res*=i doesn't give a 0
	while (ptr != NULL)
	{
		ptr = strtok(NULL, " ");
		if (ptr != NULL)
		{
			float i = atof(ptr);
			res *= i;
		}
	}
	return res;
}

float divide(char *ptr)
{
	bool flag = false;
	//check for the first numeric value
	float res;
	//result for the calculations
	while (ptr != NULL)
	{
		ptr = strtok(NULL, " ");
		if (ptr != NULL)
		{
			float i = atof(ptr);
			if (!flag)
			{
				// set res to the first value
				res = i;
				flag = true;
			}
			else
			{
				res /= i;
			}
		}
	}
	return res;
}

void run(char *ptr)
{
	int pid = fork();
	//fork a new process

	char buf[50];
	//buffer to build the path string for execl

	if (pid == -1)
	{
		perror("Fork ");
	}
	else if (pid == 0)
	{
		ptr = strtok(NULL, " \n");
		//tokenize the string further

		sprintf(buf, "/usr/bin/%s", ptr);
		//build the string
		int ex = execl(buf, ptr, NULL);
		//execute the process

		if (ex == -1)
		{
			perror("Exec");
		}
	}
}
