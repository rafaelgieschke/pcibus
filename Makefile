modname = debug
modsource = debug

bus = 0
device = 0
function = 0

run: $(modname).ko
	sudo rmmod $(modname).ko || :
	sudo insmod $(modname).ko bus=$(bus) device=$(device) function="$(function)" \
	  2>&1 | tee /dev/stderr | grep -qi "resource busy"
	journalctl -rk -n 20 -g $(modname): | sed -n '1!G;h;$$p'

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
