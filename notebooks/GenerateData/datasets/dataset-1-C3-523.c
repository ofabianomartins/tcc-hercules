#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5361, 5361, 4910, 5361, 4910, 5238, 5361, 4910, 5238, 5005, 5361, 4910, 5238, 5005, 4778, 5361, 4910, 5238, 5005, 4778, 4768, 5361, 4910, 5238, 5005, 4778, 4768, 5081, 5361, 4910, 5238, 5005, 4778, 4768, 5081, 4203, 5361, 4910, 5238, 5005, 4778, 4768, 5081, 4203, 5643, 5361, 4910, 5238, 5005, 4778, 4768, 5081, 4203, 5643, 5742, 5361, 4910, 5238, 5005, 4778, 4768, 5081, 4203, 5643, 5742, 5343, 4673, 4673, 4640, 4673, 4640, 5168, 4673, 4640, 5168, 5055, 4673, 4640, 5168, 5055, 4909, 4673, 4640, 5168, 5055, 4909, 6353, 4673, 4640, 5168, 5055, 4909, 6353, 5262, 4673, 4640, 5168, 5055, 4909, 6353, 5262, 4249, 4673, 4640, 5168, 5055, 4909, 6353, 5262, 4249, 4866, 4673, 4640, 5168, 5055, 4909, 6353, 5262, 4249, 4866, 5431, 5177, 5177, 6035, 5177, 6035, 4843, 5177, 6035, 4843, 5892, 5177, 6035, 4843, 5892, 4994, 5177, 6035, 4843, 5892, 4994, 4143, 5177, 6035, 4843, 5892, 4994, 4143, 5146, 5177, 6035, 4843, 5892, 4994, 4143, 5146, 4529, 5177, 6035, 4843, 5892, 4994, 4143, 5146, 4529, 5173, 5177, 6035, 4843, 5892, 4994, 4143, 5146, 4529, 5173, 4704, 4891, 4891, 4973, 4891, 4973, 5262, 4891, 4973, 5262, 5159, 4891, 4973, 5262, 5159, 5541, 4891, 4973, 5262, 5159, 5541, 4824, 4891, 4973, 5262, 5159, 5541, 4824, 5180, 4891, 4973, 5262, 5159, 5541, 4824, 5180, 4267, 4891, 4973, 5262, 5159, 5541, 4824, 5180, 4267, 5177, 4891, 4973, 5262, 5159, 5541, 4824, 5180, 4267, 5177, 4183, 5272, 5272, 5130, 5272, 5130, 4555, 5272, 5130, 4555, 5313, 5272, 5130, 4555, 5313, 5049, 5272, 5130, 4555, 5313, 5049, 4759, 5272, 5130, 4555, 5313, 5049, 4759, 4044, 5272, 5130, 4555, 5313, 5049, 4759, 4044, 4773, 5272, 5130, 4555, 5313, 5049, 4759, 4044, 4773, 4579, 5272, 5130, 4555, 5313, 5049, 4759, 4044, 4773, 4579, 4398, 4686, 4686, 4955, 4686, 4955, 4270, 4686, 4955, 4270, 4539, 4686, 4955, 4270, 4539, 5480, 4686, 4955, 4270, 4539, 5480, 5684, 4686, 4955, 4270, 4539, 5480, 5684, 4968, 4686, 4955, 4270, 4539, 5480, 5684, 4968, 4970, 4686, 4955, 4270, 4539, 5480, 5684, 4968, 4970, 5008, 4686, 4955, 4270, 4539, 5480, 5684, 4968, 4970, 5008, 5023, 5029, 5029, 4718, 5029, 4718, 5035, 5029, 4718, 5035, 4649, 5029, 4718, 5035, 4649, 4961, 5029, 4718, 5035, 4649, 4961, 4856, 5029, 4718, 5035, 4649, 4961, 4856, 4542, 5029, 4718, 5035, 4649, 4961, 4856, 4542, 4887, 5029, 4718, 5035, 4649, 4961, 4856, 4542, 4887, 5062, 5029, 4718, 5035, 4649, 4961, 4856, 4542, 4887, 5062, 4502, 5113, 5113, 5445, 5113, 5445, 5343, 5113, 5445, 5343, 5998, 5113, 5445, 5343, 5998, 4590, 5113, 5445, 5343, 5998, 4590, 3858, 5113, 5445, 5343, 5998, 4590, 3858, 4874, 5113, 5445, 5343, 5998, 4590, 3858, 4874, 4961, 5113, 5445, 5343, 5998, 4590, 3858, 4874, 4961, 4536, 5113, 5445, 5343, 5998, 4590, 3858, 4874, 4961, 4536, 5167, 4939, 4939, 3973, 4939, 3973, 5932, 4939, 3973, 5932, 5624, 4939, 3973, 5932, 5624, 5654, 4939, 3973, 5932, 5624, 5654, 5685, 4939, 3973, 5932, 5624, 5654, 5685, 5540, 4939, 3973, 5932, 5624, 5654, 5685, 5540, 5008, 4939, 3973, 5932, 5624, 5654, 5685, 5540, 5008, 5079, 4939, 3973, 5932, 5624, 5654, 5685, 5540, 5008, 5079, 5685, 5190, 5190, 5102, 5190, 5102, 4938, 5190, 5102, 4938, 5405, 5190, 5102, 4938, 5405, 5117, 5190, 5102, 4938, 5405, 5117, 5290, 5190, 5102, 4938, 5405, 5117, 5290, 5210, 5190, 5102, 4938, 5405, 5117, 5290, 5210, 5608, 5190, 5102, 4938, 5405, 5117, 5290, 5210, 5608, 5288, };static struct simple_udp_connection unicast_connection;
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