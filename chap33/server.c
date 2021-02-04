/***
 * ultra simple TCP web server with error checking
 *	
 *	when the server starts up it saves the current time
 *	and date in a variable. at a successful connection to
 *	a client, it sends the data through.
 *
 *	you can connect to this server through something like
 *	telnet. if you don't have it use brew install telnet
 *
 *	$ telnet hostname PORT
 *	should connect and you'll see the data
 *
 *	or you can wait until i write a client and push it
 *
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 3490
#define BACKLOG 10
#define MAXDATA 100
#define SA struct sockaddr

int main()
{
	int sockfd, newfd;
	//struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in servaddr;
	struct sockaddr_storage client_addr;
	socklen_t len;
	int rv;

	// current time and date
	// set it in a var to send through the socket
	time_t t;
	time(&t);
	char time[MAXDATA];
	strcpy(time, "🚀 ");
	strcat(time, ctime(&t));

	// clear servaddr struct
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;		// set to IPv4 for simplicity
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // use my IP
	servaddr.sin_port = htons(PORT);		// short, network byte order
	
	// create the socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	// bind the socket to the PORT 
	if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// since we a re not connecting to a remote host
	// we will listen on our own system
	if (listen(sockfd, BACKLOG) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	printf("server waiting for connection...\n");
	
	// we're just going to try to connect to one client
	// send the message once and quit
	len = sizeof client_addr;
	newfd = accept(sockfd, (SA*)&client_addr, &len);
	if (newfd < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	if (send(newfd, time, strlen(time), 0) < 0)
		perror("send");
	close(newfd);
	close(sockfd);
	return 0;
}
