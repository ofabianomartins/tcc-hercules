#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "sys/node-id.h"
#include "simple-udp.h"
#include "servreg-hack.h"
#include <stdio.h>
#include <string.h>
#define UDP_PORT 1234
#define SERVICE_ID 190
#define SEND_INTERVAL    (10 * CLOCK_SECOND)
#define SEND_TIME    (random_rand() % (SEND_INTERVAL))
#define DATA_SIZE 100

int samples[100] = {
11163, 11118, 17328, 7998, 8465, 10975, 10641, 11048, 9259, 11242, 9726, 
7748, 9426, 11038, 11498, 9004, 6263, 9580, 10084, 12831, 11690, 
6912, 9574, 9588, 11848, 9897, 6006, 10269, 7213, 8233, 7981, 
8899, 8405, 8678, 11261, 11031, 7485, 9849, 11409, 8695, 5923, 
9915, 7004, 9205, 12417, 11842, 8779, 6275, 10612, 11299, 8327, 
11840, 9505, 6922, 10066, 6599, 10968, 6524, 9028, 7482, 8509, 
10538, 7731, 9253, 8926, 12760, 12244, 8319, 9006, 10177, 9561, 
8726, 9244, 7828, 11022, 9349, 9731, 8177, 9092, 7357, 8213, 
9119, 11341, 9491, 8297, 8546, 8913, 11871, 8670, 10149, 9278, 
8254, 9898, 9126, 9116, 7141, 9851, 12019, 8690, 6784, 
};

static struct simple_udp_connection unicast_connection;
PROCESS(unicast_sender_process, "Unicast sender example process");
AUTOSTART_PROCESSES(&unicast_sender_process);

static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  printf("Data received on port %d from port %d with length %d\n",
         receiver_port, sender_port, datalen);
}

static void
set_global_address(void)
{
  uip_ipaddr_t ipaddr;
  int i;
  uint8_t state;
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
  printf("IPv6 addresses: ");  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }
}

PROCESS_THREAD(unicast_sender_process, ev, data)
{
  static struct etimer periodic_timer;
  static struct etimer send_timer;
  uip_ipaddr_t *addr;
  PROCESS_BEGIN();
  servreg_hack_init();
  set_global_address();
  simple_udp_register(&unicast_connection, UDP_PORT,
                      NULL, UDP_PORT, receiver);
  etimer_set(&periodic_timer, SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
    etimer_set(&send_timer, SEND_TIME);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
    addr = servreg_hack_lookup(SERVICE_ID);
    if(addr != NULL) {
      char buf[20];

      sprintf(buf, "c:1:%d", samples[random_rand() % DATA_SIZE]);
      simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, addr);
          } else {
      printf("Service %d not found\n", SERVICE_ID);
    }
  }
  PROCESS_END();
}
