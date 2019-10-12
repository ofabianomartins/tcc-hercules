#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5304, 5304, 4125, 5304, 4125, 5165, 5304, 4125, 5165, 5299, 5304, 4125, 5165, 5299, 4850, 5304, 4125, 5165, 5299, 4850, 5100, 5304, 4125, 5165, 5299, 4850, 5100, 5341, 5304, 4125, 5165, 5299, 4850, 5100, 5341, 5683, 5304, 4125, 5165, 5299, 4850, 5100, 5341, 5683, 5102, 5304, 4125, 5165, 5299, 4850, 5100, 5341, 5683, 5102, 5726, 5304, 4125, 5165, 5299, 4850, 5100, 5341, 5683, 5102, 5726, 4337, 5462, 5462, 5124, 5462, 5124, 4677, 5462, 5124, 4677, 4483, 5462, 5124, 4677, 4483, 5651, 5462, 5124, 4677, 4483, 5651, 4119, 5462, 5124, 4677, 4483, 5651, 4119, 4917, 5462, 5124, 4677, 4483, 5651, 4119, 4917, 5688, 5462, 5124, 4677, 4483, 5651, 4119, 4917, 5688, 4592, 5462, 5124, 4677, 4483, 5651, 4119, 4917, 5688, 4592, 5274, 4588, 4588, 5064, 4588, 5064, 5361, 4588, 5064, 5361, 4508, 4588, 5064, 5361, 4508, 4231, 4588, 5064, 5361, 4508, 4231, 5526, 4588, 5064, 5361, 4508, 4231, 5526, 5399, 4588, 5064, 5361, 4508, 4231, 5526, 5399, 5079, 4588, 5064, 5361, 4508, 4231, 5526, 5399, 5079, 5428, 4588, 5064, 5361, 4508, 4231, 5526, 5399, 5079, 5428, 4804, 5505, 5505, 6086, 5505, 6086, 5452, 5505, 6086, 5452, 4827, 5505, 6086, 5452, 4827, 5323, 5505, 6086, 5452, 4827, 5323, 4455, 5505, 6086, 5452, 4827, 5323, 4455, 4797, 5505, 6086, 5452, 4827, 5323, 4455, 4797, 4974, 5505, 6086, 5452, 4827, 5323, 4455, 4797, 4974, 5320, 5505, 6086, 5452, 4827, 5323, 4455, 4797, 4974, 5320, 4706, 5578, 5578, 5069, 5578, 5069, 6067, 5578, 5069, 6067, 4637, 5578, 5069, 6067, 4637, 5090, 5578, 5069, 6067, 4637, 5090, 5479, 5578, 5069, 6067, 4637, 5090, 5479, 4862, 5578, 5069, 6067, 4637, 5090, 5479, 4862, 4728, 5578, 5069, 6067, 4637, 5090, 5479, 4862, 4728, 4672, 5578, 5069, 6067, 4637, 5090, 5479, 4862, 4728, 4672, 5817, 5115, 5115, 4917, 5115, 4917, 4827, 5115, 4917, 4827, 4302, 5115, 4917, 4827, 4302, 4604, 5115, 4917, 4827, 4302, 4604, 6038, 5115, 4917, 4827, 4302, 4604, 6038, 5226, 5115, 4917, 4827, 4302, 4604, 6038, 5226, 5075, 5115, 4917, 4827, 4302, 4604, 6038, 5226, 5075, 5587, 5115, 4917, 4827, 4302, 4604, 6038, 5226, 5075, 5587, 4865, 5032, 5032, 4567, 5032, 4567, 5625, 5032, 4567, 5625, 5387, 5032, 4567, 5625, 5387, 5287, 5032, 4567, 5625, 5387, 5287, 5529, 5032, 4567, 5625, 5387, 5287, 5529, 4865, 5032, 4567, 5625, 5387, 5287, 5529, 4865, 4993, 5032, 4567, 5625, 5387, 5287, 5529, 4865, 4993, 5368, 5032, 4567, 5625, 5387, 5287, 5529, 4865, 4993, 5368, 5111, 5937, 5937, 4845, 5937, 4845, 4433, 5937, 4845, 4433, 5439, 5937, 4845, 4433, 5439, 4587, 5937, 4845, 4433, 5439, 4587, 4719, 5937, 4845, 4433, 5439, 4587, 4719, 4856, 5937, 4845, 4433, 5439, 4587, 4719, 4856, 5359, 5937, 4845, 4433, 5439, 4587, 4719, 4856, 5359, 4871, 5937, 4845, 4433, 5439, 4587, 4719, 4856, 5359, 4871, 4700, 5484, 5484, 5288, 5484, 5288, 4470, 5484, 5288, 4470, 4292, 5484, 5288, 4470, 4292, 4613, 5484, 5288, 4470, 4292, 4613, 4270, 5484, 5288, 4470, 4292, 4613, 4270, 5091, 5484, 5288, 4470, 4292, 4613, 4270, 5091, 5198, 5484, 5288, 4470, 4292, 4613, 4270, 5091, 5198, 4947, 5484, 5288, 4470, 4292, 4613, 4270, 5091, 5198, 4947, 5860, 4985, 4985, 5043, 4985, 5043, 4985, 4985, 5043, 4985, 4208, 4985, 5043, 4985, 4208, 5200, 4985, 5043, 4985, 4208, 5200, 4660, 4985, 5043, 4985, 4208, 5200, 4660, 4648, 4985, 5043, 4985, 4208, 5200, 4660, 4648, 5217, 4985, 5043, 4985, 4208, 5200, 4660, 4648, 5217, 5266, };static struct simple_udp_connection unicast_connection;
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