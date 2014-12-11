#############################
# Makefile
# Author: Lester
#############################

# cross-compile : set PREFIX and PATH
PREFIX     = /Space/ltib2/ltib/rootfs_l
CC_PATH    = /opt/freescale/usr/local/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/
CROSS      = arm-linux-

VERSION    = 1.2.5


# Compiler
HOST       = $(CC_PATH)$(CROSS)
CC         = $(HOST)gcc
DEFINES    = -D_GNU_SOURCE
CFLAGS     = -O2 -Wall -Werror -std=gnu99 -fPIC -march=armv7-a -mfpu=neon $(DEFINES)
INCPATH    = -I$(PREFIX)/usr/include -I$(PREFIX)/usr/local/include -I. -I./include
LINK       = $(HOST)gcc
LIBPATH    = -L$(PREFIX)/usr/lib -L$(PREFIX)/usr/local/lib -L$(PREFIX)/lib
RPATH      =
RPATH_LINK = $(PREFIX)/usr/lib
LFLAGS     = -rdynamic -shared -Wl,-rpath-link=$(RPATH_LINK),-soname=liblist.so.1
LIBS       = $(LIBPATH) -lpthread -lmtdev -lQSI-IPCLib 
AR         = $(HOST)ar


OBJECTS    =  $(shell ls ./src/*.c | sed 's/\.c/.o/g') \
			  $(shell ls ./src/lib/*.c | sed 's/\.c/.o/g')
# All Target
all: liblist.so.$(VERSION)

liblist.so.$(VERSION): $(OBJECTS)
	@echo 'Building target: $@'
	$(CC) -o $@ $(OBJECTS) $(LFLAGS) $(LIBS)  

$(OBJECTS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INCPATH) $< -o $@
	
clean:
	rm -f $(OBJECTS)
