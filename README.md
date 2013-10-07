# bcmlib_for_java
===============

Interface between Java app. and bcm2835 library(written by C)

This is the program designed to operate GPIO, I2C, UART and others on Raspberry Pi board with Java.
The existing C library for Raspberry Pi( BCM2835 offered by Mike McCauley ) is used in. 
Most functions which this program offers are the wrappers of the BCM2835 library. 
The great feature is that Java application does not run as root.

This is Shared library. So, it may use also in languages other than Java. 

## Installation

Procedure to setup this program is as follows.   
 $ git clone git://github.com/Denshikobo-Life/bcmlib_for_java.git  
 $ cd bcmlib_for_java  
 $ make  
 $ make install  
 $ make doc  

## Test Blink.java

Procedure to use this program is as follows.  
$ cd bcmlib_for_java/examples  
$ ./build.sh Blink  
$ ./run.sh Blink  

## Other Infomation

Pleas see two index.html at doxy_bcmlib_for_java/html/  
and bcm2835_for_java/html
(It will create at "make doc" operation )  
