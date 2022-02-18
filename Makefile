modname = pipestat
modsource = pipestat

all: $(modname).ko

KDIR = /lib/modules/$(shell uname -r)/build
ccflags-y += $(CFLAGS)

obj-m += $(modname).o
ifneq ($(modname), $(modsource))
	$(modname)-objs = $(modsource).o
endif

$(modname).ko: $(modsource).c
	make -C $(KDIR) M=$(PWD)

clean:
	make -C $(KDIR) M=$(PWD) clean

test: $(modname).ko
	sudo rmmod $(modname).ko || :
	sudo insmod $(modname).ko path=$(path) || :
	dmesg | grep -A10 -- $(modname):
