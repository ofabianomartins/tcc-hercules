#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15508, 15508, 15653, 15508, 15653, 15787, 15508, 15653, 15787, 15211, 15508, 15653, 15787, 15211, 15466, 15508, 15653, 15787, 15211, 15466, 15418, 15508, 15653, 15787, 15211, 15466, 15418, 15113, 15508, 15653, 15787, 15211, 15466, 15418, 15113, 15492, 15508, 15653, 15787, 15211, 15466, 15418, 15113, 15492, 15535, 15508, 15653, 15787, 15211, 15466, 15418, 15113, 15492, 15535, 15574, 15508, 15653, 15787, 15211, 15466, 15418, 15113, 15492, 15535, 15574, 15554, 15351, 15351, 15290, 15351, 15290, 15173, 15351, 15290, 15173, 15244, 15351, 15290, 15173, 15244, 15404, 15351, 15290, 15173, 15244, 15404, 15230, 15351, 15290, 15173, 15244, 15404, 15230, 15725, 15351, 15290, 15173, 15244, 15404, 15230, 15725, 15215, 15351, 15290, 15173, 15244, 15404, 15230, 15725, 15215, 15120, 15351, 15290, 15173, 15244, 15404, 15230, 15725, 15215, 15120, 15497, 15304, 15304, 15270, 15304, 15270, 15903, 15304, 15270, 15903, 15704, 15304, 15270, 15903, 15704, 15953, 15304, 15270, 15903, 15704, 15953, 15547, 15304, 15270, 15903, 15704, 15953, 15547, 15472, 15304, 15270, 15903, 15704, 15953, 15547, 15472, 15631, 15304, 15270, 15903, 15704, 15953, 15547, 15472, 15631, 15308, 15304, 15270, 15903, 15704, 15953, 15547, 15472, 15631, 15308, 15685, 15764, 15764, 15436, 15764, 15436, 15486, 15764, 15436, 15486, 15574, 15764, 15436, 15486, 15574, 15176, 15764, 15436, 15486, 15574, 15176, 15416, 15764, 15436, 15486, 15574, 15176, 15416, 15476, 15764, 15436, 15486, 15574, 15176, 15416, 15476, 15758, 15764, 15436, 15486, 15574, 15176, 15416, 15476, 15758, 15533, 15764, 15436, 15486, 15574, 15176, 15416, 15476, 15758, 15533, 15563, 15426, 15426, 15043, 15426, 15043, 15579, 15426, 15043, 15579, 15388, 15426, 15043, 15579, 15388, 15867, 15426, 15043, 15579, 15388, 15867, 15419, 15426, 15043, 15579, 15388, 15867, 15419, 15614, 15426, 15043, 15579, 15388, 15867, 15419, 15614, 15332, 15426, 15043, 15579, 15388, 15867, 15419, 15614, 15332, 15487, 15426, 15043, 15579, 15388, 15867, 15419, 15614, 15332, 15487, 15518, 15578, 15578, 15384, 15578, 15384, 15626, 15578, 15384, 15626, 15458, 15578, 15384, 15626, 15458, 15631, 15578, 15384, 15626, 15458, 15631, 15602, 15578, 15384, 15626, 15458, 15631, 15602, 15415, 15578, 15384, 15626, 15458, 15631, 15602, 15415, 15325, 15578, 15384, 15626, 15458, 15631, 15602, 15415, 15325, 15562, 15578, 15384, 15626, 15458, 15631, 15602, 15415, 15325, 15562, 15469, 15818, 15818, 15352, 15818, 15352, 15796, 15818, 15352, 15796, 15609, 15818, 15352, 15796, 15609, 15682, 15818, 15352, 15796, 15609, 15682, 15460, 15818, 15352, 15796, 15609, 15682, 15460, 15286, 15818, 15352, 15796, 15609, 15682, 15460, 15286, 16100, 15818, 15352, 15796, 15609, 15682, 15460, 15286, 16100, 15482, 15818, 15352, 15796, 15609, 15682, 15460, 15286, 16100, 15482, 15604, 15115, 15115, 15223, 15115, 15223, 15541, 15115, 15223, 15541, 15461, 15115, 15223, 15541, 15461, 15520, 15115, 15223, 15541, 15461, 15520, 15507, 15115, 15223, 15541, 15461, 15520, 15507, 15538, 15115, 15223, 15541, 15461, 15520, 15507, 15538, 15673, 15115, 15223, 15541, 15461, 15520, 15507, 15538, 15673, 15331, 15115, 15223, 15541, 15461, 15520, 15507, 15538, 15673, 15331, 15425, 15620, 15620, 15760, 15620, 15760, 15333, 15620, 15760, 15333, 15857, 15620, 15760, 15333, 15857, 15603, 15620, 15760, 15333, 15857, 15603, 15858, 15620, 15760, 15333, 15857, 15603, 15858, 15566, 15620, 15760, 15333, 15857, 15603, 15858, 15566, 15399, 15620, 15760, 15333, 15857, 15603, 15858, 15566, 15399, 15437, 15620, 15760, 15333, 15857, 15603, 15858, 15566, 15399, 15437, 15439, 15571, 15571, 15298, 15571, 15298, 14901, 15571, 15298, 14901, 15435, 15571, 15298, 14901, 15435, 15711, 15571, 15298, 14901, 15435, 15711, 15737, 15571, 15298, 14901, 15435, 15711, 15737, 14953, 15571, 15298, 14901, 15435, 15711, 15737, 14953, 15625, 15571, 15298, 14901, 15435, 15711, 15737, 14953, 15625, 15709, };static struct simple_udp_connection unicast_connection;
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