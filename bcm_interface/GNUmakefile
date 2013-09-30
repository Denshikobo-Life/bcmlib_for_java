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

SRC = bcm_interface.c bi_bcm2835.c

OBJ = $(SRC:.c=.o)

.PHONEY: all
all: bcm_interface

bcm_interface: bcm_interface.o bi_bcm2835.o
	$(CC) $(CFLAGS) -c bcm_interface.c -o bcm_interface.o
	$(CC) $(CFLAGS) -c bi_bcm2835.c -o bi_bcm2835.o
	touch bcm_interface

.PHONEY: clean
clean:
	rm -f $(OBJ) bcm_interface libbcm_interface.so.* *~ core tags *.bak

.PHONEY: install
install:
	$(CC) -shared -fpic -o libbcm_interface.so.1 bcm_interface.o ../common/bi_common.o bi_bcm2835.o
	mkdir -p ../examples/lib
	cp libbcm_interface.so.1 ../examples/lib/libbcm_interface.so
	touch bcm_interface

.PHONEY:	uninstall
uninstall:
	rm -f ../examples/lib/libbcm_interface.so bcm_interface
