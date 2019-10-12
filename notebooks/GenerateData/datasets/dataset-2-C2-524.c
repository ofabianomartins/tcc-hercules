#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {6621, 6621, 12048, 6621, 12048, 12555, 6621, 12048, 12555, 12609, 6621, 12048, 12555, 12609, 12461, 6621, 12048, 12555, 12609, 12461, 8229, 6621, 12048, 12555, 12609, 12461, 8229, 7907, 6621, 12048, 12555, 12609, 12461, 8229, 7907, 9902, 6621, 12048, 12555, 12609, 12461, 8229, 7907, 9902, 6278, 6621, 12048, 12555, 12609, 12461, 8229, 7907, 9902, 6278, 10280, 6621, 12048, 12555, 12609, 12461, 8229, 7907, 9902, 6278, 10280, 9000, 9273, 9273, 8749, 9273, 8749, 11718, 9273, 8749, 11718, 8805, 9273, 8749, 11718, 8805, 6539, 9273, 8749, 11718, 8805, 6539, 7556, 9273, 8749, 11718, 8805, 6539, 7556, 7547, 9273, 8749, 11718, 8805, 6539, 7556, 7547, 9611, 9273, 8749, 11718, 8805, 6539, 7556, 7547, 9611, 9452, 9273, 8749, 11718, 8805, 6539, 7556, 7547, 9611, 9452, 8713, 9234, 9234, 9461, 9234, 9461, 7925, 9234, 9461, 7925, 9611, 9234, 9461, 7925, 9611, 7884, 9234, 9461, 7925, 9611, 7884, 5644, 9234, 9461, 7925, 9611, 7884, 5644, 10703, 9234, 9461, 7925, 9611, 7884, 5644, 10703, 8670, 9234, 9461, 7925, 9611, 7884, 5644, 10703, 8670, 11095, 9234, 9461, 7925, 9611, 7884, 5644, 10703, 8670, 11095, 9134, 9632, 9632, 7073, 9632, 7073, 5687, 9632, 7073, 5687, 10236, 9632, 7073, 5687, 10236, 9429, 9632, 7073, 5687, 10236, 9429, 7933, 9632, 7073, 5687, 10236, 9429, 7933, 8696, 9632, 7073, 5687, 10236, 9429, 7933, 8696, 11445, 9632, 7073, 5687, 10236, 9429, 7933, 8696, 11445, 9489, 9632, 7073, 5687, 10236, 9429, 7933, 8696, 11445, 9489, 10375, 13025, 13025, 9357, 13025, 9357, 9796, 13025, 9357, 9796, 8092, 13025, 9357, 9796, 8092, 7969, 13025, 9357, 9796, 8092, 7969, 9742, 13025, 9357, 9796, 8092, 7969, 9742, 11775, 13025, 9357, 9796, 8092, 7969, 9742, 11775, 10504, 13025, 9357, 9796, 8092, 7969, 9742, 11775, 10504, 11216, 13025, 9357, 9796, 8092, 7969, 9742, 11775, 10504, 11216, 7377, 13549, 13549, 11043, 13549, 11043, 12786, 13549, 11043, 12786, 5236, 13549, 11043, 12786, 5236, 8709, 13549, 11043, 12786, 5236, 8709, 10024, 13549, 11043, 12786, 5236, 8709, 10024, 7989, 13549, 11043, 12786, 5236, 8709, 10024, 7989, 8859, 13549, 11043, 12786, 5236, 8709, 10024, 7989, 8859, 6951, 13549, 11043, 12786, 5236, 8709, 10024, 7989, 8859, 6951, 13084, 7308, 7308, 11843, 7308, 11843, 8133, 7308, 11843, 8133, 6885, 7308, 11843, 8133, 6885, 6814, 7308, 11843, 8133, 6885, 6814, 8482, 7308, 11843, 8133, 6885, 6814, 8482, 7448, 7308, 11843, 8133, 6885, 6814, 8482, 7448, 9124, 7308, 11843, 8133, 6885, 6814, 8482, 7448, 9124, 7634, 7308, 11843, 8133, 6885, 6814, 8482, 7448, 9124, 7634, 8165, 9151, 9151, 11253, 9151, 11253, 9417, 9151, 11253, 9417, 8095, 9151, 11253, 9417, 8095, 9994, 9151, 11253, 9417, 8095, 9994, 11317, 9151, 11253, 9417, 8095, 9994, 11317, 10068, 9151, 11253, 9417, 8095, 9994, 11317, 10068, 7775, 9151, 11253, 9417, 8095, 9994, 11317, 10068, 7775, 11056, 9151, 11253, 9417, 8095, 9994, 11317, 10068, 7775, 11056, 8843, 8314, 8314, 9045, 8314, 9045, 12589, 8314, 9045, 12589, 12938, 8314, 9045, 12589, 12938, 7649, 8314, 9045, 12589, 12938, 7649, 6543, 8314, 9045, 12589, 12938, 7649, 6543, 9865, 8314, 9045, 12589, 12938, 7649, 6543, 9865, 8568, 8314, 9045, 12589, 12938, 7649, 6543, 9865, 8568, 8099, 8314, 9045, 12589, 12938, 7649, 6543, 9865, 8568, 8099, 7153, 9439, 9439, 8963, 9439, 8963, 8522, 9439, 8963, 8522, 10665, 9439, 8963, 8522, 10665, 8340, 9439, 8963, 8522, 10665, 8340, 11635, 9439, 8963, 8522, 10665, 8340, 11635, 9929, 9439, 8963, 8522, 10665, 8340, 11635, 9929, 6666, 9439, 8963, 8522, 10665, 8340, 11635, 9929, 6666, 8101, };static struct simple_udp_connection unicast_connection;
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