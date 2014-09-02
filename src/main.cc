#include "Arduino.h"
#include "client.h"
#include "avrnet.h"

#define ETHADDR0		0x00
#define ETHADDR1		0xbd
#define ETHADDR2		0x3b
#define ETHADDR3		0x33
#define ETHADDR4		0x05
#define ETHADDR5		0x71

struct uip_conn *conn;

#define ISO_nl 0x0a

#define CONNECTION_RETRY_TIME 200

bool connected = false;
struct timer connect_timer;

static uip_ipaddr_t server;

void connect(void) {
  Serial.println("Starting connection");
  conn = uip_connect(&server, HTONS(1234));
  if(conn == NULL) {
    Serial.println("Algo ha ido mal");
  }
}


void reconnect(void) {
  connected = false;
  timer_set(&connect_timer, CONNECTION_RETRY_TIME);
}

static
PT_THREAD(login_thread(struct uip_context *ctx)) {

  PSOCK_BEGIN(&ctx->psock);


  PSOCK_SEND_STR(&ctx->psock, "pollo");
  PSOCK_READTO(&ctx->psock, ISO_nl);

  PSOCK_END(&ctx->psock);
}




void test_callback(void) {
  struct uip_context *ctx = &(uip_conn->appstate);

  if (uip_connected()) {
    connected = true;
    PSOCK_INIT(&ctx->psock, ctx->inputbuffer, sizeof(ctx->inputbuffer));
  }

  if (uip_closed()) {
    Serial.println("Closed");
    reconnect();
  }

  if (uip_aborted()) {
    Serial.println("Aborted");
    reconnect();
  }


  if (uip_rexmit()) {
    Serial.println("Not Aknowledge");
  }
  login_thread(ctx);
}


// the setup routine runs once when you press reset:
int main (void) {

  Serial.begin(9600);
  uip_set_callback(test_callback);
  avrnet::init();

  uip_ipaddr_t ipaddr;

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

  uip_ipaddr(&server, 10,254,0,16);

  // Setup the connection timer
  timer_set(&connect_timer, 100);

  // uip_listen(htons(1234));
  Serial.println("Start loop");
  while(true) {
    avrnet::loop();

    // Check connection
    if (!connected && timer_expired(&connect_timer)) {
      uip_abort();
      connect();

      timer_set(&connect_timer, CONNECTION_RETRY_TIME);
    }

  }
}
