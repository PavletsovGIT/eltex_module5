obj-m += knetlink.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -Wall unetlink.c -o unetlink.out

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -r unetlink.out

k:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

u:
	gcc -Wall unetlink.c -o unetlink.out

cleank:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

cleanu:
	rm -r unetlink.out