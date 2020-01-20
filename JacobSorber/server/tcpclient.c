#include "common.h"
#include "strings.h"

int main (int argc, char **argv)
{
    int sockfd, n;
    int sendbytes;
    struct sockaddr_in  servaddr;
    char    sendline[MAXLINE];
    char    recvline[MAXLINE];

    if (argc != 2) {
        err_n_die("usage: %s <server address>", argv[0]);
    }

    printf("Begin\r\n");

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_n_die("Error While creating the socket!");
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT); // chat server

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        err_n_die("inet_pton error for %s\r\n", argv[1]);
    }

    if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) < 0) {
        err_n_die("connect failed!");
    }

    // We're connected. Prepare the message.
    sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
    sendbytes = strlen(sendline);

    if (write(sockfd, sendline, sendbytes) != sendbytes) {
        err_n_die("Write Error");
    }

    memset(recvline, 0, MAXLINE);

    // Now read the server's response
    while ((n = read(sockfd, recvline, MAXLINE - 1)) > 0) {
        printf("%d\r\n", n);
        printf("%s\r\n", recvline);
        memset(recvline, 0, MAXLINE);
    }

    if (n < 0) {
        err_n_die("Read Error");
    }

    printf("Done\r\n");
    exit (0);
}
