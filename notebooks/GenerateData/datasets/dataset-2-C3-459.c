#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {15652, 15652, 15062, 15652, 15062, 15582, 15652, 15062, 15582, 15649, 15652, 15062, 15582, 15649, 15425, 15652, 15062, 15582, 15649, 15425, 15550, 15652, 15062, 15582, 15649, 15425, 15550, 15670, 15652, 15062, 15582, 15649, 15425, 15550, 15670, 15841, 15652, 15062, 15582, 15649, 15425, 15550, 15670, 15841, 15551, 15652, 15062, 15582, 15649, 15425, 15550, 15670, 15841, 15551, 15863, 15652, 15062, 15582, 15649, 15425, 15550, 15670, 15841, 15551, 15863, 15168, 15731, 15731, 15562, 15731, 15562, 15338, 15731, 15562, 15338, 15241, 15731, 15562, 15338, 15241, 15825, 15731, 15562, 15338, 15241, 15825, 15059, 15731, 15562, 15338, 15241, 15825, 15059, 15458, 15731, 15562, 15338, 15241, 15825, 15059, 15458, 15844, 15731, 15562, 15338, 15241, 15825, 15059, 15458, 15844, 15296, 15731, 15562, 15338, 15241, 15825, 15059, 15458, 15844, 15296, 15637, 15294, 15294, 15532, 15294, 15532, 15680, 15294, 15532, 15680, 15254, 15294, 15532, 15680, 15254, 15115, 15294, 15532, 15680, 15254, 15115, 15763, 15294, 15532, 15680, 15254, 15115, 15763, 15699, 15294, 15532, 15680, 15254, 15115, 15763, 15699, 15539, 15294, 15532, 15680, 15254, 15115, 15763, 15699, 15539, 15714, 15294, 15532, 15680, 15254, 15115, 15763, 15699, 15539, 15714, 15402, 15752, 15752, 16043, 15752, 16043, 15726, 15752, 16043, 15726, 15413, 15752, 16043, 15726, 15413, 15661, 15752, 16043, 15726, 15413, 15661, 15227, 15752, 16043, 15726, 15413, 15661, 15227, 15398, 15752, 16043, 15726, 15413, 15661, 15227, 15398, 15487, 15752, 16043, 15726, 15413, 15661, 15227, 15398, 15487, 15660, 15752, 16043, 15726, 15413, 15661, 15227, 15398, 15487, 15660, 15353, 15789, 15789, 15534, 15789, 15534, 16033, 15789, 15534, 16033, 15318, 15789, 15534, 16033, 15318, 15545, 15789, 15534, 16033, 15318, 15545, 15739, 15789, 15534, 16033, 15318, 15545, 15739, 15431, 15789, 15534, 16033, 15318, 15545, 15739, 15431, 15364, 15789, 15534, 16033, 15318, 15545, 15739, 15431, 15364, 15336, 15789, 15534, 16033, 15318, 15545, 15739, 15431, 15364, 15336, 15908, 15557, 15557, 15458, 15557, 15458, 15413, 15557, 15458, 15413, 15151, 15557, 15458, 15413, 15151, 15302, 15557, 15458, 15413, 15151, 15302, 16019, 15557, 15458, 15413, 15151, 15302, 16019, 15613, 15557, 15458, 15413, 15151, 15302, 16019, 15613, 15537, 15557, 15458, 15413, 15151, 15302, 16019, 15613, 15537, 15793, 15557, 15458, 15413, 15151, 15302, 16019, 15613, 15537, 15793, 15432, 15516, 15516, 15283, 15516, 15283, 15812, 15516, 15283, 15812, 15693, 15516, 15283, 15812, 15693, 15643, 15516, 15283, 15812, 15693, 15643, 15764, 15516, 15283, 15812, 15693, 15643, 15764, 15432, 15516, 15283, 15812, 15693, 15643, 15764, 15432, 15496, 15516, 15283, 15812, 15693, 15643, 15764, 15432, 15496, 15684, 15516, 15283, 15812, 15693, 15643, 15764, 15432, 15496, 15684, 15555, 15968, 15968, 15422, 15968, 15422, 15216, 15968, 15422, 15216, 15719, 15968, 15422, 15216, 15719, 15293, 15968, 15422, 15216, 15719, 15293, 15359, 15968, 15422, 15216, 15719, 15293, 15359, 15428, 15968, 15422, 15216, 15719, 15293, 15359, 15428, 15679, 15968, 15422, 15216, 15719, 15293, 15359, 15428, 15679, 15435, 15968, 15422, 15216, 15719, 15293, 15359, 15428, 15679, 15435, 15350, 15742, 15742, 15644, 15742, 15644, 15235, 15742, 15644, 15235, 15146, 15742, 15644, 15235, 15146, 15306, 15742, 15644, 15235, 15146, 15306, 15135, 15742, 15644, 15235, 15146, 15306, 15135, 15545, 15742, 15644, 15235, 15146, 15306, 15135, 15545, 15599, 15742, 15644, 15235, 15146, 15306, 15135, 15545, 15599, 15473, 15742, 15644, 15235, 15146, 15306, 15135, 15545, 15599, 15473, 15930, 15492, 15492, 15521, 15492, 15521, 15492, 15492, 15521, 15492, 15104, 15492, 15521, 15492, 15104, 15600, 15492, 15521, 15492, 15104, 15600, 15330, 15492, 15521, 15492, 15104, 15600, 15330, 15324, 15492, 15521, 15492, 15104, 15600, 15330, 15324, 15608, 15492, 15521, 15492, 15104, 15600, 15330, 15324, 15608, 15633, };static struct simple_udp_connection unicast_connection;
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