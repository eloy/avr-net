#ifndef CLIENT_H
#define CLIENT_H

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "uip.h"
#include "uip_arp.h"
#include "timer.h"
#include "network.h"


  void uip_loop(void);


#ifdef __cplusplus
}
#endif


bool test;

extern struct  uip_context s;
void logstr(const char*);

#endif //CLIENT_H
