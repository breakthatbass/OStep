/**
 * use this server with a client like telnet
 *
 * by using select() it is able to connect to multiple clients
 * at once and handle everything.
 *
 * this is pretty much copied out of Beej's Guide
 *
 * to use it, run the server, then open a bunch of
 * terminal windows and connect with telnet
 *
 * it will then read from stdin on telnet and the message
 * will send to each client
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

#define PORT "3490"
#define MAXDATA 256
#define SA struct sockaddr

int main()
{
	fd_set master;				// master file descriptor list
	fd_set read_fds;			// temp file descriptor list for select()
	int fdmax;					// max file descriptor number

	int listener;				// listening socket descriptor
	int newfd;					// newly accept()ed file descriptor
	struct sockaddr_storage remoteaddr;		// client address
	socklen_t addrlen;

	char buf[MAXDATA];		// buffer for client data
	int nbytes;

	char remote_ip[INET6_ADDRSTRLEN];

	int yes = 1;
	int i, j, rv;

	struct addrinfo hints, *ai, *p;

	FD_ZERO(&master);			// clear the master and temp sets
	FD_ZERO(&read_fds);

	// get us a socket and bind it
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;		// IPv4 for simplicity
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	// fill the struct
	if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if (bind(listener, ai->ai_addr, ai->ai_addrlen) < 0) {
		close(listener);
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(listener, 10) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// add the listener to the master set
	FD_SET(listener, &master);

	// keep track of the biggest file descriptor
	fdmax = listener; // so far, it's this one

	// main event loop
	while (1) {
		read_fds = master;	// copy it
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) < 0) {
			perror("select");
			exit(EXIT_FAILURE);
		}
		// run through existing connections looking for data to read
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) {		// got one
				if (i == listener) {
					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(listener, (SA*)&remoteaddr, &addrlen);
					if (newfd < 0) {
						perror("accept");
					} else {
						FD_SET(newfd, &master);		// add to master set
						if (newfd > fdmax) {			// keep track of the max
							fdmax = newfd;
						}
						printf("select server: new connection on socket %d\n", newfd);
					}
				} else {
					// handle data from a client
					if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
						// got error or connection closed by client
						if (nbytes == 0) {
							// connection closed
							printf("select server: socket %d hung up\n", i);
						} else {
							perror("recv");
						}
						close(i);
						FD_CLR(i, &master);  // remove from master set
					} else {
						// we got some data from a client
						for (j = 0; j <= fdmax; j++) {
							// send to everyone
							if (FD_ISSET(j, &master)) {
								// except the listener and ourselves
								if (j != listener && j != i) {
									if (send(j, buf, nbytes, 0) < 0) {
										perror("send");
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 0;
}
