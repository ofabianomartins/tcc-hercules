#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5245, 5245, 6228, 5245, 6228, 8743, 5245, 6228, 8743, 12270, 5245, 6228, 8743, 12270, 10005, 5245, 6228, 8743, 12270, 10005, 7034, 5245, 6228, 8743, 12270, 10005, 7034, 8119, 5245, 6228, 8743, 12270, 10005, 7034, 8119, 6100, 5245, 6228, 8743, 12270, 10005, 7034, 8119, 6100, 8269, 5245, 6228, 8743, 12270, 10005, 7034, 8119, 6100, 8269, 10179, 5245, 6228, 8743, 12270, 10005, 7034, 8119, 6100, 8269, 10179, 12986, 10330, 10330, 8663, 10330, 8663, 8635, 10330, 8663, 8635, 7853, 10330, 8663, 8635, 7853, 8252, 10330, 8663, 8635, 7853, 8252, 6510, 10330, 8663, 8635, 7853, 8252, 6510, 8832, 10330, 8663, 8635, 7853, 8252, 6510, 8832, 6016, 10330, 8663, 8635, 7853, 8252, 6510, 8832, 6016, 9982, 10330, 8663, 8635, 7853, 8252, 6510, 8832, 6016, 9982, 7798, 8187, 8187, 7525, 8187, 7525, 10094, 8187, 7525, 10094, 9076, 8187, 7525, 10094, 9076, 13142, 8187, 7525, 10094, 9076, 13142, 10911, 8187, 7525, 10094, 9076, 13142, 10911, 7357, 8187, 7525, 10094, 9076, 13142, 10911, 7357, 8549, 8187, 7525, 10094, 9076, 13142, 10911, 7357, 8549, 10654, 8187, 7525, 10094, 9076, 13142, 10911, 7357, 8549, 10654, 10811, 8920, 8920, 9785, 8920, 9785, 7255, 8920, 9785, 7255, 10278, 8920, 9785, 7255, 10278, 9410, 8920, 9785, 7255, 10278, 9410, 4658, 8920, 9785, 7255, 10278, 9410, 4658, 10576, 8920, 9785, 7255, 10278, 9410, 4658, 10576, 5945, 8920, 9785, 7255, 10278, 9410, 4658, 10576, 5945, 11178, 8920, 9785, 7255, 10278, 9410, 4658, 10576, 5945, 11178, 8060, 10438, 10438, 7409, 10438, 7409, 8573, 10438, 7409, 8573, 10089, 10438, 7409, 8573, 10089, 10173, 10438, 7409, 8573, 10089, 10173, 10415, 10438, 7409, 8573, 10089, 10173, 10415, 10721, 10438, 7409, 8573, 10089, 10173, 10415, 10721, 9572, 10438, 7409, 8573, 10089, 10173, 10415, 10721, 9572, 8984, 10438, 7409, 8573, 10089, 10173, 10415, 10721, 9572, 8984, 8715, 11927, 11927, 9212, 11927, 9212, 8481, 11927, 9212, 8481, 12887, 11927, 9212, 8481, 12887, 7942, 11927, 9212, 8481, 12887, 7942, 7999, 11927, 9212, 8481, 12887, 7942, 7999, 8677, 11927, 9212, 8481, 12887, 7942, 7999, 8677, 4754, 11927, 9212, 8481, 12887, 7942, 7999, 8677, 4754, 7216, 11927, 9212, 8481, 12887, 7942, 7999, 8677, 4754, 7216, 9369, 5076, 5076, 7099, 5076, 7099, 6982, 5076, 7099, 6982, 6853, 5076, 7099, 6982, 6853, 9625, 5076, 7099, 6982, 6853, 9625, 10132, 5076, 7099, 6982, 6853, 9625, 10132, 8294, 5076, 7099, 6982, 6853, 9625, 10132, 8294, 9412, 5076, 7099, 6982, 6853, 9625, 10132, 8294, 9412, 6910, 5076, 7099, 6982, 6853, 9625, 10132, 8294, 9412, 6910, 9458, 5964, 5964, 11056, 5964, 11056, 10349, 5964, 11056, 10349, 9179, 5964, 11056, 10349, 9179, 7305, 5964, 11056, 10349, 9179, 7305, 8213, 5964, 11056, 10349, 9179, 7305, 8213, 9605, 5964, 11056, 10349, 9179, 7305, 8213, 9605, 11515, 5964, 11056, 10349, 9179, 7305, 8213, 9605, 11515, 11697, 5964, 11056, 10349, 9179, 7305, 8213, 9605, 11515, 11697, 8646, 8243, 8243, 9177, 8243, 9177, 11330, 8243, 9177, 11330, 10160, 8243, 9177, 11330, 10160, 9761, 8243, 9177, 11330, 10160, 9761, 8738, 8243, 9177, 11330, 10160, 9761, 8738, 2852, 8243, 9177, 11330, 10160, 9761, 8738, 2852, 7989, 8243, 9177, 11330, 10160, 9761, 8738, 2852, 7989, 11184, 8243, 9177, 11330, 10160, 9761, 8738, 2852, 7989, 11184, 8122, 8241, 8241, 11084, 8241, 11084, 7817, 8241, 11084, 7817, 9042, 8241, 11084, 7817, 9042, 11000, 8241, 11084, 7817, 9042, 11000, 9262, 8241, 11084, 7817, 9042, 11000, 9262, 9445, 8241, 11084, 7817, 9042, 11000, 9262, 9445, 11281, 8241, 11084, 7817, 9042, 11000, 9262, 9445, 11281, 7431, };static struct simple_udp_connection unicast_connection;
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