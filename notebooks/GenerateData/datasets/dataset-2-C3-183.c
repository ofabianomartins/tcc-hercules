#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15478, 15478, 15697, 15478, 15697, 15561, 15478, 15697, 15561, 15281, 15478, 15697, 15561, 15281, 15639, 15478, 15697, 15561, 15281, 15639, 15277, 15478, 15697, 15561, 15281, 15639, 15277, 15650, 15478, 15697, 15561, 15281, 15639, 15277, 15650, 15344, 15478, 15697, 15561, 15281, 15639, 15277, 15650, 15344, 15485, 15478, 15697, 15561, 15281, 15639, 15277, 15650, 15344, 15485, 15339, 15478, 15697, 15561, 15281, 15639, 15277, 15650, 15344, 15485, 15339, 15256, 15349, 15349, 15481, 15349, 15481, 15574, 15349, 15481, 15574, 15657, 15349, 15481, 15574, 15657, 15430, 15349, 15481, 15574, 15657, 15430, 15528, 15349, 15481, 15574, 15657, 15430, 15528, 15706, 15349, 15481, 15574, 15657, 15430, 15528, 15706, 15072, 15349, 15481, 15574, 15657, 15430, 15528, 15706, 15072, 15791, 15349, 15481, 15574, 15657, 15430, 15528, 15706, 15072, 15791, 15331, 15697, 15697, 15295, 15697, 15295, 15279, 15697, 15295, 15279, 15409, 15697, 15295, 15279, 15409, 15209, 15697, 15295, 15279, 15409, 15209, 15849, 15697, 15295, 15279, 15409, 15209, 15849, 15529, 15697, 15295, 15279, 15409, 15209, 15849, 15529, 15670, 15697, 15295, 15279, 15409, 15209, 15849, 15529, 15670, 15526, 15697, 15295, 15279, 15409, 15209, 15849, 15529, 15670, 15526, 15564, 15642, 15642, 15654, 15642, 15654, 15838, 15642, 15654, 15838, 15583, 15642, 15654, 15838, 15583, 15479, 15642, 15654, 15838, 15583, 15479, 15456, 15642, 15654, 15838, 15583, 15479, 15456, 15399, 15642, 15654, 15838, 15583, 15479, 15456, 15399, 15581, 15642, 15654, 15838, 15583, 15479, 15456, 15399, 15581, 15494, 15642, 15654, 15838, 15583, 15479, 15456, 15399, 15581, 15494, 15630, 15525, 15525, 15276, 15525, 15276, 15427, 15525, 15276, 15427, 15734, 15525, 15276, 15427, 15734, 15718, 15525, 15276, 15427, 15734, 15718, 15259, 15525, 15276, 15427, 15734, 15718, 15259, 15674, 15525, 15276, 15427, 15734, 15718, 15259, 15674, 15462, 15525, 15276, 15427, 15734, 15718, 15259, 15674, 15462, 15409, 15525, 15276, 15427, 15734, 15718, 15259, 15674, 15462, 15409, 15590, 15323, 15323, 15518, 15323, 15518, 15761, 15323, 15518, 15761, 15519, 15323, 15518, 15761, 15519, 15829, 15323, 15518, 15761, 15519, 15829, 15738, 15323, 15518, 15761, 15519, 15829, 15738, 15712, 15323, 15518, 15761, 15519, 15829, 15738, 15712, 15393, 15323, 15518, 15761, 15519, 15829, 15738, 15712, 15393, 15627, 15323, 15518, 15761, 15519, 15829, 15738, 15712, 15393, 15627, 15374, 15743, 15743, 15165, 15743, 15165, 15613, 15743, 15165, 15613, 15586, 15743, 15165, 15613, 15586, 15416, 15743, 15165, 15613, 15586, 15416, 15492, 15743, 15165, 15613, 15586, 15416, 15492, 15480, 15743, 15165, 15613, 15586, 15416, 15492, 15480, 15403, 15743, 15165, 15613, 15586, 15416, 15492, 15480, 15403, 15754, 15743, 15165, 15613, 15586, 15416, 15492, 15480, 15403, 15754, 15396, 15502, 15502, 15688, 15502, 15688, 15856, 15502, 15688, 15856, 15462, 15502, 15688, 15856, 15462, 15502, 15502, 15688, 15856, 15462, 15502, 15715, 15502, 15688, 15856, 15462, 15502, 15715, 15483, 15502, 15688, 15856, 15462, 15502, 15715, 15483, 15387, 15502, 15688, 15856, 15462, 15502, 15715, 15483, 15387, 15259, 15502, 15688, 15856, 15462, 15502, 15715, 15483, 15387, 15259, 15833, 15570, 15570, 15509, 15570, 15509, 15520, 15570, 15509, 15520, 15453, 15570, 15509, 15520, 15453, 15640, 15570, 15509, 15520, 15453, 15640, 15448, 15570, 15509, 15520, 15453, 15640, 15448, 15469, 15570, 15509, 15520, 15453, 15640, 15448, 15469, 15715, 15570, 15509, 15520, 15453, 15640, 15448, 15469, 15715, 15365, 15570, 15509, 15520, 15453, 15640, 15448, 15469, 15715, 15365, 15355, 15696, 15696, 15721, 15696, 15721, 15369, 15696, 15721, 15369, 15363, 15696, 15721, 15369, 15363, 15263, 15696, 15721, 15369, 15363, 15263, 15378, 15696, 15721, 15369, 15363, 15263, 15378, 15423, 15696, 15721, 15369, 15363, 15263, 15378, 15423, 15942, 15696, 15721, 15369, 15363, 15263, 15378, 15423, 15942, 15168, };static struct simple_udp_connection unicast_connection;
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