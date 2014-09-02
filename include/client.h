#ifndef CLIENT_H
#define CLIENT_H


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

extern struct  uip_context s;

#endif //CLIENT_H
