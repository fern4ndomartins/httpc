#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>

#define PORT 8080

struct new_connection_data {
  int sockfd;
  int addrlen;
  struct sockaddr_in addr;
};

void parse_request() {
}

void * handle_connection(void *data) {
  intptr_t new_connection = (intptr_t)data;
  while (1) {
    char buf[1024] = {0};
    int n = recv(new_connection, buf, 1024, 0);
    if (n < 0) {
      break;
    }

    buf[n] = '\0';
    printf("request: \n%s\n", buf);

    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
    int writen = write(new_connection, response, strlen(response));
    if (writen<0) {
      printf("connection closed unexpectedly");
      close(new_connection);
    }
  }
  close(new_connection);

  return NULL;
}

int main() {
    signal(SIGPIPE, SIG_IGN);
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
        int new_connection;
        if ((new_connection = accept(sockfd, (struct sockaddr *)&addr, (socklen_t *)&addrlen)) < 0) {
          perror("failed to accept connection. :((");
          return -1;
        }
        printf("accepted one connection");
        pthread_t new_connection_thread;
        pthread_create(&new_connection_thread, NULL, handle_connection, (void*)new_connection);
        pthread_detach(new_connection_thread);
    }
}
