PROJ = line-follow2
OBJS =  #debug-interface.o

#CC_PREFIX = /usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/bin/
CC_PREFIX=
CC = cc
CFLAGS = -c -O3 -Wall -Wextra #-DDEBUG
LIBS = -lopencv_core -lopencv_highgui

all: $(PROJ)

$(PROJ): $(OBJS)
	$(CC_PREFIX)$(CC) $(LIBS) $(OBJS) -o $(PROJ)

clean:
	rm -rf *.o $(PROJ)

run: $(PROJ)
	./$(PROJ)

scp: $(PROJ)
	scp $(PROJ) playaudio.sh root@143.215.108.218:~
