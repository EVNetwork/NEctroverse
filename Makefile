CC = gcc
DEFS = 
FLAGS = -s -mtune=core2 -O2 --fast-math -Wall -fno-strict-aliasing
LIBS = -lm
COLORGCC = /usr/bin/colorgcc

VARIABLE := $(shell cat config.h)
COLORGCC := $(shell test -s /usr/bin/colorgcc)

SQLLIBS := $(shell mysql_config --libs)
SQLFLAG := $(shell mysql_config --cflags)

all: map server

server: colorgcc sqlcheck sv.o io.o db.o cmd.o
	$(CC) sv.o io.o db.o cmd.o $(DEFS) -o evserver $(FLAGS) $(LIBS)

colorgcc:
ifeq (,$(findstring $(COLORGCC),$(wildcard $(COLORGCC) )))
CC = colorgcc
endif

sqlcheck:
ifneq (,$(findstring MYSQLENABLE 1,$(VARIABLE)))
FLAGS = -s -mtune=core2 -O2 --fast-math -Wall $(SQLFLAG)
LIBS = $(SQLLIBS)
else
FLAGS = -s -mtune=core2 -O2 --fast-math -Wall -fno-strict-aliasing
LIBS = -lm
endif

sv.o: sv.c svban.c config.h sv.h io.h db.h cmd.h artefact.h config.h
	$(CC) sv.c $(DEFS) -o sv.o -c $(FLAGS)
io.o: io.c config.h sv.h io.h db.h cmd.h artefact.h config.h iohttpvars.c iohttp.c iohttp2.c iohttp3.c iohttpmime.c ioevm.c
	$(CC) io.c $(DEFS) -o io.o -c $(FLAGS)
db.o: db.c sv.h io.h db.h cmd.h config.h
	$(CC) db.c $(DEFS) -o db.o -c $(FLAGS)
cmd.o: cmd.c cmdexec.c cmdtick.c battle.c specop.c sv.h io.h db.h cmd.h artefact.h config.h
	$(CC) cmd.c $(DEFS) -o cmd.o -c $(FLAGS)

clean:
	rm *.o -rf
strip:
	rm *.o -rf
	rm core -rf

map: map.o
	$(CC) map.o $(DEFS) -o map $(FLAGS) $(LIBS)

map.o: map.c config.h
	$(CC) map.c $(DEFS) -o map.o -c $(FLAGS)


