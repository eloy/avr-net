#ifndef UIP_CALLBACK_H
#define UIP_CALLBACK_H

typedef void (*uip_callback)(void);

extern uip_callback callback;

void app_test_callback();
#define UIP_APPCALL callback


void uip_set_callback(uip_callback);

#endif //UIP_CALLBACK_H
