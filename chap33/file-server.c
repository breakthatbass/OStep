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

#include "tcp.h"

// get_file: open file, read contents info a buffer, return buffer
char *get_file(const char *path) {
	int n, bytes;
	static char buf[MAXDATA] = {0};
	memset(buf, 0, MAXDATA);
	// try to open file
	n = open(path, O_RDONLY);
	if (n < 0) {
		printf("problem opening file\n");
		return NULL;
	}

	// if exists, read it into buffer on
	bytes = read(n, buf, sizeof buf-1);
	if (bytes < 0) {
		printf("problem reading file\n");
		return NULL;
	}
	close(n);
	return buf;
}

// clean_str: make sure the string doesn't have junk spaces around it
void clean_str(char *s)
{
	size_t len = strlen(s);
	char tmp[25] = {0};
	strncpy(tmp, s, len-1);
	memset(s, 0, len);
	strncpy(s, tmp, len-1);
}


int main()
{
	int sockfd, filefd;
	struct sockaddr_in servaddr;
	struct sockaddr_storage client_addr;
	socklen_t len;
	int nbytes;
	char file_request[FILENAME] = {0};	// buf to hold client's request string

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

	if (listen(sockfd, 10) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("server running and waiting for connection...\n");

	int status;
	int open = 1; // keep track if there's an accepted() fd
	char *open_file, *prb;
	
	while (1) {
		// clear the file_request buffer
		memset(file_request, 0, FILENAME);
		open_file = NULL;

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
			if (!fork()) {
				
				clean_str(file_request);
				printf("recieved request from client\nFILE: %s\n", file_request);
				
				open_file = get_file(file_request);
		
				
				if (open_file != NULL) {
					if (send(filefd, open_file, strlen(open_file), 0) < 0) {
						perror("send");
					}
					printf("FILE: %s sent succesfully\n", file_request);
				} else {
						memset(file_request, 0, FILENAME);
						memset(&open_file, 0, sizeof open_file);
						open_file = NULL;
						prb = "problem opening file";
						send(filefd, prb, strlen(prb), 0);
					}
			}
		
			if (wait(&status) >= 0) {
				perror("wait");
				exit(EXIT_FAILURE);
			}
		}
	}
	close(sockfd);
	return 0;
}

