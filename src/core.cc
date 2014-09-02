#include "avrnet.h"
#include "uip_core.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

struct timer periodic_timer, arp_timer;

namespace avrnet {

  void init(void) {
    network_init();

    timer_set(&periodic_timer, CLOCK_SECOND / 2);
    timer_set(&arp_timer, CLOCK_SECOND * 10);
    clock_init();

    uip_init();
    uip_arp_init();

  }


  void loop(void) {
    uip_len = network_read();


    // If data received
    if (uip_len > 0) {
      if(BUF->type == htons(UIP_ETHTYPE_IP)){

        uip_arp_ipin(); // arp seems to have issues w/o this
        uip_input();
        if(uip_len > 0) {
          uip_arp_out();
          network_send();
        }
      } else if(BUF->type == htons(UIP_ETHTYPE_ARP)){
        uip_arp_arpin(); // this is correct
        if(uip_len > 0){
          network_send();
        }
      }

    }

    // Else check timer
    else if(timer_expired(&periodic_timer)) {
      timer_reset(&periodic_timer);

      for(int i = 0; i < UIP_CONNS; i++) {
        uip_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0) {
          uip_arp_out();
          network_send();
        }
      }

      if(timer_expired(&arp_timer)) {
        timer_reset(&arp_timer);
        uip_arp_timer();
      }

    }

  }
}
