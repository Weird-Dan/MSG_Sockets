
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080

/*

*/
void communicate(int sockfd) {

  char buffer[MAX]; // text-storage memory
  int n; // buffer iterator

  for (;;) {
    bzero(buffer, sizeof(buffer)); // clear buffer
    printf("Enter the string : ");

    n = 0;
    while ((buffer[n++] = getchar()) != '\n') {} // get input from user

    write(sockfd, buffer, sizeof(buffer)); // send to server

    bzero(buffer, sizeof(buffer)); // clear buffer
    read(sockfd, buffer, sizeof(buffer)); // receive from server

    printf("From Server : %s", buffer); // print server msg

    if (strncmp(buffer, "exit", 4) == 0) {
      printf("Client Exit...\n");
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
    printf("Socket successfully created..\n");
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
void connect_to_server(int server_fd, struct sockaddr_in address) {
  int success = connect(server_fd, (struct sockaddr *) &address, sizeof(address));

  if (success != 0) {
    break_error("Connection with the server failed...\n");
  } else {
    printf("Connected to the server..\n");
  }
}

/*

*/
int start_client(int ip, int port) {
  int socket_fd;
  struct sockaddr_in address;

  socket_fd = construct_bytestream_socket();

  address = construct_socket_address(ip, htons(port));

  connect_to_server(socket_fd, address);

  return socket_fd; // return the connected socket
}

/*

*/
int main() {

  int server = start_client(inet_addr("127.0.0.1"), PORT); // start a connection

  communicate(server); // start message excange with server

  close(server); // close connection

  return 0;
}
