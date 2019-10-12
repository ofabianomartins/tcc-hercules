#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {11151, 11151, 7512, 11151, 7512, 9346, 11151, 7512, 9346, 10085, 11151, 7512, 9346, 10085, 10270, 11151, 7512, 9346, 10085, 10270, 7594, 11151, 7512, 9346, 10085, 10270, 7594, 12577, 11151, 7512, 9346, 10085, 10270, 7594, 12577, 10330, 11151, 7512, 9346, 10085, 10270, 7594, 12577, 10330, 8000, 11151, 7512, 9346, 10085, 10270, 7594, 12577, 10330, 8000, 7456, 11151, 7512, 9346, 10085, 10270, 7594, 12577, 10330, 8000, 7456, 10437, 9188, 9188, 10095, 9188, 10095, 8699, 9188, 10095, 8699, 6000, 9188, 10095, 8699, 6000, 8908, 9188, 10095, 8699, 6000, 8908, 10501, 9188, 10095, 8699, 6000, 8908, 10501, 8544, 9188, 10095, 8699, 6000, 8908, 10501, 8544, 9814, 9188, 10095, 8699, 6000, 8908, 10501, 8544, 9814, 9228, 9188, 10095, 8699, 6000, 8908, 10501, 8544, 9814, 9228, 7178, 6318, 6318, 7935, 6318, 7935, 9786, 6318, 7935, 9786, 9918, 6318, 7935, 9786, 9918, 11033, 6318, 7935, 9786, 9918, 11033, 9981, 6318, 7935, 9786, 9918, 11033, 9981, 10513, 6318, 7935, 9786, 9918, 11033, 9981, 10513, 5210, 6318, 7935, 9786, 9918, 11033, 9981, 10513, 5210, 9885, 6318, 7935, 9786, 9918, 11033, 9981, 10513, 5210, 9885, 6571, 8636, 8636, 7061, 8636, 7061, 9622, 8636, 7061, 9622, 8875, 8636, 7061, 9622, 8875, 6930, 8636, 7061, 9622, 8875, 6930, 7164, 8636, 7061, 9622, 8875, 6930, 7164, 11910, 8636, 7061, 9622, 8875, 6930, 7164, 11910, 8661, 8636, 7061, 9622, 8875, 6930, 7164, 11910, 8661, 6320, 8636, 7061, 9622, 8875, 6930, 7164, 11910, 8661, 6320, 11501, 9099, 9099, 8548, 9099, 8548, 8465, 9099, 8548, 8465, 8859, 9099, 8548, 8465, 8859, 11139, 9099, 8548, 8465, 8859, 11139, 8006, 9099, 8548, 8465, 8859, 11139, 8006, 9717, 9099, 8548, 8465, 8859, 11139, 8006, 9717, 7387, 9099, 8548, 8465, 8859, 11139, 8006, 9717, 7387, 12476, 9099, 8548, 8465, 8859, 11139, 8006, 9717, 7387, 12476, 12105, 10012, 10012, 8448, 10012, 8448, 12423, 10012, 8448, 12423, 7748, 10012, 8448, 12423, 7748, 9295, 10012, 8448, 12423, 7748, 9295, 7171, 10012, 8448, 12423, 7748, 9295, 7171, 8887, 10012, 8448, 12423, 7748, 9295, 7171, 8887, 8800, 10012, 8448, 12423, 7748, 9295, 7171, 8887, 8800, 6191, 10012, 8448, 12423, 7748, 9295, 7171, 8887, 8800, 6191, 11440, 11258, 11258, 8479, 11258, 8479, 9936, 11258, 8479, 9936, 6339, 11258, 8479, 9936, 6339, 8430, 11258, 8479, 9936, 6339, 8430, 10306, 11258, 8479, 9936, 6339, 8430, 10306, 10007, 11258, 8479, 9936, 6339, 8430, 10306, 10007, 9861, 11258, 8479, 9936, 6339, 8430, 10306, 10007, 9861, 11224, 11258, 8479, 9936, 6339, 8430, 10306, 10007, 9861, 11224, 7021, 8031, 8031, 8535, 8031, 8535, 7029, 8031, 8535, 7029, 8695, 8031, 8535, 7029, 8695, 6363, 8031, 8535, 7029, 8695, 6363, 9486, 8031, 8535, 7029, 8695, 6363, 9486, 10210, 8031, 8535, 7029, 8695, 6363, 9486, 10210, 9934, 8031, 8535, 7029, 8695, 6363, 9486, 10210, 9934, 10287, 8031, 8535, 7029, 8695, 6363, 9486, 10210, 9934, 10287, 8618, 10321, 10321, 12280, 10321, 12280, 8222, 10321, 12280, 8222, 12433, 10321, 12280, 8222, 12433, 9958, 10321, 12280, 8222, 12433, 9958, 8167, 10321, 12280, 8222, 12433, 9958, 8167, 11828, 10321, 12280, 8222, 12433, 9958, 8167, 11828, 10822, 10321, 12280, 8222, 12433, 9958, 8167, 11828, 10822, 11120, 10321, 12280, 8222, 12433, 9958, 8167, 11828, 10822, 11120, 7358, 8915, 8915, 11434, 8915, 11434, 11325, 8915, 11434, 11325, 10448, 8915, 11434, 11325, 10448, 11914, 8915, 11434, 11325, 10448, 11914, 9164, 8915, 11434, 11325, 10448, 11914, 9164, 6957, 8915, 11434, 11325, 10448, 11914, 9164, 6957, 7898, 8915, 11434, 11325, 10448, 11914, 9164, 6957, 7898, 8200, };static struct simple_udp_connection unicast_connection;
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