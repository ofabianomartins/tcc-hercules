#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-debug.h"
#include "sys/node-id.h"
#include "simple-udp.h"
#include "powertrace.h"
#include "servreg-hack.h"
#include <stdio.h>
#include <string.h>
#define UDP_PORT 1234
#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)
#define SEND_TIME    (random_rand() % (SEND_INTERVAL))
#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)
#define DATA_SIZE 100
int samples[100] = {
8414, 8337, 8290, 8530, 8665, 8497, 8312, 8439, 8126, 8665, 8638, 8351, 8519, 8543, 8479, 8646, 8796, 8725, 8703, 8491, 8627, 8659, 8492, 8662, 8266, 8517, 8493, 8477, 8561, 8553, 8311, 8475, 8423, 8639, 8417, 8570, 8343, 8808, 8301, 8564, 8365, 8564, 8691, 8638, 8298, 8480, 8338, 8393, 8292, 8584, 8806, 8975, 8327, 8716, 8570, 8656, 8961, 8779, 8598, 8628, 8608, 8652, 8435, 8637, 8312, 8361, 8196, 8453, 8745, 8190, 8317, 8947, 8551, 8533, 8661, 8432, 8412, 9109, 8521, 8679, 8158, 8218, 8200, 9000, 8321, 8587, 8438, 8625, 8537, 8799, 8320, 8881, 8204, 8650, 8574, 8480, 8175, 8897, 8789, 8513, 
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
  powertrace_start(POWERTRACE_INTERVAL);  servreg_hack_init();
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
      printf("Sending unicast to ");
      uip_debug_ipaddr_print(addr);
      printf(": %s \n", buf);
    } else {
      printf("Service %d not found\n", SERVICE_ID);
    }
  }
  PROCESS_END();
}
