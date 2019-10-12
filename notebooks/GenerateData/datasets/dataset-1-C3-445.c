#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {4773, 4773, 5074, 4773, 5074, 5312, 4773, 5074, 5312, 5754, 4773, 5074, 5312, 5754, 4916, 4773, 5074, 5312, 5754, 4916, 5951, 4773, 5074, 5312, 5754, 4916, 5951, 5244, 4773, 5074, 5312, 5754, 4916, 5951, 5244, 5346, 4773, 5074, 5312, 5754, 4916, 5951, 5244, 5346, 4730, 4773, 5074, 5312, 5754, 4916, 5951, 5244, 5346, 4730, 4905, 4773, 5074, 5312, 5754, 4916, 5951, 5244, 5346, 4730, 4905, 4501, 4643, 4643, 5208, 4643, 5208, 4664, 4643, 5208, 4664, 4811, 4643, 5208, 4664, 4811, 5082, 4643, 5208, 4664, 4811, 5082, 5070, 4643, 5208, 4664, 4811, 5082, 5070, 4342, 4643, 5208, 4664, 4811, 5082, 5070, 4342, 5510, 4643, 5208, 4664, 4811, 5082, 5070, 4342, 5510, 4883, 4643, 5208, 4664, 4811, 5082, 5070, 4342, 5510, 4883, 4950, 5100, 5100, 5370, 5100, 5370, 5026, 5100, 5370, 5026, 4661, 5100, 5370, 5026, 4661, 4451, 5100, 5370, 5026, 4661, 4451, 5239, 5100, 5370, 5026, 4661, 4451, 5239, 4598, 5100, 5370, 5026, 4661, 4451, 5239, 4598, 4352, 5100, 5370, 5026, 4661, 4451, 5239, 4598, 4352, 5805, 5100, 5370, 5026, 4661, 4451, 5239, 4598, 4352, 5805, 4291, 5424, 5424, 5490, 5424, 5490, 4756, 5424, 5490, 4756, 5246, 5424, 5490, 4756, 5246, 4923, 5424, 5490, 4756, 5246, 4923, 5154, 5424, 5490, 4756, 5246, 4923, 5154, 4978, 5424, 5490, 4756, 5246, 4923, 5154, 4978, 5297, 5424, 5490, 4756, 5246, 4923, 5154, 4978, 5297, 4966, 5424, 5490, 4756, 5246, 4923, 5154, 4978, 5297, 4966, 5188, 5027, 5027, 5410, 5027, 5410, 4825, 5027, 5410, 4825, 5040, 5027, 5410, 4825, 5040, 5608, 5027, 5410, 4825, 5040, 5608, 4566, 5027, 5410, 4825, 5040, 5608, 4566, 4751, 5027, 5410, 4825, 5040, 5608, 4566, 4751, 4323, 5027, 5410, 4825, 5040, 5608, 4566, 4751, 4323, 4546, 5027, 5410, 4825, 5040, 5608, 4566, 4751, 4323, 4546, 4547, 5010, 5010, 5390, 5010, 5390, 5307, 5010, 5390, 5307, 5624, 5010, 5390, 5307, 5624, 5004, 5010, 5390, 5307, 5624, 5004, 6109, 5010, 5390, 5307, 5624, 5004, 6109, 4864, 5010, 5390, 5307, 5624, 5004, 6109, 4864, 4556, 5010, 5390, 5307, 5624, 5004, 6109, 4864, 4556, 4740, 5010, 5390, 5307, 5624, 5004, 6109, 4864, 4556, 4740, 5381, 4545, 4545, 4921, 4545, 4921, 4958, 4545, 4921, 4958, 5373, 4545, 4921, 4958, 5373, 5196, 4545, 4921, 4958, 5373, 5196, 5159, 4545, 4921, 4958, 5373, 5196, 5159, 4614, 4545, 4921, 4958, 5373, 5196, 5159, 4614, 4835, 4545, 4921, 4958, 5373, 5196, 5159, 4614, 4835, 5320, 4545, 4921, 4958, 5373, 5196, 5159, 4614, 4835, 5320, 4425, 5438, 5438, 5107, 5438, 5107, 4775, 5438, 5107, 4775, 5375, 5438, 5107, 4775, 5375, 5163, 5438, 5107, 4775, 5375, 5163, 4831, 5438, 5107, 4775, 5375, 5163, 4831, 4637, 5438, 5107, 4775, 5375, 5163, 4831, 4637, 4178, 5438, 5107, 4775, 5375, 5163, 4831, 4637, 4178, 5036, 5438, 5107, 4775, 5375, 5163, 4831, 4637, 4178, 5036, 4519, 4768, 4768, 4243, 4768, 4243, 4759, 4768, 4243, 4759, 5438, 4768, 4243, 4759, 5438, 4676, 4768, 4243, 4759, 5438, 4676, 5446, 4768, 4243, 4759, 5438, 4676, 5446, 5005, 4768, 4243, 4759, 5438, 4676, 5446, 5005, 4842, 4768, 4243, 4759, 5438, 4676, 5446, 5005, 4842, 4802, 4768, 4243, 4759, 5438, 4676, 5446, 5005, 4842, 4802, 5211, 4986, 4986, 4871, 4986, 4871, 5696, 4986, 4871, 5696, 5495, 4986, 4871, 5696, 5495, 5360, 4986, 4871, 5696, 5495, 5360, 5248, 4986, 4871, 5696, 5495, 5360, 5248, 5063, 4986, 4871, 5696, 5495, 5360, 5248, 5063, 5977, 4986, 4871, 5696, 5495, 5360, 5248, 5063, 5977, 5156, };static struct simple_udp_connection unicast_connection;
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