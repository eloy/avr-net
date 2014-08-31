#ifndef APP_H
#define APP_H
#include "psock.h"
#include "uipopt.h"

typedef struct uip_context {
  struct psock psock;
  char inputbuffer[128];
  char name[128];
} uip_tcp_appstate_t;


#endif //APP_H
