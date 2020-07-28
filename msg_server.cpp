
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8080

/*

*/
void communicate(int sockfd) {
  char buffer[MAX]; // text-storage memory
  int n; // buffer iterator

  for (;;) {
    bzero(buffer, MAX); // clear buffer

    read(sockfd, buffer, sizeof(buffer)); // receive from client

    printf("From client: %s\t To client : ", buffer); // print client msg

    bzero(buffer, MAX); // clear buffer
    n = 0; // reset iterator

    while ((buffer[n++] = getchar()) != '\n') {} // get user input

    write(sockfd, buffer, sizeof(buffer)); // send message to client

    if (strncmp(buffer, "exit", 4) == 0) {
      printf("Server Exit...\n");
      break;
    }
  }
}

/*

  print msg to console and exit program

*/
void break_error(const char * msg) {
  printf("%s", msg);
  exit(0);
}

/*

*/
int construct_bytestream_socket() {
  int bs_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (bs_socket < 0) {
    break_error("Socket creation failed...\n");
  } else {
    printf("Socket successfully created.\n");
  }

  return bs_socket;
}

/*

*/
struct sockaddr_in construct_socket_address(int ip, int port) {
  struct sockaddr_in address;
  bzero(&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = ip;
  address.sin_port = port;

  return address;
}

/*

*/
void bind_socket(int bs_socket, struct sockaddr_in address) {
  int success = bind(bs_socket, (struct sockaddr *) &address, sizeof(address));

  if (success != 0) {
    break_error("Socket bind failed...\n");
  } else {
    printf("Socket successfully bined.\n");
  }
}

/*

*/
int construct_server(int port) {
  int server_fd;
  struct sockaddr_in address;

  server_fd = construct_bytestream_socket();

  address = construct_socket_address(htonl(INADDR_ANY), htons(port));

  bind_socket(server_fd, address);
  return server_fd;
}

/*

*/
void listen_for_connection(int bs_socket, int backlog) {
  int success = listen(bs_socket, backlog);

  if (success != 0) {
    break_error("Listen failed...\n");
  } else {
    printf("Server listening...\n");
  }
}

/*

*/
int accept_connection(int bs_socket) {
  struct sockaddr_in client;
  socklen_t len = sizeof(client);
  int client_socket = accept(bs_socket, (struct sockaddr *) &client, &len);

  if (client_socket < 0) {
    break_error("Server accept failed...\n");
  } else {
    printf("Server accept the client...\n");
  }

  return client_socket;
}

/*

*/
int await_client_connection(int server_fd) {
  int connected_fd;

  listen_for_connection(server_fd, 5);
  connected_fd = accept_connection(server_fd);

  return connected_fd; // return client-socket
}

/*

*/
int main() {

  int server_fd = construct_server(PORT);
  int client_fd = await_client_connection(server_fd);

  communicate(client_fd); // start message excange with client

  close(client_fd); // close client socket
  close(server_fd);

  return 0;
}
