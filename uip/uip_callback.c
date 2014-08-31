#include "uip_callback.h"
#include "client.h"

uip_callback callback;

void uip_set_callback(uip_callback cb) {
  callback = cb;
}



void app_test_callback() {
  test = true;
}
