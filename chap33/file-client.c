/*
 * client.c -- a stream socket client to use with the file-server program
 *
 * usage: ./client <hostname> <port>
 *
 * exit the client by using 'exit' command while it's running
 *
 * while it's running, give a file path, it will send it to the server
 * the server will attempt to open it, read it into a buffer, and send the 
 * buffer back. the client then reads from the buffer to stdout
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXDATASIZE 100  // max number of bytes at once
#define MAXMSG 25
#define MAXDATA 4096
#define SA struct sockaddr


// clean_str: make sure the string doesn't have junk spaces around it
void clean_str(char *s)
{
	size_t len = strlen(s);
	char tmp[MAXMSG] = {0};
	strncpy(tmp, s, len-1);
	memset(s, 0, len);
	strncpy(s, tmp, len-1);

}

int main(int argc, char **argv)
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	char file_request[MAXMSG] = {0};
	char file_buf[MAXDATA];

	if (argc != 3) {
		fprintf(stderr, "usage: client <hostname> <port>\n");
		exit(EXIT_FAILURE);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	// load the struct
	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(EXIT_FAILURE);
	}

	// loop trhough all results and connect to the first one we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		// if we make it here, we've got a connection
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		exit(EXIT_FAILURE);
	}

	inet_ntop(p->ai_family, (SA*)&p->ai_addr, s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo);

	// stay connect until client exits
	int n;
	while (1) {
		// make sure everything is cleared to minimize issues
		memset(file_buf, 0, MAXDATA);  
		memset(file_request, 0, sizeof MAXMSG);
		numbytes = 0;
		// get client request from stdin
		int b = read(STDIN_FILENO, file_request, MAXMSG);

		clean_str(file_request);

		if (strcmp(file_request, "exit") == 0) {
			close(sockfd);
			exit(EXIT_SUCCESS);
		}

		// send the request to the server
		if ((numbytes = send(sockfd, file_request, strlen(file_request), 0)) < 0) {
			perror("send");
			exit(EXIT_FAILURE);
		}

		// now we wait for a response
		while ((n = read(sockfd, file_buf, MAXDATA-1)) > 0)
			printf("%s\n", file_buf);
		if (n < 0) {
			perror("read");
		}
	}

	return 0;
}
