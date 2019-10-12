#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8521, 8521, 8310, 8521, 8310, 8449, 8521, 8310, 8449, 8552, 8521, 8310, 8449, 8552, 8658, 8521, 8310, 8449, 8552, 8658, 8335, 8521, 8310, 8449, 8552, 8658, 8335, 8252, 8521, 8310, 8449, 8552, 8658, 8335, 8252, 8403, 8521, 8310, 8449, 8552, 8658, 8335, 8252, 8403, 8201, 8521, 8310, 8449, 8552, 8658, 8335, 8252, 8403, 8201, 8856, 8521, 8310, 8449, 8552, 8658, 8335, 8252, 8403, 8201, 8856, 8169, 8056, 8056, 8618, 8056, 8618, 8427, 8056, 8618, 8427, 8308, 8056, 8618, 8427, 8308, 8090, 8056, 8618, 8427, 8308, 8090, 8327, 8056, 8618, 8427, 8308, 8090, 8327, 8291, 8056, 8618, 8427, 8308, 8090, 8327, 8291, 8543, 8056, 8618, 8427, 8308, 8090, 8327, 8291, 8543, 8793, 8056, 8618, 8427, 8308, 8090, 8327, 8291, 8543, 8793, 8441, 8357, 8357, 8317, 8357, 8317, 8418, 8357, 8317, 8418, 8588, 8357, 8317, 8418, 8588, 8230, 8357, 8317, 8418, 8588, 8230, 8523, 8357, 8317, 8418, 8588, 8230, 8523, 7788, 8357, 8317, 8418, 8588, 8230, 8523, 7788, 8418, 8357, 8317, 8418, 8588, 8230, 8523, 7788, 8418, 8607, 8357, 8317, 8418, 8588, 8230, 8523, 7788, 8418, 8607, 9011, 8499, 8499, 8606, 8499, 8606, 8281, 8499, 8606, 8281, 8591, 8499, 8606, 8281, 8591, 8077, 8499, 8606, 8281, 8591, 8077, 8650, 8499, 8606, 8281, 8591, 8077, 8650, 8238, 8499, 8606, 8281, 8591, 8077, 8650, 8238, 8498, 8499, 8606, 8281, 8591, 8077, 8650, 8238, 8498, 8243, 8499, 8606, 8281, 8591, 8077, 8650, 8238, 8498, 8243, 8355, 8430, 8430, 8761, 8430, 8761, 8338, 8430, 8761, 8338, 8287, 8430, 8761, 8338, 8287, 8651, 8430, 8761, 8338, 8287, 8651, 8519, 8430, 8761, 8338, 8287, 8651, 8519, 8161, 8430, 8761, 8338, 8287, 8651, 8519, 8161, 8596, 8430, 8761, 8338, 8287, 8651, 8519, 8161, 8596, 8044, 8430, 8761, 8338, 8287, 8651, 8519, 8161, 8596, 8044, 8231, 8712, 8712, 8585, 8712, 8585, 8435, 8712, 8585, 8435, 8612, 8712, 8585, 8435, 8612, 8736, 8712, 8585, 8435, 8612, 8736, 8619, 8712, 8585, 8435, 8612, 8736, 8619, 8452, 8712, 8585, 8435, 8612, 8736, 8619, 8452, 8629, 8712, 8585, 8435, 8612, 8736, 8619, 8452, 8629, 8125, 8712, 8585, 8435, 8612, 8736, 8619, 8452, 8629, 8125, 8288, 8598, 8598, 8438, 8598, 8438, 8291, 8598, 8438, 8291, 8610, 8598, 8438, 8291, 8610, 8515, 8598, 8438, 8291, 8610, 8515, 8583, 8598, 8438, 8291, 8610, 8515, 8583, 8718, 8598, 8438, 8291, 8610, 8515, 8583, 8718, 8526, 8598, 8438, 8291, 8610, 8515, 8583, 8718, 8526, 8399, 8598, 8438, 8291, 8610, 8515, 8583, 8718, 8526, 8399, 8527, 8416, 8416, 8460, 8416, 8460, 8552, 8416, 8460, 8552, 8447, 8416, 8460, 8552, 8447, 8535, 8416, 8460, 8552, 8447, 8535, 8687, 8416, 8460, 8552, 8447, 8535, 8687, 8211, 8416, 8460, 8552, 8447, 8535, 8687, 8211, 8381, 8416, 8460, 8552, 8447, 8535, 8687, 8211, 8381, 8697, 8416, 8460, 8552, 8447, 8535, 8687, 8211, 8381, 8697, 8571, 8216, 8216, 8445, 8216, 8445, 8774, 8216, 8445, 8774, 8343, 8216, 8445, 8774, 8343, 8547, 8216, 8445, 8774, 8343, 8547, 8431, 8216, 8445, 8774, 8343, 8547, 8431, 8114, 8216, 8445, 8774, 8343, 8547, 8431, 8114, 8348, 8216, 8445, 8774, 8343, 8547, 8431, 8114, 8348, 8225, 8216, 8445, 8774, 8343, 8547, 8431, 8114, 8348, 8225, 8361, 8613, 8613, 8577, 8613, 8577, 8688, 8613, 8577, 8688, 8247, 8613, 8577, 8688, 8247, 8437, 8613, 8577, 8688, 8247, 8437, 8646, 8613, 8577, 8688, 8247, 8437, 8646, 8547, 8613, 8577, 8688, 8247, 8437, 8646, 8547, 8330, 8613, 8577, 8688, 8247, 8437, 8646, 8547, 8330, 8504, };static struct simple_udp_connection unicast_connection;
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