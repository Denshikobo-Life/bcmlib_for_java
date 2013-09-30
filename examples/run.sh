#!/bin/sh

if [ ! -f "/tmp/shm" ] ; then
     dd if=/dev/zero of=/tmp/shm bs=4096 count=1
fi

java -cp ./:lib/jna.jar:lib/jna-platform.jar:dkl/bcm2835.class:lib Blink /tmp/shm
