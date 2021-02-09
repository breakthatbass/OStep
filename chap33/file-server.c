/**
 * file-server.c -- a stream socket server to use with the file-client program
 *
 * usage: ./file-server
 *
 * that will get the server running then the client can connect.
 * the purpose is for the client to send a file path, this server then
 * attempts to open that file, and if it can, reads it into a buffer
 * and send the buffer back to the client.
 * the client then displays it.
 *
 * exit with ctrl-c for now
 *
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 3490
#define MAXDATA 4096
#define FILENAME 256
#define SA struct sockaddr		// for less messy casting

// get_file: open file, read contents info a buffer, return buffer
char *get_file(const char *path) {
	int n, bytes;
	static char buf[MAXDATA];

	// try to open file
	n = open(path, O_RDONLY);
	if (n < 0) {
		strcpy(buf, "problem opening file");
		printf("%s\n", buf);
		return buf;
	}

	// if exists, read it into buffer on
	bytes = read(n, buf, sizeof buf-1);
	if (bytes < 0) {
		strcpy(buf, "problem reading file");
		printf("%s\n", buf);
		return buf;
	}
	close(n);
	return buf;
}


int main()
{
	int sockfd, filefd;
	struct sockaddr_in servaddr;
	struct sockaddr_storage client_addr;
	socklen_t len;
	int nbytes;
	char file_request[FILENAME];	// buf to hold client's request string

	// clear servaddr struct
	memset(&servaddr, 0, sizeof servaddr);
	servaddr.sin_family = AF_INET;	// IPv4 for simplicity
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);		// use my IP
	servaddr.sin_port = htons(PORT);	// short, network by order

	// create socket file descriptor
	// @param3 is the protocol. 0 means TCP
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// bind the socket to the PORT
	if (bind(sockfd, (SA*)&servaddr, sizeof servaddr) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// this prevents the 'bind: address already in use' issue
	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) < 0) {
		perror("setsocket");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 10) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	
	printf("server running and waiting for connection...\n");

	int open = 1; // keep track if there's an accepted() fd
	char *open_file;
	while (1) {
		// clear the file_request buffer
		memset(file_request, 0, FILENAME);
		memset(&open_file, 0, sizeof open_file);
		nbytes = 0;
		if (open) {
			// we're only going to connect to one client for now
			len = sizeof client_addr;
			filefd = accept(sockfd, (SA*)&client_addr, &len);
			if (filefd < 0) {
				perror("accept");
				continue;
			} else {
				printf("connected to a client\n");
				open = 0; // keep track that there's an open fd
			}
		}
		
		// recieve data from a client
		if ((nbytes = recv(filefd, file_request, sizeof file_request, 0)) <= 0) {
			// got error or connection was closed by client
			if (nbytes == 0) {
				printf("file-server: client hung up\n");
				close(filefd);
				open = 1;
				continue;
			} else {
				perror("recv");
				close(filefd);
				open = 1;
				continue;
			}
			close(filefd);
		} else {
			// we got some data
			// manage it and get file contents
			open_file = get_file(file_request);
			if (strcmp(open_file, "0") == 0) {
				continue;
			}
			if (send(filefd, open_file, strlen(open_file), 0) < 0) {
				perror("send");
				continue;
			}
		}
	}
	close(sockfd); 
	return 0;
}

