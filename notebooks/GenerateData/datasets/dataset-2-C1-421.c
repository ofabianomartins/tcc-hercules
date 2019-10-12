#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {11010, 11010, 10291, 11010, 10291, 8786, 11010, 10291, 8786, 8883, 11010, 10291, 8786, 8883, 11356, 11010, 10291, 8786, 8883, 11356, 7714, 11010, 10291, 8786, 8883, 11356, 7714, 7383, 11010, 10291, 8786, 8883, 11356, 7714, 7383, 8745, 11010, 10291, 8786, 8883, 11356, 7714, 7383, 8745, 10566, 11010, 10291, 8786, 8883, 11356, 7714, 7383, 8745, 10566, 7512, 11010, 10291, 8786, 8883, 11356, 7714, 7383, 8745, 10566, 7512, 6940, 10788, 10788, 9758, 10788, 9758, 11460, 10788, 9758, 11460, 9020, 10788, 9758, 11460, 9020, 7618, 10788, 9758, 11460, 9020, 7618, 11221, 10788, 9758, 11460, 9020, 7618, 11221, 9626, 10788, 9758, 11460, 9020, 7618, 11221, 9626, 10890, 10788, 9758, 11460, 9020, 7618, 11221, 9626, 10890, 10663, 10788, 9758, 11460, 9020, 7618, 11221, 9626, 10890, 10663, 7758, 8367, 8367, 7545, 8367, 7545, 8235, 8367, 7545, 8235, 10058, 8367, 7545, 8235, 10058, 8585, 8367, 7545, 8235, 10058, 8585, 9276, 8367, 7545, 8235, 10058, 8585, 9276, 9911, 8367, 7545, 8235, 10058, 8585, 9276, 9911, 7959, 8367, 7545, 8235, 10058, 8585, 9276, 9911, 7959, 11714, 8367, 7545, 8235, 10058, 8585, 9276, 9911, 7959, 11714, 9153, 7912, 7912, 8533, 7912, 8533, 8604, 7912, 8533, 8604, 13730, 7912, 8533, 8604, 13730, 11925, 7912, 8533, 8604, 13730, 11925, 9859, 7912, 8533, 8604, 13730, 11925, 9859, 6884, 7912, 8533, 8604, 13730, 11925, 9859, 6884, 9859, 7912, 8533, 8604, 13730, 11925, 9859, 6884, 9859, 10623, 7912, 8533, 8604, 13730, 11925, 9859, 6884, 9859, 10623, 10687, 6352, 6352, 9885, 6352, 9885, 7465, 6352, 9885, 7465, 8324, 6352, 9885, 7465, 8324, 7617, 6352, 9885, 7465, 8324, 7617, 7490, 6352, 9885, 7465, 8324, 7617, 7490, 8523, 6352, 9885, 7465, 8324, 7617, 7490, 8523, 8520, 6352, 9885, 7465, 8324, 7617, 7490, 8523, 8520, 9050, 6352, 9885, 7465, 8324, 7617, 7490, 8523, 8520, 9050, 11539, 10262, 10262, 10060, 10262, 10060, 10759, 10262, 10060, 10759, 6711, 10262, 10060, 10759, 6711, 8771, 10262, 10060, 10759, 6711, 8771, 7477, 10262, 10060, 10759, 6711, 8771, 7477, 7801, 10262, 10060, 10759, 6711, 8771, 7477, 7801, 8951, 10262, 10060, 10759, 6711, 8771, 7477, 7801, 8951, 9368, 10262, 10060, 10759, 6711, 8771, 7477, 7801, 8951, 9368, 6710, 7283, 7283, 11592, 7283, 11592, 9038, 7283, 11592, 9038, 5997, 7283, 11592, 9038, 5997, 10160, 7283, 11592, 9038, 5997, 10160, 8392, 7283, 11592, 9038, 5997, 10160, 8392, 8503, 7283, 11592, 9038, 5997, 10160, 8392, 8503, 11726, 7283, 11592, 9038, 5997, 10160, 8392, 8503, 11726, 12569, 7283, 11592, 9038, 5997, 10160, 8392, 8503, 11726, 12569, 9613, 8335, 8335, 10567, 8335, 10567, 6914, 8335, 10567, 6914, 10540, 8335, 10567, 6914, 10540, 12299, 8335, 10567, 6914, 10540, 12299, 10094, 8335, 10567, 6914, 10540, 12299, 10094, 6542, 8335, 10567, 6914, 10540, 12299, 10094, 6542, 9163, 8335, 10567, 6914, 10540, 12299, 10094, 6542, 9163, 10796, 8335, 10567, 6914, 10540, 12299, 10094, 6542, 9163, 10796, 5091, 9358, 9358, 10982, 9358, 10982, 11852, 9358, 10982, 11852, 11288, 9358, 10982, 11852, 11288, 8193, 9358, 10982, 11852, 11288, 8193, 12258, 9358, 10982, 11852, 11288, 8193, 12258, 8445, 9358, 10982, 11852, 11288, 8193, 12258, 8445, 7579, 9358, 10982, 11852, 11288, 8193, 12258, 8445, 7579, 6570, 9358, 10982, 11852, 11288, 8193, 12258, 8445, 7579, 6570, 7910, 8421, 8421, 8545, 8421, 8545, 10927, 8421, 8545, 10927, 6852, 8421, 8545, 10927, 6852, 10207, 8421, 8545, 10927, 6852, 10207, 10206, 8421, 8545, 10927, 6852, 10207, 10206, 9140, 8421, 8545, 10927, 6852, 10207, 10206, 9140, 7583, 8421, 8545, 10927, 6852, 10207, 10206, 9140, 7583, 10548, };static struct simple_udp_connection unicast_connection;
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