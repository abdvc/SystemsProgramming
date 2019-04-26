

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
#define TRUE 1

/*
 * This program creates a socket and then begins an infinite loop. Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 */

int main()
{
	int sock, length;
	struct sockaddr_in server;
	int msgsock;
	char buf[1024], buff[1024];
	int rval;
	int i;

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
	listen(sock, 5);
	do
	{
		msgsock = accept(sock, 0, 0);
		if (msgsock == -1)
		{
			perror("accept");
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

			do
			{
				memset(buf, 0, sizeof(buf));
				memset(buff, 0, sizeof(buff));
				if ((rval = read(msgsock, buf, 1024)) < 0)
					perror("reading stream message");
				//            if((rval = recv(sock, buf, 1024, 0))<0)
				//                perror("reading stream message");
				//printf("%d\n", s);
				i = 0;
				if (rval == 0)
					printf("Ending connection\n");
				else
					printf("-->%s\n", buf);

				write(STDOUT_FILENO, "Input: ", sizeof("Input: "));
				read(STDIN_FILENO, buff, sizeof(buff));
				int s = send(msgsock, buff, strlen(buff), 0);
			} while (rval != 0);
		}
		else
		{
			//parent
			close(msgsock);
		}

		//if (msgsock == -1)
		//	perror("accept");

	} while (TRUE);
	/*
	 * Since this program has an infinite loop, the socket "sock" is
	 * never explicitly closed.  However, all sockets will be closed
	 * automatically when a process is killed or terminates normally.
	 */
}
