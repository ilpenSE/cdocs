#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8000
#define BUFFER_SIZE 4096

int main(int argc, char** argv) {
  printf("Starting server...\n");
  
  // intializing variables
  int server_fd, client_fd;
  struct sockaddr_in address;
  socklen_t addr_len = sizeof(address);
  char buffer[BUFFER_SIZE];
  memset(&address, 0, sizeof(address));

  // create socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket");
    exit(1);
  }

  // socket config
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // bind socket
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind");
    exit(1);
  }

  // start listening
  if (listen(server_fd, 10) < 0) {
    perror("listen");
    exit(1);
  }

  printf("HTTP server running on port %d\n", PORT);


  // ready to get client requests
  while (1) {
    client_fd = accept(server_fd, (struct sockaddr*)&address, &addr_len);
    if (client_fd < 0) {
      perror("accept");
      exit(1);
    }

    recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    printf("Request:\n%s\n", buffer);

    const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, World!";

    send(client_fd, response, strlen(response), 0);
    close(client_fd);
  }

  close(server_fd);
  return 0;
}
