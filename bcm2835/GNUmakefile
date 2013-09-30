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

# May not need to  alter anything below this line
###############################################################################

.PHONEY: all
all: bcm2835

bcm2835: dkl/bcm2835.java lib/jna.jar lib/jna-platform.jar
	javac -cp lib/jna.jar:lib/jna-platform.jar dkl/bcm2835.java

.PHONEY: clean
clean:
	rm -f dkl/bcm2835.class dkl/*~

.PHONEY: install
install:
	mkdir -p ../examples/dkl
	cp dkl/bcm2835.class ../examples/dkl/
	mkdir -p ../examples/lib
	cp lib/*.jar ../examples/lib/

.PHONEY: uninstall
uninstall:
	cd ../examples; rm -rf dkl lib
