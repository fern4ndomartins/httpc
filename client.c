#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8080

int main() {
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("failed to create socket!");
  }
  struct sockaddr_in *addr = malloc(sizeof(struct sockaddr_in));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(PORT);
  addr->sin_addr.s_addr = inet_addr("127.0.0.1");
  printf("ready to connect to %s\n", inet_ntoa(addr->sin_addr));
  int connection = connect(sockfd, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
  if (connection) {
    perror("failed to connect");
  } else {
    printf("connected\n");
  }
  while(1) {
    char buffer[1024];
    size_t size = 3;
    char * message = "ok\n";
    send(sockfd, message, size, 0);
    int n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n == 0) {
      printf("connection closed\n");
      break;
    }
    buffer[n] = '\0';
    printf("%s", buffer);
  }
}
