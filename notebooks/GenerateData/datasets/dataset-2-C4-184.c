#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15501, 15501, 15346, 15501, 15346, 15383, 15501, 15346, 15383, 15468, 15501, 15346, 15383, 15468, 15879, 15501, 15346, 15383, 15468, 15879, 15851, 15501, 15346, 15383, 15468, 15879, 15851, 15396, 15501, 15346, 15383, 15468, 15879, 15851, 15396, 15721, 15501, 15346, 15383, 15468, 15879, 15851, 15396, 15721, 15115, 15501, 15346, 15383, 15468, 15879, 15851, 15396, 15721, 15115, 15440, 15501, 15346, 15383, 15468, 15879, 15851, 15396, 15721, 15115, 15440, 15638, 15362, 15362, 15632, 15362, 15632, 15671, 15362, 15632, 15671, 15440, 15362, 15632, 15671, 15440, 15245, 15362, 15632, 15671, 15440, 15245, 15508, 15362, 15632, 15671, 15440, 15245, 15508, 15535, 15362, 15632, 15671, 15440, 15245, 15508, 15535, 15542, 15362, 15632, 15671, 15440, 15245, 15508, 15535, 15542, 15536, 15362, 15632, 15671, 15440, 15245, 15508, 15535, 15542, 15536, 15395, 16013, 16013, 15583, 16013, 15583, 15257, 16013, 15583, 15257, 15361, 16013, 15583, 15257, 15361, 15684, 16013, 15583, 15257, 15361, 15684, 15597, 16013, 15583, 15257, 15361, 15684, 15597, 15548, 16013, 15583, 15257, 15361, 15684, 15597, 15548, 15379, 16013, 15583, 15257, 15361, 15684, 15597, 15548, 15379, 15873, 16013, 15583, 15257, 15361, 15684, 15597, 15548, 15379, 15873, 15675, 15589, 15589, 15428, 15589, 15428, 15889, 15589, 15428, 15889, 15592, 15589, 15428, 15889, 15592, 15619, 15589, 15428, 15889, 15592, 15619, 15487, 15589, 15428, 15889, 15592, 15619, 15487, 15564, 15589, 15428, 15889, 15592, 15619, 15487, 15564, 15416, 15589, 15428, 15889, 15592, 15619, 15487, 15564, 15416, 15495, 15589, 15428, 15889, 15592, 15619, 15487, 15564, 15416, 15495, 15563, 15433, 15433, 15476, 15433, 15476, 15775, 15433, 15476, 15775, 15476, 15433, 15476, 15775, 15476, 15518, 15433, 15476, 15775, 15476, 15518, 15202, 15433, 15476, 15775, 15476, 15518, 15202, 15768, 15433, 15476, 15775, 15476, 15518, 15202, 15768, 15420, 15433, 15476, 15775, 15476, 15518, 15202, 15768, 15420, 15628, 15433, 15476, 15775, 15476, 15518, 15202, 15768, 15420, 15628, 15657, 15617, 15617, 15573, 15617, 15573, 15630, 15617, 15573, 15630, 15293, 15617, 15573, 15630, 15293, 15737, 15617, 15573, 15630, 15293, 15737, 15755, 15617, 15573, 15630, 15293, 15737, 15755, 15179, 15617, 15573, 15630, 15293, 15737, 15755, 15179, 15915, 15617, 15573, 15630, 15293, 15737, 15755, 15179, 15915, 15638, 15617, 15573, 15630, 15293, 15737, 15755, 15179, 15915, 15638, 15587, 15809, 15809, 15291, 15809, 15291, 15245, 15809, 15291, 15245, 15197, 15809, 15291, 15245, 15197, 15632, 15809, 15291, 15245, 15197, 15632, 15708, 15809, 15291, 15245, 15197, 15632, 15708, 15439, 15809, 15291, 15245, 15197, 15632, 15708, 15439, 15319, 15809, 15291, 15245, 15197, 15632, 15708, 15439, 15319, 15398, 15809, 15291, 15245, 15197, 15632, 15708, 15439, 15319, 15398, 15370, 15560, 15560, 15727, 15560, 15727, 15143, 15560, 15727, 15143, 15456, 15560, 15727, 15143, 15456, 15419, 15560, 15727, 15143, 15456, 15419, 15560, 15560, 15727, 15143, 15456, 15419, 15560, 15165, 15560, 15727, 15143, 15456, 15419, 15560, 15165, 15381, 15560, 15727, 15143, 15456, 15419, 15560, 15165, 15381, 15157, 15560, 15727, 15143, 15456, 15419, 15560, 15165, 15381, 15157, 15152, 15133, 15133, 15737, 15133, 15737, 15252, 15133, 15737, 15252, 15714, 15133, 15737, 15252, 15714, 15209, 15133, 15737, 15252, 15714, 15209, 15516, 15133, 15737, 15252, 15714, 15209, 15516, 15357, 15133, 15737, 15252, 15714, 15209, 15516, 15357, 15528, 15133, 15737, 15252, 15714, 15209, 15516, 15357, 15528, 15740, 15133, 15737, 15252, 15714, 15209, 15516, 15357, 15528, 15740, 15405, 15327, 15327, 15535, 15327, 15535, 15866, 15327, 15535, 15866, 15360, 15327, 15535, 15866, 15360, 15482, 15327, 15535, 15866, 15360, 15482, 15434, 15327, 15535, 15866, 15360, 15482, 15434, 15672, 15327, 15535, 15866, 15360, 15482, 15434, 15672, 15585, 15327, 15535, 15866, 15360, 15482, 15434, 15672, 15585, 15177, };static struct simple_udp_connection unicast_connection;
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