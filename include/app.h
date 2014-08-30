#ifndef APP_H
#define APP_H
#include "psock.h"
#include "uipopt.h"

typedef struct hello_world_state {
  struct psock p;
  char inputbuffer[10];
  char name[40];
} uip_tcp_appstate_t;


#endif //APP_H
