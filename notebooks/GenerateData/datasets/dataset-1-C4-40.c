#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8378, 8378, 8474, 8378, 8474, 8363, 8378, 8474, 8363, 8685, 8378, 8474, 8363, 8685, 8131, 8378, 8474, 8363, 8685, 8131, 8406, 8378, 8474, 8363, 8685, 8131, 8406, 8958, 8378, 8474, 8363, 8685, 8131, 8406, 8958, 8597, 8378, 8474, 8363, 8685, 8131, 8406, 8958, 8597, 8642, 8378, 8474, 8363, 8685, 8131, 8406, 8958, 8597, 8642, 8711, 8378, 8474, 8363, 8685, 8131, 8406, 8958, 8597, 8642, 8711, 8510, 8551, 8551, 8617, 8551, 8617, 8677, 8551, 8617, 8677, 8296, 8551, 8617, 8677, 8296, 8473, 8551, 8617, 8677, 8296, 8473, 8412, 8551, 8617, 8677, 8296, 8473, 8412, 8598, 8551, 8617, 8677, 8296, 8473, 8412, 8598, 8460, 8551, 8617, 8677, 8296, 8473, 8412, 8598, 8460, 8245, 8551, 8617, 8677, 8296, 8473, 8412, 8598, 8460, 8245, 8558, 8521, 8521, 8506, 8521, 8506, 8754, 8521, 8506, 8754, 8714, 8521, 8506, 8754, 8714, 8583, 8521, 8506, 8754, 8714, 8583, 8810, 8521, 8506, 8754, 8714, 8583, 8810, 8437, 8521, 8506, 8754, 8714, 8583, 8810, 8437, 8224, 8521, 8506, 8754, 8714, 8583, 8810, 8437, 8224, 8774, 8521, 8506, 8754, 8714, 8583, 8810, 8437, 8224, 8774, 8505, 8435, 8435, 8330, 8435, 8330, 8413, 8435, 8330, 8413, 8232, 8435, 8330, 8413, 8232, 8541, 8435, 8330, 8413, 8232, 8541, 8215, 8435, 8330, 8413, 8232, 8541, 8215, 8389, 8435, 8330, 8413, 8232, 8541, 8215, 8389, 8514, 8435, 8330, 8413, 8232, 8541, 8215, 8389, 8514, 8398, 8435, 8330, 8413, 8232, 8541, 8215, 8389, 8514, 8398, 8710, 8694, 8694, 8515, 8694, 8515, 8412, 8694, 8515, 8412, 8610, 8694, 8515, 8412, 8610, 8553, 8694, 8515, 8412, 8610, 8553, 8501, 8694, 8515, 8412, 8610, 8553, 8501, 8628, 8694, 8515, 8412, 8610, 8553, 8501, 8628, 8464, 8694, 8515, 8412, 8610, 8553, 8501, 8628, 8464, 8639, 8694, 8515, 8412, 8610, 8553, 8501, 8628, 8464, 8639, 8262, 8433, 8433, 8506, 8433, 8506, 8278, 8433, 8506, 8278, 8390, 8433, 8506, 8278, 8390, 8093, 8433, 8506, 8278, 8390, 8093, 8781, 8433, 8506, 8278, 8390, 8093, 8781, 8626, 8433, 8506, 8278, 8390, 8093, 8781, 8626, 8942, 8433, 8506, 8278, 8390, 8093, 8781, 8626, 8942, 8394, 8433, 8506, 8278, 8390, 8093, 8781, 8626, 8942, 8394, 8608, 8483, 8483, 8758, 8483, 8758, 8464, 8483, 8758, 8464, 8837, 8483, 8758, 8464, 8837, 8290, 8483, 8758, 8464, 8837, 8290, 8628, 8483, 8758, 8464, 8837, 8290, 8628, 8465, 8483, 8758, 8464, 8837, 8290, 8628, 8465, 8277, 8483, 8758, 8464, 8837, 8290, 8628, 8465, 8277, 8495, 8483, 8758, 8464, 8837, 8290, 8628, 8465, 8277, 8495, 8137, 8540, 8540, 8606, 8540, 8606, 8296, 8540, 8606, 8296, 8130, 8540, 8606, 8296, 8130, 8534, 8540, 8606, 8296, 8130, 8534, 8657, 8540, 8606, 8296, 8130, 8534, 8657, 8485, 8540, 8606, 8296, 8130, 8534, 8657, 8485, 8667, 8540, 8606, 8296, 8130, 8534, 8657, 8485, 8667, 8460, 8540, 8606, 8296, 8130, 8534, 8657, 8485, 8667, 8460, 8481, 8674, 8674, 8332, 8674, 8332, 8664, 8674, 8332, 8664, 8640, 8674, 8332, 8664, 8640, 8619, 8674, 8332, 8664, 8640, 8619, 8398, 8674, 8332, 8664, 8640, 8619, 8398, 8527, 8674, 8332, 8664, 8640, 8619, 8398, 8527, 8201, 8674, 8332, 8664, 8640, 8619, 8398, 8527, 8201, 8622, 8674, 8332, 8664, 8640, 8619, 8398, 8527, 8201, 8622, 8773, 8159, 8159, 8396, 8159, 8396, 8253, 8159, 8396, 8253, 7923, 8159, 8396, 8253, 7923, 8496, 8159, 8396, 8253, 7923, 8496, 8161, 8159, 8396, 8253, 7923, 8496, 8161, 8376, 8159, 8396, 8253, 7923, 8496, 8161, 8376, 8674, 8159, 8396, 8253, 7923, 8496, 8161, 8376, 8674, 8843, };static struct simple_udp_connection unicast_connection;
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