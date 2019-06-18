#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)


void print_output(int width, int height, char **p);

typedef struct coordinate {
  int x;
  int y;
} coordinate;

typedef struct server_message {
  coordinate pos;
  coordinate adv_pos;
  int object_count;
  coordinate object_pos[4];
} server_message;

int main()
{
  int i, j, c;
  int width, height;
  int num_obstacle;
  int obs1, obs2;
  int num_hunters;
  int hunt1, hunt2, hunt_energy;
  int num_prey;
  int prey1, prey2, prey_energy;
  char **output;
  int **hunter, **prey, **obstacle;

  scanf(" %d %d", &width, &height);

  output = (char**)calloc(height, sizeof(char*));

  for(i=0; i<height; i++){
    output[i] = (char*)calloc(width, sizeof(char));
  }

  for(i=0; i<height; i++){
    for(j=0; j<width; j++){
      output[i][j] = ' ';
    }
  }

  scanf(" %d", &num_obstacle);

  obstacle = (int**)calloc(num_obstacle, sizeof(int*));

  for(i=0; i<num_obstacle; i++){
      scanf(" %d %d", &obs1, &obs2);
      output[obs1][obs2] = 'X';

      obstacle[i] = (int*)calloc(2, sizeof(int));
      obstacle[i][0] = obs1;
      obstacle[i][1] = obs2;
  }

  scanf(" %d", &num_hunters);

  hunter = (int**)calloc(num_hunters, sizeof(int*));

  for(i=0; i<num_hunters; i++){
      scanf(" %d %d %d", &hunt1, &hunt2, &hunt_energy);
      output[hunt1][hunt2] = 'H';

      hunter[i] = (int*)calloc(3, sizeof(int));
      hunter[i][0] = hunt1;
      hunter[i][1] = hunt2;
      hunter[i][2] = hunt_energy;
  }

  scanf(" %d", &num_prey);

  prey = (int**)calloc(num_prey, sizeof(int*));

  for(i=0; i<num_prey; i++){
      scanf(" %d %d %d", &prey1, &prey2, &prey_energy);
      output[prey1][prey2] = 'P';

      prey[i] = (int*)calloc(3, sizeof(int));
      prey[i][0] = prey1;
      prey[i][1] = prey2;
      prey[i][2] = prey_energy;
  }

  //print_output(width, height, output);


  int filedes[num_prey + num_hunters+1][2];

  for(i=0; i<(num_prey+num_hunters); i++){

    if(PIPE(filedes[i][0]) < 0){
      perror("error");
    }

    if((c = fork()) == -1){
      perror("error");
    }

    else if(c == 0){    //child
      close(filedes[i][0]);
      dup2(filedes[i][1], 1);
      dup2(filedes[i][1], 0);

      execl("./hunter", "./hunter", (char*)0);

      close(filedes[i][1]);
    }

    else{   //parent
      sleep(3);
      close(filedes[i][1]);



      if(write(socket[i][0], &server_message, sizeof(server_message)) < 0)
        perror("error");
      if(read(socket[i][0], &coordinate, sizeof(coordinate)) < 0)
        perror("error");

        close(filedes[i][0]);
    }

  }



  return 0;
}

void print_output(int width, int height, char **p)
{
  int i, j;

  printf("+");
  for(i=0; i<width; i++){
    printf("-");
  }
  printf("+\n");

  for(i=0; i<height; i++){
    printf("|");
    for(j=0; j<width; j++){
      printf("%c", p[i][j]);
    }
    printf("|\n");
  }

  printf("+");
  for(i=0; i<width; i++){
    printf("-");
  }
  printf("+\n");
}
