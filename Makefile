CC = gcc
DEFS = -ggdb
FLAGS = -Wall -fno-strict-aliasing
LIBS = -lm
COLORGCC = /usr/bin/colorgcc

VARIABLE := $(shell cat config.h)
COLORGCC := $(shell test -s /usr/bin/colorgcc)

SQLLIBS := $(shell mysql_config --libs)
SQLFLAG := $(shell mysql_config --cflags)

server: colorgcc sqlcheck sv.o io.o db.o cmd.o map.o
	$(CC) sv.o io.o db.o cmd.o map.o $(DEFS) -o evserver $(FLAGS) $(LIBS)

colorgcc:
ifeq (,$(findstring $(COLORGCC),$(wildcard $(COLORGCC) )))
CC = colorgcc
endif

sqlcheck:
ifneq (,$(findstring MYSQLENABLE 1,$(VARIABLE)))
REALFLAGS = $(FLAGS) $(SQLFLAG)
REALLIBS = $(LIBS) $(SQLLIBS)
else
REALFLAGS = $(FLAGS)
REALLIBS = $(LIBS)
endif

sv.o: sv.c svban.c config.h sv.h io.h db.h cmd.h artefact.h config.h
	$(CC) sv.c $(DEFS) -o sv.o -c $(REALFLAGS)
io.o: io.c config.h sv.h io.h db.h cmd.h artefact.h config.h iohttpvars.c iohttp.c iohttp2.c iohttp3.c iohttpmime.c ioevm.c
	$(CC) io.c $(DEFS) -o io.o -c $(REALFLAGS)
db.o: db.c sv.h io.h db.h cmd.h config.h
	$(CC) db.c $(DEFS) -o db.o -c $(REALFLAGS)
cmd.o: cmd.c cmdexec.c cmdtick.c battle.c specop.c sv.h io.h db.h cmd.h artefact.h config.h
	$(CC) cmd.c $(DEFS) -o cmd.o -c $(REALFLAGS)
map.o: map.c config.h
	$(CC) map.c $(DEFS) -o map.o -c $(REALFLAGS)

map: map.o
	$(CC) map.o $(DEFS) -o map $(REALFLAGS) $(REALLIBS)

clean:
	rm *.o -rf
	rm *~ -rf

stripclean: clean
	rm core -rf
	rm /tmp/evcore -rf

fresh: stripclean server

