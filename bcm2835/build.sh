#!/bin/sh

javac -cp lib/jna.jar:lib/jna-platform.jar dkl/bcm2835.java
mkdir ../examples/dkl
cp dkl/bcm2835.class ../examples/dkl/
mkdir ../examples/lib
cp lib/*.jar ../examples/lib/
