#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {10771, 10771, 9738, 10771, 9738, 7654, 10771, 9738, 7654, 7192, 10771, 9738, 7654, 7192, 9613, 10771, 9738, 7654, 7192, 9613, 9369, 10771, 9738, 7654, 7192, 9613, 9369, 13315, 10771, 9738, 7654, 7192, 9613, 9369, 13315, 9245, 10771, 9738, 7654, 7192, 9613, 9369, 13315, 9245, 10707, 10771, 9738, 7654, 7192, 9613, 9369, 13315, 9245, 10707, 11449, 10771, 9738, 7654, 7192, 9613, 9369, 13315, 9245, 10707, 11449, 5387, 11823, 11823, 9973, 11823, 9973, 10679, 11823, 9973, 10679, 9403, 11823, 9973, 10679, 9403, 10328, 11823, 9973, 10679, 9403, 10328, 8918, 11823, 9973, 10679, 9403, 10328, 8918, 8699, 11823, 9973, 10679, 9403, 10328, 8918, 8699, 4541, 11823, 9973, 10679, 9403, 10328, 8918, 8699, 4541, 7897, 11823, 9973, 10679, 9403, 10328, 8918, 8699, 4541, 7897, 3836, 11791, 11791, 8545, 11791, 8545, 8398, 11791, 8545, 8398, 11150, 11791, 8545, 8398, 11150, 11273, 11791, 8545, 8398, 11150, 11273, 9479, 11791, 8545, 8398, 11150, 11273, 9479, 12542, 11791, 8545, 8398, 11150, 11273, 9479, 12542, 8321, 11791, 8545, 8398, 11150, 11273, 9479, 12542, 8321, 9697, 11791, 8545, 8398, 11150, 11273, 9479, 12542, 8321, 9697, 9312, 10925, 10925, 2925, 10925, 2925, 8886, 10925, 2925, 8886, 6949, 10925, 2925, 8886, 6949, 7618, 10925, 2925, 8886, 6949, 7618, 7196, 10925, 2925, 8886, 6949, 7618, 7196, 11951, 10925, 2925, 8886, 6949, 7618, 7196, 11951, 9451, 10925, 2925, 8886, 6949, 7618, 7196, 11951, 9451, 9590, 10925, 2925, 8886, 6949, 7618, 7196, 11951, 9451, 9590, 10542, 9544, 9544, 10655, 9544, 10655, 13112, 9544, 10655, 13112, 7514, 9544, 10655, 13112, 7514, 9447, 9544, 10655, 13112, 7514, 9447, 8684, 9544, 10655, 13112, 7514, 9447, 8684, 9437, 9544, 10655, 13112, 7514, 9447, 8684, 9437, 8831, 9544, 10655, 13112, 7514, 9447, 8684, 9437, 8831, 8780, 9544, 10655, 13112, 7514, 9447, 8684, 9437, 8831, 8780, 10345, 6308, 6308, 11190, 6308, 11190, 11086, 6308, 11190, 11086, 7138, 6308, 11190, 11086, 7138, 8660, 6308, 11190, 11086, 7138, 8660, 8994, 6308, 11190, 11086, 7138, 8660, 8994, 8011, 6308, 11190, 11086, 7138, 8660, 8994, 8011, 8182, 6308, 11190, 11086, 7138, 8660, 8994, 8011, 8182, 8658, 6308, 11190, 11086, 7138, 8660, 8994, 8011, 8182, 8658, 7410, 8835, 8835, 7572, 8835, 7572, 9990, 8835, 7572, 9990, 7565, 8835, 7572, 9990, 7565, 10562, 8835, 7572, 9990, 7565, 10562, 8635, 8835, 7572, 9990, 7565, 10562, 8635, 10894, 8835, 7572, 9990, 7565, 10562, 8635, 10894, 10679, 8835, 7572, 9990, 7565, 10562, 8635, 10894, 10679, 9954, 8835, 7572, 9990, 7565, 10562, 8635, 10894, 10679, 9954, 11120, 7981, 7981, 9701, 7981, 9701, 11106, 7981, 9701, 11106, 10899, 7981, 9701, 11106, 10899, 9730, 7981, 9701, 11106, 10899, 9730, 9817, 7981, 9701, 11106, 10899, 9730, 9817, 5544, 7981, 9701, 11106, 10899, 9730, 9817, 5544, 7005, 7981, 9701, 11106, 10899, 9730, 9817, 5544, 7005, 10918, 7981, 9701, 11106, 10899, 9730, 9817, 5544, 7005, 10918, 12095, 7216, 7216, 8982, 7216, 8982, 10965, 7216, 8982, 10965, 8251, 7216, 8982, 10965, 8251, 10057, 7216, 8982, 10965, 8251, 10057, 11268, 7216, 8982, 10965, 8251, 10057, 11268, 7493, 7216, 8982, 10965, 8251, 10057, 11268, 7493, 6507, 7216, 8982, 10965, 8251, 10057, 11268, 7493, 6507, 9000, 7216, 8982, 10965, 8251, 10057, 11268, 7493, 6507, 9000, 9790, 10379, 10379, 10155, 10379, 10155, 9567, 10379, 10155, 9567, 8546, 10379, 10155, 9567, 8546, 8386, 10379, 10155, 9567, 8546, 8386, 7022, 10379, 10155, 9567, 8546, 8386, 7022, 5706, 10379, 10155, 9567, 8546, 8386, 7022, 5706, 9221, 10379, 10155, 9567, 8546, 8386, 7022, 5706, 9221, 6900, };static struct simple_udp_connection unicast_connection;
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