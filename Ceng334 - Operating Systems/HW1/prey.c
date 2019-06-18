#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>


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


int main(int argc, char **argv)
{
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);

	while(1){
		int i, j, w;
		int flag = 0;
		int distance, a, b;
		int distance1, distance2, distance3, distance4;

		struct server_message coming;
		struct ph_message going;

		read(1, &coming, sizeof(coming));

		going.move_request.x = coming.pos.x;
		going.move_request.y = coming.pos.y;	


		
			distance = abs(coming.adv_pos.x - coming.pos.x) + abs(coming.adv_pos.y - coming.pos.y);

			a = coming.pos.x;
			b = coming.pos.y - 1;


			//Firt case
			if(coming.pos.y != 0)
			{
				a = coming.pos.x;
				b = coming.pos.y - 1;
				for(j=0; j<coming.object_count; j++){
					if(coming.object_pos[j].x == a && coming.object_pos[j].y == b){
						flag = 1;
					}
				}

				if(flag != 1){
					distance1 = abs(coming.adv_pos.x - a) + abs(coming.adv_pos.y - b);
					if(distance1 > distance){
						going.move_request.x = a;
						going.move_request.y = b;					
					}
				}
			}

			flag = 0;


			//Second case
			if(coming.pos.y < (width-1))
			{
				a = coming.pos.x;
				b = coming.pos.y + 1;
				for(j=0; j<coming.object_count; j++){
					if(coming.object_pos[j].x == a && coming.object_pos[j].y == b){
						flag = 1;
					}
				}

				if(flag != 1){
					distance2 = abs(coming.adv_pos.x - a) + abs(coming.adv_pos.y - b);
					if(distance2 > distance){
						going.move_request.x = a;
						going.move_request.y = b;					
					}
				}
			}

			flag = 0;


			//Third case
			if(coming.pos.x != 0)
			{
				a = coming.pos.x - 1;
				b = coming.pos.y;

				for(j=0; j<coming.object_count; j++){
					if(coming.object_pos[j].x == a && coming.object_pos[j].y == b){
							flag = 1;
					}
				}

				if(flag != 1){
					distance3 = abs(coming.adv_pos.x - a) + abs(coming.adv_pos.y - b);
					if(distance3 > distance){
						going.move_request.x = a;
						going.move_request.y = b;					
					}
				}
			}

			flag = 0;


			//Fourth case
			if(coming.pos.x < (height-1)){
				a = coming.pos.x + 1;
				b = coming.pos.y;
				for(j=0; j<coming.object_count; j++){
					if(coming.object_pos[j].x == a && coming.object_pos[j].y == b){
						flag = 1;
					}
				}

				if(flag != 1){
					distance4 = abs(coming.adv_pos.x - a) + abs(coming.adv_pos.y - b);
					if(distance4 > distance){
						going.move_request.x = a;
						going.move_request.y = b;					
					}
				}
			}

			flag = 0;
		usleep(10000*(1+rand()%9));
		w = write(1, &going, sizeof(going));
    	FILE *filep = fdopen(1, "w");
		fflush(filep);
	}
	return 0;
}