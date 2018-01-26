#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define STR_SPACE " "
#define STR_SEMICOLON ";"

// prints all elements of an array
void print_args(char **arr) {
  int i = 0;
  while ( *(arr + i) != NULL ) {
    printf("command: %s\n", arr[i]);
    i++;
  }
}

// separates a string based on delimteer and makes it into an array
// returns an array
char ** parse_args(char * line, char * delimiter ) {
  char **args = calloc(100, sizeof(char *));
  int i = 0;
  char *comm;
  while (line != NULL) {
    comm = strsep(&line, delimiter);
    if (strlen(comm) != 0) { //adds to array only if it is not an empty string
      args[i] = comm;
      i++;
    }
  }
  args[i] = NULL;
  return args;
}

//removes leading and trailing spaces of a given command
// assumes no spaces are in the string other than the beginning and the end
// char * trim_spaces(char *str) {
//   char **ret = parse_args(str, STR_SPACE);
//   return ret[0];
// }
//
// // trims all the leading and trailing spaces of a list of commands
// void arr_trim_spaces(char **arr) {
//   int i = 0;
//   while ( *(arr + i) != NULL ) {
//     *(arr + i) = trim_spaces(*(arr + i));
//     i++;
//   }
// }


// given array of commands such as [ls -al, cd .., less main.c]
// executes all the given commands
// array can be obtained through parse_args(args, STR_SEMICOLON)
int execute_args(char *str) {
  char *EXIT = "exit";
  char *CD = "cd";
  char **arg; //array of single command
  int status; // status of exit
  int i = 0; // incrementor for array
  int p; // used for forking

  arg = parse_args(str, STR_SPACE); //splits into command and args in non redirectional commands
  // print_args(arg);
  // trim_spaces(arg[0]);
  //checking for special cases exit and cd
  if (strcmp (EXIT, arg[0]) == 0 ) { // checking for exit command case
    printf("User exited!\n");
    exit(1);
  }
  else if (strcmp (arg[0], CD) == 0) { // checking for cd command case
    chdir(arg[1]); // changes current directory
    return 1;
  }
  //finished checking for special cases
  else if (strcmp(arg[0], "ls") == 0) {
    p = fork();
    if (p == 0) { // child process
      // trim_spaces(arg[1]);
      arg[1] = NULL;
      execvp(arg[0], arg); //prevents user from using al and seeing size of directory
      exit(1); //quits the child process
    }
    else {
      wait(&status);
      return 1;
    }
  }
  else { //meant to prevent users from using any command other than cd and ls
    // printf("that is not a valid command\n");
    return 0;
  }
} //end of execute_args

// int main() {
//   char str[100];
//   char ** args;
//   char * exit = "exit";
//
//   while( fgets(str,sizeof(str), stdin) ) {
//     str[strlen(str)-1] = 0; //Stripping input of the "\n"
//     // args = parse_args(str, STR_SEMICOLON); //separates the commands. Will provide one command when there is only one command
//     execute_args(str); //run all the commands obtained above
//   }
//   return 0;
// }
