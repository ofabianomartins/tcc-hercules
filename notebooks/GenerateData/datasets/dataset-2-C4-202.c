#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15352, 15352, 15327, 15352, 15327, 15315, 15352, 15327, 15315, 15547, 15352, 15327, 15315, 15547, 15383, 15352, 15327, 15315, 15547, 15383, 15634, 15352, 15327, 15315, 15547, 15383, 15634, 15504, 15352, 15327, 15315, 15547, 15383, 15634, 15504, 15544, 15352, 15327, 15315, 15547, 15383, 15634, 15504, 15544, 15332, 15352, 15327, 15315, 15547, 15383, 15634, 15504, 15544, 15332, 15475, 15352, 15327, 15315, 15547, 15383, 15634, 15504, 15544, 15332, 15475, 15708, 15465, 15465, 15804, 15465, 15804, 15388, 15465, 15804, 15388, 15584, 15465, 15804, 15388, 15584, 15435, 15465, 15804, 15388, 15584, 15435, 15228, 15465, 15804, 15388, 15584, 15435, 15228, 15546, 15465, 15804, 15388, 15584, 15435, 15228, 15546, 15809, 15465, 15804, 15388, 15584, 15435, 15228, 15546, 15809, 15417, 15465, 15804, 15388, 15584, 15435, 15228, 15546, 15809, 15417, 15141, 15441, 15441, 15433, 15441, 15433, 15661, 15441, 15433, 15661, 15273, 15441, 15433, 15661, 15273, 15422, 15441, 15433, 15661, 15273, 15422, 15485, 15441, 15433, 15661, 15273, 15422, 15485, 15461, 15441, 15433, 15661, 15273, 15422, 15485, 15461, 15513, 15441, 15433, 15661, 15273, 15422, 15485, 15461, 15513, 15311, 15441, 15433, 15661, 15273, 15422, 15485, 15461, 15513, 15311, 15354, 15468, 15468, 15932, 15468, 15932, 15488, 15468, 15932, 15488, 15264, 15468, 15932, 15488, 15264, 15388, 15468, 15932, 15488, 15264, 15388, 15623, 15468, 15932, 15488, 15264, 15388, 15623, 15721, 15468, 15932, 15488, 15264, 15388, 15623, 15721, 15522, 15468, 15932, 15488, 15264, 15388, 15623, 15721, 15522, 15678, 15468, 15932, 15488, 15264, 15388, 15623, 15721, 15522, 15678, 15321, 15247, 15247, 15628, 15247, 15628, 15423, 15247, 15628, 15423, 15450, 15247, 15628, 15423, 15450, 15476, 15247, 15628, 15423, 15450, 15476, 15391, 15247, 15628, 15423, 15450, 15476, 15391, 15319, 15247, 15628, 15423, 15450, 15476, 15391, 15319, 15458, 15247, 15628, 15423, 15450, 15476, 15391, 15319, 15458, 15557, 15247, 15628, 15423, 15450, 15476, 15391, 15319, 15458, 15557, 15477, 15255, 15255, 15557, 15255, 15557, 15587, 15255, 15557, 15587, 15620, 15255, 15557, 15587, 15620, 15625, 15255, 15557, 15587, 15620, 15625, 15912, 15255, 15557, 15587, 15620, 15625, 15912, 15285, 15255, 15557, 15587, 15620, 15625, 15912, 15285, 15442, 15255, 15557, 15587, 15620, 15625, 15912, 15285, 15442, 15724, 15255, 15557, 15587, 15620, 15625, 15912, 15285, 15442, 15724, 15482, 15234, 15234, 15591, 15234, 15591, 15768, 15234, 15591, 15768, 15222, 15234, 15591, 15768, 15222, 15352, 15234, 15591, 15768, 15222, 15352, 15365, 15234, 15591, 15768, 15222, 15352, 15365, 15162, 15234, 15591, 15768, 15222, 15352, 15365, 15162, 15466, 15234, 15591, 15768, 15222, 15352, 15365, 15162, 15466, 15710, 15234, 15591, 15768, 15222, 15352, 15365, 15162, 15466, 15710, 15490, 15739, 15739, 15519, 15739, 15519, 15488, 15739, 15519, 15488, 15612, 15739, 15519, 15488, 15612, 15430, 15739, 15519, 15488, 15612, 15430, 15036, 15739, 15519, 15488, 15612, 15430, 15036, 15689, 15739, 15519, 15488, 15612, 15430, 15036, 15689, 15929, 15739, 15519, 15488, 15612, 15430, 15036, 15689, 15929, 15948, 15739, 15519, 15488, 15612, 15430, 15036, 15689, 15929, 15948, 16185, 15439, 15439, 15658, 15439, 15658, 15306, 15439, 15658, 15306, 15385, 15439, 15658, 15306, 15385, 15369, 15439, 15658, 15306, 15385, 15369, 15694, 15439, 15658, 15306, 15385, 15369, 15694, 15469, 15439, 15658, 15306, 15385, 15369, 15694, 15469, 15252, 15439, 15658, 15306, 15385, 15369, 15694, 15469, 15252, 15350, 15439, 15658, 15306, 15385, 15369, 15694, 15469, 15252, 15350, 15149, 15206, 15206, 15735, 15206, 15735, 15384, 15206, 15735, 15384, 15396, 15206, 15735, 15384, 15396, 15755, 15206, 15735, 15384, 15396, 15755, 15586, 15206, 15735, 15384, 15396, 15755, 15586, 15404, 15206, 15735, 15384, 15396, 15755, 15586, 15404, 15070, 15206, 15735, 15384, 15396, 15755, 15586, 15404, 15070, 15418, };static struct simple_udp_connection unicast_connection;
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