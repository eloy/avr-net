#include "uip_callback.h"

uip_callback callback;

void uip_set_callback(uip_callback cb) {
  callback = cb;
}
