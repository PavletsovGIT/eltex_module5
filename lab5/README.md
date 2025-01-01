# Lab 5

## System Requirements

Kernel version:		6.8.0-50-generic
Operating System:	Ubuntu Mate 22.04

## Description

`knetlink` is echo module. `unetlink.out` can send message to this module and receive message + KERN_TAG.

### Example:

unetlink.out: send		`Salam`
unetlink.out: receive	`Salam by kernel`