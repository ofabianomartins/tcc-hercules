#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8779, 8779, 8934, 8779, 8934, 8647, 8779, 8934, 8647, 8612, 8779, 8934, 8647, 8612, 8746, 8779, 8934, 8647, 8612, 8746, 8690, 8779, 8934, 8647, 8612, 8746, 8690, 8394, 8779, 8934, 8647, 8612, 8746, 8690, 8394, 8630, 8779, 8934, 8647, 8612, 8746, 8690, 8394, 8630, 8571, 8779, 8934, 8647, 8612, 8746, 8690, 8394, 8630, 8571, 8460, 8779, 8934, 8647, 8612, 8746, 8690, 8394, 8630, 8571, 8460, 8674, 8386, 8386, 8422, 8386, 8422, 8607, 8386, 8422, 8607, 8542, 8386, 8422, 8607, 8542, 8482, 8386, 8422, 8607, 8542, 8482, 8428, 8386, 8422, 8607, 8542, 8482, 8428, 8359, 8386, 8422, 8607, 8542, 8482, 8428, 8359, 8533, 8386, 8422, 8607, 8542, 8482, 8428, 8359, 8533, 8605, 8386, 8422, 8607, 8542, 8482, 8428, 8359, 8533, 8605, 8747, 8568, 8568, 8657, 8568, 8657, 8555, 8568, 8657, 8555, 8429, 8568, 8657, 8555, 8429, 8184, 8568, 8657, 8555, 8429, 8184, 8244, 8568, 8657, 8555, 8429, 8184, 8244, 8649, 8568, 8657, 8555, 8429, 8184, 8244, 8649, 8345, 8568, 8657, 8555, 8429, 8184, 8244, 8649, 8345, 8777, 8568, 8657, 8555, 8429, 8184, 8244, 8649, 8345, 8777, 8557, 8845, 8845, 8755, 8845, 8755, 8390, 8845, 8755, 8390, 8382, 8845, 8755, 8390, 8382, 8312, 8845, 8755, 8390, 8382, 8312, 8238, 8845, 8755, 8390, 8382, 8312, 8238, 8584, 8845, 8755, 8390, 8382, 8312, 8238, 8584, 8243, 8845, 8755, 8390, 8382, 8312, 8238, 8584, 8243, 8778, 8845, 8755, 8390, 8382, 8312, 8238, 8584, 8243, 8778, 8351, 8687, 8687, 8278, 8687, 8278, 8441, 8687, 8278, 8441, 8175, 8687, 8278, 8441, 8175, 8465, 8687, 8278, 8441, 8175, 8465, 8592, 8687, 8278, 8441, 8175, 8465, 8592, 8236, 8687, 8278, 8441, 8175, 8465, 8592, 8236, 8471, 8687, 8278, 8441, 8175, 8465, 8592, 8236, 8471, 8609, 8687, 8278, 8441, 8175, 8465, 8592, 8236, 8471, 8609, 8990, 8244, 8244, 8441, 8244, 8441, 8431, 8244, 8441, 8431, 8498, 8244, 8441, 8431, 8498, 8694, 8244, 8441, 8431, 8498, 8694, 8837, 8244, 8441, 8431, 8498, 8694, 8837, 7762, 8244, 8441, 8431, 8498, 8694, 8837, 7762, 8636, 8244, 8441, 8431, 8498, 8694, 8837, 7762, 8636, 8632, 8244, 8441, 8431, 8498, 8694, 8837, 7762, 8636, 8632, 8406, 8345, 8345, 8443, 8345, 8443, 8708, 8345, 8443, 8708, 8405, 8345, 8443, 8708, 8405, 8156, 8345, 8443, 8708, 8405, 8156, 8760, 8345, 8443, 8708, 8405, 8156, 8760, 8830, 8345, 8443, 8708, 8405, 8156, 8760, 8830, 8494, 8345, 8443, 8708, 8405, 8156, 8760, 8830, 8494, 8621, 8345, 8443, 8708, 8405, 8156, 8760, 8830, 8494, 8621, 8304, 8622, 8622, 8767, 8622, 8767, 8274, 8622, 8767, 8274, 8731, 8622, 8767, 8274, 8731, 8576, 8622, 8767, 8274, 8731, 8576, 8613, 8622, 8767, 8274, 8731, 8576, 8613, 8596, 8622, 8767, 8274, 8731, 8576, 8613, 8596, 8462, 8622, 8767, 8274, 8731, 8576, 8613, 8596, 8462, 8292, 8622, 8767, 8274, 8731, 8576, 8613, 8596, 8462, 8292, 8330, 8419, 8419, 8086, 8419, 8086, 8495, 8419, 8086, 8495, 8183, 8419, 8086, 8495, 8183, 8775, 8419, 8086, 8495, 8183, 8775, 8497, 8419, 8086, 8495, 8183, 8775, 8497, 8688, 8419, 8086, 8495, 8183, 8775, 8497, 8688, 8602, 8419, 8086, 8495, 8183, 8775, 8497, 8688, 8602, 8818, 8419, 8086, 8495, 8183, 8775, 8497, 8688, 8602, 8818, 8192, 8266, 8266, 8848, 8266, 8848, 8389, 8266, 8848, 8389, 8729, 8266, 8848, 8389, 8729, 8800, 8266, 8848, 8389, 8729, 8800, 8332, 8266, 8848, 8389, 8729, 8800, 8332, 8486, 8266, 8848, 8389, 8729, 8800, 8332, 8486, 8632, 8266, 8848, 8389, 8729, 8800, 8332, 8486, 8632, 8449, };static struct simple_udp_connection unicast_connection;
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