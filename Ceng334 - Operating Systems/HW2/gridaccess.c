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
} Ant;


pthread_t *ant_id;
pthread_mutex_t GridAccess;

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

    pthread_mutex_init(&GridAccess, NULL);

    for (i = 0; i < GRIDSIZE; i++) {
        for (j = 0; j < GRIDSIZE; j++) {
            putCharTo(i, j, '-');
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

        drawWindow();
        
        c = 0;
        c = getch();

        if (c == 'q' || c == ESC) break;
        if (c == '+') {
            setDelay(getDelay()+10);
        }
        if (c == '-') {
            setDelay(getDelay()-10);
        }
        if (c == '*') {
            setSleeperN(getSleeperN()+1);
        }
        if (c == '/') {
            setSleeperN(getSleeperN()-1);
        }
        usleep(DRAWDELAY);
        
        // each ant thread have to sleep with code similar to this
        //usleep(getDelay() * 1000 + (rand() % 5000));
    }

	/*for (int i = 0; i < num_ants; i++)
	{
		pthread_join(ant_id[i], NULL);
	}    */


	   // do not forget freeing the resources you get
    endCurses();
	//exit(0);

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


	while(1){
	
		if(id < getSleeperN() && sleep == FALSE)
		{
			sleep = TRUE;
			if(type == '1')
			{
				type = 'S';
				putCharTo(x, y, 'S');
			}
			else
			{
				type = '$';
				putCharTo(x, y, '$');
			}

			continue;
		}

		if(id >= getSleeperN() && sleep == TRUE)
		{
			sleep = FALSE;
			if(type == 'S')
			{
				type = '1';
				putCharTo(x, y, '1');
			}
			else
			{
				type = 'P';
				putCharTo(x, y, 'P');
			}
		}


		pthread_mutex_lock(&GridAccess);
		if(type == '1' && tired == FALSE)
		{
			int flag = check_food(x, y);
			if(flag == TRUE)
			{
				int *new = move_without_food(x, y);
				putCharTo(x, y, '-');
				putCharTo(new[0], new[1], 'P');
				x = new[0];
				y = new[1];
				type = 'P';
				free(new);
			}
			else
			{
				int *new = move_with_food(x, y);
				putCharTo(x, y, '-');
				putCharTo(new[0], new[1], '1');
				x = new[0];
				y = new[1];
				free(new);
			}


		}
		else if(type == '1' && tired == TRUE)
		{
			int *new = move_with_food(x, y);
			putCharTo(x, y, '-');
			putCharTo(new[0], new[1], '1');
			tired = FALSE;
			x = new[0];
			y = new[1];
			free(new);
		}
		else if(type == 'P')
		{
			bool flag = check_food(x, y);

			if(flag == TRUE)
			{
				int *new = move_with_food(x, y);
				putCharTo(x, y, 'o');
				putCharTo(new[0], new[1], '1');
				tired = TRUE;
				x = new[0];
				y = new[1];
				type = '1';
				free(new);
			}
			else
			{
				int *new = move_with_food(x, y);
				putCharTo(x, y, '-');
				putCharTo(new[0], new[1], 'P');
				x = new[0];
				y = new[1];
				free(new);
			}

		}
		pthread_mutex_unlock(&GridAccess);
        usleep(getDelay() * 1000 + (rand() % 5000));

	}

}


int *move_without_food(int x, int y)
{
	int i, j;
	int *move;
	int start_x, end_x, start_y, end_y;
	
	move = malloc(2*sizeof(int));

	if(x == 0) {start_x = 0; end_x = x+2;}
	else if (x == GRIDSIZE-1) {start_x = x-1; end_x = x+1;}
	else {start_x = x-1; end_x = x+2;}

	if(y == 0) {start_y = 0; end_y = y+2;}
	else if (y == GRIDSIZE-1) {start_y = y-1; end_y = y+1;}
	else {start_y = y-1; end_y = y+2;}



	for(i=start_x; i<end_x; i++){
		for(j=start_y; j<end_y; j++){
			if(lookCharAt(i, j) == 'o'){
				move[0] = i;
				move[1] = j;
				return move;
			}
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
	return move;
}


int *move_with_food(int x, int y)
{
	int i, j, k=0, r;
	int *move;
	int *random = (int *)calloc(0, sizeof(int));
	int start_x, end_x, start_y, end_y;
	
	move = malloc(2*sizeof(int));

	if(x == 0) {start_x = 0; end_x = x+2;}
	else if (x == GRIDSIZE-1) {start_x = x-1; end_x = x+1;}
	else {start_x = x-1; end_x = x+2;}

	if(y == 0) {start_y = 0; end_y = y+2;}
	else if (y == GRIDSIZE-1) {start_y = y-1; end_y = y+1;}
	else {start_y = y-1; end_y = y+2;}


	for(i=start_x; i<end_x; i++){
		for(j=start_y; j<end_y; j++){
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
		r = rand() % size;

	//printf("%d %d\n", r, size);

	move[0] = random[2*r];
	move[1] = random[2*r+1];

	free(random);
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
			if(lookCharAt(i, j) == 'o'){
				return TRUE;
			}
		}
	}
	return FALSE;
}