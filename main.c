#include "networking.h"
/*
Function to setup server for clients to join
*/
void game_setup() {
  char str[5];
  printf("1: Host game\n2: Join game\n");
  while (fgets(str,sizeof(str), stdin) ) {
    if (strcmp(str, "1")) {
      char res[5];
      printf("please create a server by running [make server] in a new terminal\n");
      printf("You will need to acquire your IP address. You can do this by running [ifconfig]\n1: Done\n");
      fgets(res, sizeof(res), stdin);
      if (strcmp(res, "1")) { // user should've set up server by now
        client_setup("127.0.0.1"); // connects client to local
        break;
      }
    } //end of host game
    else if (strcmp(str, "2")) {
      char res[100];
      printf("enter ip address/domain of server:\n");
      fgets(res, sizeof(res), stdin);
      client_setup(res); //connect client to hosting computer
      break;
    } //end of join game
    else {
      printf("Not a valid input\n");
    } //if user inputs anything other than 1 or 2
  } //end while
} //end of game_setup()


int main() {
  game_setup();
}