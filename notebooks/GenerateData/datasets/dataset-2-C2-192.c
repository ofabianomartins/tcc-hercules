#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {7972, 7972, 11555, 7972, 11555, 10481, 7972, 11555, 10481, 8972, 7972, 11555, 10481, 8972, 9050, 7972, 11555, 10481, 8972, 9050, 7977, 7972, 11555, 10481, 8972, 9050, 7977, 7564, 7972, 11555, 10481, 8972, 9050, 7977, 7564, 8248, 7972, 11555, 10481, 8972, 9050, 7977, 7564, 8248, 7704, 7972, 11555, 10481, 8972, 9050, 7977, 7564, 8248, 7704, 9870, 7972, 11555, 10481, 8972, 9050, 7977, 7564, 8248, 7704, 9870, 8695, 7401, 7401, 7360, 7401, 7360, 10490, 7401, 7360, 10490, 7767, 7401, 7360, 10490, 7767, 11146, 7401, 7360, 10490, 7767, 11146, 7071, 7401, 7360, 10490, 7767, 11146, 7071, 9321, 7401, 7360, 10490, 7767, 11146, 7071, 9321, 10406, 7401, 7360, 10490, 7767, 11146, 7071, 9321, 10406, 7571, 7401, 7360, 10490, 7767, 11146, 7071, 9321, 10406, 7571, 6183, 11566, 11566, 10319, 11566, 10319, 10193, 11566, 10319, 10193, 6296, 11566, 10319, 10193, 6296, 8609, 11566, 10319, 10193, 6296, 8609, 4909, 11566, 10319, 10193, 6296, 8609, 4909, 10291, 11566, 10319, 10193, 6296, 8609, 4909, 10291, 9319, 11566, 10319, 10193, 6296, 8609, 4909, 10291, 9319, 12123, 11566, 10319, 10193, 6296, 8609, 4909, 10291, 9319, 12123, 8854, 11119, 11119, 10658, 11119, 10658, 10410, 11119, 10658, 10410, 11106, 11119, 10658, 10410, 11106, 7567, 11119, 10658, 10410, 11106, 7567, 10482, 11119, 10658, 10410, 11106, 7567, 10482, 7673, 11119, 10658, 10410, 11106, 7567, 10482, 7673, 7368, 11119, 10658, 10410, 11106, 7567, 10482, 7673, 7368, 12770, 11119, 10658, 10410, 11106, 7567, 10482, 7673, 7368, 12770, 11296, 9094, 9094, 7074, 9094, 7074, 7271, 9094, 7074, 7271, 4600, 9094, 7074, 7271, 4600, 8956, 9094, 7074, 7271, 4600, 8956, 9311, 9094, 7074, 7271, 4600, 8956, 9311, 9971, 9094, 7074, 7271, 4600, 8956, 9311, 9971, 11380, 9094, 7074, 7271, 4600, 8956, 9311, 9971, 11380, 9189, 9094, 7074, 7271, 4600, 8956, 9311, 9971, 11380, 9189, 12287, 9376, 9376, 5989, 9376, 5989, 10294, 9376, 5989, 10294, 6344, 9376, 5989, 10294, 6344, 11715, 9376, 5989, 10294, 6344, 11715, 10525, 9376, 5989, 10294, 6344, 11715, 10525, 9319, 9376, 5989, 10294, 6344, 11715, 10525, 9319, 11930, 9376, 5989, 10294, 6344, 11715, 10525, 9319, 11930, 7135, 9376, 5989, 10294, 6344, 11715, 10525, 9319, 11930, 7135, 11673, 13161, 13161, 11043, 13161, 11043, 9633, 13161, 11043, 9633, 10939, 13161, 11043, 9633, 10939, 12976, 13161, 11043, 9633, 10939, 12976, 8081, 13161, 11043, 9633, 10939, 12976, 8081, 10132, 13161, 11043, 9633, 10939, 12976, 8081, 10132, 6073, 13161, 11043, 9633, 10939, 12976, 8081, 10132, 6073, 10272, 13161, 11043, 9633, 10939, 12976, 8081, 10132, 6073, 10272, 7711, 6303, 6303, 11479, 6303, 11479, 9102, 6303, 11479, 9102, 8767, 6303, 11479, 9102, 8767, 11278, 6303, 11479, 9102, 8767, 11278, 8195, 6303, 11479, 9102, 8767, 11278, 8195, 9346, 6303, 11479, 9102, 8767, 11278, 8195, 9346, 9456, 6303, 11479, 9102, 8767, 11278, 8195, 9346, 9456, 8173, 6303, 11479, 9102, 8767, 11278, 8195, 9346, 9456, 8173, 8273, 9759, 9759, 6071, 9759, 6071, 10339, 9759, 6071, 10339, 11541, 9759, 6071, 10339, 11541, 9060, 9759, 6071, 10339, 11541, 9060, 8326, 9759, 6071, 10339, 11541, 9060, 8326, 8620, 9759, 6071, 10339, 11541, 9060, 8326, 8620, 8219, 9759, 6071, 10339, 11541, 9060, 8326, 8620, 8219, 9254, 9759, 6071, 10339, 11541, 9060, 8326, 8620, 8219, 9254, 7735, 8054, 8054, 8970, 8054, 8970, 7998, 8054, 8970, 7998, 11338, 8054, 8970, 7998, 11338, 7899, 8054, 8970, 7998, 11338, 7899, 6400, 8054, 8970, 7998, 11338, 7899, 6400, 12033, 8054, 8970, 7998, 11338, 7899, 6400, 12033, 10172, 8054, 8970, 7998, 11338, 7899, 6400, 12033, 10172, 8161, };static struct simple_udp_connection unicast_connection;
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