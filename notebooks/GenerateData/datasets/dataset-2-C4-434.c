#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15228, 15228, 15286, 15228, 15286, 15758, 15228, 15286, 15758, 15422, 15228, 15286, 15758, 15422, 15301, 15228, 15286, 15758, 15422, 15301, 15805, 15228, 15286, 15758, 15422, 15301, 15805, 15560, 15228, 15286, 15758, 15422, 15301, 15805, 15560, 15625, 15228, 15286, 15758, 15422, 15301, 15805, 15560, 15625, 15789, 15228, 15286, 15758, 15422, 15301, 15805, 15560, 15625, 15789, 15468, 15228, 15286, 15758, 15422, 15301, 15805, 15560, 15625, 15789, 15468, 15463, 15329, 15329, 15486, 15329, 15486, 15449, 15329, 15486, 15449, 15628, 15329, 15486, 15449, 15628, 14891, 15329, 15486, 15449, 15628, 14891, 15082, 15329, 15486, 15449, 15628, 14891, 15082, 15411, 15329, 15486, 15449, 15628, 14891, 15082, 15411, 15561, 15329, 15486, 15449, 15628, 14891, 15082, 15411, 15561, 15366, 15329, 15486, 15449, 15628, 14891, 15082, 15411, 15561, 15366, 15952, 15700, 15700, 15388, 15700, 15388, 15761, 15700, 15388, 15761, 15865, 15700, 15388, 15761, 15865, 15292, 15700, 15388, 15761, 15865, 15292, 15656, 15700, 15388, 15761, 15865, 15292, 15656, 15398, 15700, 15388, 15761, 15865, 15292, 15656, 15398, 15396, 15700, 15388, 15761, 15865, 15292, 15656, 15398, 15396, 15314, 15700, 15388, 15761, 15865, 15292, 15656, 15398, 15396, 15314, 15541, 15311, 15311, 15475, 15311, 15475, 15350, 15311, 15475, 15350, 15551, 15311, 15475, 15350, 15551, 15589, 15311, 15475, 15350, 15551, 15589, 15586, 15311, 15475, 15350, 15551, 15589, 15586, 15374, 15311, 15475, 15350, 15551, 15589, 15586, 15374, 15113, 15311, 15475, 15350, 15551, 15589, 15586, 15374, 15113, 15016, 15311, 15475, 15350, 15551, 15589, 15586, 15374, 15113, 15016, 15549, 15320, 15320, 15400, 15320, 15400, 15278, 15320, 15400, 15278, 15352, 15320, 15400, 15278, 15352, 15532, 15320, 15400, 15278, 15352, 15532, 15662, 15320, 15400, 15278, 15352, 15532, 15662, 15623, 15320, 15400, 15278, 15352, 15532, 15662, 15623, 15535, 15320, 15400, 15278, 15352, 15532, 15662, 15623, 15535, 15465, 15320, 15400, 15278, 15352, 15532, 15662, 15623, 15535, 15465, 15446, 15297, 15297, 15675, 15297, 15675, 15196, 15297, 15675, 15196, 15335, 15297, 15675, 15196, 15335, 15566, 15297, 15675, 15196, 15335, 15566, 15769, 15297, 15675, 15196, 15335, 15566, 15769, 15572, 15297, 15675, 15196, 15335, 15566, 15769, 15572, 15730, 15297, 15675, 15196, 15335, 15566, 15769, 15572, 15730, 15442, 15297, 15675, 15196, 15335, 15566, 15769, 15572, 15730, 15442, 15362, 15220, 15220, 15274, 15220, 15274, 15732, 15220, 15274, 15732, 15206, 15220, 15274, 15732, 15206, 15258, 15220, 15274, 15732, 15206, 15258, 15616, 15220, 15274, 15732, 15206, 15258, 15616, 15651, 15220, 15274, 15732, 15206, 15258, 15616, 15651, 15481, 15220, 15274, 15732, 15206, 15258, 15616, 15651, 15481, 15742, 15220, 15274, 15732, 15206, 15258, 15616, 15651, 15481, 15742, 15775, 15734, 15734, 15876, 15734, 15876, 15544, 15734, 15876, 15544, 15319, 15734, 15876, 15544, 15319, 15843, 15734, 15876, 15544, 15319, 15843, 15189, 15734, 15876, 15544, 15319, 15843, 15189, 15671, 15734, 15876, 15544, 15319, 15843, 15189, 15671, 15590, 15734, 15876, 15544, 15319, 15843, 15189, 15671, 15590, 15549, 15734, 15876, 15544, 15319, 15843, 15189, 15671, 15590, 15549, 15635, 15442, 15442, 15413, 15442, 15413, 15409, 15442, 15413, 15409, 15481, 15442, 15413, 15409, 15481, 15505, 15442, 15413, 15409, 15481, 15505, 15569, 15442, 15413, 15409, 15481, 15505, 15569, 15403, 15442, 15413, 15409, 15481, 15505, 15569, 15403, 15468, 15442, 15413, 15409, 15481, 15505, 15569, 15403, 15468, 15561, 15442, 15413, 15409, 15481, 15505, 15569, 15403, 15468, 15561, 15353, 15565, 15565, 15954, 15565, 15954, 15356, 15565, 15954, 15356, 15383, 15565, 15954, 15356, 15383, 15722, 15565, 15954, 15356, 15383, 15722, 15395, 15565, 15954, 15356, 15383, 15722, 15395, 15468, 15565, 15954, 15356, 15383, 15722, 15395, 15468, 15346, 15565, 15954, 15356, 15383, 15722, 15395, 15468, 15346, 15867, };static struct simple_udp_connection unicast_connection;
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