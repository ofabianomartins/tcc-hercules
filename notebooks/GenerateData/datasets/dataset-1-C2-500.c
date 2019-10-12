#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8424, 8424, 8533, 8424, 8533, 8636, 8424, 8533, 8636, 8884, 8424, 8533, 8636, 8884, 8460, 8424, 8533, 8636, 8884, 8460, 8348, 8424, 8533, 8636, 8884, 8460, 8348, 8082, 8424, 8533, 8636, 8884, 8460, 8348, 8082, 8492, 8424, 8533, 8636, 8884, 8460, 8348, 8082, 8492, 8256, 8424, 8533, 8636, 8884, 8460, 8348, 8082, 8492, 8256, 8214, 8424, 8533, 8636, 8884, 8460, 8348, 8082, 8492, 8256, 8214, 8465, 8732, 8732, 8287, 8732, 8287, 8218, 8732, 8287, 8218, 8667, 8732, 8287, 8218, 8667, 8480, 8732, 8287, 8218, 8667, 8480, 8152, 8732, 8287, 8218, 8667, 8480, 8152, 8438, 8732, 8287, 8218, 8667, 8480, 8152, 8438, 8483, 8732, 8287, 8218, 8667, 8480, 8152, 8438, 8483, 8666, 8732, 8287, 8218, 8667, 8480, 8152, 8438, 8483, 8666, 8877, 8588, 8588, 8587, 8588, 8587, 8714, 8588, 8587, 8714, 8580, 8588, 8587, 8714, 8580, 8526, 8588, 8587, 8714, 8580, 8526, 8887, 8588, 8587, 8714, 8580, 8526, 8887, 8700, 8588, 8587, 8714, 8580, 8526, 8887, 8700, 8490, 8588, 8587, 8714, 8580, 8526, 8887, 8700, 8490, 8730, 8588, 8587, 8714, 8580, 8526, 8887, 8700, 8490, 8730, 8503, 8184, 8184, 8460, 8184, 8460, 8455, 8184, 8460, 8455, 8446, 8184, 8460, 8455, 8446, 8417, 8184, 8460, 8455, 8446, 8417, 8709, 8184, 8460, 8455, 8446, 8417, 8709, 8832, 8184, 8460, 8455, 8446, 8417, 8709, 8832, 8620, 8184, 8460, 8455, 8446, 8417, 8709, 8832, 8620, 8546, 8184, 8460, 8455, 8446, 8417, 8709, 8832, 8620, 8546, 8573, 8400, 8400, 8813, 8400, 8813, 8593, 8400, 8813, 8593, 8609, 8400, 8813, 8593, 8609, 8433, 8400, 8813, 8593, 8609, 8433, 8307, 8400, 8813, 8593, 8609, 8433, 8307, 8329, 8400, 8813, 8593, 8609, 8433, 8307, 8329, 8835, 8400, 8813, 8593, 8609, 8433, 8307, 8329, 8835, 8407, 8400, 8813, 8593, 8609, 8433, 8307, 8329, 8835, 8407, 8550, 8652, 8652, 8556, 8652, 8556, 8406, 8652, 8556, 8406, 8347, 8652, 8556, 8406, 8347, 8745, 8652, 8556, 8406, 8347, 8745, 8504, 8652, 8556, 8406, 8347, 8745, 8504, 8522, 8652, 8556, 8406, 8347, 8745, 8504, 8522, 8627, 8652, 8556, 8406, 8347, 8745, 8504, 8522, 8627, 8368, 8652, 8556, 8406, 8347, 8745, 8504, 8522, 8627, 8368, 8613, 8689, 8689, 8711, 8689, 8711, 8620, 8689, 8711, 8620, 8501, 8689, 8711, 8620, 8501, 8511, 8689, 8711, 8620, 8501, 8511, 8679, 8689, 8711, 8620, 8501, 8511, 8679, 8692, 8689, 8711, 8620, 8501, 8511, 8679, 8692, 8507, 8689, 8711, 8620, 8501, 8511, 8679, 8692, 8507, 8752, 8689, 8711, 8620, 8501, 8511, 8679, 8692, 8507, 8752, 8344, 8652, 8652, 8578, 8652, 8578, 8521, 8652, 8578, 8521, 8533, 8652, 8578, 8521, 8533, 8481, 8652, 8578, 8521, 8533, 8481, 8523, 8652, 8578, 8521, 8533, 8481, 8523, 8719, 8652, 8578, 8521, 8533, 8481, 8523, 8719, 8289, 8652, 8578, 8521, 8533, 8481, 8523, 8719, 8289, 8651, 8652, 8578, 8521, 8533, 8481, 8523, 8719, 8289, 8651, 8769, 8336, 8336, 8698, 8336, 8698, 8598, 8336, 8698, 8598, 8687, 8336, 8698, 8598, 8687, 8727, 8336, 8698, 8598, 8687, 8727, 8641, 8336, 8698, 8598, 8687, 8727, 8641, 8299, 8336, 8698, 8598, 8687, 8727, 8641, 8299, 8137, 8336, 8698, 8598, 8687, 8727, 8641, 8299, 8137, 8462, 8336, 8698, 8598, 8687, 8727, 8641, 8299, 8137, 8462, 8700, 8490, 8490, 8296, 8490, 8296, 8585, 8490, 8296, 8585, 8448, 8490, 8296, 8585, 8448, 8518, 8490, 8296, 8585, 8448, 8518, 8301, 8490, 8296, 8585, 8448, 8518, 8301, 8438, 8490, 8296, 8585, 8448, 8518, 8301, 8438, 8400, 8490, 8296, 8585, 8448, 8518, 8301, 8438, 8400, 8341, };static struct simple_udp_connection unicast_connection;
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