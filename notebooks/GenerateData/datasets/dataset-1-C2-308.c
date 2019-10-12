#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {8636, 8636, 8705, 8636, 8705, 8511, 8636, 8705, 8511, 8434, 8636, 8705, 8511, 8434, 8195, 8636, 8705, 8511, 8434, 8195, 8561, 8636, 8705, 8511, 8434, 8195, 8561, 8510, 8636, 8705, 8511, 8434, 8195, 8561, 8510, 8938, 8636, 8705, 8511, 8434, 8195, 8561, 8510, 8938, 8487, 8636, 8705, 8511, 8434, 8195, 8561, 8510, 8938, 8487, 8077, 8636, 8705, 8511, 8434, 8195, 8561, 8510, 8938, 8487, 8077, 8398, 8092, 8092, 8345, 8092, 8345, 8885, 8092, 8345, 8885, 8315, 8092, 8345, 8885, 8315, 8214, 8092, 8345, 8885, 8315, 8214, 8820, 8092, 8345, 8885, 8315, 8214, 8820, 8834, 8092, 8345, 8885, 8315, 8214, 8820, 8834, 8546, 8092, 8345, 8885, 8315, 8214, 8820, 8834, 8546, 8439, 8092, 8345, 8885, 8315, 8214, 8820, 8834, 8546, 8439, 8355, 8294, 8294, 8601, 8294, 8601, 8530, 8294, 8601, 8530, 8367, 8294, 8601, 8530, 8367, 8449, 8294, 8601, 8530, 8367, 8449, 8267, 8294, 8601, 8530, 8367, 8449, 8267, 8243, 8294, 8601, 8530, 8367, 8449, 8267, 8243, 8455, 8294, 8601, 8530, 8367, 8449, 8267, 8243, 8455, 8490, 8294, 8601, 8530, 8367, 8449, 8267, 8243, 8455, 8490, 8618, 8343, 8343, 8176, 8343, 8176, 8390, 8343, 8176, 8390, 8493, 8343, 8176, 8390, 8493, 8342, 8343, 8176, 8390, 8493, 8342, 8918, 8343, 8176, 8390, 8493, 8342, 8918, 8193, 8343, 8176, 8390, 8493, 8342, 8918, 8193, 8670, 8343, 8176, 8390, 8493, 8342, 8918, 8193, 8670, 8746, 8343, 8176, 8390, 8493, 8342, 8918, 8193, 8670, 8746, 8369, 8528, 8528, 8679, 8528, 8679, 8518, 8528, 8679, 8518, 8425, 8528, 8679, 8518, 8425, 8560, 8528, 8679, 8518, 8425, 8560, 8866, 8528, 8679, 8518, 8425, 8560, 8866, 8397, 8528, 8679, 8518, 8425, 8560, 8866, 8397, 8335, 8528, 8679, 8518, 8425, 8560, 8866, 8397, 8335, 8727, 8528, 8679, 8518, 8425, 8560, 8866, 8397, 8335, 8727, 8488, 8364, 8364, 8624, 8364, 8624, 8537, 8364, 8624, 8537, 8415, 8364, 8624, 8537, 8415, 8433, 8364, 8624, 8537, 8415, 8433, 8557, 8364, 8624, 8537, 8415, 8433, 8557, 8427, 8364, 8624, 8537, 8415, 8433, 8557, 8427, 8398, 8364, 8624, 8537, 8415, 8433, 8557, 8427, 8398, 8547, 8364, 8624, 8537, 8415, 8433, 8557, 8427, 8398, 8547, 8284, 8513, 8513, 8415, 8513, 8415, 8332, 8513, 8415, 8332, 8520, 8513, 8415, 8332, 8520, 8539, 8513, 8415, 8332, 8520, 8539, 8346, 8513, 8415, 8332, 8520, 8539, 8346, 8745, 8513, 8415, 8332, 8520, 8539, 8346, 8745, 8322, 8513, 8415, 8332, 8520, 8539, 8346, 8745, 8322, 8307, 8513, 8415, 8332, 8520, 8539, 8346, 8745, 8322, 8307, 8352, 8545, 8545, 8517, 8545, 8517, 8747, 8545, 8517, 8747, 8672, 8545, 8517, 8747, 8672, 8786, 8545, 8517, 8747, 8672, 8786, 8276, 8545, 8517, 8747, 8672, 8786, 8276, 8597, 8545, 8517, 8747, 8672, 8786, 8276, 8597, 8528, 8545, 8517, 8747, 8672, 8786, 8276, 8597, 8528, 8463, 8545, 8517, 8747, 8672, 8786, 8276, 8597, 8528, 8463, 8542, 8674, 8674, 8599, 8674, 8599, 8522, 8674, 8599, 8522, 8650, 8674, 8599, 8522, 8650, 8884, 8674, 8599, 8522, 8650, 8884, 8352, 8674, 8599, 8522, 8650, 8884, 8352, 8328, 8674, 8599, 8522, 8650, 8884, 8352, 8328, 8483, 8674, 8599, 8522, 8650, 8884, 8352, 8328, 8483, 8706, 8674, 8599, 8522, 8650, 8884, 8352, 8328, 8483, 8706, 8461, 8152, 8152, 8214, 8152, 8214, 8493, 8152, 8214, 8493, 8634, 8152, 8214, 8493, 8634, 8225, 8152, 8214, 8493, 8634, 8225, 8426, 8152, 8214, 8493, 8634, 8225, 8426, 8545, 8152, 8214, 8493, 8634, 8225, 8426, 8545, 8761, 8152, 8214, 8493, 8634, 8225, 8426, 8545, 8761, 8402, };static struct simple_udp_connection unicast_connection;
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