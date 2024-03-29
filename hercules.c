#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"

#include "simple-udp.h"
#include "servreg-hack.h"
#include "powertrace.h"

#include "net/rpl/rpl.h"

#include <stdio.h>
#include <stdlib.h>

#define UDP_PORT 1234
#define SERVICE_ID 190

#define SEND_INTERVAL	(30 * CLOCK_SECOND)
#define POWERTRACE_INTERVAL (30 * CLOCK_SECOND)

#define NUMBER_OF_SAMPLES 200
#define NUMBER_OF_BINS 30
#define NUMBER_OF_INTERVALS 15

#define ROUND_CLOSEST(dividend, divisor) ((dividend + (divisor / 2)) / divisor)

static struct simple_udp_connection hercules_connection;

uint16_t locked = 0;

// Definição da curva de frequência
uint16_t curve_frequency[NUMBER_OF_BINS];
uint16_t curve_data[NUMBER_OF_BINS];

// Definição da função de média
uint16_t function_frequency[NUMBER_OF_INTERVALS];
uint16_t function_data[NUMBER_OF_INTERVALS];
uint16_t function_status[NUMBER_OF_INTERVALS];

// Definições dos pontos de cisão
uint16_t split_points[NUMBER_OF_INTERVALS];
uint16_t split_counter = 0;

uint16_t samples[NUMBER_OF_SAMPLES];
uint16_t samples_counter = 0;

char message_buffer[100];

uint16_t discrete_mean(uint16_t *y,uint16_t begin,uint16_t end){
	uint16_t number_of_elements = end - begin, i, aux = 0;
	
	if( end <= begin ) return 0;
	
	for(i = begin; i < end; i++){
		aux = aux + ROUND_CLOSEST(y[i],number_of_elements);
	}

	return aux;
}

uint16_t weight_mean(uint16_t *x, uint16_t *y, uint16_t begin, uint16_t end){
	uint16_t sum_mean = 0, total_sample = 0, i;

	if( end <= begin ) return 0;
	
	for(i = begin; i < end; i++){
		sum_mean = sum_mean + x[i]*y[i];
		total_sample = total_sample + y[i];
	}

	return ROUND_CLOSEST(sum_mean, total_sample);
}

uint16_t select_maximun(uint16_t *data, uint16_t datalen, uint16_t value){
	uint16_t upinterval = 0;
	
	while( data[upinterval] < value && upinterval < (datalen - 1))
		upinterval = upinterval + 1;
	
	return upinterval;
}

// Funções auxiliares ao processo principais
void set_histogram(uint16_t *data, uint16_t data_index){
	uint16_t index = 0, curve_index = 0;
	uint16_t min_value = data[0];
	uint16_t max_value = data[data_index - 1];
	uint16_t step  = ROUND_CLOSEST((max_value - min_value), NUMBER_OF_BINS);
	uint16_t step2 = ROUND_CLOSEST(step,2);

	uint16_t sup_data_range = min_value + step;
   
	curve_frequency[0] = 0;
	curve_data[0] = min_value + step2;

	// printf("Histogram: %u - %u - %u\n", NUMBER_OF_BINS, data_index, max_value);
	while(index < data_index){
		if( data[index] <= sup_data_range ){
			curve_frequency[curve_index] = curve_frequency[curve_index] + 1;
			index = index + 1;
		}else { // This is to not write on other vector
			curve_index = curve_index + 1;
			curve_data[curve_index] = sup_data_range + step/2;
			curve_frequency[curve_index] = 0;
			sup_data_range = sup_data_range + step;
		}
	}
}

void set_average_function(uint16_t K, uint16_t step){
	uint16_t k;

	for(k = 0; k < K; k++ ){
		function_frequency[k] = discrete_mean(curve_frequency, k*step, (k+1)*step);
		function_data[k]      = discrete_mean(curve_data, k*step, (k+1)*step);
		function_status[k]    = 1;
	}
}

void classify_points(){
	uint16_t index;

	// Classificando os pontos do meio
	for(index = 1; index < NUMBER_OF_INTERVALS; index++ ){
		// printf("Status[index-1]: %u - %u\n",function_status[index-1],function_status[index]);
		if( function_frequency[index-1] <= function_frequency[index]){
			if( function_status[index-1] < 2){
				function_status[index] = 2;
			} else {
				function_status[index-1] = 1;
				function_status[index] = 2;
			}
		} else if( function_frequency[index-1] > function_frequency[index]){
			if( function_status[index-1] > 0 ){
				function_status[index] = 0;
			} else {
				function_status[index-1] = 1;
				function_status[index] = 0;
			}
		}
		// printf("Data: %u - %u\n",function_frequency[index-1],function_frequency[index]);
		// printf("Status[index]: %u - %u\n",function_status[index-1],function_status[index]);
	}
	function_status[NUMBER_OF_INTERVALS - 1] = 1;
}

// Função processo principal
void hercules(){
	uint begin = 0, end, i, j, value;

	// Ordenando vertor de leitura 
	for(i = 0; i < samples_counter - 1; i++){
		for(j = i; j < samples_counter; j++){
			if( samples[i] > samples[j]){
				value = samples[i];
				samples[i] = samples[j];
				samples[j] = value;
			}
		}
	}

	// Preencher os vetores da curva de frequência
	set_histogram(samples, samples_counter);

	// for(i = 0; i < NUMBER_OF_BINS ; i++){
	// 	printf("Histogram: %d - %d\n",
	// 		curve_data[i],
	// 		curve_frequency[i]
	// 	);
	// }

	// Preencher os vetores da função de média
	set_average_function(NUMBER_OF_INTERVALS, NUMBER_OF_BINS/NUMBER_OF_INTERVALS);

	// Classificar os pontos da função de médias
	classify_points();

	// for(i = 0; i < NUMBER_OF_INTERVALS; i++){
	// 	printf("mean_function: %d - %d - %d\n",
	// 		function_data[i], 
	// 		function_frequency[i], 
	// 		function_status[i] 
	// 	);
	// }

	// selecionar os vales, excluindo os vales do extremo
	split_counter = 0;
	// selecionando os vales que foram identificados
	for(i = 0; i < NUMBER_OF_INTERVALS; i++){
		if(function_status[i] == 0){ // Zero é valor para vale
			split_points[split_counter++] = function_data[i];
		}
	}

	sprintf(message_buffer, "H:(");
	// Gerar as mensagens que serão enviadas para as aplicações
	for(i = 0; i < split_counter; i++){
		end = select_maximun(samples, samples_counter, split_points[i]);
		value = discrete_mean(samples,begin, end);

		// printf("Data[%u] = %u\n", i, value);
		sprintf(message_buffer, "%s%u.%u,",message_buffer, value/100, value % 100);
		begin = end + 1;
	}

	value = discrete_mean(samples,begin, samples_counter);
	sprintf(message_buffer, "%s%u.%u)",message_buffer, value/100, value % 100);
	printf("%s\n", message_buffer);
}

/*---------------------------------------------------------------------------*/
PROCESS(unicast_receiver_process, "Hercules process");
AUTOSTART_PROCESSES(&unicast_receiver_process);
/*---------------------------------------------------------------------------*/
static void receiver(struct simple_udp_connection *c,
		const uip_ipaddr_t *sender_addr,   uint16_t sender_port,
		const uip_ipaddr_t *receiver_addr, uint16_t receiver_port,
		const uint8_t      *data,          uint16_t datalen){
  	uint16_t i, localnumbersamples, pointer;	
  	char subbuff[20];

  	if( locked == 0 && data[0] == 'c'){
  		for(i = 2; data[i] != ':'; i++);
		memcpy( subbuff, &data[2], i );
		subbuff[i] = '\0';
		localnumbersamples = atoi(subbuff);
		
		for(; localnumbersamples > 0; localnumbersamples--){
			pointer = i + 1;
			for(; data[i] != ',' && i < (datalen - 1); i++);
			memcpy( subbuff, &data[pointer], i );
			subbuff[i] = '\0';
			// printf("Number: %s\n",subbuff);
			samples[samples_counter] = atoi(subbuff);
			// printf("Received: %u\n", samples[samples_counter]);
			samples_counter = (samples_counter + 1) % NUMBER_OF_SAMPLES;
		}
  	}
}

/*---------------------------------------------------------------------------*/
static uip_ipaddr_t * set_global_address(void){
  static uip_ipaddr_t ipaddr;
  int i;
  uint8_t state;

  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

  printf("IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
	state = uip_ds6_if.addr_list[i].state;
	if(uip_ds6_if.addr_list[i].isused &&
	   (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
	  uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
	  printf("\n");
	}
  }

  return &ipaddr;
}
/*---------------------------------------------------------------------------*/
static void create_rpl_dag(uip_ipaddr_t *ipaddr){
  struct uip_ds6_addr *root_if;

  root_if = uip_ds6_addr_lookup(ipaddr);
  if(root_if != NULL) {
	rpl_dag_t *dag;
	uip_ipaddr_t prefix;
	
	rpl_set_root(RPL_DEFAULT_INSTANCE, ipaddr);
	dag = rpl_get_any_dag();
	uip_ip6addr(&prefix, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
	rpl_set_prefix(dag, &prefix, 64);
	PRINTF("created a new RPL dag\n");
  } else {
	PRINTF("failed to create a new RPL DAG\n");
  }
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_receiver_process, ev, data){
  static struct etimer periodic_timer;
  uip_ipaddr_t *ipaddr;
  // uint16_t counter = 0;

  PROCESS_BEGIN();

  // Iniciando o powertrace
  powertrace_start(POWERTRACE_INTERVAL);

  // Iniciando o serviço de rede
  servreg_hack_init();
  ipaddr = set_global_address();
  create_rpl_dag(ipaddr);
  servreg_hack_register(SERVICE_ID, ipaddr);
  simple_udp_register(&hercules_connection, UDP_PORT,NULL, UDP_PORT, receiver);
  // Iniciando o temporizador
  etimer_set(&periodic_timer, SEND_INTERVAL);

  // printf("Open collector window\n");
  while(1) {
	// PROCESS_WAIT_EVENT();
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

	// printf("Close collector window\n");
	// printf("Open fusion window\n");
	locked = 1;

	hercules();

	// printf("Close fusion window\n");
	// counter = counter + 1;
	locked = 0;
	// printf("Open collector window\n");

	etimer_reset(&periodic_timer);    
  }

  // printf("End of process\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
