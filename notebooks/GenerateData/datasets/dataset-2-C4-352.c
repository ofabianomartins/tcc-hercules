#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15534, 15534, 15708, 15534, 15708, 15434, 15534, 15708, 15434, 15456, 15534, 15708, 15434, 15456, 15455, 15534, 15708, 15434, 15456, 15455, 15557, 15534, 15708, 15434, 15456, 15455, 15557, 15564, 15534, 15708, 15434, 15456, 15455, 15557, 15564, 15329, 15534, 15708, 15434, 15456, 15455, 15557, 15564, 15329, 15478, 15534, 15708, 15434, 15456, 15455, 15557, 15564, 15329, 15478, 15398, 15534, 15708, 15434, 15456, 15455, 15557, 15564, 15329, 15478, 15398, 15550, 15830, 15830, 15402, 15830, 15402, 14971, 15830, 15402, 14971, 15716, 15830, 15402, 14971, 15716, 15356, 15830, 15402, 14971, 15716, 15356, 15629, 15830, 15402, 14971, 15716, 15356, 15629, 15435, 15830, 15402, 14971, 15716, 15356, 15629, 15435, 15665, 15830, 15402, 14971, 15716, 15356, 15629, 15435, 15665, 15785, 15830, 15402, 14971, 15716, 15356, 15629, 15435, 15665, 15785, 15452, 15550, 15550, 15650, 15550, 15650, 15374, 15550, 15650, 15374, 15656, 15550, 15650, 15374, 15656, 15581, 15550, 15650, 15374, 15656, 15581, 15806, 15550, 15650, 15374, 15656, 15581, 15806, 15692, 15550, 15650, 15374, 15656, 15581, 15806, 15692, 15559, 15550, 15650, 15374, 15656, 15581, 15806, 15692, 15559, 15539, 15550, 15650, 15374, 15656, 15581, 15806, 15692, 15559, 15539, 15596, 15412, 15412, 15541, 15412, 15541, 15521, 15412, 15541, 15521, 15315, 15412, 15541, 15521, 15315, 15465, 15412, 15541, 15521, 15315, 15465, 15513, 15412, 15541, 15521, 15315, 15465, 15513, 15477, 15412, 15541, 15521, 15315, 15465, 15513, 15477, 15688, 15412, 15541, 15521, 15315, 15465, 15513, 15477, 15688, 15457, 15412, 15541, 15521, 15315, 15465, 15513, 15477, 15688, 15457, 15565, 15271, 15271, 15599, 15271, 15599, 15741, 15271, 15599, 15741, 15712, 15271, 15599, 15741, 15712, 15784, 15271, 15599, 15741, 15712, 15784, 15202, 15271, 15599, 15741, 15712, 15784, 15202, 15634, 15271, 15599, 15741, 15712, 15784, 15202, 15634, 15698, 15271, 15599, 15741, 15712, 15784, 15202, 15634, 15698, 15676, 15271, 15599, 15741, 15712, 15784, 15202, 15634, 15698, 15676, 15581, 15499, 15499, 15489, 15499, 15489, 15860, 15499, 15489, 15860, 15102, 15499, 15489, 15860, 15102, 15489, 15499, 15489, 15860, 15102, 15489, 15339, 15499, 15489, 15860, 15102, 15489, 15339, 15370, 15499, 15489, 15860, 15102, 15489, 15339, 15370, 15530, 15499, 15489, 15860, 15102, 15489, 15339, 15370, 15530, 15499, 15499, 15489, 15860, 15102, 15489, 15339, 15370, 15530, 15499, 15243, 15528, 15528, 15646, 15528, 15646, 15613, 15528, 15646, 15613, 15493, 15528, 15646, 15613, 15493, 15372, 15528, 15646, 15613, 15493, 15372, 15440, 15528, 15646, 15613, 15493, 15372, 15440, 15537, 15528, 15646, 15613, 15493, 15372, 15440, 15537, 15315, 15528, 15646, 15613, 15493, 15372, 15440, 15537, 15315, 15432, 15528, 15646, 15613, 15493, 15372, 15440, 15537, 15315, 15432, 15619, 15691, 15691, 15110, 15691, 15110, 15464, 15691, 15110, 15464, 15598, 15691, 15110, 15464, 15598, 15252, 15691, 15110, 15464, 15598, 15252, 15373, 15691, 15110, 15464, 15598, 15252, 15373, 15539, 15691, 15110, 15464, 15598, 15252, 15373, 15539, 15458, 15691, 15110, 15464, 15598, 15252, 15373, 15539, 15458, 15575, 15691, 15110, 15464, 15598, 15252, 15373, 15539, 15458, 15575, 15480, 15181, 15181, 15552, 15181, 15552, 15314, 15181, 15552, 15314, 15427, 15181, 15552, 15314, 15427, 15592, 15181, 15552, 15314, 15427, 15592, 15423, 15181, 15552, 15314, 15427, 15592, 15423, 15457, 15181, 15552, 15314, 15427, 15592, 15423, 15457, 15446, 15181, 15552, 15314, 15427, 15592, 15423, 15457, 15446, 15385, 15181, 15552, 15314, 15427, 15592, 15423, 15457, 15446, 15385, 15707, 15212, 15212, 15271, 15212, 15271, 15628, 15212, 15271, 15628, 15602, 15212, 15271, 15628, 15602, 15381, 15212, 15271, 15628, 15602, 15381, 15450, 15212, 15271, 15628, 15602, 15381, 15450, 15388, 15212, 15271, 15628, 15602, 15381, 15450, 15388, 15457, 15212, 15271, 15628, 15602, 15381, 15450, 15388, 15457, 15613, };static struct simple_udp_connection unicast_connection;
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