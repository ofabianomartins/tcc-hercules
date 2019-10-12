#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8362, 8362, 8233, 8362, 8233, 8610, 8362, 8233, 8610, 8345, 8362, 8233, 8610, 8345, 8513, 8362, 8233, 8610, 8345, 8513, 8600, 8362, 8233, 8610, 8345, 8513, 8600, 8572, 8362, 8233, 8610, 8345, 8513, 8600, 8572, 8618, 8362, 8233, 8610, 8345, 8513, 8600, 8572, 8618, 8635, 8362, 8233, 8610, 8345, 8513, 8600, 8572, 8618, 8635, 8729, 8362, 8233, 8610, 8345, 8513, 8600, 8572, 8618, 8635, 8729, 8394, 8523, 8523, 8367, 8523, 8367, 8862, 8523, 8367, 8862, 8215, 8523, 8367, 8862, 8215, 7938, 8523, 8367, 8862, 8215, 7938, 8331, 8523, 8367, 8862, 8215, 7938, 8331, 8388, 8523, 8367, 8862, 8215, 7938, 8331, 8388, 8715, 8523, 8367, 8862, 8215, 7938, 8331, 8388, 8715, 8276, 8523, 8367, 8862, 8215, 7938, 8331, 8388, 8715, 8276, 8563, 8103, 8103, 8608, 8103, 8608, 7917, 8103, 8608, 7917, 8717, 8103, 8608, 7917, 8717, 8215, 8103, 8608, 7917, 8717, 8215, 8416, 8103, 8608, 7917, 8717, 8215, 8416, 8785, 8103, 8608, 7917, 8717, 8215, 8416, 8785, 8299, 8103, 8608, 7917, 8717, 8215, 8416, 8785, 8299, 8713, 8103, 8608, 7917, 8717, 8215, 8416, 8785, 8299, 8713, 8546, 8639, 8639, 8634, 8639, 8634, 8070, 8639, 8634, 8070, 8377, 8639, 8634, 8070, 8377, 8719, 8639, 8634, 8070, 8377, 8719, 8557, 8639, 8634, 8070, 8377, 8719, 8557, 8145, 8639, 8634, 8070, 8377, 8719, 8557, 8145, 8344, 8639, 8634, 8070, 8377, 8719, 8557, 8145, 8344, 8626, 8639, 8634, 8070, 8377, 8719, 8557, 8145, 8344, 8626, 8527, 8731, 8731, 8548, 8731, 8548, 8506, 8731, 8548, 8506, 8564, 8731, 8548, 8506, 8564, 8102, 8731, 8548, 8506, 8564, 8102, 8573, 8731, 8548, 8506, 8564, 8102, 8573, 8532, 8731, 8548, 8506, 8564, 8102, 8573, 8532, 8298, 8731, 8548, 8506, 8564, 8102, 8573, 8532, 8298, 8572, 8731, 8548, 8506, 8564, 8102, 8573, 8532, 8298, 8572, 8444, 8567, 8567, 8450, 8567, 8450, 8413, 8567, 8450, 8413, 8295, 8567, 8450, 8413, 8295, 8329, 8567, 8450, 8413, 8295, 8329, 8270, 8567, 8450, 8413, 8295, 8329, 8270, 8103, 8567, 8450, 8413, 8295, 8329, 8270, 8103, 8531, 8567, 8450, 8413, 8295, 8329, 8270, 8103, 8531, 8527, 8567, 8450, 8413, 8295, 8329, 8270, 8103, 8531, 8527, 8618, 7981, 7981, 8396, 7981, 8396, 9073, 7981, 8396, 9073, 8421, 7981, 8396, 9073, 8421, 8687, 7981, 8396, 9073, 8421, 8687, 8689, 7981, 8396, 9073, 8421, 8687, 8689, 8406, 7981, 8396, 9073, 8421, 8687, 8689, 8406, 8354, 7981, 8396, 9073, 8421, 8687, 8689, 8406, 8354, 8603, 7981, 8396, 9073, 8421, 8687, 8689, 8406, 8354, 8603, 8855, 8297, 8297, 8552, 8297, 8552, 8547, 8297, 8552, 8547, 8365, 8297, 8552, 8547, 8365, 8648, 8297, 8552, 8547, 8365, 8648, 8319, 8297, 8552, 8547, 8365, 8648, 8319, 8635, 8297, 8552, 8547, 8365, 8648, 8319, 8635, 8213, 8297, 8552, 8547, 8365, 8648, 8319, 8635, 8213, 8655, 8297, 8552, 8547, 8365, 8648, 8319, 8635, 8213, 8655, 8838, 8634, 8634, 8308, 8634, 8308, 8495, 8634, 8308, 8495, 8483, 8634, 8308, 8495, 8483, 8653, 8634, 8308, 8495, 8483, 8653, 8454, 8634, 8308, 8495, 8483, 8653, 8454, 8558, 8634, 8308, 8495, 8483, 8653, 8454, 8558, 8722, 8634, 8308, 8495, 8483, 8653, 8454, 8558, 8722, 9010, 8634, 8308, 8495, 8483, 8653, 8454, 8558, 8722, 9010, 8517, 7998, 7998, 8452, 7998, 8452, 8559, 7998, 8452, 8559, 8395, 7998, 8452, 8559, 8395, 8221, 7998, 8452, 8559, 8395, 8221, 8220, 7998, 8452, 8559, 8395, 8221, 8220, 8189, 7998, 8452, 8559, 8395, 8221, 8220, 8189, 8729, 7998, 8452, 8559, 8395, 8221, 8220, 8189, 8729, 8384, };static struct simple_udp_connection unicast_connection;
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