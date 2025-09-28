#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PORT 8080

void parse_request() {
}

void * handle_connection() {

}

int main() {
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!sockfd) {
        perror("failed to create socket!");   
    }
  //    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    struct sockaddr_in addr;
    int opt = 1;
    int addrlen = sizeof(addr);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("failed to set opt.");
    }     

    addr.sin_addr.s_addr = INADDR_ANY; 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&addr, addrlen)) {
        perror("binding to address failed.");
    }

    if (listen(sockfd, 3) < 0) {
        perror("listen failed.");
    }
    while (1) {
        printf("waiting for new connections\n");
        pthread new_connection_thread;
        int new_connection;
        if ((new_connection = accept(sockfd, (struct sockaddr *)&addr, (socklen_t *)&addrlen)) < 0) {
            perror("failed to accept connection. :((");
        }

        char buf[1024] = {0};
        int n = recv(new_connection, buf, 1024, 0);
        buf[n] = '\0';
        printf("request: \n%s\n", buf);

        char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
        write(new_connection, response, strlen(response));

        close(new_connection);
    }
}
