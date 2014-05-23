#!/bin/sh
. /root/qt-4.7.4.mk 
#arm-linux-gcc list.c -o list -O2 -march=armv7-a -mfpu=neon -std=gnu99 -llist -pthread -L/Space/ltib2/ltib/rootfs/usr/lib -I/Space/ltib2/ltib/rootfs/usr/include
#arm-linux-gcc sample_list.c -o list -O2 -march=armv7-a -mfpu=neon -std=gnu99 -llist -pthread -L/Space/ltib2/ltib/rootfs/usr/lib -I/Space/ltib2/ltib/rootfs/usr/include
#gcc list.c -o list -Wall -g3 -std=gnu99 -llist


#arm-linux-gcc list_interactive.c -o list -O2 -march=armv7-a -mfpu=neon -std=gnu99 -llist -pthread -lncurses -lreadline -L/Space/ltib2/ltib/rootfs/usr/lib -I/Space/ltib2/ltib/rootfs/usr/include

gcc list_interactive.c -o list -Wall -g3 -std=gnu99 -llist -lreadline -lncurses
