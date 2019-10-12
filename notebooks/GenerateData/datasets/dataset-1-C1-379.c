#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {4816, 4816, 5165, 4816, 5165, 5562, 4816, 5165, 5562, 5729, 4816, 5165, 5562, 5729, 5768, 4816, 5165, 5562, 5729, 5768, 5019, 4816, 5165, 5562, 5729, 5768, 5019, 5528, 4816, 5165, 5562, 5729, 5768, 5019, 5528, 4967, 4816, 5165, 5562, 5729, 5768, 5019, 5528, 4967, 5248, 4816, 5165, 5562, 5729, 5768, 5019, 5528, 4967, 5248, 5196, 4816, 5165, 5562, 5729, 5768, 5019, 5528, 4967, 5248, 5196, 5294, 5966, 5966, 4648, 5966, 4648, 5401, 5966, 4648, 5401, 5301, 5966, 4648, 5401, 5301, 6230, 5966, 4648, 5401, 5301, 6230, 5131, 5966, 4648, 5401, 5301, 6230, 5131, 4890, 5966, 4648, 5401, 5301, 6230, 5131, 4890, 4766, 5966, 4648, 5401, 5301, 6230, 5131, 4890, 4766, 4673, 5966, 4648, 5401, 5301, 6230, 5131, 4890, 4766, 4673, 4576, 5628, 5628, 5522, 5628, 5522, 4848, 5628, 5522, 4848, 5013, 5628, 5522, 4848, 5013, 5446, 5628, 5522, 4848, 5013, 5446, 4513, 5628, 5522, 4848, 5013, 5446, 4513, 5724, 5628, 5522, 4848, 5013, 5446, 4513, 5724, 4191, 5628, 5522, 4848, 5013, 5446, 4513, 5724, 4191, 4926, 5628, 5522, 4848, 5013, 5446, 4513, 5724, 4191, 4926, 5053, 4735, 4735, 4788, 4735, 4788, 4614, 4735, 4788, 4614, 5626, 4735, 4788, 4614, 5626, 5098, 4735, 4788, 4614, 5626, 5098, 5596, 4735, 4788, 4614, 5626, 5098, 5596, 4278, 4735, 4788, 4614, 5626, 5098, 5596, 4278, 4782, 4735, 4788, 4614, 5626, 5098, 5596, 4278, 4782, 5157, 4735, 4788, 4614, 5626, 5098, 5596, 4278, 4782, 5157, 4514, 4751, 4751, 5973, 4751, 5973, 4443, 4751, 5973, 4443, 4931, 4751, 5973, 4443, 4931, 4553, 4751, 5973, 4443, 4931, 4553, 5221, 4751, 5973, 4443, 4931, 4553, 5221, 5543, 4751, 5973, 4443, 4931, 4553, 5221, 5543, 5004, 4751, 5973, 4443, 4931, 4553, 5221, 5543, 5004, 5139, 4751, 5973, 4443, 4931, 4553, 5221, 5543, 5004, 5139, 5211, 4821, 4821, 5195, 4821, 5195, 5274, 4821, 5195, 5274, 5096, 4821, 5195, 5274, 5096, 4712, 4821, 5195, 5274, 5096, 4712, 5617, 4821, 5195, 5274, 5096, 4712, 5617, 5081, 4821, 5195, 5274, 5096, 4712, 5617, 5081, 5454, 4821, 5195, 5274, 5096, 4712, 5617, 5081, 5454, 4742, 4821, 5195, 5274, 5096, 4712, 5617, 5081, 5454, 4742, 4791, 5331, 5331, 4863, 5331, 4863, 5390, 5331, 4863, 5390, 4512, 5331, 4863, 5390, 4512, 4730, 5331, 4863, 5390, 4512, 4730, 5341, 5331, 4863, 5390, 4512, 4730, 5341, 5565, 5331, 4863, 5390, 4512, 4730, 5341, 5565, 4111, 5331, 4863, 5390, 4512, 4730, 5341, 5565, 4111, 5227, 5331, 4863, 5390, 4512, 4730, 5341, 5565, 4111, 5227, 4971, 4826, 4826, 5913, 4826, 5913, 5219, 4826, 5913, 5219, 5579, 4826, 5913, 5219, 5579, 4782, 4826, 5913, 5219, 5579, 4782, 4922, 4826, 5913, 5219, 5579, 4782, 4922, 4177, 4826, 5913, 5219, 5579, 4782, 4922, 4177, 4621, 4826, 5913, 5219, 5579, 4782, 4922, 4177, 4621, 5065, 4826, 5913, 5219, 5579, 4782, 4922, 4177, 4621, 5065, 4447, 5285, 5285, 5446, 5285, 5446, 4705, 5285, 5446, 4705, 4387, 5285, 5446, 4705, 4387, 4926, 5285, 5446, 4705, 4387, 4926, 4405, 5285, 5446, 4705, 4387, 4926, 4405, 5267, 5285, 5446, 4705, 4387, 4926, 4405, 5267, 5166, 5285, 5446, 4705, 4387, 4926, 4405, 5267, 5166, 4519, 5285, 5446, 4705, 4387, 4926, 4405, 5267, 5166, 4519, 5191, 5739, 5739, 5289, 5739, 5289, 5665, 5739, 5289, 5665, 4693, 5739, 5289, 5665, 4693, 4481, 5739, 5289, 5665, 4693, 4481, 5961, 5739, 5289, 5665, 4693, 4481, 5961, 4711, 5739, 5289, 5665, 4693, 4481, 5961, 4711, 4754, 5739, 5289, 5665, 4693, 4481, 5961, 4711, 4754, 4994, };static struct simple_udp_connection unicast_connection;
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