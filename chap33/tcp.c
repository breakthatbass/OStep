/******************************************************************************
*
*   I mostly learned from this YouTube video from Jacob Sorber:
*   https://www.youtube.com/watch?v=esXw4bdaZkc&t=506s
*
*   I also followed this tutorial: https://ops.tips/blog/a-tcp-server-in-c/
*
*   this program gets the current time, day, and date and send it as as
*   string through the socket. to see it, open up a browser and type 
*   localhost:8080 in the url bar. you will see that the data was sent
*   and recieved!
*   
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define MAXLINE 40

#define SA struct sockaddr

int main(int argc, char **argv)
{
    int sockfd, n, connfd;
    struct sockaddr_in servaddr = {0};
    socklen_t len;
    time_t t;
    time(&t);   // get current time to use with the server
    char send[MAXLINE];
    char recv[MAXLINE];
    fd_set afds, rfds;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    // bind is used for a local connection
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        exit(2);
    }
    // kind of weird that bind doesn't take care of this
    // but listen just says, 'yes, sockfd is listening...'
    if ((listen(sockfd, 8)) < 0) {
        perror("listen");
        exit(3);
    }

    while (1) {
        struct sockaddr_in addr;
        socklen_t addlen;

        printf("waiting...\n");
        fflush(stdout);

        // now accept connections
        connfd = accept(sockfd, (SA*) NULL, NULL);

        memset(recv, 0, MAXLINE);

        while ((n = read(connfd, recv, MAXLINE-1)) > 0) {
            fprintf(stdout, "%s\n", recv);
            if (recv[n-1] == '\n') {
                break;
            }
            memset(recv, 0, MAXLINE);
        }
        if (n < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        strcpy(send, ctime(&t));

        n = write(connfd, (char*)send, strlen((char*)send));
        if (n < 0) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(connfd);
    }

    return 0;
}