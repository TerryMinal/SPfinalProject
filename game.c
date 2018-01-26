#include <ctype.h>
#include "networking.h"
#include "command.c"


void remove_end_newline(char *str);
void game();
void talk_to_client(int client_socket);
void talk_to_server(int server_socket);

void remove_end_newline(char *str) {
  str[strlen(str) - 1] = 0;
}
/*
Function to setup server for clients to join
*/
void game() {
  char str[5];
  printf("1: Host game\n2: Join game\n");
  while (fgets(str,sizeof(str), stdin) ) {
    str[strlen(str)-1] = 0; //Stripping input of the "\n"
    if (strcmp(str, "1") == 0) { //user host game
      char res[5];
      printf("You will need to acquire your IP address and send it to your friend. You can do this by running [ifconfig]\n1: Done\n");
      fgets(res, sizeof(res), stdin);
      remove_end_newline(res);
      if (strcmp(res, "1") == 0) { // user should've set up server by now
        printf("waiting on player to join...\n");
        int listen_socket = server_setup();
        int client_socket = server_connect(listen_socket);
        //random directory setup goes here
        talk_to_client(client_socket);
      }
    } //end of host game
    else if (strcmp(str, "2") == 0) { //user
      char res[100];
      printf("Enter ip address/domain of server:\n");
      fgets(res, sizeof(res), stdin);
      remove_end_newline(res);
      int server_socket = client_setup(res); //connect client to hosting computer
      printf("connected to host\n");
      talk_to_server(server_socket);
    } //end of join game
    else {
      printf("Not a valid input\n");
    } //if user inputs anything other than 1 or 2
  } //end while
} //end of game_setup()

void talk_to_client(int client_socket) {
  char buffer[BUFFER_SIZE];
  int state = 0;
  char *ans;
  printf("talk to your drivee\n");
  while (1) {
    read(client_socket, buffer, sizeof(buffer));
    if (state == 1) {
      int i = 0;
      for(; buffer[i]; i++) {
        buffer[i] = tolower(buffer[i]);
      } // make into all lowercase
      if (strcmp(ans,buffer) == 0) {
        //send next letter to the drivee... drivee then has to send it back to the driver
        //drivee then tells driver where next to go
        state = 0;
      }
    }
    else if (state == 0) {
      printf("player2: %s\n", buffer);
      fgets(buffer, sizeof(buffer), stdin); //gets input from user
      remove_end_newline(buffer);
      if (execute_args(buffer)) {
        if (strstr(buffer, "cd") != NULL) { //if user cd's into a directory, send the drivee a question
          //send a question to the drivee
          //keep answer to that question in this string var
          // ans = ;
          state = 1;
        }
      }
      else
        write(client_socket, buffer, sizeof(buffer)); // writes to other user
    }
  }
  close(client_socket);
  exit(0);
} //end of talk to client

void talk_to_server(int server_socket) {
  char buffer[BUFFER_SIZE];
  printf("talk to your driver\n");
  while (1) {
    fgets(buffer, sizeof(buffer), stdin);
    // *strchr(buffer, '\n') = 0;
    remove_end_newline(buffer);
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("player1: %s\n", buffer);
  }
  close(server_socket);
  exit(0);
} //end of talk to server

int main() {
  game();
}
