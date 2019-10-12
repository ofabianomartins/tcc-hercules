#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {10429, 10429, 11052, 10429, 11052, 9307, 10429, 11052, 9307, 8614, 10429, 11052, 9307, 8614, 6455, 10429, 11052, 9307, 8614, 6455, 9750, 10429, 11052, 9307, 8614, 6455, 9750, 9290, 10429, 11052, 9307, 8614, 6455, 9750, 9290, 13143, 10429, 11052, 9307, 8614, 6455, 9750, 9290, 13143, 9091, 10429, 11052, 9307, 8614, 6455, 9750, 9290, 13143, 9091, 5401, 10429, 11052, 9307, 8614, 6455, 9750, 9290, 13143, 9091, 5401, 8283, 5535, 5535, 7811, 5535, 7811, 12667, 5535, 7811, 12667, 7536, 5535, 7811, 12667, 7536, 6629, 5535, 7811, 12667, 7536, 6629, 12084, 5535, 7811, 12667, 7536, 6629, 12084, 12207, 5535, 7811, 12667, 7536, 6629, 12084, 12207, 9616, 5535, 7811, 12667, 7536, 6629, 12084, 12207, 9616, 8658, 5535, 7811, 12667, 7536, 6629, 12084, 12207, 9616, 8658, 7903, 7346, 7346, 10112, 7346, 10112, 9478, 7346, 10112, 9478, 8009, 7346, 10112, 9478, 8009, 8745, 7346, 10112, 9478, 8009, 8745, 7107, 7346, 10112, 9478, 8009, 8745, 7107, 6893, 7346, 10112, 9478, 8009, 8745, 7107, 6893, 8799, 7346, 10112, 9478, 8009, 8745, 7107, 6893, 8799, 9110, 7346, 10112, 9478, 8009, 8745, 7107, 6893, 8799, 9110, 10265, 7791, 7791, 6289, 7791, 6289, 8218, 7791, 6289, 8218, 9137, 7791, 6289, 8218, 9137, 7779, 7791, 6289, 8218, 9137, 7779, 12962, 7791, 6289, 8218, 9137, 7779, 12962, 6440, 7791, 6289, 8218, 9137, 7779, 12962, 6440, 10737, 7791, 6289, 8218, 9137, 7779, 12962, 6440, 10737, 11421, 7791, 6289, 8218, 9137, 7779, 12962, 6440, 10737, 11421, 8026, 9457, 9457, 10819, 9457, 10819, 9369, 9457, 10819, 9369, 8533, 9457, 10819, 9369, 8533, 9740, 9457, 10819, 9369, 8533, 9740, 12499, 9457, 10819, 9369, 8533, 9740, 12499, 8281, 9457, 10819, 9369, 8533, 9740, 12499, 8281, 7715, 9457, 10819, 9369, 8533, 9740, 12499, 8281, 7715, 11245, 9457, 10819, 9369, 8533, 9740, 12499, 8281, 7715, 11245, 9097, 7976, 7976, 10316, 7976, 10316, 9533, 7976, 10316, 9533, 8436, 7976, 10316, 9533, 8436, 8603, 7976, 10316, 9533, 8436, 8603, 9715, 7976, 10316, 9533, 8436, 8603, 9715, 8549, 7976, 10316, 9533, 8436, 8603, 9715, 8549, 8287, 7976, 10316, 9533, 8436, 8603, 9715, 8549, 8287, 9630, 7976, 10316, 9533, 8436, 8603, 9715, 8549, 8287, 9630, 7258, 9321, 9321, 8437, 9321, 8437, 7696, 9321, 8437, 7696, 9385, 9321, 8437, 7696, 9385, 9551, 9321, 8437, 7696, 9385, 9551, 7822, 9321, 8437, 7696, 9385, 9551, 7822, 11409, 9321, 8437, 7696, 9385, 9551, 7822, 11409, 7601, 9321, 8437, 7696, 9385, 9551, 7822, 11409, 7601, 7466, 9321, 8437, 7696, 9385, 9551, 7822, 11409, 7601, 7466, 7868, 9611, 9611, 9359, 9611, 9359, 11425, 9611, 9359, 11425, 10755, 9611, 9359, 11425, 10755, 11782, 9611, 9359, 11425, 10755, 11782, 7187, 9611, 9359, 11425, 10755, 11782, 7187, 10074, 9611, 9359, 11425, 10755, 11782, 7187, 10074, 9456, 9611, 9359, 11425, 10755, 11782, 7187, 10074, 9456, 8874, 9611, 9359, 11425, 10755, 11782, 7187, 10074, 9456, 8874, 9581, 10766, 10766, 10097, 10766, 10097, 9403, 10766, 10097, 9403, 10557, 10766, 10097, 9403, 10557, 12662, 10766, 10097, 9403, 10557, 12662, 7872, 10766, 10097, 9403, 10557, 12662, 7872, 7653, 10766, 10097, 9403, 10557, 12662, 7872, 7653, 9052, 10766, 10097, 9403, 10557, 12662, 7872, 7653, 9052, 11059, 10766, 10097, 9403, 10557, 12662, 7872, 7653, 9052, 11059, 8853, 6072, 6072, 6630, 6072, 6630, 9140, 6072, 6630, 9140, 10411, 6072, 6630, 9140, 10411, 6729, 6072, 6630, 9140, 10411, 6729, 8537, 6072, 6630, 9140, 10411, 6729, 8537, 9609, 6072, 6630, 9140, 10411, 6729, 8537, 9609, 11549, 6072, 6630, 9140, 10411, 6729, 8537, 9609, 11549, 8326, };static struct simple_udp_connection unicast_connection;
PROCESS(unicast_sender_process, "Unicast sender example process");AUTOSTART_PROCESSES(&unicast_sender_process);
static voidreceiver(struct simple_udp_connection *c,         const uip_ipaddr_t *sender_addr,         uint16_t sender_port,         const uip_ipaddr_t *receiver_addr,         uint16_t receiver_port,         const uint8_t *data,         uint16_t datalen){  printf("Data received on port %d from port %d with length %d\n",         receiver_port, sender_port, datalen);}
static voidset_global_address(void){  uip_ipaddr_t ipaddr;  int i;  uint8_t state;
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
  printf("IPv6 addresses: ");  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {    state = uip_ds6_if.addr_list[i].state;    if(uip_ds6_if.addr_list[i].isused &&       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);      printf("\n");    }  }}
PROCESS_THREAD(unicast_sender_process, ev, data){  static struct etimer periodic_timer;  static struct etimer send_timer;  uip_ipaddr_t *addr;
  PROCESS_BEGIN();
  powertrace_start(POWERTRACE_INTERVAL);  servreg_hack_init();
  set_global_address();
  simple_udp_register(&unicast_connection, UDP_PORT,                      NULL, UDP_PORT, receiver);
  etimer_set(&periodic_timer, SEND_INTERVAL);  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));    etimer_reset(&periodic_timer);    etimer_set(&send_timer, SEND_TIME);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));    addr = servreg_hack_lookup(SERVICE_ID);    if(addr != NULL) {      char buf[20];
      sprintf(buf, "c:1:%d", samples[random_rand() % DATA_SIZE]);      simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, addr);      printf("Sending unicast to ");      uip_debug_ipaddr_print(addr);      printf(": %s \n", buf);    } else {      printf("Service %d not found\n", SERVICE_ID);    }  }
  PROCESS_END();}