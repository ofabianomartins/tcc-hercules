#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5582, 5582, 4955, 5582, 4955, 5111, 5582, 4955, 5111, 5730, 5582, 4955, 5111, 5730, 4475, 5582, 4955, 5111, 5730, 4475, 3504, 5582, 4955, 5111, 5730, 4475, 3504, 5420, 5582, 4955, 5111, 5730, 4475, 3504, 5420, 4910, 5582, 4955, 5111, 5730, 4475, 3504, 5420, 4910, 5160, 5582, 4955, 5111, 5730, 4475, 3504, 5420, 4910, 5160, 5376, 5582, 4955, 5111, 5730, 4475, 3504, 5420, 4910, 5160, 5376, 5134, 5765, 5765, 4836, 5765, 4836, 5211, 5765, 4836, 5211, 5502, 5765, 4836, 5211, 5502, 5257, 5765, 4836, 5211, 5502, 5257, 4667, 5765, 4836, 5211, 5502, 5257, 4667, 5314, 5765, 4836, 5211, 5502, 5257, 4667, 5314, 4300, 5765, 4836, 5211, 5502, 5257, 4667, 5314, 4300, 4500, 5765, 4836, 5211, 5502, 5257, 4667, 5314, 4300, 4500, 5510, 5089, 5089, 4729, 5089, 4729, 5447, 5089, 4729, 5447, 5084, 5089, 4729, 5447, 5084, 5027, 5089, 4729, 5447, 5084, 5027, 4865, 5089, 4729, 5447, 5084, 5027, 4865, 4417, 5089, 4729, 5447, 5084, 5027, 4865, 4417, 5191, 5089, 4729, 5447, 5084, 5027, 4865, 4417, 5191, 5558, 5089, 4729, 5447, 5084, 5027, 4865, 4417, 5191, 5558, 5037, 4887, 4887, 5381, 4887, 5381, 5276, 4887, 5381, 5276, 5174, 4887, 5381, 5276, 5174, 4475, 4887, 5381, 5276, 5174, 4475, 4614, 4887, 5381, 5276, 5174, 4475, 4614, 5320, 4887, 5381, 5276, 5174, 4475, 4614, 5320, 4258, 4887, 5381, 5276, 5174, 4475, 4614, 5320, 4258, 5460, 4887, 5381, 5276, 5174, 4475, 4614, 5320, 4258, 5460, 4047, 5216, 5216, 4595, 5216, 4595, 5292, 5216, 4595, 5292, 4350, 5216, 4595, 5292, 4350, 4586, 5216, 4595, 5292, 4350, 4586, 4524, 5216, 4595, 5292, 4350, 4586, 4524, 4924, 5216, 4595, 5292, 4350, 4586, 4524, 4924, 5439, 5216, 4595, 5292, 4350, 4586, 4524, 4924, 5439, 4131, 5216, 4595, 5292, 4350, 4586, 4524, 4924, 5439, 4131, 5004, 5091, 5091, 5266, 5091, 5266, 4790, 5091, 5266, 4790, 5355, 5091, 5266, 4790, 5355, 5765, 5091, 5266, 4790, 5355, 5765, 4671, 5091, 5266, 4790, 5355, 5765, 4671, 5109, 5091, 5266, 4790, 5355, 5765, 4671, 5109, 5283, 5091, 5266, 4790, 5355, 5765, 4671, 5109, 5283, 5903, 5091, 5266, 4790, 5355, 5765, 4671, 5109, 5283, 5903, 4813, 4807, 4807, 4882, 4807, 4882, 5519, 4807, 4882, 5519, 5176, 4807, 4882, 5519, 5176, 4556, 4807, 4882, 5519, 5176, 4556, 4801, 4807, 4882, 5519, 5176, 4556, 4801, 5370, 4807, 4882, 5519, 5176, 4556, 4801, 5370, 4943, 4807, 4882, 5519, 5176, 4556, 4801, 5370, 4943, 5476, 4807, 4882, 5519, 5176, 4556, 4801, 5370, 4943, 5476, 4875, 5194, 5194, 5537, 5194, 5537, 4794, 5194, 5537, 4794, 4717, 5194, 5537, 4794, 4717, 4372, 5194, 5537, 4794, 4717, 4372, 4631, 5194, 5537, 4794, 4717, 4372, 4631, 5044, 5194, 5537, 4794, 4717, 4372, 4631, 5044, 4584, 5194, 5537, 4794, 4717, 4372, 4631, 5044, 4584, 3961, 5194, 5537, 4794, 4717, 4372, 4631, 5044, 4584, 3961, 5318, 4413, 4413, 4773, 4413, 4773, 4874, 4413, 4773, 4874, 4361, 4413, 4773, 4874, 4361, 4501, 4413, 4773, 4874, 4361, 4501, 4594, 4413, 4773, 4874, 4361, 4501, 4594, 5129, 4413, 4773, 4874, 4361, 4501, 4594, 5129, 5179, 4413, 4773, 4874, 4361, 4501, 4594, 5129, 5179, 5027, 4413, 4773, 4874, 4361, 4501, 4594, 5129, 5179, 5027, 5289, 4362, 4362, 5341, 4362, 5341, 3776, 4362, 5341, 3776, 4965, 4362, 5341, 3776, 4965, 5118, 4362, 5341, 3776, 4965, 5118, 5045, 4362, 5341, 3776, 4965, 5118, 5045, 5410, 4362, 5341, 3776, 4965, 5118, 5045, 5410, 4416, 4362, 5341, 3776, 4965, 5118, 5045, 5410, 4416, 4541, };static struct simple_udp_connection unicast_connection;
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