#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15302, 15302, 15673, 15302, 15673, 15673, 15302, 15673, 15673, 15793, 15302, 15673, 15673, 15793, 15389, 15302, 15673, 15673, 15793, 15389, 15526, 15302, 15673, 15673, 15793, 15389, 15526, 14982, 15302, 15673, 15673, 15793, 15389, 15526, 14982, 15535, 15302, 15673, 15673, 15793, 15389, 15526, 14982, 15535, 15435, 15302, 15673, 15673, 15793, 15389, 15526, 14982, 15535, 15435, 15888, 15302, 15673, 15673, 15793, 15389, 15526, 14982, 15535, 15435, 15888, 15640, 15527, 15527, 15072, 15527, 15072, 15684, 15527, 15072, 15684, 15537, 15527, 15072, 15684, 15537, 15754, 15527, 15072, 15684, 15537, 15754, 15225, 15527, 15072, 15684, 15537, 15754, 15225, 15409, 15527, 15072, 15684, 15537, 15754, 15225, 15409, 15151, 15527, 15072, 15684, 15537, 15754, 15225, 15409, 15151, 15306, 15527, 15072, 15684, 15537, 15754, 15225, 15409, 15151, 15306, 15414, 15472, 15472, 15515, 15472, 15515, 15677, 15472, 15515, 15677, 15932, 15472, 15515, 15677, 15932, 15495, 15472, 15515, 15677, 15932, 15495, 15297, 15472, 15515, 15677, 15932, 15495, 15297, 15467, 15472, 15515, 15677, 15932, 15495, 15297, 15467, 15389, 15472, 15515, 15677, 15932, 15495, 15297, 15467, 15389, 15211, 15472, 15515, 15677, 15932, 15495, 15297, 15467, 15389, 15211, 15312, 15422, 15422, 15469, 15422, 15469, 15455, 15422, 15469, 15455, 15420, 15422, 15469, 15455, 15420, 15643, 15422, 15469, 15455, 15420, 15643, 15348, 15422, 15469, 15455, 15420, 15643, 15348, 15366, 15422, 15469, 15455, 15420, 15643, 15348, 15366, 15477, 15422, 15469, 15455, 15420, 15643, 15348, 15366, 15477, 15161, 15422, 15469, 15455, 15420, 15643, 15348, 15366, 15477, 15161, 15451, 15664, 15664, 15632, 15664, 15632, 15716, 15664, 15632, 15716, 15486, 15664, 15632, 15716, 15486, 15427, 15664, 15632, 15716, 15486, 15427, 15261, 15664, 15632, 15716, 15486, 15427, 15261, 15453, 15664, 15632, 15716, 15486, 15427, 15261, 15453, 15322, 15664, 15632, 15716, 15486, 15427, 15261, 15453, 15322, 15901, 15664, 15632, 15716, 15486, 15427, 15261, 15453, 15322, 15901, 15419, 15521, 15521, 15532, 15521, 15532, 15244, 15521, 15532, 15244, 15434, 15521, 15532, 15244, 15434, 15542, 15521, 15532, 15244, 15434, 15542, 15507, 15521, 15532, 15244, 15434, 15542, 15507, 15251, 15521, 15532, 15244, 15434, 15542, 15507, 15251, 15341, 15521, 15532, 15244, 15434, 15542, 15507, 15251, 15341, 15096, 15521, 15532, 15244, 15434, 15542, 15507, 15251, 15341, 15096, 15601, 15444, 15444, 15614, 15444, 15614, 15291, 15444, 15614, 15291, 15463, 15444, 15614, 15291, 15463, 15520, 15444, 15614, 15291, 15463, 15520, 15778, 15444, 15614, 15291, 15463, 15520, 15778, 15616, 15444, 15614, 15291, 15463, 15520, 15778, 15616, 15865, 15444, 15614, 15291, 15463, 15520, 15778, 15616, 15865, 15892, 15444, 15614, 15291, 15463, 15520, 15778, 15616, 15865, 15892, 15629, 15765, 15765, 15715, 15765, 15715, 15264, 15765, 15715, 15264, 15458, 15765, 15715, 15264, 15458, 15458, 15765, 15715, 15264, 15458, 15458, 15482, 15765, 15715, 15264, 15458, 15458, 15482, 15637, 15765, 15715, 15264, 15458, 15458, 15482, 15637, 15550, 15765, 15715, 15264, 15458, 15458, 15482, 15637, 15550, 15344, 15765, 15715, 15264, 15458, 15458, 15482, 15637, 15550, 15344, 15774, 15405, 15405, 15456, 15405, 15456, 15533, 15405, 15456, 15533, 15230, 15405, 15456, 15533, 15230, 15537, 15405, 15456, 15533, 15230, 15537, 15429, 15405, 15456, 15533, 15230, 15537, 15429, 15713, 15405, 15456, 15533, 15230, 15537, 15429, 15713, 15843, 15405, 15456, 15533, 15230, 15537, 15429, 15713, 15843, 15828, 15405, 15456, 15533, 15230, 15537, 15429, 15713, 15843, 15828, 15007, 15607, 15607, 15396, 15607, 15396, 15084, 15607, 15396, 15084, 15547, 15607, 15396, 15084, 15547, 15484, 15607, 15396, 15084, 15547, 15484, 15546, 15607, 15396, 15084, 15547, 15484, 15546, 15657, 15607, 15396, 15084, 15547, 15484, 15546, 15657, 15747, 15607, 15396, 15084, 15547, 15484, 15546, 15657, 15747, 15011, };static struct simple_udp_connection unicast_connection;
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