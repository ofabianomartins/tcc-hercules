CONTIKI_PROJECT = hercules
all: $(CONTIKI_PROJECT) collect_battery collect_OLPM
APPS+=servreg-hack
APPS+=powertrace
CONTIKI = ../..

WITH_UIP6=1
UIP_CONF_IPV6=1
CFLAGS+= -DUIP_CONF_IPV6_RPL

include $(CONTIKI)/Makefile.include