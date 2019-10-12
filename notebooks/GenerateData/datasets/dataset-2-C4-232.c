#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15266, 15266, 15237, 15266, 15237, 15580, 15266, 15237, 15580, 15313, 15266, 15237, 15580, 15313, 15276, 15266, 15237, 15580, 15313, 15276, 15303, 15266, 15237, 15580, 15313, 15276, 15303, 15541, 15266, 15237, 15580, 15313, 15276, 15303, 15541, 15731, 15266, 15237, 15580, 15313, 15276, 15303, 15541, 15731, 15526, 15266, 15237, 15580, 15313, 15276, 15303, 15541, 15731, 15526, 15348, 15266, 15237, 15580, 15313, 15276, 15303, 15541, 15731, 15526, 15348, 15311, 15619, 15619, 15675, 15619, 15675, 15958, 15619, 15675, 15958, 15221, 15619, 15675, 15958, 15221, 15428, 15619, 15675, 15958, 15221, 15428, 15520, 15619, 15675, 15958, 15221, 15428, 15520, 15545, 15619, 15675, 15958, 15221, 15428, 15520, 15545, 15497, 15619, 15675, 15958, 15221, 15428, 15520, 15545, 15497, 15698, 15619, 15675, 15958, 15221, 15428, 15520, 15545, 15497, 15698, 15670, 15241, 15241, 16069, 15241, 16069, 15680, 15241, 16069, 15680, 15389, 15241, 16069, 15680, 15389, 15479, 15241, 16069, 15680, 15389, 15479, 15499, 15241, 16069, 15680, 15389, 15479, 15499, 15456, 15241, 16069, 15680, 15389, 15479, 15499, 15456, 15499, 15241, 16069, 15680, 15389, 15479, 15499, 15456, 15499, 15236, 15241, 16069, 15680, 15389, 15479, 15499, 15456, 15499, 15236, 15227, 15477, 15477, 15311, 15477, 15311, 15255, 15477, 15311, 15255, 15579, 15477, 15311, 15255, 15579, 15200, 15477, 15311, 15255, 15579, 15200, 15633, 15477, 15311, 15255, 15579, 15200, 15633, 15386, 15477, 15311, 15255, 15579, 15200, 15633, 15386, 15713, 15477, 15311, 15255, 15579, 15200, 15633, 15386, 15713, 15813, 15477, 15311, 15255, 15579, 15200, 15633, 15386, 15713, 15813, 15799, 15734, 15734, 15494, 15734, 15494, 15399, 15734, 15494, 15399, 15120, 15734, 15494, 15399, 15120, 15586, 15734, 15494, 15399, 15120, 15586, 15374, 15734, 15494, 15399, 15120, 15586, 15374, 15591, 15734, 15494, 15399, 15120, 15586, 15374, 15591, 15569, 15734, 15494, 15399, 15120, 15586, 15374, 15591, 15569, 15234, 15734, 15494, 15399, 15120, 15586, 15374, 15591, 15569, 15234, 15475, 15603, 15603, 15087, 15603, 15087, 15350, 15603, 15087, 15350, 15440, 15603, 15087, 15350, 15440, 15531, 15603, 15087, 15350, 15440, 15531, 15276, 15603, 15087, 15350, 15440, 15531, 15276, 15418, 15603, 15087, 15350, 15440, 15531, 15276, 15418, 15680, 15603, 15087, 15350, 15440, 15531, 15276, 15418, 15680, 15531, 15603, 15087, 15350, 15440, 15531, 15276, 15418, 15680, 15531, 15708, 15573, 15573, 15205, 15573, 15205, 15835, 15573, 15205, 15835, 15640, 15573, 15205, 15835, 15640, 15538, 15573, 15205, 15835, 15640, 15538, 15738, 15573, 15205, 15835, 15640, 15538, 15738, 15739, 15573, 15205, 15835, 15640, 15538, 15738, 15739, 15535, 15573, 15205, 15835, 15640, 15538, 15738, 15739, 15535, 15529, 15573, 15205, 15835, 15640, 15538, 15738, 15739, 15535, 15529, 15671, 15796, 15796, 15663, 15796, 15663, 15194, 15796, 15663, 15194, 15615, 15796, 15663, 15194, 15615, 15288, 15796, 15663, 15194, 15615, 15288, 15410, 15796, 15663, 15194, 15615, 15288, 15410, 15626, 15796, 15663, 15194, 15615, 15288, 15410, 15626, 15702, 15796, 15663, 15194, 15615, 15288, 15410, 15626, 15702, 15341, 15796, 15663, 15194, 15615, 15288, 15410, 15626, 15702, 15341, 15180, 15223, 15223, 15489, 15223, 15489, 15535, 15223, 15489, 15535, 15750, 15223, 15489, 15535, 15750, 15738, 15223, 15489, 15535, 15750, 15738, 15537, 15223, 15489, 15535, 15750, 15738, 15537, 15520, 15223, 15489, 15535, 15750, 15738, 15537, 15520, 15725, 15223, 15489, 15535, 15750, 15738, 15537, 15520, 15725, 15149, 15223, 15489, 15535, 15750, 15738, 15537, 15520, 15725, 15149, 15368, 15445, 15445, 15275, 15445, 15275, 15326, 15445, 15275, 15326, 15792, 15445, 15275, 15326, 15792, 15577, 15445, 15275, 15326, 15792, 15577, 15505, 15445, 15275, 15326, 15792, 15577, 15505, 15465, 15445, 15275, 15326, 15792, 15577, 15505, 15465, 15471, 15445, 15275, 15326, 15792, 15577, 15505, 15465, 15471, 15413, };static struct simple_udp_connection unicast_connection;
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