#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15091, 15091, 15613, 15091, 15613, 15156, 15091, 15613, 15156, 15491, 15091, 15613, 15156, 15491, 15342, 15091, 15613, 15156, 15491, 15342, 15433, 15091, 15613, 15156, 15491, 15342, 15433, 15161, 15091, 15613, 15156, 15491, 15342, 15433, 15161, 14852, 15091, 15613, 15156, 15491, 15342, 15433, 15161, 14852, 15509, 15091, 15613, 15156, 15491, 15342, 15433, 15161, 14852, 15509, 15253, 15091, 15613, 15156, 15491, 15342, 15433, 15161, 14852, 15509, 15253, 15508, 15317, 15317, 15613, 15317, 15613, 15241, 15317, 15613, 15241, 15658, 15317, 15613, 15241, 15658, 15157, 15317, 15613, 15241, 15658, 15157, 15803, 15317, 15613, 15241, 15658, 15157, 15803, 15843, 15317, 15613, 15241, 15658, 15157, 15803, 15843, 15563, 15317, 15613, 15241, 15658, 15157, 15803, 15843, 15563, 15685, 15317, 15613, 15241, 15658, 15157, 15803, 15843, 15563, 15685, 15545, 15094, 15094, 15454, 15094, 15454, 15276, 15094, 15454, 15276, 15476, 15094, 15454, 15276, 15476, 15318, 15094, 15454, 15276, 15476, 15318, 15568, 15094, 15454, 15276, 15476, 15318, 15568, 15627, 15094, 15454, 15276, 15476, 15318, 15568, 15627, 15771, 15094, 15454, 15276, 15476, 15318, 15568, 15627, 15771, 15358, 15094, 15454, 15276, 15476, 15318, 15568, 15627, 15771, 15358, 15463, 15314, 15314, 15858, 15314, 15858, 15531, 15314, 15858, 15531, 15728, 15314, 15858, 15531, 15728, 15800, 15314, 15858, 15531, 15728, 15800, 15402, 15314, 15858, 15531, 15728, 15800, 15402, 15322, 15314, 15858, 15531, 15728, 15800, 15402, 15322, 15825, 15314, 15858, 15531, 15728, 15800, 15402, 15322, 15825, 15542, 15314, 15858, 15531, 15728, 15800, 15402, 15322, 15825, 15542, 15561, 15278, 15278, 15005, 15278, 15005, 15509, 15278, 15005, 15509, 15559, 15278, 15005, 15509, 15559, 15640, 15278, 15005, 15509, 15559, 15640, 15675, 15278, 15005, 15509, 15559, 15640, 15675, 15779, 15278, 15005, 15509, 15559, 15640, 15675, 15779, 15206, 15278, 15005, 15509, 15559, 15640, 15675, 15779, 15206, 15594, 15278, 15005, 15509, 15559, 15640, 15675, 15779, 15206, 15594, 15386, 15337, 15337, 15490, 15337, 15490, 15690, 15337, 15490, 15690, 15323, 15337, 15490, 15690, 15323, 15731, 15337, 15490, 15690, 15323, 15731, 15693, 15337, 15490, 15690, 15323, 15731, 15693, 15606, 15337, 15490, 15690, 15323, 15731, 15693, 15606, 15656, 15337, 15490, 15690, 15323, 15731, 15693, 15606, 15656, 15539, 15337, 15490, 15690, 15323, 15731, 15693, 15606, 15656, 15539, 15116, 15430, 15430, 15477, 15430, 15477, 15152, 15430, 15477, 15152, 15315, 15430, 15477, 15152, 15315, 15823, 15430, 15477, 15152, 15315, 15823, 15590, 15430, 15477, 15152, 15315, 15823, 15590, 15297, 15430, 15477, 15152, 15315, 15823, 15590, 15297, 14996, 15430, 15477, 15152, 15315, 15823, 15590, 15297, 14996, 15436, 15430, 15477, 15152, 15315, 15823, 15590, 15297, 14996, 15436, 15394, 15493, 15493, 15429, 15493, 15429, 15400, 15493, 15429, 15400, 15512, 15493, 15429, 15400, 15512, 15311, 15493, 15429, 15400, 15512, 15311, 15712, 15493, 15429, 15400, 15512, 15311, 15712, 15662, 15493, 15429, 15400, 15512, 15311, 15712, 15662, 15645, 15493, 15429, 15400, 15512, 15311, 15712, 15662, 15645, 15393, 15493, 15429, 15400, 15512, 15311, 15712, 15662, 15645, 15393, 15139, 15636, 15636, 15937, 15636, 15937, 15629, 15636, 15937, 15629, 15313, 15636, 15937, 15629, 15313, 15218, 15636, 15937, 15629, 15313, 15218, 15489, 15636, 15937, 15629, 15313, 15218, 15489, 15616, 15636, 15937, 15629, 15313, 15218, 15489, 15616, 15606, 15636, 15937, 15629, 15313, 15218, 15489, 15616, 15606, 15228, 15636, 15937, 15629, 15313, 15218, 15489, 15616, 15606, 15228, 15576, 15421, 15421, 15838, 15421, 15838, 15493, 15421, 15838, 15493, 15543, 15421, 15838, 15493, 15543, 15329, 15421, 15838, 15493, 15543, 15329, 15388, 15421, 15838, 15493, 15543, 15329, 15388, 15384, 15421, 15838, 15493, 15543, 15329, 15388, 15384, 15255, 15421, 15838, 15493, 15543, 15329, 15388, 15384, 15255, 15831, };static struct simple_udp_connection unicast_connection;
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