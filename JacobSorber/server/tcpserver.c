#include "common.h"
#include "strings.h"

int main(int argc, char **argv)
{
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    uint8_t buff[MAXLINE + 1];
    uint8_t recvline[MAXLINE + 1];

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        err_n_die("Socket Error!");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERVER_PORT);

    if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0) {
        err_n_die("Bind Error!");
    }

    if ((listen(listenfd, 10)) < 0) {
        err_n_die("Listen Erro!");
    }

    while (1) {
        struct sockaddr_in addr;
        socklen_t addr_len;

        // accept blocks until an incoming connection arrives
        // it returns a "file descriptor" to the connection
        printf("waiting for a connection on port %d\r\n", SERVER_PORT);
        fflush(stdout);
        connfd = accept(listenfd, (SA *) NULL, NULL);

        // zero out the receive buffer to make sure it ends up null terminated
        memset(recvline, 0, MAXLINE);

        // Now read the clinet's message.
        while ((n = read(connfd, recvline, MAXLINE - 1)) > 0) {
            fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);
            // hacky way to detect the end of the message.
            if (recvline[n - 1] == '\n') {
                break;
            }
            memset(recvline, 0, MAXLINE);
        }
        if (n < 0) {
            err_n_die("Read Error!");
        }

        // now send a respones.
        snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello Masson, I am the server!");

        // Normally, you may wat to chek the results form write and close
        // in case errors occur. For now, I'm ignoring them.
        write(connfd, (char*)buff, strlen((char*)buff));
        printf("Dissconnect\r\n");
        close(connfd);
    }
    // exit(0);
}
