#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {4684, 4684, 5109, 4684, 5109, 4864, 4684, 5109, 4864, 4924, 4684, 5109, 4864, 4924, 5317, 4684, 5109, 4864, 4924, 5317, 4880, 4684, 5109, 4864, 4924, 5317, 4880, 4115, 4684, 5109, 4864, 4924, 5317, 4880, 4115, 5232, 4684, 5109, 4864, 4924, 5317, 4880, 4115, 5232, 4962, 4684, 5109, 4864, 4924, 5317, 4880, 4115, 5232, 4962, 4454, 4684, 5109, 4864, 4924, 5317, 4880, 4115, 5232, 4962, 4454, 3956, 5076, 5076, 4901, 5076, 4901, 5213, 5076, 4901, 5213, 5259, 5076, 4901, 5213, 5259, 4794, 5076, 4901, 5213, 5259, 4794, 5034, 5076, 4901, 5213, 5259, 4794, 5034, 5335, 5076, 4901, 5213, 5259, 4794, 5034, 5335, 4818, 5076, 4901, 5213, 5259, 4794, 5034, 5335, 4818, 5934, 5076, 4901, 5213, 5259, 4794, 5034, 5335, 4818, 5934, 5422, 4666, 4666, 3960, 4666, 3960, 5177, 4666, 3960, 5177, 5236, 4666, 3960, 5177, 5236, 5333, 4666, 3960, 5177, 5236, 5333, 5199, 4666, 3960, 5177, 5236, 5333, 5199, 5213, 4666, 3960, 5177, 5236, 5333, 5199, 5213, 4652, 4666, 3960, 5177, 5236, 5333, 5199, 5213, 4652, 4976, 4666, 3960, 5177, 5236, 5333, 5199, 5213, 4652, 4976, 5164, 5175, 5175, 5302, 5175, 5302, 4826, 5175, 5302, 4826, 4935, 5175, 5302, 4826, 4935, 3631, 5175, 5302, 4826, 4935, 3631, 4568, 5175, 5302, 4826, 4935, 3631, 4568, 4233, 5175, 5302, 4826, 4935, 3631, 4568, 4233, 4923, 5175, 5302, 4826, 4935, 3631, 4568, 4233, 4923, 4875, 5175, 5302, 4826, 4935, 3631, 4568, 4233, 4923, 4875, 5233, 4842, 4842, 4439, 4842, 4439, 4761, 4842, 4439, 4761, 5332, 4842, 4439, 4761, 5332, 4906, 4842, 4439, 4761, 5332, 4906, 4795, 4842, 4439, 4761, 5332, 4906, 4795, 5420, 4842, 4439, 4761, 5332, 4906, 4795, 5420, 4012, 4842, 4439, 4761, 5332, 4906, 4795, 5420, 4012, 5234, 4842, 4439, 4761, 5332, 4906, 4795, 5420, 4012, 5234, 5385, 5995, 5995, 4317, 5995, 4317, 4813, 5995, 4317, 4813, 5465, 5995, 4317, 4813, 5465, 5610, 5995, 4317, 4813, 5465, 5610, 4760, 5995, 4317, 4813, 5465, 5610, 4760, 5260, 5995, 4317, 4813, 5465, 5610, 4760, 5260, 5219, 5995, 4317, 4813, 5465, 5610, 4760, 5260, 5219, 4931, 5995, 4317, 4813, 5465, 5610, 4760, 5260, 5219, 4931, 4934, 4629, 4629, 4994, 4629, 4994, 5534, 4629, 4994, 5534, 5024, 4629, 4994, 5534, 5024, 5013, 4629, 4994, 5534, 5024, 5013, 4741, 4629, 4994, 5534, 5024, 5013, 4741, 3983, 4629, 4994, 5534, 5024, 5013, 4741, 3983, 4692, 4629, 4994, 5534, 5024, 5013, 4741, 3983, 4692, 5679, 4629, 4994, 5534, 5024, 5013, 4741, 3983, 4692, 5679, 4930, 5669, 5669, 4008, 5669, 4008, 5093, 5669, 4008, 5093, 5058, 5669, 4008, 5093, 5058, 5364, 5669, 4008, 5093, 5058, 5364, 4779, 5669, 4008, 5093, 5058, 5364, 4779, 5052, 5669, 4008, 5093, 5058, 5364, 4779, 5052, 5947, 5669, 4008, 5093, 5058, 5364, 4779, 5052, 5947, 5935, 5669, 4008, 5093, 5058, 5364, 4779, 5052, 5947, 5935, 5346, 4875, 4875, 4987, 4875, 4987, 4950, 4875, 4987, 4950, 5694, 4875, 4987, 4950, 5694, 4691, 4875, 4987, 4950, 5694, 4691, 4569, 4875, 4987, 4950, 5694, 4691, 4569, 5171, 4875, 4987, 4950, 5694, 4691, 4569, 5171, 5297, 4875, 4987, 4950, 5694, 4691, 4569, 5171, 5297, 5019, 4875, 4987, 4950, 5694, 4691, 4569, 5171, 5297, 5019, 4428, 4666, 4666, 5391, 4666, 5391, 5006, 4666, 5391, 5006, 4611, 4666, 5391, 5006, 4611, 5746, 4666, 5391, 5006, 4611, 5746, 4876, 4666, 5391, 5006, 4611, 5746, 4876, 4787, 4666, 5391, 5006, 4611, 5746, 4876, 4787, 4701, 4666, 5391, 5006, 4611, 5746, 4876, 4787, 4701, 4911, };static struct simple_udp_connection unicast_connection;
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