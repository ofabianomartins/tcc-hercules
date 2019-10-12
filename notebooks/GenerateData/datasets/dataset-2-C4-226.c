#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15440, 15440, 15650, 15440, 15650, 15728, 15440, 15650, 15728, 15697, 15440, 15650, 15728, 15697, 15586, 15440, 15650, 15728, 15697, 15586, 15463, 15440, 15650, 15728, 15697, 15586, 15463, 15018, 15440, 15650, 15728, 15697, 15586, 15463, 15018, 15377, 15440, 15650, 15728, 15697, 15586, 15463, 15018, 15377, 15305, 15440, 15650, 15728, 15697, 15586, 15463, 15018, 15377, 15305, 15394, 15440, 15650, 15728, 15697, 15586, 15463, 15018, 15377, 15305, 15394, 15825, 15587, 15587, 15359, 15587, 15359, 15506, 15587, 15359, 15506, 15580, 15587, 15359, 15506, 15580, 15732, 15587, 15359, 15506, 15580, 15732, 15565, 15587, 15359, 15506, 15580, 15732, 15565, 15355, 15587, 15359, 15506, 15580, 15732, 15565, 15355, 15280, 15587, 15359, 15506, 15580, 15732, 15565, 15355, 15280, 15312, 15587, 15359, 15506, 15580, 15732, 15565, 15355, 15280, 15312, 15582, 15501, 15501, 15567, 15501, 15567, 15324, 15501, 15567, 15324, 15341, 15501, 15567, 15324, 15341, 15643, 15501, 15567, 15324, 15341, 15643, 15657, 15501, 15567, 15324, 15341, 15643, 15657, 15434, 15501, 15567, 15324, 15341, 15643, 15657, 15434, 15464, 15501, 15567, 15324, 15341, 15643, 15657, 15434, 15464, 15490, 15501, 15567, 15324, 15341, 15643, 15657, 15434, 15464, 15490, 15311, 15463, 15463, 15969, 15463, 15969, 15427, 15463, 15969, 15427, 16086, 15463, 15969, 15427, 16086, 15569, 15463, 15969, 15427, 16086, 15569, 15664, 15463, 15969, 15427, 16086, 15569, 15664, 15556, 15463, 15969, 15427, 16086, 15569, 15664, 15556, 15104, 15463, 15969, 15427, 16086, 15569, 15664, 15556, 15104, 15355, 15463, 15969, 15427, 16086, 15569, 15664, 15556, 15104, 15355, 15107, 15699, 15699, 15465, 15699, 15465, 15490, 15699, 15465, 15490, 15667, 15699, 15465, 15490, 15667, 15442, 15699, 15465, 15490, 15667, 15442, 15871, 15699, 15465, 15490, 15667, 15442, 15871, 15553, 15699, 15465, 15490, 15667, 15442, 15871, 15553, 15551, 15699, 15465, 15490, 15667, 15442, 15871, 15553, 15551, 15683, 15699, 15465, 15490, 15667, 15442, 15871, 15553, 15551, 15683, 15698, 15340, 15340, 15481, 15340, 15481, 15702, 15340, 15481, 15702, 15456, 15340, 15481, 15702, 15456, 15521, 15340, 15481, 15702, 15456, 15521, 15471, 15340, 15481, 15702, 15456, 15521, 15471, 15772, 15340, 15481, 15702, 15456, 15521, 15471, 15772, 15420, 15340, 15481, 15702, 15456, 15521, 15471, 15772, 15420, 15504, 15340, 15481, 15702, 15456, 15521, 15471, 15772, 15420, 15504, 15495, 15654, 15654, 15428, 15654, 15428, 15460, 15654, 15428, 15460, 15221, 15654, 15428, 15460, 15221, 15748, 15654, 15428, 15460, 15221, 15748, 15977, 15654, 15428, 15460, 15221, 15748, 15977, 15168, 15654, 15428, 15460, 15221, 15748, 15977, 15168, 15141, 15654, 15428, 15460, 15221, 15748, 15977, 15168, 15141, 15710, 15654, 15428, 15460, 15221, 15748, 15977, 15168, 15141, 15710, 15467, 15850, 15850, 15463, 15850, 15463, 15595, 15850, 15463, 15595, 15272, 15850, 15463, 15595, 15272, 15709, 15850, 15463, 15595, 15272, 15709, 15654, 15850, 15463, 15595, 15272, 15709, 15654, 15688, 15850, 15463, 15595, 15272, 15709, 15654, 15688, 15416, 15850, 15463, 15595, 15272, 15709, 15654, 15688, 15416, 15712, 15850, 15463, 15595, 15272, 15709, 15654, 15688, 15416, 15712, 15118, 15319, 15319, 15753, 15319, 15753, 15346, 15319, 15753, 15346, 15521, 15319, 15753, 15346, 15521, 15449, 15319, 15753, 15346, 15521, 15449, 15559, 15319, 15753, 15346, 15521, 15449, 15559, 15039, 15319, 15753, 15346, 15521, 15449, 15559, 15039, 15256, 15319, 15753, 15346, 15521, 15449, 15559, 15039, 15256, 15310, 15319, 15753, 15346, 15521, 15449, 15559, 15039, 15256, 15310, 15681, 15491, 15491, 15362, 15491, 15362, 15977, 15491, 15362, 15977, 15397, 15491, 15362, 15977, 15397, 15636, 15491, 15362, 15977, 15397, 15636, 15434, 15491, 15362, 15977, 15397, 15636, 15434, 15593, 15491, 15362, 15977, 15397, 15636, 15434, 15593, 15715, 15491, 15362, 15977, 15397, 15636, 15434, 15593, 15715, 15385, };static struct simple_udp_connection unicast_connection;
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