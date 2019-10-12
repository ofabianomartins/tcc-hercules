#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {10400, 10400, 9246, 10400, 9246, 7800, 10400, 9246, 7800, 10907, 10400, 9246, 7800, 10907, 10463, 10400, 9246, 7800, 10907, 10463, 7308, 10400, 9246, 7800, 10907, 10463, 7308, 8538, 10400, 9246, 7800, 10907, 10463, 7308, 8538, 7152, 10400, 9246, 7800, 10907, 10463, 7308, 8538, 7152, 6820, 10400, 9246, 7800, 10907, 10463, 7308, 8538, 7152, 6820, 12390, 10400, 9246, 7800, 10907, 10463, 7308, 8538, 7152, 6820, 12390, 8574, 10406, 10406, 9780, 10406, 9780, 9308, 10406, 9780, 9308, 7321, 10406, 9780, 9308, 7321, 7382, 10406, 9780, 9308, 7321, 7382, 9995, 10406, 9780, 9308, 7321, 7382, 9995, 11231, 10406, 9780, 9308, 7321, 7382, 9995, 11231, 5891, 10406, 9780, 9308, 7321, 7382, 9995, 11231, 5891, 7510, 10406, 9780, 9308, 7321, 7382, 9995, 11231, 5891, 7510, 8836, 11081, 11081, 10168, 11081, 10168, 10661, 11081, 10168, 10661, 9633, 11081, 10168, 10661, 9633, 7485, 11081, 10168, 10661, 9633, 7485, 8954, 11081, 10168, 10661, 9633, 7485, 8954, 11481, 11081, 10168, 10661, 9633, 7485, 8954, 11481, 9512, 11081, 10168, 10661, 9633, 7485, 8954, 11481, 9512, 6998, 11081, 10168, 10661, 9633, 7485, 8954, 11481, 9512, 6998, 11747, 10023, 10023, 10511, 10023, 10511, 12743, 10023, 10511, 12743, 8213, 10023, 10511, 12743, 8213, 7977, 10023, 10511, 12743, 8213, 7977, 4688, 10023, 10511, 12743, 8213, 7977, 4688, 9464, 10023, 10511, 12743, 8213, 7977, 4688, 9464, 10291, 10023, 10511, 12743, 8213, 7977, 4688, 9464, 10291, 9159, 10023, 10511, 12743, 8213, 7977, 4688, 9464, 10291, 9159, 9224, 10884, 10884, 9957, 10884, 9957, 9940, 10884, 9957, 9940, 9071, 10884, 9957, 9940, 9071, 9118, 10884, 9957, 9940, 9071, 9118, 11073, 10884, 9957, 9940, 9071, 9118, 11073, 9030, 10884, 9957, 9940, 9071, 9118, 11073, 9030, 8442, 10884, 9957, 9940, 9071, 9118, 11073, 9030, 8442, 8206, 10884, 9957, 9940, 9071, 9118, 11073, 9030, 8442, 8206, 8982, 9542, 9542, 10121, 9542, 10121, 9436, 9542, 10121, 9436, 8603, 9542, 10121, 9436, 8603, 6261, 9542, 10121, 9436, 8603, 6261, 10314, 9542, 10121, 9436, 8603, 6261, 10314, 7413, 9542, 10121, 9436, 8603, 6261, 10314, 7413, 8909, 9542, 10121, 9436, 8603, 6261, 10314, 7413, 8909, 11346, 9542, 10121, 9436, 8603, 6261, 10314, 7413, 8909, 11346, 9651, 7735, 7735, 10466, 7735, 10466, 8717, 7735, 10466, 8717, 8331, 7735, 10466, 8717, 8331, 11440, 7735, 10466, 8717, 8331, 11440, 10418, 7735, 10466, 8717, 8331, 11440, 10418, 14937, 7735, 10466, 8717, 8331, 11440, 10418, 14937, 7254, 7735, 10466, 8717, 8331, 11440, 10418, 14937, 7254, 9218, 7735, 10466, 8717, 8331, 11440, 10418, 14937, 7254, 9218, 9988, 11590, 11590, 8747, 11590, 8747, 12067, 11590, 8747, 12067, 9507, 11590, 8747, 12067, 9507, 7923, 11590, 8747, 12067, 9507, 7923, 8960, 11590, 8747, 12067, 9507, 7923, 8960, 9168, 11590, 8747, 12067, 9507, 7923, 8960, 9168, 9018, 11590, 8747, 12067, 9507, 7923, 8960, 9168, 9018, 8184, 11590, 8747, 12067, 9507, 7923, 8960, 9168, 9018, 8184, 8877, 11019, 11019, 10857, 11019, 10857, 12679, 11019, 10857, 12679, 7768, 11019, 10857, 12679, 7768, 7379, 11019, 10857, 12679, 7768, 7379, 13071, 11019, 10857, 12679, 7768, 7379, 13071, 9965, 11019, 10857, 12679, 7768, 7379, 13071, 9965, 9994, 11019, 10857, 12679, 7768, 7379, 13071, 9965, 9994, 7728, 11019, 10857, 12679, 7768, 7379, 13071, 9965, 9994, 7728, 9988, 9379, 9379, 9317, 9379, 9317, 11403, 9379, 9317, 11403, 7814, 9379, 9317, 11403, 7814, 9545, 9379, 9317, 11403, 7814, 9545, 6098, 9379, 9317, 11403, 7814, 9545, 6098, 10030, 9379, 9317, 11403, 7814, 9545, 6098, 10030, 7246, 9379, 9317, 11403, 7814, 9545, 6098, 10030, 7246, 12482, };static struct simple_udp_connection unicast_connection;
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