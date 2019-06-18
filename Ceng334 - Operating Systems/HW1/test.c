#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

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

int main() {

  struct ph_message a;
  a.move_request.x = 2;
  a.move_request.y = 2;

  read(0, &server_message, sizeof(server_message));

  write(1, &a, sizeof(a));


    return 0;
}
