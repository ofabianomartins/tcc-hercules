#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {11194, 11194, 8619, 11194, 8619, 13319, 11194, 8619, 13319, 5878, 11194, 8619, 13319, 5878, 6438, 11194, 8619, 13319, 5878, 6438, 11161, 11194, 8619, 13319, 5878, 6438, 11161, 11377, 11194, 8619, 13319, 5878, 6438, 11161, 11377, 8673, 11194, 8619, 13319, 5878, 6438, 11161, 11377, 8673, 7466, 11194, 8619, 13319, 5878, 6438, 11161, 11377, 8673, 7466, 9564, 11194, 8619, 13319, 5878, 6438, 11161, 11377, 8673, 7466, 9564, 12122, 5039, 5039, 11065, 5039, 11065, 13413, 5039, 11065, 13413, 8798, 5039, 11065, 13413, 8798, 8023, 5039, 11065, 13413, 8798, 8023, 10666, 5039, 11065, 13413, 8798, 8023, 10666, 8424, 5039, 11065, 13413, 8798, 8023, 10666, 8424, 9177, 5039, 11065, 13413, 8798, 8023, 10666, 8424, 9177, 11543, 5039, 11065, 13413, 8798, 8023, 10666, 8424, 9177, 11543, 9036, 11739, 11739, 9972, 11739, 9972, 8517, 11739, 9972, 8517, 7232, 11739, 9972, 8517, 7232, 7552, 11739, 9972, 8517, 7232, 7552, 8984, 11739, 9972, 8517, 7232, 7552, 8984, 11315, 11739, 9972, 8517, 7232, 7552, 8984, 11315, 10039, 11739, 9972, 8517, 7232, 7552, 8984, 11315, 10039, 9345, 11739, 9972, 8517, 7232, 7552, 8984, 11315, 10039, 9345, 10431, 8487, 8487, 8543, 8487, 8543, 8748, 8487, 8543, 8748, 9010, 8487, 8543, 8748, 9010, 9724, 8487, 8543, 8748, 9010, 9724, 12222, 8487, 8543, 8748, 9010, 9724, 12222, 9927, 8487, 8543, 8748, 9010, 9724, 12222, 9927, 10829, 8487, 8543, 8748, 9010, 9724, 12222, 9927, 10829, 8639, 8487, 8543, 8748, 9010, 9724, 12222, 9927, 10829, 8639, 9416, 10832, 10832, 10546, 10832, 10546, 8337, 10832, 10546, 8337, 8747, 10832, 10546, 8337, 8747, 10283, 10832, 10546, 8337, 8747, 10283, 7122, 10832, 10546, 8337, 8747, 10283, 7122, 9327, 10832, 10546, 8337, 8747, 10283, 7122, 9327, 8882, 10832, 10546, 8337, 8747, 10283, 7122, 9327, 8882, 7848, 10832, 10546, 8337, 8747, 10283, 7122, 9327, 8882, 7848, 6010, 10300, 10300, 8021, 10300, 8021, 8309, 10300, 8021, 8309, 8867, 10300, 8021, 8309, 8867, 8588, 10300, 8021, 8309, 8867, 8588, 11851, 10300, 8021, 8309, 8867, 8588, 11851, 8895, 10300, 8021, 8309, 8867, 8588, 11851, 8895, 8723, 10300, 8021, 8309, 8867, 8588, 11851, 8895, 8723, 6384, 10300, 8021, 8309, 8867, 8588, 11851, 8895, 8723, 6384, 8579, 4610, 4610, 11976, 4610, 11976, 7527, 4610, 11976, 7527, 9187, 4610, 11976, 7527, 9187, 10355, 4610, 11976, 7527, 9187, 10355, 10077, 4610, 11976, 7527, 9187, 10355, 10077, 11531, 4610, 11976, 7527, 9187, 10355, 10077, 11531, 6451, 4610, 11976, 7527, 9187, 10355, 10077, 11531, 6451, 10802, 4610, 11976, 7527, 9187, 10355, 10077, 11531, 6451, 10802, 11585, 6002, 6002, 8829, 6002, 8829, 8740, 6002, 8829, 8740, 7960, 6002, 8829, 8740, 7960, 7833, 6002, 8829, 8740, 7960, 7833, 7351, 6002, 8829, 8740, 7960, 7833, 7351, 5473, 6002, 8829, 8740, 7960, 7833, 7351, 5473, 8785, 6002, 8829, 8740, 7960, 7833, 7351, 5473, 8785, 11020, 6002, 8829, 8740, 7960, 7833, 7351, 5473, 8785, 11020, 11441, 6021, 6021, 9266, 6021, 9266, 8700, 6021, 9266, 8700, 9988, 6021, 9266, 8700, 9988, 9434, 6021, 9266, 8700, 9988, 9434, 9421, 6021, 9266, 8700, 9988, 9434, 9421, 7971, 6021, 9266, 8700, 9988, 9434, 9421, 7971, 7808, 6021, 9266, 8700, 9988, 9434, 9421, 7971, 7808, 11683, 6021, 9266, 8700, 9988, 9434, 9421, 7971, 7808, 11683, 9215, 9516, 9516, 9399, 9516, 9399, 10079, 9516, 9399, 10079, 9893, 9516, 9399, 10079, 9893, 10395, 9516, 9399, 10079, 9893, 10395, 12130, 9516, 9399, 10079, 9893, 10395, 12130, 9395, 9516, 9399, 10079, 9893, 10395, 12130, 9395, 10521, 9516, 9399, 10079, 9893, 10395, 12130, 9395, 10521, 12008, };static struct simple_udp_connection unicast_connection;
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