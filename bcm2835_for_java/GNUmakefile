#
# Makefile:
#
#	Copyright (c) 2013 Denshikobo Life Ltd.
#################################################################################
# This file is part of bcmlib_for_java:
#	library for the Raspberry Pi
#
#    bcmlib_for_javais free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    bcmlib_for_java is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with bcmlib_for_java. If not, see <http://www.gnu.org/licenses/>.
#################################################################################

DESTDIR=/usr
PREFIX=/local

CC	= gcc
INCLUDE	= -I$(DESTDIR)$(PREFIX)/include
CFLAGS	= -O2 -w $(INCLUDE)

LDFLAGS	= -L$(DESTDIR)$(PREFIX)/lib

# May not need to  alter anything below this line
###############################################################################

SRC = bcm2835_for_java.c bcm2835.c

OBJ = $(SRC:.c=.o)

.PHONEY: all
all: bcm2835_for_java

bcm2835_for_java: bcm2835_for_java.o bcm2835.o
	$(CC) $(CFLAGS) -c bcm2835_for_java.c -o bcm2835_for_java.o
	$(CC) $(CFLAGS) -c bcm2835.c -o bcm2835.o

.PHONEY: clean
clean:
	rm -rf $(OBJ)  *.o *~ core tags *.bak html

.PHONEY: install
install:
	$(CC) bcm2835_for_java.o ../common/bi_common.o bcm2835.o -o bcm2835_for_java
	sudo chown root:root bcm2835_for_java
	sudo chmod 4755 bcm2835_for_java

.PHONEY: uninstall
uninstall:
	sudo rm -f bcm2835_for_java

bcm2835_for_java.o:	bcm2835_for_java.h

