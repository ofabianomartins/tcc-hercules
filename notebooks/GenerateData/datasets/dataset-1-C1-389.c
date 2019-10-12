#include "contiki.h"#include "lib/random.h"#include "sys/ctimer.h"#include "sys/etimer.h"#include "net/uip.h"#include "net/uip-ds6.h"#include "net/uip-debug.h"#include "sys/node-id.h"#include "simple-udp.h"#include "powertrace.h"#include "servreg-hack.h"
#include <stdio.h>#include <string.h>
#define UDP_PORT 1234#define SERVICE_ID 190
#define SEND_INTERVAL    (5 * CLOCK_SECOND)#define SEND_TIME    (random_rand() % (SEND_INTERVAL))#define POWERTRACE_INTERVAL  (2 * CLOCK_SECOND)#define DATA_SIZE 100
int samples[100] = {5128, 5128, 4938, 5128, 4938, 5284, 5128, 4938, 5284, 4390, 5128, 4938, 5284, 4390, 5569, 5128, 4938, 5284, 4390, 5569, 5076, 5128, 4938, 5284, 4390, 5569, 5076, 4818, 5128, 4938, 5284, 4390, 5569, 5076, 4818, 4706, 5128, 4938, 5284, 4390, 5569, 5076, 4818, 4706, 5397, 5128, 4938, 5284, 4390, 5569, 5076, 4818, 4706, 5397, 5020, 5128, 4938, 5284, 4390, 5569, 5076, 4818, 4706, 5397, 5020, 4657, 5036, 5036, 6016, 5036, 6016, 5154, 5036, 6016, 5154, 4597, 5036, 6016, 5154, 4597, 4502, 5036, 6016, 5154, 4597, 4502, 5450, 5036, 6016, 5154, 4597, 4502, 5450, 4201, 5036, 6016, 5154, 4597, 4502, 5450, 4201, 4236, 5036, 6016, 5154, 4597, 4502, 5450, 4201, 4236, 5244, 5036, 6016, 5154, 4597, 4502, 5450, 4201, 4236, 5244, 4497, 5126, 5126, 4928, 5126, 4928, 4795, 5126, 4928, 4795, 4598, 5126, 4928, 4795, 4598, 5000, 5126, 4928, 4795, 4598, 5000, 5502, 5126, 4928, 4795, 4598, 5000, 5502, 4299, 5126, 4928, 4795, 4598, 5000, 5502, 4299, 5313, 5126, 4928, 4795, 4598, 5000, 5502, 4299, 5313, 4995, 5126, 4928, 4795, 4598, 5000, 5502, 4299, 5313, 4995, 3779, 5304, 5304, 4809, 5304, 4809, 4606, 5304, 4809, 4606, 5410, 5304, 4809, 4606, 5410, 4343, 5304, 4809, 4606, 5410, 4343, 4944, 5304, 4809, 4606, 5410, 4343, 4944, 4995, 5304, 4809, 4606, 5410, 4343, 4944, 4995, 4796, 5304, 4809, 4606, 5410, 4343, 4944, 4995, 4796, 5150, 5304, 4809, 4606, 5410, 4343, 4944, 4995, 4796, 5150, 5120, 5260, 5260, 4188, 5260, 4188, 5162, 5260, 4188, 5162, 5133, 5260, 4188, 5162, 5133, 5124, 5260, 4188, 5162, 5133, 5124, 4633, 5260, 4188, 5162, 5133, 5124, 4633, 5535, 5260, 4188, 5162, 5133, 5124, 4633, 5535, 5120, 5260, 4188, 5162, 5133, 5124, 4633, 5535, 5120, 5791, 5260, 4188, 5162, 5133, 5124, 4633, 5535, 5120, 5791, 4957, 5642, 5642, 4377, 5642, 4377, 4431, 5642, 4377, 4431, 3922, 5642, 4377, 4431, 3922, 4397, 5642, 4377, 4431, 3922, 4397, 4773, 5642, 4377, 4431, 3922, 4397, 4773, 4961, 5642, 4377, 4431, 3922, 4397, 4773, 4961, 5747, 5642, 4377, 4431, 3922, 4397, 4773, 4961, 5747, 4806, 5642, 4377, 4431, 3922, 4397, 4773, 4961, 5747, 4806, 4625, 4859, 4859, 4926, 4859, 4926, 5103, 4859, 4926, 5103, 5043, 4859, 4926, 5103, 5043, 4238, 4859, 4926, 5103, 5043, 4238, 4519, 4859, 4926, 5103, 5043, 4238, 4519, 4205, 4859, 4926, 5103, 5043, 4238, 4519, 4205, 5205, 4859, 4926, 5103, 5043, 4238, 4519, 4205, 5205, 4612, 4859, 4926, 5103, 5043, 4238, 4519, 4205, 5205, 4612, 5119, 5283, 5283, 4923, 5283, 4923, 5129, 5283, 4923, 5129, 4836, 5283, 4923, 5129, 4836, 5192, 5283, 4923, 5129, 4836, 5192, 5496, 5283, 4923, 5129, 4836, 5192, 5496, 4957, 5283, 4923, 5129, 4836, 5192, 5496, 4957, 4161, 5283, 4923, 5129, 4836, 5192, 5496, 4957, 4161, 4481, 5283, 4923, 5129, 4836, 5192, 5496, 4957, 4161, 4481, 5202, 5105, 5105, 4695, 5105, 4695, 5680, 5105, 4695, 5680, 4868, 5105, 4695, 5680, 4868, 4997, 5105, 4695, 5680, 4868, 4997, 4965, 5105, 4695, 5680, 4868, 4997, 4965, 5261, 5105, 4695, 5680, 4868, 4997, 4965, 5261, 4929, 5105, 4695, 5680, 4868, 4997, 4965, 5261, 4929, 4888, 5105, 4695, 5680, 4868, 4997, 4965, 5261, 4929, 4888, 5531, 4840, 4840, 5257, 4840, 5257, 5013, 4840, 5257, 5013, 5809, 4840, 5257, 5013, 5809, 4891, 4840, 5257, 5013, 5809, 4891, 4484, 4840, 5257, 5013, 5809, 4891, 4484, 5046, 4840, 5257, 5013, 5809, 4891, 4484, 5046, 5399, 4840, 5257, 5013, 5809, 4891, 4484, 5046, 5399, 4111, };static struct simple_udp_connection unicast_connection;
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