/*
 Copyright (c) 1986 Regents of the University of California.
 All rights reserved.  The Berkeley software License Agreement
 specifies the terms and conditions for redistribution.

	@(#)streamwrite.c	6.2 (Berkeley) 5/8/86
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
 * This program creates a socket and initiates a connection with the socket
 * given in the command line.  One message is sent over the connection and
 * then the socket is closed, ending the connection. The form of the command
 * line is streamwrite hostname portnumber
 */

int main(int argc, char *argv[])
	{
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buf[1024], buff[1024];

	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}
	/* Connect socket using name specified by command line. */
	server.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == 0) {
		write(STDOUT_FILENO,"unknown host",sizeof("unknown host"));
		exit(2);
	}
	bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
	server.sin_port = htons(atoi(argv[2]));

	if (connect(sock,(struct sockaddr *) &server,sizeof(server)) < 0) {
		perror("connecting stream socket");
		exit(1);
	}
	while(1) {
        memset(buff, 0, sizeof(buff));
        memset(buf, 0, sizeof(buf));
        int wr = write(STDOUT_FILENO, "Input: ", sizeof("Input: "));
        if (wr < 0){
            perror("write");
            exit(0);
        }

        int rd = read(STDIN_FILENO, buff, sizeof(buff));
        if (rd < 0) {
            perror("read");
            exit(0);
        }

		if (strcmp(buff, "exit\n") == 0) {
			write(STDOUT_FILENO, "Disconnected from server\n", strlen("Disconnected from server\n"));
			exit(0);
		}

        int s = send(sock, buff, sizeof(buff), 0);
        if (s < 0) {
            perror("send");
            exit(0);
        }
        int r = recv(sock, buf, sizeof(buf), 0);
        if (r < 0) {
            perror("recv");
            exit(0);
        }
        write(STDOUT_FILENO,buf, strlen(buf));
	}
	close(sock);
}
