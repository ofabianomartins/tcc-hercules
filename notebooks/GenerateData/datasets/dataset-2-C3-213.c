#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15526, 15526, 15618, 15526, 15618, 15658, 15526, 15618, 15658, 15687, 15526, 15618, 15658, 15687, 15304, 15526, 15618, 15658, 15687, 15304, 15430, 15526, 15618, 15658, 15687, 15304, 15430, 15377, 15526, 15618, 15658, 15687, 15304, 15430, 15377, 15447, 15526, 15618, 15658, 15687, 15304, 15430, 15377, 15447, 15411, 15526, 15618, 15658, 15687, 15304, 15430, 15377, 15447, 15411, 15697, 15526, 15618, 15658, 15687, 15304, 15430, 15377, 15447, 15411, 15697, 15462, 15359, 15359, 15682, 15359, 15682, 15806, 15359, 15682, 15806, 15874, 15359, 15682, 15806, 15874, 15343, 15359, 15682, 15806, 15874, 15343, 15570, 15359, 15682, 15806, 15874, 15343, 15570, 15638, 15359, 15682, 15806, 15874, 15343, 15570, 15638, 15619, 15359, 15682, 15806, 15874, 15343, 15570, 15638, 15619, 15441, 15359, 15682, 15806, 15874, 15343, 15570, 15638, 15619, 15441, 15258, 15608, 15608, 15358, 15608, 15358, 15547, 15608, 15358, 15547, 15286, 15608, 15358, 15547, 15286, 15178, 15608, 15358, 15547, 15286, 15178, 15466, 15608, 15358, 15547, 15286, 15178, 15466, 15379, 15608, 15358, 15547, 15286, 15178, 15466, 15379, 15432, 15608, 15358, 15547, 15286, 15178, 15466, 15379, 15432, 15570, 15608, 15358, 15547, 15286, 15178, 15466, 15379, 15432, 15570, 15437, 15558, 15558, 15421, 15558, 15421, 15677, 15558, 15421, 15677, 15643, 15558, 15421, 15677, 15643, 15450, 15558, 15421, 15677, 15643, 15450, 15650, 15558, 15421, 15677, 15643, 15450, 15650, 15517, 15558, 15421, 15677, 15643, 15450, 15650, 15517, 15662, 15558, 15421, 15677, 15643, 15450, 15650, 15517, 15662, 15602, 15558, 15421, 15677, 15643, 15450, 15650, 15517, 15662, 15602, 15368, 15657, 15657, 15144, 15657, 15144, 15444, 15657, 15144, 15444, 15429, 15657, 15144, 15444, 15429, 15660, 15657, 15144, 15444, 15429, 15660, 15189, 15657, 15144, 15444, 15429, 15660, 15189, 15663, 15657, 15144, 15444, 15429, 15660, 15189, 15663, 15336, 15657, 15144, 15444, 15429, 15660, 15189, 15663, 15336, 15272, 15657, 15144, 15444, 15429, 15660, 15189, 15663, 15336, 15272, 15762, 15545, 15545, 15469, 15545, 15469, 15679, 15545, 15469, 15679, 15940, 15545, 15469, 15679, 15940, 15612, 15545, 15469, 15679, 15940, 15612, 15310, 15545, 15469, 15679, 15940, 15612, 15310, 15365, 15545, 15469, 15679, 15940, 15612, 15310, 15365, 15426, 15545, 15469, 15679, 15940, 15612, 15310, 15365, 15426, 15437, 15545, 15469, 15679, 15940, 15612, 15310, 15365, 15426, 15437, 15438, 15410, 15410, 15384, 15410, 15384, 15330, 15410, 15384, 15330, 15661, 15410, 15384, 15330, 15661, 15606, 15410, 15384, 15330, 15661, 15606, 15601, 15410, 15384, 15330, 15661, 15606, 15601, 15255, 15410, 15384, 15330, 15661, 15606, 15601, 15255, 15422, 15410, 15384, 15330, 15661, 15606, 15601, 15255, 15422, 15106, 15410, 15384, 15330, 15661, 15606, 15601, 15255, 15422, 15106, 15596, 15270, 15270, 15575, 15270, 15575, 15680, 15270, 15575, 15680, 15452, 15270, 15575, 15680, 15452, 15582, 15270, 15575, 15680, 15452, 15582, 15334, 15270, 15575, 15680, 15452, 15582, 15334, 15945, 15270, 15575, 15680, 15452, 15582, 15334, 15945, 15573, 15270, 15575, 15680, 15452, 15582, 15334, 15945, 15573, 15175, 15270, 15575, 15680, 15452, 15582, 15334, 15945, 15573, 15175, 15476, 15567, 15567, 15314, 15567, 15314, 15671, 15567, 15314, 15671, 15848, 15567, 15314, 15671, 15848, 15728, 15567, 15314, 15671, 15848, 15728, 15341, 15567, 15314, 15671, 15848, 15728, 15341, 15308, 15567, 15314, 15671, 15848, 15728, 15341, 15308, 15551, 15567, 15314, 15671, 15848, 15728, 15341, 15308, 15551, 15630, 15567, 15314, 15671, 15848, 15728, 15341, 15308, 15551, 15630, 15203, 15498, 15498, 15610, 15498, 15610, 15060, 15498, 15610, 15060, 15304, 15498, 15610, 15060, 15304, 15400, 15498, 15610, 15060, 15304, 15400, 15026, 15498, 15610, 15060, 15304, 15400, 15026, 15616, 15498, 15610, 15060, 15304, 15400, 15026, 15616, 15160, 15498, 15610, 15060, 15304, 15400, 15026, 15616, 15160, 15233, };static struct simple_udp_connection unicast_connection;
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