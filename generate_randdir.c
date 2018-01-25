#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>


int setup_directories() {
  pid_t f1;
  char alphabet[] = "abcedfghijklmnopqrstuvwxyz";
  char * name = &(alphabet[ (int) (rand()%26) ]);
  printf("%s\n", name);

  int rannum;
  srand(time(NULL));
  int rand_num1 = 2; //rand()%10;

  printf("Rand num1: %d\n", rand_num1);
  int rand_num2 = 1; //rand()%10;
  int rand_num2_copy = rand_num2;
  printf("Rand num2: %d\n", rand_num2);



  for (; rand_num1 > 0; rand_num1--) {
    //printf("Randnum1: %d\n", rand_num1);
    char * name = &(alphabet[ (int) (rand()%26) ]);
    printf("%s\n", name);
    char dir_1[1024] = "./";
    strcat(dir_1, name);
    printf("Dirname: %s\n", dir_1);
    mkdir(dir_1, 0700);

    int ret = chdir(name);
    printf("Return: %d\n", ret);

    /*
    char * const argv[] = {"ls", "-l"};

    f1 = fork();

    if (f1 == 0) {
    */
    for (; rand_num2 > 0; rand_num2--) {
      printf("Making directories\n");

      //Name of directory
      char * name = &(alphabet[ (int) (rand()%26) ]);

      //Formatting name of directory
      char dir[1024] = "./";
      strcat(dir, name);
      printf("Dirname: %s\n", dir);

      //Making directory
      mkdir(dir, 0700);

      /*
      int exec_ret = execvp("ls", argv);
      printf("exec_ret: %d\n", exec_ret);
      */


      //printf("Randnum2: %d\n", rand_num2);
    }
    char * root = "../";
    int back = chdir(root);
    printf("Went back to root: %d\n", back);
    rand_num2 = rand_num2_copy;
  }
}



int main() {
  setup_directories();
}
