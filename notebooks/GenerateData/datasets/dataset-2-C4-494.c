#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15287, 15287, 15690, 15287, 15690, 15517, 15287, 15690, 15517, 15796, 15287, 15690, 15517, 15796, 15621, 15287, 15690, 15517, 15796, 15621, 15536, 15287, 15690, 15517, 15796, 15621, 15536, 15501, 15287, 15690, 15517, 15796, 15621, 15536, 15501, 15600, 15287, 15690, 15517, 15796, 15621, 15536, 15501, 15600, 15399, 15287, 15690, 15517, 15796, 15621, 15536, 15501, 15600, 15399, 15463, 15287, 15690, 15517, 15796, 15621, 15536, 15501, 15600, 15399, 15463, 15532, 15586, 15586, 15425, 15586, 15425, 15309, 15586, 15425, 15309, 15466, 15586, 15425, 15309, 15466, 15502, 15586, 15425, 15309, 15466, 15502, 15408, 15586, 15425, 15309, 15466, 15502, 15408, 15568, 15586, 15425, 15309, 15466, 15502, 15408, 15568, 15479, 15586, 15425, 15309, 15466, 15502, 15408, 15568, 15479, 15648, 15586, 15425, 15309, 15466, 15502, 15408, 15568, 15479, 15648, 15507, 15653, 15653, 15692, 15653, 15692, 16009, 15653, 15692, 16009, 15415, 15653, 15692, 16009, 15415, 15539, 15653, 15692, 16009, 15415, 15539, 15356, 15653, 15692, 16009, 15415, 15539, 15356, 15662, 15653, 15692, 16009, 15415, 15539, 15356, 15662, 15231, 15653, 15692, 16009, 15415, 15539, 15356, 15662, 15231, 15516, 15653, 15692, 16009, 15415, 15539, 15356, 15662, 15231, 15516, 15549, 15520, 15520, 15672, 15520, 15672, 15807, 15520, 15672, 15807, 15650, 15520, 15672, 15807, 15650, 15185, 15520, 15672, 15807, 15650, 15185, 15692, 15520, 15672, 15807, 15650, 15185, 15692, 15492, 15520, 15672, 15807, 15650, 15185, 15692, 15492, 15427, 15520, 15672, 15807, 15650, 15185, 15692, 15492, 15427, 15732, 15520, 15672, 15807, 15650, 15185, 15692, 15492, 15427, 15732, 15565, 15612, 15612, 15565, 15612, 15565, 15511, 15612, 15565, 15511, 15510, 15612, 15565, 15511, 15510, 15611, 15612, 15565, 15511, 15510, 15611, 15756, 15612, 15565, 15511, 15510, 15611, 15756, 15789, 15612, 15565, 15511, 15510, 15611, 15756, 15789, 15555, 15612, 15565, 15511, 15510, 15611, 15756, 15789, 15555, 15466, 15612, 15565, 15511, 15510, 15611, 15756, 15789, 15555, 15466, 15419, 15340, 15340, 15833, 15340, 15833, 15450, 15340, 15833, 15450, 15766, 15340, 15833, 15450, 15766, 15528, 15340, 15833, 15450, 15766, 15528, 15569, 15340, 15833, 15450, 15766, 15528, 15569, 15691, 15340, 15833, 15450, 15766, 15528, 15569, 15691, 15524, 15340, 15833, 15450, 15766, 15528, 15569, 15691, 15524, 15839, 15340, 15833, 15450, 15766, 15528, 15569, 15691, 15524, 15839, 15029, 15614, 15614, 15536, 15614, 15536, 15266, 15614, 15536, 15266, 15392, 15614, 15536, 15266, 15392, 16032, 15614, 15536, 15266, 15392, 16032, 15601, 15614, 15536, 15266, 15392, 16032, 15601, 15297, 15614, 15536, 15266, 15392, 16032, 15601, 15297, 15783, 15614, 15536, 15266, 15392, 16032, 15601, 15297, 15783, 15263, 15614, 15536, 15266, 15392, 16032, 15601, 15297, 15783, 15263, 15515, 15498, 15498, 15632, 15498, 15632, 15321, 15498, 15632, 15321, 15249, 15498, 15632, 15321, 15249, 15470, 15498, 15632, 15321, 15249, 15470, 15445, 15498, 15632, 15321, 15249, 15470, 15445, 15572, 15498, 15632, 15321, 15249, 15470, 15445, 15572, 15370, 15498, 15632, 15321, 15249, 15470, 15445, 15572, 15370, 15661, 15498, 15632, 15321, 15249, 15470, 15445, 15572, 15370, 15661, 15657, 15664, 15664, 15480, 15664, 15480, 15235, 15664, 15480, 15235, 15391, 15664, 15480, 15235, 15391, 15429, 15664, 15480, 15235, 15391, 15429, 15503, 15664, 15480, 15235, 15391, 15429, 15503, 15394, 15664, 15480, 15235, 15391, 15429, 15503, 15394, 15462, 15664, 15480, 15235, 15391, 15429, 15503, 15394, 15462, 15279, 15664, 15480, 15235, 15391, 15429, 15503, 15394, 15462, 15279, 15598, 15507, 15507, 15608, 15507, 15608, 15392, 15507, 15608, 15392, 15285, 15507, 15608, 15392, 15285, 15510, 15507, 15608, 15392, 15285, 15510, 15404, 15507, 15608, 15392, 15285, 15510, 15404, 15813, 15507, 15608, 15392, 15285, 15510, 15404, 15813, 15266, 15507, 15608, 15392, 15285, 15510, 15404, 15813, 15266, 15509, };static struct simple_udp_connection unicast_connection;
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