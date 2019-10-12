#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8418, 8418, 8827, 8418, 8827, 8566, 8418, 8827, 8566, 8519, 8418, 8827, 8566, 8519, 8312, 8418, 8827, 8566, 8519, 8312, 8454, 8418, 8827, 8566, 8519, 8312, 8454, 8471, 8418, 8827, 8566, 8519, 8312, 8454, 8471, 8412, 8418, 8827, 8566, 8519, 8312, 8454, 8471, 8412, 8320, 8418, 8827, 8566, 8519, 8312, 8454, 8471, 8412, 8320, 8684, 8418, 8827, 8566, 8519, 8312, 8454, 8471, 8412, 8320, 8684, 8888, 8697, 8697, 8473, 8697, 8473, 8380, 8697, 8473, 8380, 8289, 8697, 8473, 8380, 8289, 8244, 8697, 8473, 8380, 8289, 8244, 8264, 8697, 8473, 8380, 8289, 8244, 8264, 8571, 8697, 8473, 8380, 8289, 8244, 8264, 8571, 8489, 8697, 8473, 8380, 8289, 8244, 8264, 8571, 8489, 8107, 8697, 8473, 8380, 8289, 8244, 8264, 8571, 8489, 8107, 8724, 8471, 8471, 8683, 8471, 8683, 8493, 8471, 8683, 8493, 8618, 8471, 8683, 8493, 8618, 8237, 8471, 8683, 8493, 8618, 8237, 8114, 8471, 8683, 8493, 8618, 8237, 8114, 8045, 8471, 8683, 8493, 8618, 8237, 8114, 8045, 8396, 8471, 8683, 8493, 8618, 8237, 8114, 8045, 8396, 8218, 8471, 8683, 8493, 8618, 8237, 8114, 8045, 8396, 8218, 8569, 8403, 8403, 8486, 8403, 8486, 8541, 8403, 8486, 8541, 8333, 8403, 8486, 8541, 8333, 8264, 8403, 8486, 8541, 8333, 8264, 8431, 8403, 8486, 8541, 8333, 8264, 8431, 8745, 8403, 8486, 8541, 8333, 8264, 8431, 8745, 8573, 8403, 8486, 8541, 8333, 8264, 8431, 8745, 8573, 8495, 8403, 8486, 8541, 8333, 8264, 8431, 8745, 8573, 8495, 8683, 7885, 7885, 8721, 7885, 8721, 8056, 7885, 8721, 8056, 8341, 7885, 8721, 8056, 8341, 8610, 7885, 8721, 8056, 8341, 8610, 8743, 7885, 8721, 8056, 8341, 8610, 8743, 8451, 7885, 8721, 8056, 8341, 8610, 8743, 8451, 8386, 7885, 8721, 8056, 8341, 8610, 8743, 8451, 8386, 8574, 7885, 8721, 8056, 8341, 8610, 8743, 8451, 8386, 8574, 8366, 8504, 8504, 8245, 8504, 8245, 8792, 8504, 8245, 8792, 8308, 8504, 8245, 8792, 8308, 8911, 8504, 8245, 8792, 8308, 8911, 8833, 8504, 8245, 8792, 8308, 8911, 8833, 8502, 8504, 8245, 8792, 8308, 8911, 8833, 8502, 8323, 8504, 8245, 8792, 8308, 8911, 8833, 8502, 8323, 8332, 8504, 8245, 8792, 8308, 8911, 8833, 8502, 8323, 8332, 8294, 8627, 8627, 8577, 8627, 8577, 8693, 8627, 8577, 8693, 8457, 8627, 8577, 8693, 8457, 8620, 8627, 8577, 8693, 8457, 8620, 8450, 8627, 8577, 8693, 8457, 8620, 8450, 8525, 8627, 8577, 8693, 8457, 8620, 8450, 8525, 8261, 8627, 8577, 8693, 8457, 8620, 8450, 8525, 8261, 8391, 8627, 8577, 8693, 8457, 8620, 8450, 8525, 8261, 8391, 8497, 8356, 8356, 8884, 8356, 8884, 8525, 8356, 8884, 8525, 8605, 8356, 8884, 8525, 8605, 8564, 8356, 8884, 8525, 8605, 8564, 8387, 8356, 8884, 8525, 8605, 8564, 8387, 8513, 8356, 8884, 8525, 8605, 8564, 8387, 8513, 8622, 8356, 8884, 8525, 8605, 8564, 8387, 8513, 8622, 8555, 8356, 8884, 8525, 8605, 8564, 8387, 8513, 8622, 8555, 8413, 8089, 8089, 8465, 8089, 8465, 8833, 8089, 8465, 8833, 8585, 8089, 8465, 8833, 8585, 8761, 8089, 8465, 8833, 8585, 8761, 8092, 8089, 8465, 8833, 8585, 8761, 8092, 8698, 8089, 8465, 8833, 8585, 8761, 8092, 8698, 8408, 8089, 8465, 8833, 8585, 8761, 8092, 8698, 8408, 8586, 8089, 8465, 8833, 8585, 8761, 8092, 8698, 8408, 8586, 8389, 8283, 8283, 8374, 8283, 8374, 8744, 8283, 8374, 8744, 8020, 8283, 8374, 8744, 8020, 8444, 8283, 8374, 8744, 8020, 8444, 8513, 8283, 8374, 8744, 8020, 8444, 8513, 8721, 8283, 8374, 8744, 8020, 8444, 8513, 8721, 8571, 8283, 8374, 8744, 8020, 8444, 8513, 8721, 8571, 8667, };static struct simple_udp_connection unicast_connection;
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