#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {4570, 4570, 5180, 4570, 5180, 5069, 4570, 5180, 5069, 4296, 4570, 5180, 5069, 4296, 5128, 4570, 5180, 5069, 4296, 5128, 4810, 4570, 5180, 5069, 4296, 5128, 4810, 4724, 4570, 5180, 5069, 4296, 5128, 4810, 4724, 5164, 4570, 5180, 5069, 4296, 5128, 4810, 4724, 5164, 4966, 4570, 5180, 5069, 4296, 5128, 4810, 4724, 5164, 4966, 4504, 4570, 5180, 5069, 4296, 5128, 4810, 4724, 5164, 4966, 4504, 5032, 5327, 5327, 5072, 5327, 5072, 4877, 5327, 5072, 4877, 4998, 5327, 5072, 4877, 4998, 5027, 5327, 5072, 4877, 4998, 5027, 5420, 5327, 5072, 4877, 4998, 5027, 5420, 5724, 5327, 5072, 4877, 4998, 5027, 5420, 5724, 4724, 5327, 5072, 4877, 4998, 5027, 5420, 5724, 4724, 5132, 5327, 5072, 4877, 4998, 5027, 5420, 5724, 4724, 5132, 4956, 4845, 4845, 5441, 4845, 5441, 4658, 4845, 5441, 4658, 4458, 4845, 5441, 4658, 4458, 4893, 4845, 5441, 4658, 4458, 4893, 5637, 4845, 5441, 4658, 4458, 4893, 5637, 5161, 4845, 5441, 4658, 4458, 4893, 5637, 5161, 4976, 4845, 5441, 4658, 4458, 4893, 5637, 5161, 4976, 4198, 4845, 5441, 4658, 4458, 4893, 5637, 5161, 4976, 4198, 5289, 4560, 4560, 4449, 4560, 4449, 4975, 4560, 4449, 4975, 5283, 4560, 4449, 4975, 5283, 4683, 4560, 4449, 4975, 5283, 4683, 4388, 4560, 4449, 4975, 5283, 4683, 4388, 4801, 4560, 4449, 4975, 5283, 4683, 4388, 4801, 4917, 4560, 4449, 4975, 5283, 4683, 4388, 4801, 4917, 4824, 4560, 4449, 4975, 5283, 4683, 4388, 4801, 4917, 4824, 5207, 5244, 5244, 5214, 5244, 5214, 4391, 5244, 5214, 4391, 4682, 5244, 5214, 4391, 4682, 4515, 5244, 5214, 4391, 4682, 4515, 4872, 5244, 5214, 4391, 4682, 4515, 4872, 4775, 5244, 5214, 4391, 4682, 4515, 4872, 4775, 4505, 5244, 5214, 4391, 4682, 4515, 4872, 4775, 4505, 5312, 5244, 5214, 4391, 4682, 4515, 4872, 4775, 4505, 5312, 5238, 5505, 5505, 4900, 5505, 4900, 5109, 5505, 4900, 5109, 4762, 5505, 4900, 5109, 4762, 4617, 5505, 4900, 5109, 4762, 4617, 4572, 5505, 4900, 5109, 4762, 4617, 4572, 4955, 5505, 4900, 5109, 4762, 4617, 4572, 4955, 4902, 5505, 4900, 5109, 4762, 4617, 4572, 4955, 4902, 5238, 5505, 4900, 5109, 4762, 4617, 4572, 4955, 4902, 5238, 4570, 5124, 5124, 4659, 5124, 4659, 5260, 5124, 4659, 5260, 5224, 5124, 4659, 5260, 5224, 5199, 5124, 4659, 5260, 5224, 5199, 5212, 5124, 4659, 5260, 5224, 5199, 5212, 5464, 5124, 4659, 5260, 5224, 5199, 5212, 5464, 4910, 5124, 4659, 5260, 5224, 5199, 5212, 5464, 4910, 4964, 5124, 4659, 5260, 5224, 5199, 5212, 5464, 4910, 4964, 5299, 5048, 5048, 5047, 5048, 5047, 4877, 5048, 5047, 4877, 4647, 5048, 5047, 4877, 4647, 4769, 5048, 5047, 4877, 4647, 4769, 4845, 5048, 5047, 4877, 4647, 4769, 4845, 4756, 5048, 5047, 4877, 4647, 4769, 4845, 4756, 4602, 5048, 5047, 4877, 4647, 4769, 4845, 4756, 4602, 5706, 5048, 5047, 4877, 4647, 4769, 4845, 4756, 4602, 5706, 5225, 4822, 4822, 4982, 4822, 4982, 4669, 4822, 4982, 4669, 4725, 4822, 4982, 4669, 4725, 4763, 4822, 4982, 4669, 4725, 4763, 5775, 4822, 4982, 4669, 4725, 4763, 5775, 4829, 4822, 4982, 4669, 4725, 4763, 5775, 4829, 4995, 4822, 4982, 4669, 4725, 4763, 5775, 4829, 4995, 5624, 4822, 4982, 4669, 4725, 4763, 5775, 4829, 4995, 5624, 4558, 4770, 4770, 4974, 4770, 4974, 4999, 4770, 4974, 4999, 4975, 4770, 4974, 4999, 4975, 5864, 4770, 4974, 4999, 4975, 5864, 4876, 4770, 4974, 4999, 4975, 5864, 4876, 4432, 4770, 4974, 4999, 4975, 5864, 4876, 4432, 4937, 4770, 4974, 4999, 4975, 5864, 4876, 4432, 4937, 5060, };static struct simple_udp_connection unicast_connection;
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