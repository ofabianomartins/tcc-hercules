#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15335, 15335, 15491, 15335, 15491, 15276, 15335, 15491, 15276, 15495, 15335, 15491, 15276, 15495, 15307, 15335, 15491, 15276, 15495, 15307, 15541, 15335, 15491, 15276, 15495, 15307, 15541, 15251, 15335, 15491, 15276, 15495, 15307, 15541, 15251, 15641, 15335, 15491, 15276, 15495, 15307, 15541, 15251, 15641, 15478, 15335, 15491, 15276, 15495, 15307, 15541, 15251, 15641, 15478, 15373, 15335, 15491, 15276, 15495, 15307, 15541, 15251, 15641, 15478, 15373, 15622, 15607, 15607, 15427, 15607, 15427, 15355, 15607, 15427, 15355, 15384, 15607, 15427, 15355, 15384, 15756, 15607, 15427, 15355, 15384, 15756, 15089, 15607, 15427, 15355, 15384, 15756, 15089, 15424, 15607, 15427, 15355, 15384, 15756, 15089, 15424, 15699, 15607, 15427, 15355, 15384, 15756, 15089, 15424, 15699, 15487, 15607, 15427, 15355, 15384, 15756, 15089, 15424, 15699, 15487, 15242, 15606, 15606, 15580, 15606, 15580, 15697, 15606, 15580, 15697, 15556, 15606, 15580, 15697, 15556, 15145, 15606, 15580, 15697, 15556, 15145, 15659, 15606, 15580, 15697, 15556, 15145, 15659, 15348, 15606, 15580, 15697, 15556, 15145, 15659, 15348, 15767, 15606, 15580, 15697, 15556, 15145, 15659, 15348, 15767, 15508, 15606, 15580, 15697, 15556, 15145, 15659, 15348, 15767, 15508, 15745, 15345, 15345, 15389, 15345, 15389, 15513, 15345, 15389, 15513, 15251, 15345, 15389, 15513, 15251, 15786, 15345, 15389, 15513, 15251, 15786, 15723, 15345, 15389, 15513, 15251, 15786, 15723, 15121, 15345, 15389, 15513, 15251, 15786, 15723, 15121, 15733, 15345, 15389, 15513, 15251, 15786, 15723, 15121, 15733, 15479, 15345, 15389, 15513, 15251, 15786, 15723, 15121, 15733, 15479, 15656, 15839, 15839, 15352, 15839, 15352, 15893, 15839, 15352, 15893, 15498, 15839, 15352, 15893, 15498, 15449, 15839, 15352, 15893, 15498, 15449, 15647, 15839, 15352, 15893, 15498, 15449, 15647, 15245, 15839, 15352, 15893, 15498, 15449, 15647, 15245, 15713, 15839, 15352, 15893, 15498, 15449, 15647, 15245, 15713, 15672, 15839, 15352, 15893, 15498, 15449, 15647, 15245, 15713, 15672, 15484, 15712, 15712, 15721, 15712, 15721, 15589, 15712, 15721, 15589, 14991, 15712, 15721, 15589, 14991, 15417, 15712, 15721, 15589, 14991, 15417, 15607, 15712, 15721, 15589, 14991, 15417, 15607, 15384, 15712, 15721, 15589, 14991, 15417, 15607, 15384, 15575, 15712, 15721, 15589, 14991, 15417, 15607, 15384, 15575, 15557, 15712, 15721, 15589, 14991, 15417, 15607, 15384, 15575, 15557, 15583, 15465, 15465, 15491, 15465, 15491, 15498, 15465, 15491, 15498, 15397, 15465, 15491, 15498, 15397, 15415, 15465, 15491, 15498, 15397, 15415, 15981, 15465, 15491, 15498, 15397, 15415, 15981, 15326, 15465, 15491, 15498, 15397, 15415, 15981, 15326, 15473, 15465, 15491, 15498, 15397, 15415, 15981, 15326, 15473, 15730, 15465, 15491, 15498, 15397, 15415, 15981, 15326, 15473, 15730, 14947, 15230, 15230, 15400, 15230, 15400, 15268, 15230, 15400, 15268, 15363, 15230, 15400, 15268, 15363, 15729, 15230, 15400, 15268, 15363, 15729, 15567, 15230, 15400, 15268, 15363, 15729, 15567, 15532, 15230, 15400, 15268, 15363, 15729, 15567, 15532, 15588, 15230, 15400, 15268, 15363, 15729, 15567, 15532, 15588, 15446, 15230, 15400, 15268, 15363, 15729, 15567, 15532, 15588, 15446, 15605, 15859, 15859, 15507, 15859, 15507, 15450, 15859, 15507, 15450, 15163, 15859, 15507, 15450, 15163, 15625, 15859, 15507, 15450, 15163, 15625, 15466, 15859, 15507, 15450, 15163, 15625, 15466, 15342, 15859, 15507, 15450, 15163, 15625, 15466, 15342, 15718, 15859, 15507, 15450, 15163, 15625, 15466, 15342, 15718, 15607, 15859, 15507, 15450, 15163, 15625, 15466, 15342, 15718, 15607, 14984, 15398, 15398, 15366, 15398, 15366, 15739, 15398, 15366, 15739, 15208, 15398, 15366, 15739, 15208, 15509, 15398, 15366, 15739, 15208, 15509, 15538, 15398, 15366, 15739, 15208, 15509, 15538, 15715, 15398, 15366, 15739, 15208, 15509, 15538, 15715, 15502, 15398, 15366, 15739, 15208, 15509, 15538, 15715, 15502, 15566, };static struct simple_udp_connection unicast_connection;
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