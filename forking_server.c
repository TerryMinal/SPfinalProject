#include "networking.h"

void talk(int client_socket);

int main() {

  int listen_socket;
  int f;
  listen_socket = server_setup();

  // while (1) {

    // blocks
    int client_socket = server_connect(listen_socket);
    talk(client_socket);
    // f = fork();
    // if (f == 0)
    //   subserver(client_socket);
    // else
    close(client_socket);
  // }
}
