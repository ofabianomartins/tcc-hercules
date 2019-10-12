#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {10515, 10515, 12270, 10515, 12270, 8153, 10515, 12270, 8153, 9955, 10515, 12270, 8153, 9955, 10887, 10515, 12270, 8153, 9955, 10887, 11514, 10515, 12270, 8153, 9955, 10887, 11514, 8334, 10515, 12270, 8153, 9955, 10887, 11514, 8334, 11860, 10515, 12270, 8153, 9955, 10887, 11514, 8334, 11860, 6611, 10515, 12270, 8153, 9955, 10887, 11514, 8334, 11860, 6611, 9301, 10515, 12270, 8153, 9955, 10887, 11514, 8334, 11860, 6611, 9301, 9526, 8832, 8832, 10153, 8832, 10153, 12998, 8832, 10153, 12998, 7591, 8832, 10153, 12998, 7591, 12375, 8832, 10153, 12998, 7591, 12375, 7440, 8832, 10153, 12998, 7591, 12375, 7440, 10952, 8832, 10153, 12998, 7591, 12375, 7440, 10952, 10609, 8832, 10153, 12998, 7591, 12375, 7440, 10952, 10609, 10028, 8832, 10153, 12998, 7591, 12375, 7440, 10952, 10609, 10028, 8540, 10081, 10081, 9017, 10081, 9017, 9184, 10081, 9017, 9184, 7220, 10081, 9017, 9184, 7220, 10011, 10081, 9017, 9184, 7220, 10011, 11453, 10081, 9017, 9184, 7220, 10011, 11453, 10482, 10081, 9017, 9184, 7220, 10011, 11453, 10482, 8760, 10081, 9017, 9184, 7220, 10011, 11453, 10482, 8760, 10682, 10081, 9017, 9184, 7220, 10011, 11453, 10482, 8760, 10682, 10667, 9370, 9370, 9999, 9370, 9999, 8196, 9370, 9999, 8196, 8629, 9370, 9999, 8196, 8629, 12793, 9370, 9999, 8196, 8629, 12793, 9116, 9370, 9999, 8196, 8629, 12793, 9116, 10480, 9370, 9999, 8196, 8629, 12793, 9116, 10480, 8266, 9370, 9999, 8196, 8629, 12793, 9116, 10480, 8266, 9134, 9370, 9999, 8196, 8629, 12793, 9116, 10480, 8266, 9134, 8577, 10195, 10195, 11949, 10195, 11949, 7779, 10195, 11949, 7779, 13381, 10195, 11949, 7779, 13381, 10062, 10195, 11949, 7779, 13381, 10062, 9915, 10195, 11949, 7779, 13381, 10062, 9915, 9911, 10195, 11949, 7779, 13381, 10062, 9915, 9911, 8427, 10195, 11949, 7779, 13381, 10062, 9915, 9911, 8427, 6965, 10195, 11949, 7779, 13381, 10062, 9915, 9911, 8427, 6965, 8666, 10262, 10262, 10431, 10262, 10431, 6134, 10262, 10431, 6134, 9328, 10262, 10431, 6134, 9328, 8836, 10262, 10431, 6134, 9328, 8836, 6733, 10262, 10431, 6134, 9328, 8836, 6733, 7271, 10262, 10431, 6134, 9328, 8836, 6733, 7271, 7103, 10262, 10431, 6134, 9328, 8836, 6733, 7271, 7103, 9199, 10262, 10431, 6134, 9328, 8836, 6733, 7271, 7103, 9199, 7765, 5986, 5986, 7956, 5986, 7956, 8306, 5986, 7956, 8306, 8009, 5986, 7956, 8306, 8009, 11447, 5986, 7956, 8306, 8009, 11447, 9299, 5986, 7956, 8306, 8009, 11447, 9299, 8305, 5986, 7956, 8306, 8009, 11447, 9299, 8305, 9126, 5986, 7956, 8306, 8009, 11447, 9299, 8305, 9126, 9208, 5986, 7956, 8306, 8009, 11447, 9299, 8305, 9126, 9208, 6876, 11684, 11684, 10534, 11684, 10534, 10613, 11684, 10534, 10613, 9126, 11684, 10534, 10613, 9126, 10690, 11684, 10534, 10613, 9126, 10690, 7391, 11684, 10534, 10613, 9126, 10690, 7391, 9635, 11684, 10534, 10613, 9126, 10690, 7391, 9635, 13063, 11684, 10534, 10613, 9126, 10690, 7391, 9635, 13063, 7389, 11684, 10534, 10613, 9126, 10690, 7391, 9635, 13063, 7389, 8993, 11737, 11737, 11740, 11737, 11740, 10963, 11737, 11740, 10963, 10389, 11737, 11740, 10963, 10389, 7432, 11737, 11740, 10963, 10389, 7432, 8035, 11737, 11740, 10963, 10389, 7432, 8035, 10047, 11737, 11740, 10963, 10389, 7432, 8035, 10047, 8821, 11737, 11740, 10963, 10389, 7432, 8035, 10047, 8821, 5030, 11737, 11740, 10963, 10389, 7432, 8035, 10047, 8821, 5030, 7787, 7400, 7400, 10146, 7400, 10146, 7513, 7400, 10146, 7513, 6902, 7400, 10146, 7513, 6902, 9247, 7400, 10146, 7513, 6902, 9247, 9145, 7400, 10146, 7513, 6902, 9247, 9145, 7278, 7400, 10146, 7513, 6902, 9247, 9145, 7278, 8046, 7400, 10146, 7513, 6902, 9247, 9145, 7278, 8046, 10821, };static struct simple_udp_connection unicast_connection;
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