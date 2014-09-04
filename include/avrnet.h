#ifndef AVRNET_H
#define AVRNET_H

namespace avrnet {
  void init(void);
  void loop(void);
}

#ifdef __cplusplus
extern "C" {
#endif

#include "uip.h"
#include "uip_arp.h"
#include "timer.h"
#include "network.h"


#ifdef __cplusplus
}
#endif


#endif //AVRNET_H
