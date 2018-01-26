#include <ctype.h>
#include "networking.h"
#include "command.c"
#include <time.h>

char directory[1024] = "";
char * dir_pointer;

void remove_end_newline(char *str);
void game();
void talk_to_client(int client_socket, char* directory);
void talk_to_server(int server_socket);

void remove_end_newline(char *str) {
  str[strlen(str) - 1] = 0;
}
/*
  Function to setup server for clients to join
*/


char ** QnA_generator(char letter_dir) {

  char questions_file[4096] = "questions";
  char txt_ending[1024] = ".txt";
    
  letter_dir = toupper(letter_dir);    
  //printf("*dest: %c\n", letter_dir);
    
  char * letter_ptr = &letter_dir;

  //printf("letter: %c\n", *letter_ptr);
  strcat(questions_file, letter_ptr);
  strcat(questions_file, txt_ending);
  // printf("questions_file: %s\n", questions_file);

  char * root = "../";
  int back = chdir(root);
  int fd = open(questions_file, O_RDONLY);
  //printf("fd: %d\n", fd);

  char buffer[8192];
  read(fd, buffer, sizeof(buffer));

  //printf("buffer: %s", buffer);
  
  char ** arr = (char **) calloc(12, sizeof(buffer));
  char ** arr_ptr = arr;
  char * ptr = buffer;
      
  while (ptr != NULL) {
    //printf("%s\n", strsep(&ptr, "\n"));
    //The pointer points to a new location, which is why the address of it is passed into strsep
    *(arr) = strsep( &ptr, "\n");
    //printf("%s\n", *arr);
    arr++; 
  }
    
  int i = 0;
  int count = 0; 

  char ** arr_questions = (char **) calloc(12, sizeof(buffer));
  char ** arr_answers  = (char **) calloc(12, sizeof(buffer));
    
  arr = arr_ptr;
    
  while (arr[i+2]) {
    //printf("arr[%d]:%s\n", i, arr[i]);
    if (strcmp(arr[i], "") == 0) {
      //printf("Nothing here, breaking!\n");
      break;
    }
      
    if (i % 2 == 0) {
      arr_questions[count] = arr[i];
      arr_answers[count] = arr[i+1];
      //printf("arrQ[%d]:%s\n", count, arr_questions[count]);
      //printf("arrA[%d]:%s\n", count, arr_answers[count]);
      count++;
    }
    i++;
  }
    
  srand(time(NULL));
  int rand_num = rand()%5;
  //printf("\nrandQ: %d\n", rand_num);
  //printf("arrQ[%d]:%s\n", rand_num, arr_questions[rand_num]);
  //printf("arrA[%d]:%s\n", rand_num, arr_answers[rand_num]);

  char ** arr_QnA = (char **) calloc(2, sizeof(buffer));

  arr_QnA[0] = arr_questions[rand_num];
  arr_QnA[1] = arr_answers[rand_num];
  //printf("arrQ[0]:%s\n", arr_QnA[0]);
  //printf("arrA[0]:%s\n", arr_QnA[1]);
  


  free(arr_questions);
  free(arr_answers);
  free(arr);
  return arr_QnA;
}



void game() {
  char str[5];
  int setup = 0;
  int created = 0;
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
	
	printf("Type q to start\n");
	if (!setup) {
	  //Setting up random directories: 
	  char alphabet[] = "abcedfghijklmnopqrstuvwxyz";
	  char * name = &(alphabet[ (int) (rand()%26) ]);
	  //printf("%s\n", name);

	  //Generating the random number of directories being made
	  int rannum;
	  srand(time(NULL));
	
	  int rand_num1 = rand()%10;
	  //printf("Rand num1: %d\n", rand_num1);
	
	  int rand_num2 = rand()%10;
	  int rand_num2_copy = rand_num2;
	  //printf("Rand num2: %d\n", rand_num2);

	  int percentage = rand()%10;

	  for (; rand_num1 > 0; rand_num1--) {
	    //printf("Randnum1: %d\n", rand_num1);
	  
	    //Generating random name for directory
	    char * name = &(alphabet[ (int) (rand()%26) ]);
	    //printf("%s\n", name);
	  
	    //Formatting name of directory
	    char dir_1[1024] = "./";
	    strcat(dir_1, name);
	    //printf("Dirname: %s\n", dir_1);
	    mkdir(dir_1, 0700);

	    int ret = chdir(name);
	    //printf("Successful cd-ing into directory?: %d\n", ret);

	    for (; rand_num2 > 0; rand_num2--) {
	      //printf("Randnum2: %d\n", rand_num2);
	      //printf("Making directories\n");

	      //Generating random name for directory
	      char * name = &(alphabet[ (int) (rand()%26) ]);

	      //Formatting name of directory
	      char dir[1024] = "./";
	      strcat(dir, name);
	      //printf("Dirname: %s\n", dir);

	      //Making directory
	      mkdir(dir, 0700);

	      if (percentage < 6 && !(created)) {
		chdir(name);
		strcat(directory, name);
		//printf("dir: %s\n", directory);
		dir_pointer = directory;
		//printf("file made\n");
		int fd = open("victory_file.txt", O_RDWR | O_CREAT);
		//printf("fd: %d\n", fd);
		if (fd != -1) {
		  write(fd, "Congratulations, you've reached the correct destination!", 1024);
		  close(fd); 	 
		}
		created = 1;
	      }

	      else {
		percentage--;
	      }
		
	    }
	    
	    
	    char * root = "../";
	    int back = chdir(root);
	    //printf("Went back to root: %d\n", back);
	    rand_num2 = rand_num2_copy;
	  }
	}
	talk_to_client(client_socket, directory);
	//dir_pointer = directory;
      }
    }
	
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

void talk_to_client(int client_socket, char* directory) {
  char buffer[BUFFER_SIZE];
  int i = 0;
  int state = 0;
  char *ans;
  char * space = " ";
  char ** QnA_arr = (char **)calloc(2, 4096);
  printf("talk to your drivee\n");
  while (1) {
    //printf("dir: %s", dir_pointer);
    read(client_socket, buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin); //gets input from user
    
    
    if (state == 1) {
      printf("Answer:\n");
      printf("ans: %s", ans);
      //fgets(buffer, sizeof(buffer), stdin);

      int count = 0;
      for(; buffer[count]; count++) {
        buffer[count] = tolower(buffer[count]);
      } // make into all lowercase
      
      if (strstr(ans,buffer) != NULL) {
        //send next letter to the drivee... drivee then has to send it back to the driver
        //drivee then tells driver where next to go
	char letter = directory[i];
	char * letter_ptr = &letter;
	write(client_socket, letter_ptr, sizeof(buffer));
	directory++; 
        state = 0;
      }
    }

    printf("Please type q for question\n");
    if (state == 0) {
      printf("player2: %s\n", buffer);
      fgets(buffer, sizeof(buffer), stdin); //gets input from user
      remove_end_newline(buffer);
      
      execute_args(parse_args(buffer, space));

      if (strstr(buffer, "q") != NULL) {
	QnA_arr = QnA_generator(directory[i]); 
	printf("directory: %s\n", directory);
	printf("letter: %c\n", directory[i]);
	//printf("\nAsked FOR Question\n");
	//printf("Question:\n"); 
	//printf("Q: %s", QnA_arr[0]);
	printf("Question: %s\n", QnA_arr[0]);
	ans = QnA_arr[1];
	//state = 1;
      }
      
      else if (strstr(buffer, "cd") != NULL) { //if user cd's into a directory, send the drivee a question
	printf("Working cd\n");
	
	//state = 1;
      }
    
      
        write(client_socket, buffer, sizeof(buffer)); // writes to other user
    }
  }
  close(client_socket);
  free(QnA_arr);
  exit(0);
} //end of talk to client

void talk_to_server(int server_socket) {
  char buffer[BUFFER_SIZE];
  int i = 0;
  char * space = " ";
  printf("talk to your driver\n");
  while (1) {
    fgets(buffer, sizeof(buffer), stdin);
    // *strchr(buffer, '\n') = 0;
    
    remove_end_newline(buffer);
    execute_args(parse_args(buffer, space));
    write(server_socket, buffer, sizeof(buffer));
    
    read(server_socket, buffer, sizeof(buffer));
    printf("player1: %s\n", buffer);
    if (strstr(buffer, "q") != NULL) {
      printf("Press enter to give question\n");
      // printf("directory: %s", dir_pointer);
      //printf("\nAsked FOR Question\n");
      //printf("Question:\n");
      //printf("dir: %s", directory);
      //char * letter = &directory[i];
      //printf("dir: %s",letter);
      // QnA_arr = QnA_generator(*directory);
      //printf("Q: %s", QnA_arr[0]);
      //printf("Question: %s\n", QnA_arr[0]);
      //ans = QnA_arr[1];
    }
  }
  close(server_socket);
  exit(0);
} //end of talk to server

int main() {
  game();
  printf("dir: %s", directory);
}
