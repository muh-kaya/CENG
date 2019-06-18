#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <string.h>
#include <poll.h>
#include <sys/time.h>

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

typedef struct ph_message {
	coordinate move_request;
} ph_message;

typedef struct object {
	int x;
	int y;
	int energy;
	char type;
	pid_t pid;
	int fd[2];
	bool dead;
} object;


int main()
{
	int i, j, a, b;
	int width, height;
	int num_obstacles, num_hunters, num_preys;
	int obs1, obs2, hunter1, hunter2, hunter_energy, prey1, prey2, prey_energy;
	struct object *objects;
	int **obstacle;

	scanf(" %d %d", &width, &height);

	char **output;
	output = (char **)malloc(height*sizeof(char *));

  	for(i=0; i<height; i++){
    	output[i] = (char *)malloc(width*sizeof(char));
  	}

	for(i=0; i<height; i++){
		for(j=0; j<width; j++){
			output[i][j] = ' ';
		}
	}

	scanf(" %d", &num_obstacles);

	obstacle = (int **)malloc(num_obstacles*sizeof(int*));
	for(i=0; i<num_obstacles; i++){
		scanf(" %d %d", &obs1, &obs2);

		obstacle[i] = (int *)malloc(2*sizeof(int));
		obstacle[i][0] = obs1;
		obstacle[i][1] = obs2;

		output[obs1][obs2] = 'X';
	}

	scanf(" %d", &num_hunters);
	objects = malloc(num_hunters*sizeof(object));

	for(i=0; i<num_hunters; i++){
		scanf(" %d %d %d", &hunter1, &hunter2, &hunter_energy);
		
		objects[i].x = hunter1;
		objects[i].y = hunter2;
		objects[i].energy = hunter_energy;
		objects[i].type = 'H';
		objects[i].dead = false;

		output[hunter1][hunter2] = 'H';
	}

	scanf(" %d", &num_preys);
	objects = realloc(objects, (num_hunters+num_preys)*sizeof(object));

	for(i=num_hunters; i<(num_preys+num_hunters); i++){
		scanf(" %d %d %d", &prey1, &prey2, &prey_energy);
		
		objects[i].x = prey1;
		objects[i].y = prey2;
		objects[i].energy = prey_energy;
		objects[i].type = 'P';
		objects[i].dead = false;

		output[prey1][prey2] = 'P';
	}

	//Print
	print_output(width, height, output);

	char wid[15], hei[15];
	sprintf(wid, "%d", width);
	sprintf(hei, "%d", height);

	//Fork and execute

	for(i=0; i<num_hunters+num_preys; i++){
			PIPE(objects[i].fd);
			if((objects[i].pid = fork()) == 0){
				close(objects[i].fd[0]);
				dup2(objects[i].fd[1], 1);
				dup2(objects[i].fd[1], 0);
				close(objects[i].fd[1]);
				if(i < num_hunters)
					execl("./hunter", "./hunter", wid, hei, (char *)0);
				else
					execl("./prey", "./prey", wid, hei, (char *)0);

			}
			else
				;
	}

	//POLL
    struct pollfd pfd[num_hunters + num_preys];
    struct server_message servermessage;
    struct ph_message phmessage;

    int r, w, n, k, l;
    int distance = 0;
    int dist = width+height;
    int c_flag = 0;
    int konum = 0;
    int status;
    int cont = 1;
    int cont2 = 1;
    int dead_prey = 0;
    int nhunter = num_hunters;
    int nprey = num_preys;
    //int int_array[num_hunters+num_preys];


    for(i=0; i<num_hunters+num_preys; i++){
    	pfd[i].fd = objects[i].fd[0];
    	pfd[i].events = POLLOUT | POLLIN;
    	pfd[i].revents = 0;
    }

    for (i = 0; i<num_hunters+num_preys; i++)
    {
        close(objects[i].fd[1]);
    }

    while(cont && cont2){
    	cont = 0;
    	cont2 = 0;
    	for(n=0; n<num_hunters+num_preys;n++){
    		if(pfd[n].fd >= 0){
    			pfd[0].revents = 0;
    			//break;
    		}
    	}
    	n = num_hunters+num_preys;
    	poll(pfd, n, 0);
    	//////////////////////////////////random generator

    	/*srand(time(NULL));

	    for (i=0; i<num_hunters+num_preys; i++)
	    {
	    	int_array[i] = i;
		}
		for(i=0; i<num_hunters+num_preys; i++)
		{
			int temp = int_array[i];
			int randomIndex = rand() % (num_hunters+num_preys);

			int_array[i] = int_array[randomIndex];
			int_array[randomIndex] = temp;
		}*/


    	////////////////////////////////////
    	for(i=0; i<n; i++){//l=0; l<n; l++){
    		//i = int_array[l];

    		if(objects[i].dead)
    				continue;

    		if(pfd[i].revents && POLLOUT){
    			//pos
    			servermessage.pos.x = objects[i].x;
    			servermessage.pos.y = objects[i].y;


    			//En yakın pos
    			if(i < num_hunters){
    				dist = width + height;

    				for(j=num_hunters; j<num_hunters+num_preys; j++){
    					if(!objects[j].dead){
    						distance = abs(objects[j].x - objects[i].x) + abs(objects[j].y - objects[i].y);
    						if(distance < dist){
    							dist = distance;
    							servermessage.adv_pos.x = objects[j].x;
    							servermessage.adv_pos.y = objects[j].y;
    						}
    					}
    				}
    			}
    			else{
    				dist = width + height;

    				for(j=0; j<num_hunters; j++){
    					if(!objects[j].dead){
    						distance = abs(objects[j].x - objects[i].x) + abs(objects[j].y - objects[i].y);
    						if(distance < dist){
    							dist = distance;
    							servermessage.adv_pos.x = objects[j].x;
    							servermessage.adv_pos.y = objects[j].y;
    						}
    					}
    				}
    			}

    			servermessage.object_count = 0;
    			c_flag = 0;
    			konum = 0;

    			//First case
    			a = objects[i].x;
    			b = objects[i].y - 1;

    			for(j=0; j<num_obstacles; j++){
    				if(obstacle[j][0] == a && obstacle[j][1] == b){
    					c_flag = 1;
    				}
    			}

    			if(objects[i].type == 'H'){
    				for(j=0; j<num_hunters; j++){
    					if(!objects[j].dead && objects[j].x == a && objects[j].y == b)
    						c_flag = 1;
    				}
    			}

    			if(objects[i].type == 'P'){
    				for(j=num_hunters; j<num_hunters+num_preys; j++){
    					if(!objects[j].dead && objects[j].x == a && objects[j].y == b)
    						c_flag = 1;
    				}
    			}

    			if(c_flag == 1){
    				servermessage.object_count++;
    				servermessage.object_pos[konum].x = a;
    				servermessage.object_pos[konum].y = b;
    				konum++;
    			}

    			c_flag = 0;


    			//Second case
    			a = objects[i].x;
    			b = objects[i].y + 1;

    			for(j=0; j<num_obstacles; j++){
    				if(obstacle[j][0] == a && obstacle[j][1] == b){
    					c_flag = 1;
    				}
    			}

    			if(objects[i].type == 'H'){
    				for(j=0; j<num_hunters; j++){
    					if(!objects[j].dead && objects[j].x == a && objects[j].y == b)
    						c_flag = 1;
    				}
    			}

    			if(objects[i].type == 'P'){
    				for(j=num_hunters; j<num_hunters+num_preys; j++){
    					if(!objects[j].dead && objects[j].x == a && objects[j].y == b)
    						c_flag = 1;
    				}
    			}

    			if(c_flag == 1){
    				servermessage.object_count++;
    				servermessage.object_pos[konum].x = a;
    				servermessage.object_pos[konum].y = b;
    				konum++;
    			}

    			c_flag = 0;


    			//Third case
    			a = objects[i].x - 1;
    			b = objects[i].y;

    			for(j=0; j<num_obstacles; j++){
    				if(obstacle[j][0] == a && obstacle[j][1] == b){
    					c_flag = 1;
    				}
    			}

    			if(objects[i].type == 'H'){
    				for(j=0; j<num_hunters; j++){
    					if(!objects[j].dead && objects[j].x == a && objects[j].y == b)
    						c_flag = 1;
    				}
    			}

    			if(objects[i].type == 'P'){
    				for(j=num_hunters; j<num_hunters+num_preys; j++){
    					if(!objects[j].dead && objects[j].x == a && objects[j].y == b)
    						c_flag = 1;
    				}
    			}

    			if(c_flag == 1){
    				servermessage.object_count++;
    				servermessage.object_pos[konum].x = a;
    				servermessage.object_pos[konum].y = b;
    				konum++;
    			}

    			c_flag = 0;

    			//Fourth case
    			a = objects[i].x + 1;
    			b = objects[i].y;

    			for(j=0; j<num_obstacles; j++){
    				if(obstacle[j][0] == a && obstacle[j][1] == b){
    					c_flag = 1;
    				}
    			}

    			if(objects[i].type == 'H'){
    				for(j=0; j<num_hunters; j++){
    					if(!objects[j].dead && objects[j].x == a && objects[j].y == b)
    						c_flag = 1;
    				}
    			}

    			if(objects[i].type == 'P'){
    				for(j=num_hunters; j<num_hunters+num_preys; j++){
    					if(!objects[j].dead && objects[j].x == a && objects[j].y == b)
    						c_flag = 1;
    				}
    			}

    			if(c_flag == 1){
    				servermessage.object_count++;
    				servermessage.object_pos[konum].x = a;
    				servermessage.object_pos[konum].y = b;
    				konum++;
    			}

    			//printf("%d %d : %d %d : %d\n : ", servermessage.pos.x, servermessage.pos.y, servermessage.adv_pos.x, servermessage.adv_pos.y, servermessage.object_count);
    			w = write(pfd[i].fd, &servermessage, sizeof(servermessage));
    			FILE *file = fdopen(objects[i].fd[0], "w");
				fflush(file);
    		}

    		if(pfd[i].revents && POLLIN){
    			r = read(pfd[i].fd, &phmessage, sizeof(phmessage));
    			//printf("here : %d %d\n", phmessage.move_request.x, phmessage.move_request.y);

    			if(i<num_hunters){
    				if(objects[i].x == phmessage.move_request.x && objects[i].y == phmessage.move_request.y){
    					continue;
    				}
    				else{
    					for(j=num_hunters; j<num_hunters+num_preys; j++){
    						if(!objects[j].dead && objects[j].x == phmessage.move_request.x && objects[j].y == phmessage.move_request.y){

    							objects[j].dead = true;
    							objects[i].energy += objects[j].energy;
    							nprey--;

    							close(objects[j].fd[0]);
    							kill(objects[j].pid, SIGTERM);
    							waitpid(objects[j].pid, &status, WNOHANG);
    							pfd[j].fd = -1;
    							break;
    						}
    					}

    					output[objects[i].x][objects[i].y] = ' ';
    					output[phmessage.move_request.x][phmessage.move_request.y] = 'H';
    					
    					objects[i].x = phmessage.move_request.x;
    					objects[i].y = phmessage.move_request.y;
    					objects[i].energy--;



    					if(objects[i].energy == 0){
    						output[objects[i].x][objects[i].y] = ' ';
    						objects[i].dead = true;
    						nhunter--;

    						close(objects[i].fd[0]);
    						kill(objects[i].pid, SIGTERM);
    						waitpid(objects[i].pid, &status, WNOHANG);
    						pfd[i].fd = -1;

    					}
    				}
    				print_output(width, height, output);
    				FILE *file2 = fdopen(objects[i].fd[0], "w");
					fflush(file2);
    				if(nhunter == 0 || nprey == 0)
    					break;
    			}
    			else{
    				if(objects[i].x == phmessage.move_request.x && objects[i].y == phmessage.move_request.y){
    					continue;
    				}
    				else{
    					for(j=0; j<num_hunters; j++){
    						if(!objects[j].dead && objects[j].x == phmessage.move_request.x && objects[j].y == phmessage.move_request.y){
    							output[objects[i].x][objects[i].y] = ' ';

    							dead_prey = 1;
    							objects[i].dead = true;
    							objects[j].energy += objects[i].energy;
    							nprey--;

    							close(objects[i].fd[0]);
    							kill(objects[i].pid, SIGTERM);
    							waitpid(objects[i].pid, &status, WNOHANG);
    							pfd[i].fd = -1;
    							break;
    						}
    					}

    					if(dead_prey == 0){
    						output[objects[i].x][objects[i].y] = ' ';
    						output[phmessage.move_request.x][phmessage.move_request.y] = 'P';
    						
    						objects[i].x = phmessage.move_request.x;
    						objects[i].y = phmessage.move_request.y;
    					}
    					dead_prey = 0;

    				}
    				print_output(width, height, output);
    				FILE *file3 = fdopen(objects[i].fd[0], "w");
					fflush(file3);
    				if(nprey == 0)
    					break;

    			}
    			//printf("%d\n", objects[i].energy);
    		}
    	}
    	for(k=0; k<num_hunters; k++){
    		if(pfd[k].fd >= 0)
    			cont = 1;
    	}
    	for(k=num_hunters; k<num_hunters+num_preys; k++){
    		if(pfd[k].fd >= 0)
    			cont2 = 1;
    	}
    }
    for (i=0; i<num_hunters+num_preys; i++)
    {
    	if(!objects[i].dead){
    		objects[i].dead = true;
    		close(objects[i].fd[0]);
    		kill(objects[i].pid, SIGTERM);
    		waitpid(objects[i].pid, &status, WNOHANG);
    		pfd[i].fd = -1;
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
