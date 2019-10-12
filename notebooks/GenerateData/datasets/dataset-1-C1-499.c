#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5053, 5053, 5166, 5053, 5166, 5518, 5053, 5166, 5518, 4790, 5053, 5166, 5518, 4790, 4591, 5053, 5166, 5518, 4790, 4591, 5130, 5053, 5166, 5518, 4790, 4591, 5130, 5511, 5053, 5166, 5518, 4790, 4591, 5130, 5511, 5328, 5053, 5166, 5518, 4790, 4591, 5130, 5511, 5328, 5013, 5053, 5166, 5518, 4790, 4591, 5130, 5511, 5328, 5013, 4507, 5053, 5166, 5518, 4790, 4591, 5130, 5511, 5328, 5013, 4507, 4193, 5015, 5015, 4410, 5015, 4410, 4382, 5015, 4410, 4382, 5627, 5015, 4410, 4382, 5627, 4998, 5015, 4410, 4382, 5627, 4998, 5125, 5015, 4410, 4382, 5627, 4998, 5125, 5318, 5015, 4410, 4382, 5627, 4998, 5125, 5318, 4934, 5015, 4410, 4382, 5627, 4998, 5125, 5318, 4934, 5309, 5015, 4410, 4382, 5627, 4998, 5125, 5318, 4934, 5309, 5315, 5545, 5545, 4609, 5545, 4609, 4949, 5545, 4609, 4949, 5423, 5545, 4609, 4949, 5423, 5049, 5545, 4609, 4949, 5423, 5049, 5184, 5545, 4609, 4949, 5423, 5049, 5184, 5063, 5545, 4609, 4949, 5423, 5049, 5184, 5063, 4235, 5545, 4609, 4949, 5423, 5049, 5184, 5063, 4235, 4812, 5545, 4609, 4949, 5423, 5049, 5184, 5063, 4235, 4812, 5040, 5298, 5298, 4864, 5298, 4864, 4636, 5298, 4864, 4636, 4464, 5298, 4864, 4636, 4464, 5677, 5298, 4864, 4636, 4464, 5677, 5339, 5298, 4864, 4636, 4464, 5677, 5339, 5444, 5298, 4864, 4636, 4464, 5677, 5339, 5444, 5179, 5298, 4864, 4636, 4464, 5677, 5339, 5444, 5179, 5007, 5298, 4864, 4636, 4464, 5677, 5339, 5444, 5179, 5007, 5545, 4705, 4705, 5491, 4705, 5491, 4975, 4705, 5491, 4975, 4762, 4705, 5491, 4975, 4762, 4525, 4705, 5491, 4975, 4762, 4525, 5351, 4705, 5491, 4975, 4762, 4525, 5351, 5254, 4705, 5491, 4975, 4762, 4525, 5351, 5254, 4958, 4705, 5491, 4975, 4762, 4525, 5351, 5254, 4958, 4632, 4705, 5491, 4975, 4762, 4525, 5351, 5254, 4958, 4632, 4931, 5269, 5269, 5037, 5269, 5037, 5530, 5269, 5037, 5530, 4319, 5269, 5037, 5530, 4319, 4977, 5269, 5037, 5530, 4319, 4977, 5422, 5269, 5037, 5530, 4319, 4977, 5422, 4687, 5269, 5037, 5530, 4319, 4977, 5422, 4687, 4412, 5269, 5037, 5530, 4319, 4977, 5422, 4687, 4412, 5372, 5269, 5037, 5530, 4319, 4977, 5422, 4687, 4412, 5372, 5467, 4874, 4874, 5131, 4874, 5131, 5327, 4874, 5131, 5327, 5533, 4874, 5131, 5327, 5533, 4673, 4874, 5131, 5327, 5533, 4673, 4357, 4874, 5131, 5327, 5533, 4673, 4357, 5357, 4874, 5131, 5327, 5533, 4673, 4357, 5357, 4890, 4874, 5131, 5327, 5533, 4673, 4357, 5357, 4890, 5179, 4874, 5131, 5327, 5533, 4673, 4357, 5357, 4890, 5179, 4925, 4412, 4412, 5256, 4412, 5256, 4731, 4412, 5256, 4731, 3877, 4412, 5256, 4731, 3877, 4931, 4412, 5256, 4731, 3877, 4931, 4513, 4412, 5256, 4731, 3877, 4931, 4513, 4798, 4412, 5256, 4731, 3877, 4931, 4513, 4798, 5283, 4412, 5256, 4731, 3877, 4931, 4513, 4798, 5283, 5086, 4412, 5256, 4731, 3877, 4931, 4513, 4798, 5283, 5086, 4755, 4954, 4954, 5462, 4954, 5462, 5486, 4954, 5462, 5486, 5160, 4954, 5462, 5486, 5160, 4944, 4954, 5462, 5486, 5160, 4944, 5397, 4954, 5462, 5486, 5160, 4944, 5397, 5015, 4954, 5462, 5486, 5160, 4944, 5397, 5015, 5276, 4954, 5462, 5486, 5160, 4944, 5397, 5015, 5276, 4751, 4954, 5462, 5486, 5160, 4944, 5397, 5015, 5276, 4751, 4651, 4744, 4744, 5132, 4744, 5132, 4582, 4744, 5132, 4582, 4934, 4744, 5132, 4582, 4934, 5310, 4744, 5132, 4582, 4934, 5310, 4129, 4744, 5132, 4582, 4934, 5310, 4129, 4941, 4744, 5132, 4582, 4934, 5310, 4129, 4941, 4873, 4744, 5132, 4582, 4934, 5310, 4129, 4941, 4873, 4963, };static struct simple_udp_connection unicast_connection;
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