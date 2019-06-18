#include "do_not_submit.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct Ant {
	int id;
	char type;
	int x;
	int y;
	bool tired;
	bool sleep;
	sem_t sl;
} Ant;

bool finish = false;

pthread_t *ant_id;
//pthread_mutex_t GridAccess;

sem_t s[GRIDSIZE][GRIDSIZE];

void *main_thread();
void *ant_move(void *ant);
int move_ant(char type, int x, int y, bool tired);
int *move_without_food(int x, int y);
int *move_with_food(int x, int y);
bool check_food(int x, int y);



int main(int argc, char *argv[]){

    int i,j;
    int a,b;
    int num_ants = atoi(argv[1]);
    int num_foods = atoi(argv[2]);
    int ti = atoi(argv[3]);
	srand(time(NULL));

	time_t begin = time(NULL);
    time_t end;
    end = begin + ti;

    //pthread_mutex_init(&GridAccess, NULL);

    for (i = 0; i < GRIDSIZE; i++) {
        for (j = 0; j < GRIDSIZE; j++) {
            putCharTo(i, j, '-');
        }
    }

    for (i = 0; i < GRIDSIZE; i++) {
        for (j = 0; j < GRIDSIZE; j++) {
        	sem_init(&s[i][j], 0, 1);
        }
    }


    Ant *ants = malloc(num_ants*sizeof(Ant));

    for (i = 0; i < num_ants; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b)!= '-');
        putCharTo(a, b, '1');
        ants[i].id = i;
        ants[i].type = '1';
        ants[i].x = a;
        ants[i].y = b;
        ants[i].tired = false;
        ants[i].sleep = false;
        sem_init(&ants[i].sl, 0, 1);
    }

    for (i = 0; i < num_foods; i++) {
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b) != '-');
        putCharTo(a, b, 'o');
    }

    ant_id = (pthread_t *)malloc(num_ants*sizeof(pthread_t));


    startCurses();

    for (int i = 0; i < num_ants; i++)
    {
    	pthread_create(&ant_id[i], NULL, ant_move,(void *) (ants+i));
    }

    // You can use following loop in your program. But pay attention to 
    // the function calls, they do not have any access control, you 
    // have to ensure that.
    char c;
    while (begin < end) {
    	begin = time(NULL);

    	for (i = getSleeperN(); i < num_ants; i++)
    	{
    		sem_wait(&ants[i].sl);
    	}

    	//pthread_mutex_lock(&GridAccess);
        drawWindow();
        //pthread_mutex_unlock(&GridAccess);

        for (i = getSleeperN(); i < num_ants; i++)
    	{
    		sem_post(&ants[i].sl);
    	}


        c = 0;
        c = getch();

        if (c == 'q' || c == ESC){
        	finish = TRUE;
        	break;
        }
        if (c == '+') {
            setDelay(getDelay()+10);
        }
        if (c == '-') {
            setDelay(getDelay()-10);
        }
        if (c == '*') {
        	if(getSleeperN() >= num_ants){
	            setSleeperN(getSleeperN()+1);
        	}
        	else{
            	sem_wait(&ants[getSleeperN()].sl);
            	setSleeperN(getSleeperN()+1);
        	}
        }
        if (c == '/') {
        	if(getSleeperN() > num_ants){
            	setSleeperN(getSleeperN()-1);
        	}
        	else{
            	setSleeperN(getSleeperN()-1);
            	sem_post(&ants[getSleeperN()].sl);
        	}
        }
        usleep(DRAWDELAY);
        
        // each ant thread have to sleep with code similar to this
        //usleep(getDelay() * 1000 + (rand() % 5000));
    }
    for (int i = 0; i < getSleeperN(); ++i)
    {
        sem_post(&ants[i].sl);
    }
    finish = TRUE;

	for (int i = 0; i < num_ants; i++)
	{
		pthread_join(ant_id[i], NULL);
	}


	   // do not forget freeing the resources you get
    endCurses();
	//exit(0);

    free(ants);
    free(ant_id);

    return 0;
}



void *ant_move(void *ant){

	Ant *new_ant = (Ant *)ant;

	int id = new_ant->id;
	int x = new_ant->x;
	int y = new_ant->y;
	char type = new_ant->type;
	bool tired = new_ant->tired;
	bool sleep = new_ant->sleep;


	while(!finish){

		if(id < getSleeperN() && sleep == FALSE)
		{
			sleep = TRUE;
			if(type == '1')
			{
				type = 'S';
				sem_wait(&s[x][y]);
				putCharTo(x, y, 'S');
				sem_post(&s[x][y]);
			}
			else
			{
				type = '$';
				sem_wait(&s[x][y]);
				putCharTo(x, y, '$');
				sem_post(&s[x][y]);

			}

			continue;
		}

		sem_wait(&new_ant->sl);

		if(id >= getSleeperN() && sleep == TRUE)
		{
			sleep = FALSE;
			if(type == 'S')
			{
				type = '1';
				sem_wait(&s[x][y]);
				putCharTo(x, y, '1');
				sem_post(&s[x][y]);
			}
			else
			{
				type = 'P';
				sem_wait(&s[x][y]);
				putCharTo(x, y, 'P');
				sem_post(&s[x][y]);
			}
		}


		//pthread_mutex_lock(&GridAccess);
		if(type == '1' && tired == FALSE)
		{
			int *new = move_without_food(x, y);
			if(new[2] == 1)
			{
				sem_wait(&s[x][y]);
				putCharTo(x, y, '-');
				sem_post(&s[x][y]);

				putCharTo(new[0], new[1], 'P');
				sem_post(&s[new[0]][new[1]]);
				x = new[0];
				y = new[1];
				type = 'P';
				free(new);
			}
			else
			{
				free(new);
				int *new = move_with_food(x, y);
				if(new[2] == 1) {
					sem_wait(&s[x][y]);
					putCharTo(x, y, '-');
					sem_post(&s[x][y]);

					putCharTo(new[0], new[1], '1');
					sem_post(&s[new[0]][new[1]]);
					x = new[0];
					y = new[1];
					free(new);
				}
				else{
					x = new[0];
					y = new[1];
					free(new);					
				}

			}


		}
		else if(type == '1' && tired == TRUE)
		{
			int *new = move_with_food(x, y);
			if(new[2] == 1){
				sem_wait(&s[x][y]);
				putCharTo(x, y, '-');
				sem_post(&s[x][y]);

				putCharTo(new[0], new[1], '1');
				sem_post(&s[new[0]][new[1]]);
				tired = FALSE;
				x = new[0];
				y = new[1];
				free(new);
			}
			else{
				tired = FALSE;
				x = new[0];
				y = new[1];
				free(new);
			}
		}
		else if(type == 'P')
		{
			bool flag = check_food(x, y);

			if(flag == TRUE)
			{
				int *new = move_with_food(x, y);
				if(new[2] == 1){
					sem_wait(&s[x][y]);
					putCharTo(x, y, 'o');
					sem_post(&s[x][y]);

					putCharTo(new[0], new[1], '1');
					sem_post(&s[new[0]][new[1]]);
					tired = TRUE;
					x = new[0];
					y = new[1];
					type = '1';
					free(new);
				}
				else{
					if(new[0] == x && new[1] == y){
						free(new);
					}
					else{
						tired = TRUE;
						x = new[0];
						y = new[1];
						type = 'P';
						free(new);
					}
				}

			}
			else
			{
				int *new = move_with_food(x, y);
				if(new[2] == 1){
					sem_wait(&s[x][y]);
					putCharTo(x, y, '-');
					sem_post(&s[x][y]);

					putCharTo(new[0], new[1], 'P');
					sem_post(&s[new[0]][new[1]]);
					x = new[0];
					y = new[1];
					free(new);
				}
				else{
					x = new[0];
					y = new[1];
					free(new);
				}

			}

		}
		sem_post(&new_ant->sl);

		//pthread_mutex_unlock(&GridAccess);
        usleep(getDelay() * 1000 + (rand() % 5000));
	}
}


int *move_without_food(int x, int y)
{
	int i, j;
	int *move;
	int start_x, end_x, start_y, end_y;
	
	move = malloc(3*sizeof(int));

	if(x == 0) {start_x = 0; end_x = x+2;}
	else if (x == GRIDSIZE-1) {start_x = x-1; end_x = x+1;}
	else {start_x = x-1; end_x = x+2;}

	if(y == 0) {start_y = 0; end_y = y+2;}
	else if (y == GRIDSIZE-1) {start_y = y-1; end_y = y+1;}
	else {start_y = y-1; end_y = y+2;}



	for(i=start_x; i<end_x; i++){
		for(j=start_y; j<end_y; j++){
			sem_wait(&s[i][j]);
			if(lookCharAt(i, j) == 'o'){
				move[0] = i;
				move[1] = j;
				move[2] = 1;
				return move;
			}
			sem_post(&s[i][j]);
		}
	}
	/*for(i=start_x; i<end_x; i++){
		for(j=start_y; j<end_y; j++){
			if(lookCharAt(i, j) == '-'){
				move[0] = i;
				move[1] = j;
				return move;
			}
		}
	}*/
	move[0] = x;
	move[1] = y;
	move[2] = 0;
	return move;
}


int *move_with_food(int x, int y)
{
	int i, j, k=0, r, r2, q = 0, c1, c2;
	int *move;
	//int *random = (int *)calloc(0, sizeof(int));
	int start_x, end_x, start_y, end_y;
	
	move = malloc(3*sizeof(int));

	if(x == 0) {start_x = 0; end_x = x+2;}
	else if (x == GRIDSIZE-1) {start_x = x-1; end_x = x+1;}
	else {start_x = x-1; end_x = x+2;}

	if(y == 0) {start_y = 0; end_y = y+2;}
	else if (y == GRIDSIZE-1) {start_y = y-1; end_y = y+1;}
	else {start_y = y-1; end_y = y+2;}

	//printf("\n%d %d %d %d\n", start_x, end_x, start_y, end_y);
	while(q < 80){
		q++;
		r = rand() % (end_x-start_x);
		r2 = rand() % (end_y-start_y);
		//printf("%d\n", r);

		c1 = start_x+r;
		c2 = start_y+r2;
		//printf("%d %d \n", c1, c2);

		//if(c1 < GRIDSIZE && c2 && GRIDSIZE){
			sem_wait(&s[c1][c2]);
			if(lookCharAt(c1, c2) == '-'){
				move[0] = c1;
				move[1] = c2;
				move[2] = 1;
				return move;
			}
			sem_post(&s[c1][c2]);
		//}
	}
	
	/*for(i=start_x; i<end_x; i++){
		for(j=start_y; j<end_y; j++){
			sem_wait(&s[i][j]);
			if(lookCharAt(i, j) == '-'){
				random = (int *)realloc(random, (k+2)*sizeof(int));
				random[k] = i;
				random[k+1] = j;
				k = k+2;
			}
		}
	}
	int size = ((k))/2;
	if (size == 0)
		r = 0;
	else
		r = rand() % size;*/

	//printf("%d %d\n", r, size);

	move[0] = x;
	move[1] = y;
	move[2] = 0;

	//free(random);
	return move;
}


bool check_food(int x, int y)
{
	int i, j;
	int start_x, end_x, start_y, end_y;

	if(x == 0) {start_x = 0; end_x = x+2;}
	else if (x == GRIDSIZE-1) {start_x = x-1; end_x = x+1;}
	else {start_x = x-1; end_x = x+2;}

	if(y == 0) {start_y = 0; end_y = y+2;}
	else if (y == GRIDSIZE-1) {start_y = y-1; end_y = y+1;}
	else {start_y = y-1; end_y = y+2;}

	for(i=start_x; i<end_x; i++){
		for(j=start_y; j<end_y; j++){
			sem_wait(&s[i][j]);
			if(lookCharAt(i, j) == 'o'){
				sem_post(&s[i][j]);
				return TRUE;
			}
			sem_post(&s[i][j]);
		}
	}
	return FALSE;
}