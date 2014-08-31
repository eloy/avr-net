#include "client.h"
#define ETHADDR0		0x00
#define ETHADDR1		0xbd
#define ETHADDR2		0x3b
#define ETHADDR3		0x33
#define ETHADDR4		0x05
#define ETHADDR5		0x71

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
struct uip_conn *conn;

#define ISO_nl 0x0a
#define ISO_cr 0x0d

#define ISO_period 0x2e

#define ISO_2  0x32
#define ISO_3  0x33
#define ISO_4  0x34
#define ISO_5  0x35


struct uip_context s;


static
PT_THREAD(login_thread(void)) {

  PSOCK_BEGIN(&s.psock);
  Serial.println("Here");
  PSOCK_READTO(&s.psock, ISO_nl);
  PSOCK_SEND_STR(&s.psock, "pollo");
  Serial.println(&s.inputbuffer[0]);
  PSOCK_END(&s.psock);
}


void logstr(const char *str) {
  Serial.println(str);
}


void test_callback(void) {
  Serial.println("YAaaaaaaa");
  uip_send("1234", 5);
}


void other_callback(void) {
  Serial.println("OOOOOOOOOOO");
}


static uip_ipaddr_t server;
struct timer periodic_timer, arp_timer;

// the setup routine runs once when you press reset:
int main(void) {

  Serial.begin(9600);

	clock_init();
  timer_set(&periodic_timer, CLOCK_SECOND / 2);
  timer_set(&arp_timer, CLOCK_SECOND * 10);


  Serial.println("Starting Network");
  network_init();
  Serial.println("Network Started");

  uip_ipaddr_t ipaddr;
  /* struct timer periodic_timer, arp_timer; */

  /* timer_set(&periodic_timer, CLOCK_SECOND / 2); */
  /* timer_set(&arp_timer, CLOCK_SECOND * 10); */
  uip_set_callback(test_callback);
  uip_init();
  uip_arp_init();

  // IP
  uip_ipaddr(ipaddr, 10,254,0,77);
  uip_sethostaddr(ipaddr);

  // Netmask
  uip_ipaddr(ipaddr, 255,255,255,0);
  uip_setnetmask(ipaddr);


  // Router
  uip_ipaddr(ipaddr, 10,254,0,254);
  uip_setdraddr(ipaddr);

  struct uip_eth_addr  my_eth_addr;
  my_eth_addr.addr[0] = ETHADDR0;
	my_eth_addr.addr[1] = ETHADDR1;
	my_eth_addr.addr[2] = ETHADDR2;
	my_eth_addr.addr[3] = ETHADDR3;
	my_eth_addr.addr[4] = ETHADDR4;
	my_eth_addr.addr[5] = ETHADDR5;

  uip_setethaddr(my_eth_addr);
  // uip_ipaddr(ipaddr, 10,254,0,16);
  // conn = uip_connect(&ipaddr, HTONS(1234));

  // if(conn == NULL) {
  //   Serial.println("Algo ha ido mal");
  // }
  // uip_send("123", 4);

  uip_ipaddr(&server, 10,254,0,16);

  Serial.println("Connected");

  conn = uip_connect(&server, HTONS(1234));
  if(conn == NULL) {
    Serial.println("Algo ha ido mal");
  }
  // else {
  //   Serial.println("Worked");
  //   uip_arp_out();
  //   network_send();
  // }


  // uip_listen(htons(1234));

  while(true) {
    uip_loop();
  }
}

bool connected;

void uip_loop(void) {

  uip_len = network_read();
  if (uip_len != 0)
    Serial.println(uip_len);

  if(uip_len > 0) {
    if(BUF->type == htons(UIP_ETHTYPE_IP)){
      Serial.println(uip_len);
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
    } else {
      Serial.println("Don't know what to do with this");
    }

  } else if(timer_expired(&periodic_timer)) {
    Serial.println("Expired");
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

void xloop(void) {
  if(test) {
    Serial.println("XXXX");
  }

  uip_loop();
}
