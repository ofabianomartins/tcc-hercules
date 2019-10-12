#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15675, 15675, 15672, 15675, 15672, 15510, 15675, 15672, 15510, 15292, 15675, 15672, 15510, 15292, 15788, 15675, 15672, 15510, 15292, 15788, 15499, 15675, 15672, 15510, 15292, 15788, 15499, 15632, 15675, 15672, 15510, 15292, 15788, 15499, 15632, 15850, 15675, 15672, 15510, 15292, 15788, 15499, 15632, 15850, 15865, 15675, 15672, 15510, 15292, 15788, 15499, 15632, 15850, 15865, 15754, 15675, 15672, 15510, 15292, 15788, 15499, 15632, 15850, 15865, 15754, 14910, 15597, 15597, 15876, 15597, 15876, 15241, 15597, 15876, 15241, 15643, 15597, 15876, 15241, 15643, 15494, 15597, 15876, 15241, 15643, 15494, 15624, 15597, 15876, 15241, 15643, 15494, 15624, 15269, 15597, 15876, 15241, 15643, 15494, 15624, 15269, 15183, 15597, 15876, 15241, 15643, 15494, 15624, 15269, 15183, 15655, 15597, 15876, 15241, 15643, 15494, 15624, 15269, 15183, 15655, 15218, 15551, 15551, 15813, 15551, 15813, 15340, 15551, 15813, 15340, 15815, 15551, 15813, 15340, 15815, 15345, 15551, 15813, 15340, 15815, 15345, 15933, 15551, 15813, 15340, 15815, 15345, 15933, 15363, 15551, 15813, 15340, 15815, 15345, 15933, 15363, 15611, 15551, 15813, 15340, 15815, 15345, 15933, 15363, 15611, 15794, 15551, 15813, 15340, 15815, 15345, 15933, 15363, 15611, 15794, 15301, 15491, 15491, 15549, 15491, 15549, 15616, 15491, 15549, 15616, 15705, 15491, 15549, 15616, 15705, 15489, 15491, 15549, 15616, 15705, 15489, 15547, 15491, 15549, 15616, 15705, 15489, 15547, 15667, 15491, 15549, 15616, 15705, 15489, 15547, 15667, 15667, 15491, 15549, 15616, 15705, 15489, 15547, 15667, 15667, 15682, 15491, 15549, 15616, 15705, 15489, 15547, 15667, 15667, 15682, 15132, 15564, 15564, 16007, 15564, 16007, 15615, 15564, 16007, 15615, 15517, 15564, 16007, 15615, 15517, 15551, 15564, 16007, 15615, 15517, 15551, 15384, 15564, 16007, 15615, 15517, 15551, 15384, 15193, 15564, 16007, 15615, 15517, 15551, 15384, 15193, 15649, 15564, 16007, 15615, 15517, 15551, 15384, 15193, 15649, 15592, 15564, 16007, 15615, 15517, 15551, 15384, 15193, 15649, 15592, 15687, 15540, 15540, 15639, 15540, 15639, 15710, 15540, 15639, 15710, 15423, 15540, 15639, 15710, 15423, 15547, 15540, 15639, 15710, 15423, 15547, 15670, 15540, 15639, 15710, 15423, 15547, 15670, 15840, 15540, 15639, 15710, 15423, 15547, 15670, 15840, 15782, 15540, 15639, 15710, 15423, 15547, 15670, 15840, 15782, 15623, 15540, 15639, 15710, 15423, 15547, 15670, 15840, 15782, 15623, 15462, 15532, 15532, 15747, 15532, 15747, 15476, 15532, 15747, 15476, 15524, 15532, 15747, 15476, 15524, 15425, 15532, 15747, 15476, 15524, 15425, 15472, 15532, 15747, 15476, 15524, 15425, 15472, 15627, 15532, 15747, 15476, 15524, 15425, 15472, 15627, 15576, 15532, 15747, 15476, 15524, 15425, 15472, 15627, 15576, 15896, 15532, 15747, 15476, 15524, 15425, 15472, 15627, 15576, 15896, 15556, 15508, 15508, 15475, 15508, 15475, 15130, 15508, 15475, 15130, 15708, 15508, 15475, 15130, 15708, 15832, 15508, 15475, 15130, 15708, 15832, 15093, 15508, 15475, 15130, 15708, 15832, 15093, 15500, 15508, 15475, 15130, 15708, 15832, 15093, 15500, 15611, 15508, 15475, 15130, 15708, 15832, 15093, 15500, 15611, 15493, 15508, 15475, 15130, 15708, 15832, 15093, 15500, 15611, 15493, 15610, 15020, 15020, 15607, 15020, 15607, 15680, 15020, 15607, 15680, 15464, 15020, 15607, 15680, 15464, 15425, 15020, 15607, 15680, 15464, 15425, 15335, 15020, 15607, 15680, 15464, 15425, 15335, 15497, 15020, 15607, 15680, 15464, 15425, 15335, 15497, 15503, 15020, 15607, 15680, 15464, 15425, 15335, 15497, 15503, 15385, 15020, 15607, 15680, 15464, 15425, 15335, 15497, 15503, 15385, 15511, 15561, 15561, 15667, 15561, 15667, 15417, 15561, 15667, 15417, 15527, 15561, 15667, 15417, 15527, 15672, 15561, 15667, 15417, 15527, 15672, 15508, 15561, 15667, 15417, 15527, 15672, 15508, 15494, 15561, 15667, 15417, 15527, 15672, 15508, 15494, 15888, 15561, 15667, 15417, 15527, 15672, 15508, 15494, 15888, 15640, };static struct simple_udp_connection unicast_connection;
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