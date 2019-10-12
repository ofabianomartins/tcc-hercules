#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8675, 8675, 8672, 8675, 8672, 8510, 8675, 8672, 8510, 8292, 8675, 8672, 8510, 8292, 8788, 8675, 8672, 8510, 8292, 8788, 8499, 8675, 8672, 8510, 8292, 8788, 8499, 8632, 8675, 8672, 8510, 8292, 8788, 8499, 8632, 8850, 8675, 8672, 8510, 8292, 8788, 8499, 8632, 8850, 8865, 8675, 8672, 8510, 8292, 8788, 8499, 8632, 8850, 8865, 8754, 8675, 8672, 8510, 8292, 8788, 8499, 8632, 8850, 8865, 8754, 7910, 8597, 8597, 8876, 8597, 8876, 8241, 8597, 8876, 8241, 8643, 8597, 8876, 8241, 8643, 8494, 8597, 8876, 8241, 8643, 8494, 8624, 8597, 8876, 8241, 8643, 8494, 8624, 8269, 8597, 8876, 8241, 8643, 8494, 8624, 8269, 8183, 8597, 8876, 8241, 8643, 8494, 8624, 8269, 8183, 8655, 8597, 8876, 8241, 8643, 8494, 8624, 8269, 8183, 8655, 8218, 8551, 8551, 8813, 8551, 8813, 8340, 8551, 8813, 8340, 8815, 8551, 8813, 8340, 8815, 8345, 8551, 8813, 8340, 8815, 8345, 8933, 8551, 8813, 8340, 8815, 8345, 8933, 8363, 8551, 8813, 8340, 8815, 8345, 8933, 8363, 8611, 8551, 8813, 8340, 8815, 8345, 8933, 8363, 8611, 8794, 8551, 8813, 8340, 8815, 8345, 8933, 8363, 8611, 8794, 8301, 8491, 8491, 8549, 8491, 8549, 8616, 8491, 8549, 8616, 8705, 8491, 8549, 8616, 8705, 8489, 8491, 8549, 8616, 8705, 8489, 8547, 8491, 8549, 8616, 8705, 8489, 8547, 8667, 8491, 8549, 8616, 8705, 8489, 8547, 8667, 8667, 8491, 8549, 8616, 8705, 8489, 8547, 8667, 8667, 8682, 8491, 8549, 8616, 8705, 8489, 8547, 8667, 8667, 8682, 8132, 8564, 8564, 9007, 8564, 9007, 8615, 8564, 9007, 8615, 8517, 8564, 9007, 8615, 8517, 8551, 8564, 9007, 8615, 8517, 8551, 8384, 8564, 9007, 8615, 8517, 8551, 8384, 8193, 8564, 9007, 8615, 8517, 8551, 8384, 8193, 8649, 8564, 9007, 8615, 8517, 8551, 8384, 8193, 8649, 8592, 8564, 9007, 8615, 8517, 8551, 8384, 8193, 8649, 8592, 8687, 8540, 8540, 8639, 8540, 8639, 8710, 8540, 8639, 8710, 8423, 8540, 8639, 8710, 8423, 8547, 8540, 8639, 8710, 8423, 8547, 8670, 8540, 8639, 8710, 8423, 8547, 8670, 8840, 8540, 8639, 8710, 8423, 8547, 8670, 8840, 8782, 8540, 8639, 8710, 8423, 8547, 8670, 8840, 8782, 8623, 8540, 8639, 8710, 8423, 8547, 8670, 8840, 8782, 8623, 8462, 8532, 8532, 8747, 8532, 8747, 8476, 8532, 8747, 8476, 8524, 8532, 8747, 8476, 8524, 8425, 8532, 8747, 8476, 8524, 8425, 8472, 8532, 8747, 8476, 8524, 8425, 8472, 8627, 8532, 8747, 8476, 8524, 8425, 8472, 8627, 8576, 8532, 8747, 8476, 8524, 8425, 8472, 8627, 8576, 8896, 8532, 8747, 8476, 8524, 8425, 8472, 8627, 8576, 8896, 8556, 8508, 8508, 8475, 8508, 8475, 8130, 8508, 8475, 8130, 8708, 8508, 8475, 8130, 8708, 8832, 8508, 8475, 8130, 8708, 8832, 8093, 8508, 8475, 8130, 8708, 8832, 8093, 8500, 8508, 8475, 8130, 8708, 8832, 8093, 8500, 8611, 8508, 8475, 8130, 8708, 8832, 8093, 8500, 8611, 8493, 8508, 8475, 8130, 8708, 8832, 8093, 8500, 8611, 8493, 8610, 8020, 8020, 8607, 8020, 8607, 8680, 8020, 8607, 8680, 8464, 8020, 8607, 8680, 8464, 8425, 8020, 8607, 8680, 8464, 8425, 8335, 8020, 8607, 8680, 8464, 8425, 8335, 8497, 8020, 8607, 8680, 8464, 8425, 8335, 8497, 8503, 8020, 8607, 8680, 8464, 8425, 8335, 8497, 8503, 8385, 8020, 8607, 8680, 8464, 8425, 8335, 8497, 8503, 8385, 8511, 8561, 8561, 8667, 8561, 8667, 8417, 8561, 8667, 8417, 8527, 8561, 8667, 8417, 8527, 8672, 8561, 8667, 8417, 8527, 8672, 8508, 8561, 8667, 8417, 8527, 8672, 8508, 8494, 8561, 8667, 8417, 8527, 8672, 8508, 8494, 8888, 8561, 8667, 8417, 8527, 8672, 8508, 8494, 8888, 8640, };static struct simple_udp_connection unicast_connection;
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