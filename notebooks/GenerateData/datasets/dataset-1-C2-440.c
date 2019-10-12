#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8435, 8435, 8718, 8435, 8718, 8805, 8435, 8718, 8805, 8775, 8435, 8718, 8805, 8775, 8438, 8435, 8718, 8805, 8775, 8438, 8434, 8435, 8718, 8805, 8775, 8438, 8434, 8212, 8435, 8718, 8805, 8775, 8438, 8434, 8212, 8354, 8435, 8718, 8805, 8775, 8438, 8434, 8212, 8354, 8221, 8435, 8718, 8805, 8775, 8438, 8434, 8212, 8354, 8221, 8674, 8435, 8718, 8805, 8775, 8438, 8434, 8212, 8354, 8221, 8674, 8188, 8684, 8684, 8514, 8684, 8514, 8810, 8684, 8514, 8810, 8220, 8684, 8514, 8810, 8220, 8474, 8684, 8514, 8810, 8220, 8474, 8433, 8684, 8514, 8810, 8220, 8474, 8433, 8494, 8684, 8514, 8810, 8220, 8474, 8433, 8494, 8377, 8684, 8514, 8810, 8220, 8474, 8433, 8494, 8377, 8569, 8684, 8514, 8810, 8220, 8474, 8433, 8494, 8377, 8569, 8409, 8410, 8410, 8318, 8410, 8318, 8451, 8410, 8318, 8451, 8480, 8410, 8318, 8451, 8480, 8477, 8410, 8318, 8451, 8480, 8477, 8368, 8410, 8318, 8451, 8480, 8477, 8368, 8793, 8410, 8318, 8451, 8480, 8477, 8368, 8793, 8332, 8410, 8318, 8451, 8480, 8477, 8368, 8793, 8332, 8401, 8410, 8318, 8451, 8480, 8477, 8368, 8793, 8332, 8401, 8819, 8482, 8482, 8146, 8482, 8146, 8418, 8482, 8146, 8418, 7993, 8482, 8146, 8418, 7993, 8486, 8482, 8146, 8418, 7993, 8486, 8524, 8482, 8146, 8418, 7993, 8486, 8524, 8328, 8482, 8146, 8418, 7993, 8486, 8524, 8328, 8400, 8482, 8146, 8418, 7993, 8486, 8524, 8328, 8400, 8383, 8482, 8146, 8418, 7993, 8486, 8524, 8328, 8400, 8383, 8358, 8741, 8741, 8745, 8741, 8745, 8563, 8741, 8745, 8563, 8195, 8741, 8745, 8563, 8195, 8158, 8741, 8745, 8563, 8195, 8158, 8414, 8741, 8745, 8563, 8195, 8158, 8414, 8748, 8741, 8745, 8563, 8195, 8158, 8414, 8748, 8598, 8741, 8745, 8563, 8195, 8158, 8414, 8748, 8598, 8874, 8741, 8745, 8563, 8195, 8158, 8414, 8748, 8598, 8874, 8487, 8780, 8780, 8325, 8780, 8325, 8340, 8780, 8325, 8340, 8490, 8780, 8325, 8340, 8490, 8384, 8780, 8325, 8340, 8490, 8384, 8663, 8780, 8325, 8340, 8490, 8384, 8663, 8415, 8780, 8325, 8340, 8490, 8384, 8663, 8415, 8319, 8780, 8325, 8340, 8490, 8384, 8663, 8415, 8319, 8593, 8780, 8325, 8340, 8490, 8384, 8663, 8415, 8319, 8593, 8723, 8680, 8680, 8398, 8680, 8398, 8315, 8680, 8398, 8315, 8598, 8680, 8398, 8315, 8598, 8157, 8680, 8398, 8315, 8598, 8157, 8456, 8680, 8398, 8315, 8598, 8157, 8456, 8234, 8680, 8398, 8315, 8598, 8157, 8456, 8234, 8510, 8680, 8398, 8315, 8598, 8157, 8456, 8234, 8510, 8172, 8680, 8398, 8315, 8598, 8157, 8456, 8234, 8510, 8172, 8199, 8610, 8610, 8420, 8610, 8420, 8759, 8610, 8420, 8759, 8309, 8610, 8420, 8759, 8309, 8652, 8610, 8420, 8759, 8309, 8652, 8255, 8610, 8420, 8759, 8309, 8652, 8255, 8516, 8610, 8420, 8759, 8309, 8652, 8255, 8516, 8711, 8610, 8420, 8759, 8309, 8652, 8255, 8516, 8711, 8661, 8610, 8420, 8759, 8309, 8652, 8255, 8516, 8711, 8661, 8687, 8582, 8582, 8335, 8582, 8335, 8371, 8582, 8335, 8371, 8757, 8582, 8335, 8371, 8757, 8349, 8582, 8335, 8371, 8757, 8349, 8699, 8582, 8335, 8371, 8757, 8349, 8699, 8104, 8582, 8335, 8371, 8757, 8349, 8699, 8104, 8594, 8582, 8335, 8371, 8757, 8349, 8699, 8104, 8594, 8721, 8582, 8335, 8371, 8757, 8349, 8699, 8104, 8594, 8721, 8710, 8585, 8585, 8306, 8585, 8306, 8527, 8585, 8306, 8527, 8363, 8585, 8306, 8527, 8363, 8643, 8585, 8306, 8527, 8363, 8643, 8515, 8585, 8306, 8527, 8363, 8643, 8515, 8244, 8585, 8306, 8527, 8363, 8643, 8515, 8244, 8749, 8585, 8306, 8527, 8363, 8643, 8515, 8244, 8749, 8538, };static struct simple_udp_connection unicast_connection;
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