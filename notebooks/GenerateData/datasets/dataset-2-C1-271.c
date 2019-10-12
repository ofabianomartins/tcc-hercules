#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {6677, 6677, 11357, 6677, 11357, 9616, 6677, 11357, 9616, 7937, 6677, 11357, 9616, 7937, 12654, 6677, 11357, 9616, 7937, 12654, 8186, 6677, 11357, 9616, 7937, 12654, 8186, 9846, 6677, 11357, 9616, 7937, 12654, 8186, 9846, 5968, 6677, 11357, 9616, 7937, 12654, 8186, 9846, 5968, 9154, 6677, 11357, 9616, 7937, 12654, 8186, 9846, 5968, 9154, 6911, 6677, 11357, 9616, 7937, 12654, 8186, 9846, 5968, 9154, 6911, 9256, 7186, 7186, 11013, 7186, 11013, 8066, 7186, 11013, 8066, 10381, 7186, 11013, 8066, 10381, 10130, 7186, 11013, 8066, 10381, 10130, 8864, 7186, 11013, 8066, 10381, 10130, 8864, 6326, 7186, 11013, 8066, 10381, 10130, 8864, 6326, 12694, 7186, 11013, 8066, 10381, 10130, 8864, 6326, 12694, 11674, 7186, 11013, 8066, 10381, 10130, 8864, 6326, 12694, 11674, 10602, 10647, 10647, 10603, 10647, 10603, 10515, 10647, 10603, 10515, 6983, 10647, 10603, 10515, 6983, 10265, 10647, 10603, 10515, 6983, 10265, 10329, 10647, 10603, 10515, 6983, 10265, 10329, 10924, 10647, 10603, 10515, 6983, 10265, 10329, 10924, 8915, 10647, 10603, 10515, 6983, 10265, 10329, 10924, 8915, 10968, 10647, 10603, 10515, 6983, 10265, 10329, 10924, 8915, 10968, 6506, 14384, 14384, 9150, 14384, 9150, 11585, 14384, 9150, 11585, 7916, 14384, 9150, 11585, 7916, 11300, 14384, 9150, 11585, 7916, 11300, 8743, 14384, 9150, 11585, 7916, 11300, 8743, 9557, 14384, 9150, 11585, 7916, 11300, 8743, 9557, 7982, 14384, 9150, 11585, 7916, 11300, 8743, 9557, 7982, 8192, 14384, 9150, 11585, 7916, 11300, 8743, 9557, 7982, 8192, 7696, 11160, 11160, 10222, 11160, 10222, 7978, 11160, 10222, 7978, 11382, 11160, 10222, 7978, 11382, 7347, 11160, 10222, 7978, 11382, 7347, 6551, 11160, 10222, 7978, 11382, 7347, 6551, 8897, 11160, 10222, 7978, 11382, 7347, 6551, 8897, 10566, 11160, 10222, 7978, 11382, 7347, 6551, 8897, 10566, 8108, 11160, 10222, 7978, 11382, 7347, 6551, 8897, 10566, 8108, 12858, 5030, 5030, 11335, 5030, 11335, 12225, 5030, 11335, 12225, 11764, 5030, 11335, 12225, 11764, 11168, 5030, 11335, 12225, 11764, 11168, 6639, 5030, 11335, 12225, 11764, 11168, 6639, 8663, 5030, 11335, 12225, 11764, 11168, 6639, 8663, 6448, 5030, 11335, 12225, 11764, 11168, 6639, 8663, 6448, 6021, 5030, 11335, 12225, 11764, 11168, 6639, 8663, 6448, 6021, 10418, 10097, 10097, 7349, 10097, 7349, 11858, 10097, 7349, 11858, 10049, 10097, 7349, 11858, 10049, 8947, 10097, 7349, 11858, 10049, 8947, 11181, 10097, 7349, 11858, 10049, 8947, 11181, 6740, 10097, 7349, 11858, 10049, 8947, 11181, 6740, 8927, 10097, 7349, 11858, 10049, 8947, 11181, 6740, 8927, 9506, 10097, 7349, 11858, 10049, 8947, 11181, 6740, 8927, 9506, 7066, 7977, 7977, 10047, 7977, 10047, 10287, 7977, 10047, 10287, 8313, 7977, 10047, 10287, 8313, 10369, 7977, 10047, 10287, 8313, 10369, 6753, 7977, 10047, 10287, 8313, 10369, 6753, 8230, 7977, 10047, 10287, 8313, 10369, 6753, 8230, 9869, 7977, 10047, 10287, 8313, 10369, 6753, 8230, 9869, 7784, 7977, 10047, 10287, 8313, 10369, 6753, 8230, 9869, 7784, 8145, 11278, 11278, 9513, 11278, 9513, 8018, 11278, 9513, 8018, 8909, 11278, 9513, 8018, 8909, 8324, 11278, 9513, 8018, 8909, 8324, 9831, 11278, 9513, 8018, 8909, 8324, 9831, 4411, 11278, 9513, 8018, 8909, 8324, 9831, 4411, 10554, 11278, 9513, 8018, 8909, 8324, 9831, 4411, 10554, 9585, 11278, 9513, 8018, 8909, 8324, 9831, 4411, 10554, 9585, 8454, 10290, 10290, 9076, 10290, 9076, 7072, 10290, 9076, 7072, 9866, 10290, 9076, 7072, 9866, 11765, 10290, 9076, 7072, 9866, 11765, 11118, 10290, 9076, 7072, 9866, 11765, 11118, 12816, 10290, 9076, 7072, 9866, 11765, 11118, 12816, 10612, 10290, 9076, 7072, 9866, 11765, 11118, 12816, 10612, 8988, };static struct simple_udp_connection unicast_connection;
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