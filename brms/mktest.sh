TRICEPSBASE=$(HOME)/qiuhj/cep/triceps-2.0.0
CLIPSBASE=$(HOME)/qiuhj/cep/clips_core_source_630
export CFLAGS = -I$(TRICEPSBASE)/cpp -I$(TRICEPSBASE)/cpp/app -I$(TRICEPSBASE)/cpp/pw -I$(TRICEPSBASE)/cpp/common -I$(TRICEPSBASE)/cpp/mem -I$(TRICEPSBASE)/cpp/sched  \
                -I$(TRICEPSBASE)/cpp/table -I$(TRICEPSBASE)/cpp/type -I$(TRICEPSBASE)/cpp/wrap  -DTRICEPS_NSPR4 -I$(CLIPSBASE)/core -I./common -I./event -I./signaladapter -I./streaming -I./utility -g
export LDFLAGS = -L$(TRICEPSBASE)/cpp/build -L$(CLIPSBASE)/core

