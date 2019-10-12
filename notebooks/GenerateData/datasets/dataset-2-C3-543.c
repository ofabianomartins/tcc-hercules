#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15396, 15396, 15404, 15396, 15404, 15528, 15396, 15404, 15528, 15445, 15396, 15404, 15528, 15445, 15336, 15396, 15404, 15528, 15445, 15336, 15672, 15396, 15404, 15528, 15445, 15336, 15672, 15657, 15396, 15404, 15528, 15445, 15336, 15672, 15657, 15498, 15396, 15404, 15528, 15445, 15336, 15672, 15657, 15498, 15397, 15396, 15404, 15528, 15445, 15336, 15672, 15657, 15498, 15397, 15297, 15396, 15404, 15528, 15445, 15336, 15672, 15657, 15498, 15397, 15297, 15482, 15594, 15594, 15001, 15594, 15001, 15542, 15594, 15001, 15542, 15699, 15594, 15001, 15542, 15699, 15238, 15594, 15001, 15542, 15699, 15238, 15825, 15594, 15001, 15542, 15699, 15238, 15825, 15715, 15594, 15001, 15542, 15699, 15238, 15825, 15715, 15931, 15594, 15001, 15542, 15699, 15238, 15825, 15715, 15931, 15962, 15594, 15001, 15542, 15699, 15238, 15825, 15715, 15931, 15962, 15728, 15738, 15738, 15810, 15738, 15810, 15294, 15738, 15810, 15294, 15528, 15738, 15810, 15294, 15528, 16051, 15738, 15810, 15294, 15528, 16051, 15359, 15738, 15810, 15294, 15528, 16051, 15359, 15168, 15738, 15810, 15294, 15528, 16051, 15359, 15168, 15120, 15738, 15810, 15294, 15528, 16051, 15359, 15168, 15120, 15465, 15738, 15810, 15294, 15528, 16051, 15359, 15168, 15120, 15465, 15694, 15345, 15345, 15393, 15345, 15393, 15872, 15345, 15393, 15872, 15598, 15345, 15393, 15872, 15598, 15648, 15345, 15393, 15872, 15598, 15648, 15185, 15345, 15393, 15872, 15598, 15648, 15185, 15508, 15345, 15393, 15872, 15598, 15648, 15185, 15508, 15651, 15345, 15393, 15872, 15598, 15648, 15185, 15508, 15651, 15444, 15345, 15393, 15872, 15598, 15648, 15185, 15508, 15651, 15444, 15433, 15590, 15590, 15268, 15590, 15268, 15159, 15590, 15268, 15159, 15879, 15590, 15268, 15159, 15879, 15826, 15590, 15268, 15159, 15879, 15826, 15455, 15590, 15268, 15159, 15879, 15826, 15455, 15209, 15590, 15268, 15159, 15879, 15826, 15455, 15209, 15558, 15590, 15268, 15159, 15879, 15826, 15455, 15209, 15558, 15247, 15590, 15268, 15159, 15879, 15826, 15455, 15209, 15558, 15247, 15627, 15476, 15476, 15485, 15476, 15485, 15548, 15476, 15485, 15548, 15258, 15476, 15485, 15548, 15258, 15807, 15476, 15485, 15548, 15258, 15807, 15228, 15476, 15485, 15548, 15258, 15807, 15228, 15459, 15476, 15485, 15548, 15258, 15807, 15228, 15459, 15244, 15476, 15485, 15548, 15258, 15807, 15228, 15459, 15244, 15442, 15476, 15485, 15548, 15258, 15807, 15228, 15459, 15244, 15442, 15503, 15592, 15592, 15433, 15592, 15433, 15512, 15592, 15433, 15512, 15603, 15592, 15433, 15512, 15603, 15420, 15592, 15433, 15512, 15603, 15420, 15365, 15592, 15433, 15512, 15603, 15420, 15365, 15591, 15592, 15433, 15512, 15603, 15420, 15365, 15591, 15271, 15592, 15433, 15512, 15603, 15420, 15365, 15591, 15271, 15501, 15592, 15433, 15512, 15603, 15420, 15365, 15591, 15271, 15501, 15890, 15499, 15499, 15522, 15499, 15522, 15413, 15499, 15522, 15413, 15739, 15499, 15522, 15413, 15739, 15265, 15499, 15522, 15413, 15739, 15265, 15943, 15499, 15522, 15413, 15739, 15265, 15943, 15687, 15499, 15522, 15413, 15739, 15265, 15943, 15687, 15634, 15499, 15522, 15413, 15739, 15265, 15943, 15687, 15634, 15488, 15499, 15522, 15413, 15739, 15265, 15943, 15687, 15634, 15488, 15280, 15711, 15711, 15105, 15711, 15105, 15629, 15711, 15105, 15629, 15514, 15711, 15105, 15629, 15514, 15399, 15711, 15105, 15629, 15514, 15399, 15757, 15711, 15105, 15629, 15514, 15399, 15757, 15492, 15711, 15105, 15629, 15514, 15399, 15757, 15492, 15370, 15711, 15105, 15629, 15514, 15399, 15757, 15492, 15370, 15211, 15711, 15105, 15629, 15514, 15399, 15757, 15492, 15370, 15211, 15505, 15791, 15791, 15597, 15791, 15597, 15439, 15791, 15597, 15439, 15437, 15791, 15597, 15439, 15437, 15350, 15791, 15597, 15439, 15437, 15350, 15734, 15791, 15597, 15439, 15437, 15350, 15734, 15668, 15791, 15597, 15439, 15437, 15350, 15734, 15668, 15241, 15791, 15597, 15439, 15437, 15350, 15734, 15668, 15241, 15271, };static struct simple_udp_connection unicast_connection;
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