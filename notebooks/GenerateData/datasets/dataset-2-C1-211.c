#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {10458, 10458, 11036, 10458, 11036, 9026, 10458, 11036, 9026, 12453, 10458, 11036, 9026, 12453, 8301, 10458, 11036, 9026, 12453, 8301, 10879, 10458, 11036, 9026, 12453, 8301, 10879, 10511, 10458, 11036, 9026, 12453, 8301, 10879, 10511, 7976, 10458, 11036, 9026, 12453, 8301, 10879, 10511, 7976, 10133, 10458, 11036, 9026, 12453, 8301, 10879, 10511, 7976, 10133, 6891, 10458, 11036, 9026, 12453, 8301, 10879, 10511, 7976, 10133, 6891, 5179, 9445, 9445, 7859, 9445, 7859, 11048, 9445, 7859, 11048, 12277, 9445, 7859, 11048, 12277, 7340, 9445, 7859, 11048, 12277, 7340, 9449, 9445, 7859, 11048, 12277, 7340, 9449, 10479, 9445, 7859, 11048, 12277, 7340, 9449, 10479, 7603, 9445, 7859, 11048, 12277, 7340, 9449, 10479, 7603, 11510, 9445, 7859, 11048, 12277, 7340, 9449, 10479, 7603, 11510, 6277, 10397, 10397, 8608, 10397, 8608, 8626, 10397, 8608, 8626, 8479, 10397, 8608, 8626, 8479, 10300, 10397, 8608, 8626, 8479, 10300, 10638, 10397, 8608, 8626, 8479, 10300, 10638, 8249, 10397, 8608, 8626, 8479, 10300, 10638, 8249, 8060, 10397, 8608, 8626, 8479, 10300, 10638, 8249, 8060, 7403, 10397, 8608, 8626, 8479, 10300, 10638, 8249, 8060, 7403, 7387, 7473, 7473, 10298, 7473, 10298, 11533, 7473, 10298, 11533, 9395, 7473, 10298, 11533, 9395, 7530, 7473, 10298, 11533, 9395, 7530, 10111, 7473, 10298, 11533, 9395, 7530, 10111, 12001, 7473, 10298, 11533, 9395, 7530, 10111, 12001, 12490, 7473, 10298, 11533, 9395, 7530, 10111, 12001, 12490, 12168, 7473, 10298, 11533, 9395, 7530, 10111, 12001, 12490, 12168, 8813, 9949, 9949, 10959, 9949, 10959, 9918, 9949, 10959, 9918, 6844, 9949, 10959, 9918, 6844, 8825, 9949, 10959, 9918, 6844, 8825, 5251, 9949, 10959, 9918, 6844, 8825, 5251, 9768, 9949, 10959, 9918, 6844, 8825, 5251, 9768, 9128, 9949, 10959, 9918, 6844, 8825, 5251, 9768, 9128, 11167, 9949, 10959, 9918, 6844, 8825, 5251, 9768, 9128, 11167, 8516, 8941, 8941, 12417, 8941, 12417, 7571, 8941, 12417, 7571, 9896, 8941, 12417, 7571, 9896, 7703, 8941, 12417, 7571, 9896, 7703, 8195, 8941, 12417, 7571, 9896, 7703, 8195, 11121, 8941, 12417, 7571, 9896, 7703, 8195, 11121, 8935, 8941, 12417, 7571, 9896, 7703, 8195, 11121, 8935, 13011, 8941, 12417, 7571, 9896, 7703, 8195, 11121, 8935, 13011, 10162, 10523, 10523, 7327, 10523, 7327, 11684, 10523, 7327, 11684, 9465, 10523, 7327, 11684, 9465, 8206, 10523, 7327, 11684, 9465, 8206, 10253, 10523, 7327, 11684, 9465, 8206, 10253, 7961, 10523, 7327, 11684, 9465, 8206, 10253, 7961, 9697, 10523, 7327, 11684, 9465, 8206, 10253, 7961, 9697, 8335, 10523, 7327, 11684, 9465, 8206, 10253, 7961, 9697, 8335, 10018, 7761, 7761, 9158, 7761, 9158, 7041, 7761, 9158, 7041, 12138, 7761, 9158, 7041, 12138, 9140, 7761, 9158, 7041, 12138, 9140, 8376, 7761, 9158, 7041, 12138, 9140, 8376, 9341, 7761, 9158, 7041, 12138, 9140, 8376, 9341, 8477, 7761, 9158, 7041, 12138, 9140, 8376, 9341, 8477, 8061, 7761, 9158, 7041, 12138, 9140, 8376, 9341, 8477, 8061, 9701, 10679, 10679, 10048, 10679, 10048, 10103, 10679, 10048, 10103, 6457, 10679, 10048, 10103, 6457, 9512, 10679, 10048, 10103, 6457, 9512, 8687, 10679, 10048, 10103, 6457, 9512, 8687, 11771, 10679, 10048, 10103, 6457, 9512, 8687, 11771, 8158, 10679, 10048, 10103, 6457, 9512, 8687, 11771, 8158, 9365, 10679, 10048, 10103, 6457, 9512, 8687, 11771, 8158, 9365, 7887, 6122, 6122, 8828, 6122, 8828, 11056, 6122, 8828, 11056, 8225, 6122, 8828, 11056, 8225, 11688, 6122, 8828, 11056, 8225, 11688, 10436, 6122, 8828, 11056, 8225, 11688, 10436, 9389, 6122, 8828, 11056, 8225, 11688, 10436, 9389, 8457, 6122, 8828, 11056, 8225, 11688, 10436, 9389, 8457, 9281, };static struct simple_udp_connection unicast_connection;
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