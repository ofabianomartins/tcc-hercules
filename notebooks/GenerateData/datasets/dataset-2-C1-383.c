#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {6690, 6690, 8689, 6690, 8689, 11352, 6690, 8689, 11352, 7014, 6690, 8689, 11352, 7014, 6367, 6690, 8689, 11352, 7014, 6367, 11888, 6690, 8689, 11352, 7014, 6367, 11888, 9457, 6690, 8689, 11352, 7014, 6367, 11888, 9457, 8165, 6690, 8689, 11352, 7014, 6367, 11888, 9457, 8165, 11514, 6690, 8689, 11352, 7014, 6367, 11888, 9457, 8165, 11514, 10165, 6690, 8689, 11352, 7014, 6367, 11888, 9457, 8165, 11514, 10165, 10496, 13223, 13223, 9951, 13223, 9951, 9998, 13223, 9951, 9998, 7532, 13223, 9951, 9998, 7532, 11966, 13223, 9951, 9998, 7532, 11966, 8218, 13223, 9951, 9998, 7532, 11966, 8218, 10541, 13223, 9951, 9998, 7532, 11966, 8218, 10541, 9122, 13223, 9951, 9998, 7532, 11966, 8218, 10541, 9122, 9471, 13223, 9951, 9998, 7532, 11966, 8218, 10541, 9122, 9471, 7622, 9484, 9484, 11485, 9484, 11485, 7683, 9484, 11485, 7683, 10031, 9484, 11485, 7683, 10031, 6778, 9484, 11485, 7683, 10031, 6778, 10336, 9484, 11485, 7683, 10031, 6778, 10336, 9737, 9484, 11485, 7683, 10031, 6778, 10336, 9737, 11228, 9484, 11485, 7683, 10031, 6778, 10336, 9737, 11228, 8506, 9484, 11485, 7683, 10031, 6778, 10336, 9737, 11228, 8506, 10338, 10088, 10088, 9478, 10088, 9478, 10828, 10088, 9478, 10828, 13363, 10088, 9478, 10828, 13363, 9515, 10088, 9478, 10828, 13363, 9515, 6471, 10088, 9478, 10828, 13363, 9515, 6471, 13309, 10088, 9478, 10828, 13363, 9515, 6471, 13309, 8172, 10088, 9478, 10828, 13363, 9515, 6471, 13309, 8172, 11097, 10088, 9478, 10828, 13363, 9515, 6471, 13309, 8172, 11097, 11408, 8948, 8948, 12629, 8948, 12629, 10491, 8948, 12629, 10491, 10140, 8948, 12629, 10491, 10140, 6316, 8948, 12629, 10491, 10140, 6316, 8795, 8948, 12629, 10491, 10140, 6316, 8795, 10483, 8948, 12629, 10491, 10140, 6316, 8795, 10483, 7877, 8948, 12629, 10491, 10140, 6316, 8795, 10483, 7877, 6064, 8948, 12629, 10491, 10140, 6316, 8795, 10483, 7877, 6064, 13084, 6771, 6771, 8309, 6771, 8309, 9747, 6771, 8309, 9747, 10227, 6771, 8309, 9747, 10227, 9374, 6771, 8309, 9747, 10227, 9374, 10613, 6771, 8309, 9747, 10227, 9374, 10613, 11528, 6771, 8309, 9747, 10227, 9374, 10613, 11528, 10959, 6771, 8309, 9747, 10227, 9374, 10613, 11528, 10959, 11676, 6771, 8309, 9747, 10227, 9374, 10613, 11528, 10959, 11676, 9930, 9411, 9411, 4644, 9411, 4644, 7135, 9411, 4644, 7135, 6804, 9411, 4644, 7135, 6804, 9736, 9411, 4644, 7135, 6804, 9736, 10980, 9411, 4644, 7135, 6804, 9736, 10980, 8133, 9411, 4644, 7135, 6804, 9736, 10980, 8133, 11769, 9411, 4644, 7135, 6804, 9736, 10980, 8133, 11769, 5992, 9411, 4644, 7135, 6804, 9736, 10980, 8133, 11769, 5992, 12480, 12066, 12066, 9708, 12066, 9708, 8414, 12066, 9708, 8414, 8673, 12066, 9708, 8414, 8673, 9105, 12066, 9708, 8414, 8673, 9105, 9546, 12066, 9708, 8414, 8673, 9105, 9546, 7860, 12066, 9708, 8414, 8673, 9105, 9546, 7860, 8804, 12066, 9708, 8414, 8673, 9105, 9546, 7860, 8804, 8981, 12066, 9708, 8414, 8673, 9105, 9546, 7860, 8804, 8981, 10437, 10601, 10601, 8515, 10601, 8515, 7738, 10601, 8515, 7738, 9841, 10601, 8515, 7738, 9841, 9457, 10601, 8515, 7738, 9841, 9457, 9540, 10601, 8515, 7738, 9841, 9457, 9540, 10362, 10601, 8515, 7738, 9841, 9457, 9540, 10362, 7533, 10601, 8515, 7738, 9841, 9457, 9540, 10362, 7533, 10822, 10601, 8515, 7738, 9841, 9457, 9540, 10362, 7533, 10822, 7121, 9682, 9682, 11119, 9682, 11119, 9390, 9682, 11119, 9390, 7670, 9682, 11119, 9390, 7670, 7256, 9682, 11119, 9390, 7670, 7256, 12231, 9682, 11119, 9390, 7670, 7256, 12231, 6929, 9682, 11119, 9390, 7670, 7256, 12231, 6929, 7649, 9682, 11119, 9390, 7670, 7256, 12231, 6929, 7649, 10519, };static struct simple_udp_connection unicast_connection;
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