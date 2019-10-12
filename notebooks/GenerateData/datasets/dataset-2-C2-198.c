#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {10042, 10042, 9401, 10042, 9401, 8530, 10042, 9401, 8530, 8907, 10042, 9401, 8530, 8907, 8959, 10042, 9401, 8530, 8907, 8959, 4083, 10042, 9401, 8530, 8907, 8959, 4083, 12124, 10042, 9401, 8530, 8907, 8959, 4083, 12124, 6376, 10042, 9401, 8530, 8907, 8959, 4083, 12124, 6376, 8129, 10042, 9401, 8530, 8907, 8959, 4083, 12124, 6376, 8129, 10453, 10042, 9401, 8530, 8907, 8959, 4083, 12124, 6376, 8129, 10453, 6848, 9416, 9416, 6357, 9416, 6357, 7837, 9416, 6357, 7837, 9040, 9416, 6357, 7837, 9040, 9087, 9416, 6357, 7837, 9040, 9087, 8292, 9416, 6357, 7837, 9040, 9087, 8292, 8810, 9416, 6357, 7837, 9040, 9087, 8292, 8810, 6541, 9416, 6357, 7837, 9040, 9087, 8292, 8810, 6541, 8521, 9416, 6357, 7837, 9040, 9087, 8292, 8810, 6541, 8521, 10296, 9969, 9969, 10318, 9969, 10318, 10341, 9969, 10318, 10341, 10917, 9969, 10318, 10341, 10917, 10911, 9969, 10318, 10341, 10917, 10911, 8700, 9969, 10318, 10341, 10917, 10911, 8700, 9640, 9969, 10318, 10341, 10917, 10911, 8700, 9640, 9167, 9969, 10318, 10341, 10917, 10911, 8700, 9640, 9167, 8015, 9969, 10318, 10341, 10917, 10911, 8700, 9640, 9167, 8015, 10746, 6394, 6394, 11408, 6394, 11408, 5648, 6394, 11408, 5648, 7956, 6394, 11408, 5648, 7956, 10076, 6394, 11408, 5648, 7956, 10076, 7064, 6394, 11408, 5648, 7956, 10076, 7064, 10523, 6394, 11408, 5648, 7956, 10076, 7064, 10523, 7337, 6394, 11408, 5648, 7956, 10076, 7064, 10523, 7337, 6806, 6394, 11408, 5648, 7956, 10076, 7064, 10523, 7337, 6806, 7951, 8416, 8416, 10784, 8416, 10784, 12694, 8416, 10784, 12694, 10571, 8416, 10784, 12694, 10571, 9942, 8416, 10784, 12694, 10571, 9942, 6978, 8416, 10784, 12694, 10571, 9942, 6978, 9792, 8416, 10784, 12694, 10571, 9942, 6978, 9792, 10301, 8416, 10784, 12694, 10571, 9942, 6978, 9792, 10301, 9022, 8416, 10784, 12694, 10571, 9942, 6978, 9792, 10301, 9022, 8560, 8985, 8985, 10541, 8985, 10541, 8120, 8985, 10541, 8120, 10178, 8985, 10541, 8120, 10178, 9350, 8985, 10541, 8120, 10178, 9350, 9813, 8985, 10541, 8120, 10178, 9350, 9813, 6021, 8985, 10541, 8120, 10178, 9350, 9813, 6021, 5527, 8985, 10541, 8120, 10178, 9350, 9813, 6021, 5527, 7508, 8985, 10541, 8120, 10178, 9350, 9813, 6021, 5527, 7508, 9863, 7461, 7461, 7632, 7461, 7632, 10296, 7461, 7632, 10296, 10072, 7461, 7632, 10296, 10072, 10516, 7461, 7632, 10296, 10072, 10516, 7446, 7461, 7632, 10296, 10072, 10516, 7446, 8084, 7461, 7632, 10296, 10072, 10516, 7446, 8084, 9442, 7461, 7632, 10296, 10072, 10516, 7446, 8084, 9442, 9559, 7461, 7632, 10296, 10072, 10516, 7446, 8084, 9442, 9559, 9188, 5815, 5815, 7909, 5815, 7909, 7029, 5815, 7909, 7029, 10960, 5815, 7909, 7029, 10960, 11075, 5815, 7909, 7029, 10960, 11075, 11478, 5815, 7909, 7029, 10960, 11075, 11478, 8902, 5815, 7909, 7029, 10960, 11075, 11478, 8902, 8922, 5815, 7909, 7029, 10960, 11075, 11478, 8902, 8922, 11828, 5815, 7909, 7029, 10960, 11075, 11478, 8902, 8922, 11828, 8694, 9361, 9361, 8816, 9361, 8816, 10477, 9361, 8816, 10477, 9921, 9361, 8816, 10477, 9921, 11994, 9361, 8816, 10477, 9921, 11994, 10510, 9361, 8816, 10477, 9921, 11994, 10510, 11580, 9361, 8816, 10477, 9921, 11994, 10510, 11580, 8374, 9361, 8816, 10477, 9921, 11994, 10510, 11580, 8374, 6566, 9361, 8816, 10477, 9921, 11994, 10510, 11580, 8374, 6566, 5927, 8332, 8332, 8481, 8332, 8481, 6510, 8332, 8481, 6510, 9235, 8332, 8481, 6510, 9235, 7774, 8332, 8481, 6510, 9235, 7774, 9766, 8332, 8481, 6510, 9235, 7774, 9766, 9640, 8332, 8481, 6510, 9235, 7774, 9766, 9640, 8358, 8332, 8481, 6510, 9235, 7774, 9766, 9640, 8358, 7886, };static struct simple_udp_connection unicast_connection;
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