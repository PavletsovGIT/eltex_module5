obj-m += knetlink.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -Wall unetlink.c -o unetlink

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -r unetlink

kernnl:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

usersp:
	gcc -Wall unetlink.c -o unetlink

cleank:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

cleanu:
	rm -r unetlink