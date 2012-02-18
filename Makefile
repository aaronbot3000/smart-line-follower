PROJ = smart-line-follower
OBJS =  line-follow.o camera.o line-find.o serial-interface.o#debug-interface.o

#CC_PREFIX = /media/storage/LargeFiles/angstrom/setup-scripts/build/tmp-angstrom_2010_x-eglibc/sysroots/x86_64-linux/usr/bin/armv7a-angstrom-linux-gnueabi/arm-angstrom-linux-gnueabi-
CC_PREFIX=
CC=g++
CFLAGS= -c -O3 -Wall -Wextra #-DDEBUG
LIBS=-lopencv_core -lopencv_highgui -lopencv_imgproc

all: $(PROJ)

$(PROJ): $(OBJS)
	$(CC_PREFIX)$(CC) $(LIBS) $(OBJS) -o $(PROJ)

line-follow.o: line-follow.cpp line-follow.hpp
	$(CC_PREFIX)$(CC) $(CFLAGS) line-follow.cpp

line-find.o: line-find.cpp line-find.hpp
	$(CC_PREFIX)$(CC) $(CFLAGS) line-find.cpp

camera.o: camera.cpp camera.hpp
	$(CC_PREFIX)$(CC) $(CFLAGS) camera.cpp

serial-interface.o: serial-interface.cpp serial-interface.hpp
	$(CC_PREFIX)$(CC) $(CFLAGS) serial-interface.cpp

clean:
	rm -rf *.o $(PROJ)

run: $(PROJ)
	./$(PROJ)

scp: $(PROJ)
	scp $(PROJ) beagleboard:~
