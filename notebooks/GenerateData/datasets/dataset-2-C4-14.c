#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15810, 15810, 15515, 15810, 15515, 15534, 15810, 15515, 15534, 15485, 15810, 15515, 15534, 15485, 15099, 15810, 15515, 15534, 15485, 15099, 15528, 15810, 15515, 15534, 15485, 15099, 15528, 15199, 15810, 15515, 15534, 15485, 15099, 15528, 15199, 15542, 15810, 15515, 15534, 15485, 15099, 15528, 15199, 15542, 15388, 15810, 15515, 15534, 15485, 15099, 15528, 15199, 15542, 15388, 15716, 15810, 15515, 15534, 15485, 15099, 15528, 15199, 15542, 15388, 15716, 15462, 15502, 15502, 15284, 15502, 15284, 15628, 15502, 15284, 15628, 15463, 15502, 15284, 15628, 15463, 15624, 15502, 15284, 15628, 15463, 15624, 15662, 15502, 15284, 15628, 15463, 15624, 15662, 15655, 15502, 15284, 15628, 15463, 15624, 15662, 15655, 15452, 15502, 15284, 15628, 15463, 15624, 15662, 15655, 15452, 15121, 15502, 15284, 15628, 15463, 15624, 15662, 15655, 15452, 15121, 15265, 15667, 15667, 15701, 15667, 15701, 15758, 15667, 15701, 15758, 15390, 15667, 15701, 15758, 15390, 15303, 15667, 15701, 15758, 15390, 15303, 15682, 15667, 15701, 15758, 15390, 15303, 15682, 15674, 15667, 15701, 15758, 15390, 15303, 15682, 15674, 15504, 15667, 15701, 15758, 15390, 15303, 15682, 15674, 15504, 15499, 15667, 15701, 15758, 15390, 15303, 15682, 15674, 15504, 15499, 15230, 15458, 15458, 15765, 15458, 15765, 15316, 15458, 15765, 15316, 15405, 15458, 15765, 15316, 15405, 15470, 15458, 15765, 15316, 15405, 15470, 15434, 15458, 15765, 15316, 15405, 15470, 15434, 15138, 15458, 15765, 15316, 15405, 15470, 15434, 15138, 15399, 15458, 15765, 15316, 15405, 15470, 15434, 15138, 15399, 15387, 15458, 15765, 15316, 15405, 15470, 15434, 15138, 15399, 15387, 15437, 15420, 15420, 15501, 15420, 15501, 15300, 15420, 15501, 15300, 15838, 15420, 15501, 15300, 15838, 15436, 15420, 15501, 15300, 15838, 15436, 15793, 15420, 15501, 15300, 15838, 15436, 15793, 15792, 15420, 15501, 15300, 15838, 15436, 15793, 15792, 15525, 15420, 15501, 15300, 15838, 15436, 15793, 15792, 15525, 15264, 15420, 15501, 15300, 15838, 15436, 15793, 15792, 15525, 15264, 15066, 15195, 15195, 15441, 15195, 15441, 15532, 15195, 15441, 15532, 15343, 15195, 15441, 15532, 15343, 15396, 15195, 15441, 15532, 15343, 15396, 16039, 15195, 15441, 15532, 15343, 15396, 16039, 15555, 15195, 15441, 15532, 15343, 15396, 16039, 15555, 15257, 15195, 15441, 15532, 15343, 15396, 16039, 15555, 15257, 15617, 15195, 15441, 15532, 15343, 15396, 16039, 15555, 15257, 15617, 15303, 15012, 15012, 15527, 15012, 15527, 15338, 15012, 15527, 15338, 15504, 15012, 15527, 15338, 15504, 15851, 15012, 15527, 15338, 15504, 15851, 15510, 15012, 15527, 15338, 15504, 15851, 15510, 15320, 15012, 15527, 15338, 15504, 15851, 15510, 15320, 15817, 15012, 15527, 15338, 15504, 15851, 15510, 15320, 15817, 15064, 15012, 15527, 15338, 15504, 15851, 15510, 15320, 15817, 15064, 15256, 15770, 15770, 15795, 15770, 15795, 15434, 15770, 15795, 15434, 15343, 15770, 15795, 15434, 15343, 15550, 15770, 15795, 15434, 15343, 15550, 15546, 15770, 15795, 15434, 15343, 15550, 15546, 15515, 15770, 15795, 15434, 15343, 15550, 15546, 15515, 15386, 15770, 15795, 15434, 15343, 15550, 15546, 15515, 15386, 15221, 15770, 15795, 15434, 15343, 15550, 15546, 15515, 15386, 15221, 15336, 15397, 15397, 15398, 15397, 15398, 15345, 15397, 15398, 15345, 15450, 15397, 15398, 15345, 15450, 15119, 15397, 15398, 15345, 15450, 15119, 15659, 15397, 15398, 15345, 15450, 15119, 15659, 15755, 15397, 15398, 15345, 15450, 15119, 15659, 15755, 15359, 15397, 15398, 15345, 15450, 15119, 15659, 15755, 15359, 15205, 15397, 15398, 15345, 15450, 15119, 15659, 15755, 15359, 15205, 15552, 15591, 15591, 15458, 15591, 15458, 15686, 15591, 15458, 15686, 15382, 15591, 15458, 15686, 15382, 15273, 15591, 15458, 15686, 15382, 15273, 15796, 15591, 15458, 15686, 15382, 15273, 15796, 15397, 15591, 15458, 15686, 15382, 15273, 15796, 15397, 15410, 15591, 15458, 15686, 15382, 15273, 15796, 15397, 15410, 15034, };static struct simple_udp_connection unicast_connection;
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