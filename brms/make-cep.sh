#! /bin/bash
TRICEPSBASE=$(HOME)/cep/triceps-2.0.0
CFLAGS += -I$(TRICEPSBASE)/cpp -DTRICEPS_NSPR4
LDFLAGS += -L$(TRICEPSBASE)/cpp/build -ltriceps -lnspr4 -pthread

##g++ main.c entryPoint.c -o testCep -L. -lclips
g++ -g cep-main.c entryPoint.c -o testCep -L. -lclips -lpthread
