#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {4827, 4827, 5175, 4827, 5175, 4999, 4827, 5175, 4999, 5424, 4827, 5175, 4999, 5424, 5081, 4827, 5175, 4999, 5424, 5081, 5266, 4827, 5175, 4999, 5424, 5081, 5266, 4582, 4827, 5175, 4999, 5424, 5081, 5266, 4582, 4486, 4827, 5175, 4999, 5424, 5081, 5266, 4582, 4486, 4983, 4827, 5175, 4999, 5424, 5081, 5266, 4582, 4486, 4983, 4540, 4827, 5175, 4999, 5424, 5081, 5266, 4582, 4486, 4983, 4540, 4949, 5408, 5408, 5411, 5408, 5411, 4973, 5408, 5411, 4973, 4760, 5408, 5411, 4973, 4760, 5096, 5408, 5411, 4973, 4760, 5096, 4117, 5408, 5411, 4973, 4760, 5096, 4117, 4773, 5408, 5411, 4973, 4760, 5096, 4117, 4773, 5616, 5408, 5411, 4973, 4760, 5096, 4117, 4773, 5616, 5272, 5408, 5411, 4973, 4760, 5096, 4117, 4773, 5616, 5272, 5050, 4447, 4447, 4703, 4447, 4703, 4828, 4447, 4703, 4828, 5046, 4447, 4703, 4828, 5046, 5200, 4447, 4703, 4828, 5046, 5200, 4964, 4447, 4703, 4828, 5046, 5200, 4964, 5349, 4447, 4703, 4828, 5046, 5200, 4964, 5349, 4752, 4447, 4703, 4828, 5046, 5200, 4964, 5349, 4752, 4214, 4447, 4703, 4828, 5046, 5200, 4964, 5349, 4752, 4214, 5580, 4122, 4122, 4663, 4122, 4663, 5404, 4122, 4663, 5404, 4753, 4122, 4663, 5404, 4753, 4684, 4122, 4663, 5404, 4753, 4684, 4861, 4122, 4663, 5404, 4753, 4684, 4861, 5689, 4122, 4663, 5404, 4753, 4684, 4861, 5689, 5183, 4122, 4663, 5404, 4753, 4684, 4861, 5689, 5183, 4576, 4122, 4663, 5404, 4753, 4684, 4861, 5689, 5183, 4576, 4649, 4634, 4634, 4181, 4634, 4181, 4927, 4634, 4181, 4927, 4684, 4634, 4181, 4927, 4684, 4900, 4634, 4181, 4927, 4684, 4900, 4843, 4634, 4181, 4927, 4684, 4900, 4843, 4644, 4634, 4181, 4927, 4684, 4900, 4843, 4644, 5321, 4634, 4181, 4927, 4684, 4900, 4843, 4644, 5321, 5736, 4634, 4181, 4927, 4684, 4900, 4843, 4644, 5321, 5736, 4836, 5675, 5675, 4346, 5675, 4346, 4944, 5675, 4346, 4944, 5021, 5675, 4346, 4944, 5021, 4976, 5675, 4346, 4944, 5021, 4976, 4495, 5675, 4346, 4944, 5021, 4976, 4495, 5510, 5675, 4346, 4944, 5021, 4976, 4495, 5510, 5199, 5675, 4346, 4944, 5021, 4976, 4495, 5510, 5199, 5218, 5675, 4346, 4944, 5021, 4976, 4495, 5510, 5199, 5218, 3953, 4600, 4600, 4537, 4600, 4537, 5227, 4600, 4537, 5227, 5257, 4600, 4537, 5227, 5257, 5099, 4600, 4537, 5227, 5257, 5099, 4365, 4600, 4537, 5227, 5257, 5099, 4365, 5572, 4600, 4537, 5227, 5257, 5099, 4365, 5572, 4788, 4600, 4537, 5227, 5257, 5099, 4365, 5572, 4788, 5455, 4600, 4537, 5227, 5257, 5099, 4365, 5572, 4788, 5455, 5128, 5276, 5276, 5311, 5276, 5311, 5088, 5276, 5311, 5088, 4600, 5276, 5311, 5088, 4600, 5134, 5276, 5311, 5088, 4600, 5134, 5053, 5276, 5311, 5088, 4600, 5134, 5053, 5357, 5276, 5311, 5088, 4600, 5134, 5053, 5357, 4482, 5276, 5311, 5088, 4600, 5134, 5053, 5357, 4482, 5026, 5276, 5311, 5088, 4600, 5134, 5053, 5357, 4482, 5026, 4780, 5732, 5732, 4929, 5732, 4929, 4390, 5732, 4929, 4390, 4905, 5732, 4929, 4390, 4905, 5394, 5732, 4929, 4390, 4905, 5394, 5311, 5732, 4929, 4390, 4905, 5394, 5311, 4843, 5732, 4929, 4390, 4905, 5394, 5311, 4843, 4745, 5732, 4929, 4390, 4905, 5394, 5311, 4843, 4745, 4558, 5732, 4929, 4390, 4905, 5394, 5311, 4843, 4745, 4558, 5234, 5249, 5249, 5325, 5249, 5325, 4010, 5249, 5325, 4010, 4976, 5249, 5325, 4010, 4976, 5072, 5249, 5325, 4010, 4976, 5072, 5742, 5249, 5325, 4010, 4976, 5072, 5742, 5161, 5249, 5325, 4010, 4976, 5072, 5742, 5161, 4966, 5249, 5325, 4010, 4976, 5072, 5742, 5161, 4966, 5152, };static struct simple_udp_connection unicast_connection;
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