#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8181, 8181, 8111, 8181, 8111, 8544, 8181, 8111, 8544, 8510, 8181, 8111, 8544, 8510, 8695, 8181, 8111, 8544, 8510, 8695, 8662, 8181, 8111, 8544, 8510, 8695, 8662, 8390, 8181, 8111, 8544, 8510, 8695, 8662, 8390, 8453, 8181, 8111, 8544, 8510, 8695, 8662, 8390, 8453, 8894, 8181, 8111, 8544, 8510, 8695, 8662, 8390, 8453, 8894, 8298, 8181, 8111, 8544, 8510, 8695, 8662, 8390, 8453, 8894, 8298, 8512, 8366, 8366, 8884, 8366, 8884, 8365, 8366, 8884, 8365, 8723, 8366, 8884, 8365, 8723, 8283, 8366, 8884, 8365, 8723, 8283, 8118, 8366, 8884, 8365, 8723, 8283, 8118, 8273, 8366, 8884, 8365, 8723, 8283, 8118, 8273, 8563, 8366, 8884, 8365, 8723, 8283, 8118, 8273, 8563, 8195, 8366, 8884, 8365, 8723, 8283, 8118, 8273, 8563, 8195, 8720, 8839, 8839, 8826, 8839, 8826, 8213, 8839, 8826, 8213, 8478, 8839, 8826, 8213, 8478, 8344, 8839, 8826, 8213, 8478, 8344, 8438, 8839, 8826, 8213, 8478, 8344, 8438, 8224, 8839, 8826, 8213, 8478, 8344, 8438, 8224, 8476, 8839, 8826, 8213, 8478, 8344, 8438, 8224, 8476, 8477, 8839, 8826, 8213, 8478, 8344, 8438, 8224, 8476, 8477, 8446, 8429, 8429, 8356, 8429, 8356, 8570, 8429, 8356, 8570, 8441, 8429, 8356, 8570, 8441, 8464, 8429, 8356, 8570, 8441, 8464, 8173, 8429, 8356, 8570, 8441, 8464, 8173, 8450, 8429, 8356, 8570, 8441, 8464, 8173, 8450, 8323, 8429, 8356, 8570, 8441, 8464, 8173, 8450, 8323, 8629, 8429, 8356, 8570, 8441, 8464, 8173, 8450, 8323, 8629, 8430, 8337, 8337, 8324, 8337, 8324, 8554, 8337, 8324, 8554, 8673, 8337, 8324, 8554, 8673, 8337, 8337, 8324, 8554, 8673, 8337, 8664, 8337, 8324, 8554, 8673, 8337, 8664, 8639, 8337, 8324, 8554, 8673, 8337, 8664, 8639, 8389, 8337, 8324, 8554, 8673, 8337, 8664, 8639, 8389, 8532, 8337, 8324, 8554, 8673, 8337, 8664, 8639, 8389, 8532, 8127, 8431, 8431, 8518, 8431, 8518, 8262, 8431, 8518, 8262, 8532, 8431, 8518, 8262, 8532, 8462, 8431, 8518, 8262, 8532, 8462, 8662, 8431, 8518, 8262, 8532, 8462, 8662, 8588, 8431, 8518, 8262, 8532, 8462, 8662, 8588, 8579, 8431, 8518, 8262, 8532, 8462, 8662, 8588, 8579, 8307, 8431, 8518, 8262, 8532, 8462, 8662, 8588, 8579, 8307, 8999, 8654, 8654, 8397, 8654, 8397, 8395, 8654, 8397, 8395, 8705, 8654, 8397, 8395, 8705, 8332, 8654, 8397, 8395, 8705, 8332, 8411, 8654, 8397, 8395, 8705, 8332, 8411, 8426, 8654, 8397, 8395, 8705, 8332, 8411, 8426, 8650, 8654, 8397, 8395, 8705, 8332, 8411, 8426, 8650, 8880, 8654, 8397, 8395, 8705, 8332, 8411, 8426, 8650, 8880, 8681, 8570, 8570, 8595, 8570, 8595, 9024, 8570, 8595, 9024, 8739, 8570, 8595, 9024, 8739, 8659, 8570, 8595, 9024, 8739, 8659, 8694, 8570, 8595, 9024, 8739, 8659, 8694, 8713, 8570, 8595, 9024, 8739, 8659, 8694, 8713, 8662, 8570, 8595, 9024, 8739, 8659, 8694, 8713, 8662, 8385, 8570, 8595, 9024, 8739, 8659, 8694, 8713, 8662, 8385, 8834, 8412, 8412, 8740, 8412, 8740, 8708, 8412, 8740, 8708, 8150, 8412, 8740, 8708, 8150, 8492, 8412, 8740, 8708, 8150, 8492, 8543, 8412, 8740, 8708, 8150, 8492, 8543, 8387, 8412, 8740, 8708, 8150, 8492, 8543, 8387, 8786, 8412, 8740, 8708, 8150, 8492, 8543, 8387, 8786, 8681, 8412, 8740, 8708, 8150, 8492, 8543, 8387, 8786, 8681, 8619, 8226, 8226, 8052, 8226, 8052, 8410, 8226, 8052, 8410, 8294, 8226, 8052, 8410, 8294, 8399, 8226, 8052, 8410, 8294, 8399, 8310, 8226, 8052, 8410, 8294, 8399, 8310, 8641, 8226, 8052, 8410, 8294, 8399, 8310, 8641, 8339, 8226, 8052, 8410, 8294, 8399, 8310, 8641, 8339, 8566, };static struct simple_udp_connection unicast_connection;
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