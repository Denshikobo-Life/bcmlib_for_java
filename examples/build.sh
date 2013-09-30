#!/bin/sh

if [ ! -f "dkl/bcm2835.class" ] ; then
     cd ../bcm2835; ./build.sh
fi

javac -cp ./:dkl/bcm2835.class $1.java

