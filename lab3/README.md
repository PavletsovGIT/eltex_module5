# Lab 3

Pavletsov Feodor

## Test 1

start blindings mode: 100

### Zametki

1. foo_show - cat
2. foo-store - echo

## Algorithm

1. 
```C
insmod lab3.ko
```
2. 
``` C
echo [num] > /sys/lab3test/test
```
	2.1 sfs_store()
		2.1.1 module set num to `sys/lab3/test`
		2.1.2 module update vaulue of timer mode
		2.1.3 `lab3_restart_bleds()`  - update blinding lights
