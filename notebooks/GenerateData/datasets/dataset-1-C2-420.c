#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8402, 8402, 8118, 8402, 8118, 8394, 8402, 8118, 8394, 8437, 8402, 8118, 8394, 8437, 8566, 8402, 8118, 8394, 8437, 8566, 8687, 8402, 8118, 8394, 8437, 8566, 8687, 8386, 8402, 8118, 8394, 8437, 8566, 8687, 8386, 8461, 8402, 8118, 8394, 8437, 8566, 8687, 8386, 8461, 8762, 8402, 8118, 8394, 8437, 8566, 8687, 8386, 8461, 8762, 8836, 8402, 8118, 8394, 8437, 8566, 8687, 8386, 8461, 8762, 8836, 8346, 8347, 8347, 8408, 8347, 8408, 8068, 8347, 8408, 8068, 8603, 8347, 8408, 8068, 8603, 8575, 8347, 8408, 8068, 8603, 8575, 8360, 8347, 8408, 8068, 8603, 8575, 8360, 8641, 8347, 8408, 8068, 8603, 8575, 8360, 8641, 8430, 8347, 8408, 8068, 8603, 8575, 8360, 8641, 8430, 8809, 8347, 8408, 8068, 8603, 8575, 8360, 8641, 8430, 8809, 8609, 8243, 8243, 8152, 8243, 8152, 8397, 8243, 8152, 8397, 8574, 8243, 8152, 8397, 8574, 8157, 8243, 8152, 8397, 8574, 8157, 8765, 8243, 8152, 8397, 8574, 8157, 8765, 8670, 8243, 8152, 8397, 8574, 8157, 8765, 8670, 8264, 8243, 8152, 8397, 8574, 8157, 8765, 8670, 8264, 8725, 8243, 8152, 8397, 8574, 8157, 8765, 8670, 8264, 8725, 8599, 8766, 8766, 8629, 8766, 8629, 8100, 8766, 8629, 8100, 8568, 8766, 8629, 8100, 8568, 8560, 8766, 8629, 8100, 8568, 8560, 8296, 8766, 8629, 8100, 8568, 8560, 8296, 8573, 8766, 8629, 8100, 8568, 8560, 8296, 8573, 8301, 8766, 8629, 8100, 8568, 8560, 8296, 8573, 8301, 8734, 8766, 8629, 8100, 8568, 8560, 8296, 8573, 8301, 8734, 8957, 8721, 8721, 8386, 8721, 8386, 8641, 8721, 8386, 8641, 8533, 8721, 8386, 8641, 8533, 8353, 8721, 8386, 8641, 8533, 8353, 8394, 8721, 8386, 8641, 8533, 8353, 8394, 8380, 8721, 8386, 8641, 8533, 8353, 8394, 8380, 8369, 8721, 8386, 8641, 8533, 8353, 8394, 8380, 8369, 8503, 8721, 8386, 8641, 8533, 8353, 8394, 8380, 8369, 8503, 8327, 8328, 8328, 8512, 8328, 8512, 8051, 8328, 8512, 8051, 8552, 8328, 8512, 8051, 8552, 8274, 8328, 8512, 8051, 8552, 8274, 8387, 8328, 8512, 8051, 8552, 8274, 8387, 8293, 8328, 8512, 8051, 8552, 8274, 8387, 8293, 8230, 8328, 8512, 8051, 8552, 8274, 8387, 8293, 8230, 8126, 8328, 8512, 8051, 8552, 8274, 8387, 8293, 8230, 8126, 8812, 8150, 8150, 8777, 8150, 8777, 8506, 8150, 8777, 8506, 8389, 8150, 8777, 8506, 8389, 8649, 8150, 8777, 8506, 8389, 8649, 8252, 8150, 8777, 8506, 8389, 8649, 8252, 8641, 8150, 8777, 8506, 8389, 8649, 8252, 8641, 8623, 8150, 8777, 8506, 8389, 8649, 8252, 8641, 8623, 8640, 8150, 8777, 8506, 8389, 8649, 8252, 8641, 8623, 8640, 8474, 8482, 8482, 8385, 8482, 8385, 8860, 8482, 8385, 8860, 8831, 8482, 8385, 8860, 8831, 8620, 8482, 8385, 8860, 8831, 8620, 8230, 8482, 8385, 8860, 8831, 8620, 8230, 8291, 8482, 8385, 8860, 8831, 8620, 8230, 8291, 8473, 8482, 8385, 8860, 8831, 8620, 8230, 8291, 8473, 8359, 8482, 8385, 8860, 8831, 8620, 8230, 8291, 8473, 8359, 8350, 8292, 8292, 8627, 8292, 8627, 8472, 8292, 8627, 8472, 8402, 8292, 8627, 8472, 8402, 8398, 8292, 8627, 8472, 8402, 8398, 8666, 8292, 8627, 8472, 8402, 8398, 8666, 8482, 8292, 8627, 8472, 8402, 8398, 8666, 8482, 8450, 8292, 8627, 8472, 8402, 8398, 8666, 8482, 8450, 8326, 8292, 8627, 8472, 8402, 8398, 8666, 8482, 8450, 8326, 8464, 8351, 8351, 8428, 8351, 8428, 8660, 8351, 8428, 8660, 8600, 8351, 8428, 8660, 8600, 8535, 8351, 8428, 8660, 8600, 8535, 8578, 8351, 8428, 8660, 8600, 8535, 8578, 8269, 8351, 8428, 8660, 8600, 8535, 8578, 8269, 8278, 8351, 8428, 8660, 8600, 8535, 8578, 8269, 8278, 8446, };static struct simple_udp_connection unicast_connection;
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