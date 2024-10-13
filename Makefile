# The path to the kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build

# The directory where your module source code is located
PWD := $(shell pwd)

# List of object files to build
obj-m := main.o

# Specify the source file locations
main-objs := src/main.o src/udp_server.o

# Define rules for building the module
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
