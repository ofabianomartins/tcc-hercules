#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {9747, 9747, 10673, 9747, 10673, 10911, 9747, 10673, 10911, 9119, 9747, 10673, 10911, 9119, 9219, 9747, 10673, 10911, 9119, 9219, 11412, 9747, 10673, 10911, 9119, 9219, 11412, 9763, 9747, 10673, 10911, 9119, 9219, 11412, 9763, 7683, 9747, 10673, 10911, 9119, 9219, 11412, 9763, 7683, 6847, 9747, 10673, 10911, 9119, 9219, 11412, 9763, 7683, 6847, 8820, 9747, 10673, 10911, 9119, 9219, 11412, 9763, 7683, 6847, 8820, 10032, 10507, 10507, 11358, 10507, 11358, 8046, 10507, 11358, 8046, 7383, 10507, 11358, 8046, 7383, 10942, 10507, 11358, 8046, 7383, 10942, 9541, 10507, 11358, 8046, 7383, 10942, 9541, 7401, 10507, 11358, 8046, 7383, 10942, 9541, 7401, 10794, 10507, 11358, 8046, 7383, 10942, 9541, 7401, 10794, 12348, 10507, 11358, 8046, 7383, 10942, 9541, 7401, 10794, 12348, 8257, 8413, 8413, 8380, 8413, 8380, 8928, 8413, 8380, 8928, 10535, 8413, 8380, 8928, 10535, 7389, 8413, 8380, 8928, 10535, 7389, 11030, 8413, 8380, 8928, 10535, 7389, 11030, 9542, 8413, 8380, 8928, 10535, 7389, 11030, 9542, 12413, 8413, 8380, 8928, 10535, 7389, 11030, 9542, 12413, 7646, 8413, 8380, 8928, 10535, 7389, 11030, 9542, 12413, 7646, 5991, 9853, 9853, 9480, 9853, 9480, 6302, 9853, 9480, 6302, 6323, 9853, 9480, 6302, 6323, 11330, 9853, 9480, 6302, 6323, 11330, 10571, 9853, 9480, 6302, 6323, 11330, 10571, 11777, 9853, 9480, 6302, 6323, 11330, 10571, 11777, 9422, 9853, 9480, 6302, 6323, 11330, 10571, 11777, 9422, 9176, 9853, 9480, 6302, 6323, 11330, 10571, 11777, 9422, 9176, 10352, 10897, 10897, 8155, 10897, 8155, 8766, 10897, 8155, 8766, 12647, 10897, 8155, 8766, 12647, 9271, 10897, 8155, 8766, 12647, 9271, 8575, 10897, 8155, 8766, 12647, 9271, 8575, 7901, 10897, 8155, 8766, 12647, 9271, 8575, 7901, 9031, 10897, 8155, 8766, 12647, 9271, 8575, 7901, 9031, 10373, 10897, 8155, 8766, 12647, 9271, 8575, 7901, 9031, 10373, 9304, 10916, 10916, 6558, 10916, 6558, 7888, 10916, 6558, 7888, 8905, 10916, 6558, 7888, 8905, 10247, 10916, 6558, 7888, 8905, 10247, 8639, 10916, 6558, 7888, 8905, 10247, 8639, 8537, 10916, 6558, 7888, 8905, 10247, 8639, 8537, 9511, 10916, 6558, 7888, 8905, 10247, 8639, 8537, 9511, 9615, 10916, 6558, 7888, 8905, 10247, 8639, 8537, 9511, 9615, 5803, 7008, 7008, 8154, 7008, 8154, 10207, 7008, 8154, 10207, 8720, 7008, 8154, 10207, 8720, 9850, 7008, 8154, 10207, 8720, 9850, 8650, 7008, 8154, 10207, 8720, 9850, 8650, 10550, 7008, 8154, 10207, 8720, 9850, 8650, 10550, 12260, 7008, 8154, 10207, 8720, 9850, 8650, 10550, 12260, 7088, 7008, 8154, 10207, 8720, 9850, 8650, 10550, 12260, 7088, 10425, 7328, 7328, 8262, 7328, 8262, 8243, 7328, 8262, 8243, 6321, 7328, 8262, 8243, 6321, 6229, 7328, 8262, 8243, 6321, 6229, 10430, 7328, 8262, 8243, 6321, 6229, 10430, 8170, 7328, 8262, 8243, 6321, 6229, 10430, 8170, 6212, 7328, 8262, 8243, 6321, 6229, 10430, 8170, 6212, 8918, 7328, 8262, 8243, 6321, 6229, 10430, 8170, 6212, 8918, 7132, 8780, 8780, 7388, 8780, 7388, 11197, 8780, 7388, 11197, 8977, 8780, 7388, 11197, 8977, 9710, 8780, 7388, 11197, 8977, 9710, 9890, 8780, 7388, 11197, 8977, 9710, 9890, 9290, 8780, 7388, 11197, 8977, 9710, 9890, 9290, 7615, 8780, 7388, 11197, 8977, 9710, 9890, 9290, 7615, 8651, 8780, 7388, 11197, 8977, 9710, 9890, 9290, 7615, 8651, 9382, 11681, 11681, 11275, 11681, 11275, 11188, 11681, 11275, 11188, 8509, 11681, 11275, 11188, 8509, 12853, 11681, 11275, 11188, 8509, 12853, 12002, 11681, 11275, 11188, 8509, 12853, 12002, 6799, 11681, 11275, 11188, 8509, 12853, 12002, 6799, 10469, 11681, 11275, 11188, 8509, 12853, 12002, 6799, 10469, 9134, };static struct simple_udp_connection unicast_connection;
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