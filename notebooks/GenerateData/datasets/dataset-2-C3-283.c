#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15165, 15165, 15688, 15165, 15688, 15286, 15165, 15688, 15286, 15591, 15165, 15688, 15286, 15591, 15600, 15165, 15688, 15286, 15591, 15600, 15307, 15165, 15688, 15286, 15591, 15600, 15307, 15599, 15165, 15688, 15286, 15591, 15600, 15307, 15599, 15784, 15165, 15688, 15286, 15591, 15600, 15307, 15599, 15784, 15755, 15165, 15688, 15286, 15591, 15600, 15307, 15599, 15784, 15755, 15061, 15165, 15688, 15286, 15591, 15600, 15307, 15599, 15784, 15755, 15061, 15697, 15874, 15874, 15547, 15874, 15547, 15737, 15874, 15547, 15737, 15420, 15874, 15547, 15737, 15420, 15615, 15874, 15547, 15737, 15420, 15615, 15290, 15874, 15547, 15737, 15420, 15615, 15290, 15471, 15874, 15547, 15737, 15420, 15615, 15290, 15471, 15181, 15874, 15547, 15737, 15420, 15615, 15290, 15471, 15181, 14898, 15874, 15547, 15737, 15420, 15615, 15290, 15471, 15181, 14898, 15630, 15558, 15558, 15665, 15558, 15665, 15381, 15558, 15665, 15381, 15728, 15558, 15665, 15381, 15728, 15519, 15558, 15665, 15381, 15728, 15519, 15726, 15558, 15665, 15381, 15728, 15519, 15726, 15361, 15558, 15665, 15381, 15728, 15519, 15726, 15361, 15516, 15558, 15665, 15381, 15728, 15519, 15726, 15361, 15516, 15680, 15558, 15665, 15381, 15728, 15519, 15726, 15361, 15516, 15680, 15557, 15273, 15273, 15380, 15273, 15380, 15496, 15273, 15380, 15496, 15509, 15273, 15380, 15496, 15509, 15278, 15273, 15380, 15496, 15509, 15278, 15616, 15273, 15380, 15496, 15509, 15278, 15616, 15243, 15273, 15380, 15496, 15509, 15278, 15616, 15243, 15581, 15273, 15380, 15496, 15509, 15278, 15616, 15243, 15581, 15639, 15273, 15380, 15496, 15509, 15278, 15616, 15243, 15581, 15639, 15637, 15436, 15436, 15272, 15436, 15272, 15135, 15436, 15272, 15135, 15494, 15436, 15272, 15135, 15494, 15612, 15436, 15272, 15135, 15494, 15612, 15607, 15436, 15272, 15135, 15494, 15612, 15607, 15180, 15436, 15272, 15135, 15494, 15612, 15607, 15180, 15488, 15436, 15272, 15135, 15494, 15612, 15607, 15180, 15488, 14993, 15436, 15272, 15135, 15494, 15612, 15607, 15180, 15488, 14993, 15873, 15259, 15259, 15669, 15259, 15669, 15298, 15259, 15669, 15298, 15608, 15259, 15669, 15298, 15608, 15403, 15259, 15669, 15298, 15608, 15403, 15317, 15259, 15669, 15298, 15608, 15403, 15317, 15417, 15259, 15669, 15298, 15608, 15403, 15317, 15417, 15695, 15259, 15669, 15298, 15608, 15403, 15317, 15417, 15695, 15369, 15259, 15669, 15298, 15608, 15403, 15317, 15417, 15695, 15369, 15528, 15404, 15404, 15497, 15404, 15497, 15530, 15404, 15497, 15530, 15378, 15404, 15497, 15530, 15378, 15641, 15404, 15497, 15530, 15378, 15641, 15505, 15404, 15497, 15530, 15378, 15641, 15505, 15372, 15404, 15497, 15530, 15378, 15641, 15505, 15372, 14960, 15404, 15497, 15530, 15378, 15641, 15505, 15372, 14960, 15278, 15404, 15497, 15530, 15378, 15641, 15505, 15372, 14960, 15278, 15381, 15504, 15504, 15433, 15504, 15433, 14972, 15504, 15433, 14972, 15500, 15504, 15433, 14972, 15500, 15469, 15504, 15433, 14972, 15500, 15469, 15267, 15504, 15433, 14972, 15500, 15469, 15267, 15360, 15504, 15433, 14972, 15500, 15469, 15267, 15360, 15641, 15504, 15433, 14972, 15500, 15469, 15267, 15360, 15641, 15678, 15504, 15433, 14972, 15500, 15469, 15267, 15360, 15641, 15678, 15210, 15373, 15373, 15733, 15373, 15733, 15324, 15373, 15733, 15324, 15655, 15373, 15733, 15324, 15655, 15916, 15373, 15733, 15324, 15655, 15916, 15419, 15373, 15733, 15324, 15655, 15916, 15419, 15309, 15373, 15733, 15324, 15655, 15916, 15419, 15309, 15387, 15373, 15733, 15324, 15655, 15916, 15419, 15309, 15387, 15981, 15373, 15733, 15324, 15655, 15916, 15419, 15309, 15387, 15981, 15771, 15498, 15498, 15008, 15498, 15008, 15553, 15498, 15008, 15553, 15336, 15498, 15008, 15553, 15336, 15687, 15498, 15008, 15553, 15336, 15687, 15835, 15498, 15008, 15553, 15336, 15687, 15835, 15447, 15498, 15008, 15553, 15336, 15687, 15835, 15447, 15592, 15498, 15008, 15553, 15336, 15687, 15835, 15447, 15592, 15575, };static struct simple_udp_connection unicast_connection;
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