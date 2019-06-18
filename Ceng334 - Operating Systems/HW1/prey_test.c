#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)
#define Data1 "In Xanadu, did Kublai Khan..."



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



void main(){
    int P_no;
    scanf("%d", &P_no);
    int *Preys[P_no];
    for (int i=0; i<P_no; i++)
        Preys[i] = (int *)malloc(3 * sizeof(int));


    for (int i = 0; i < P_no; i++)
    {
        scanf(" %d %d %d", &Preys[i][0], &Preys[i][1], &Preys[i][2]);
    }
    int sockets[P_no][2];
    int a = 55;

    //printf(" PREY 1 0----- > %d\n", Preys[1][0]);
    for (int i = 0; i < P_no; i++)
    {
        server_message ser_mes;
        ser_mes.pos.x = 2;
        ser_mes.pos.y = 0;

        ser_mes.adv_pos.x = 1;
        ser_mes.adv_pos.y = 0;

        ser_mes.object_count = 0;

        //ser_mes.object_pos[0].x = 1;
        //ser_mes.object_pos[0].y = 1;




        ph_message ph_mes;
        ser_mes.pos.x =Preys[i][0];
        ser_mes.pos.y =Preys[i][1];
        PIPE(sockets[i]);
        if(fork() == 0){
            close(sockets[i][0]);
            dup2(sockets[i][1],1);
            dup2(sockets[i][1],0);
            execl("./prey", "./prey", "3", "3", NULL);
            close(sockets[i][1]);
        }else{
            sleep(2);
            close(sockets[i][1]);
            write(sockets[i][0], &ser_mes, sizeof(ser_mes));
            read(sockets[i][0], &ph_mes, sizeof(ph_mes));

            printf("request x is %d\n", ph_mes.move_request.x);
            printf("request y is %d\n", ph_mes.move_request.y);
            close(sockets[i][0]);
            printf("\n");
        }
    }
}

