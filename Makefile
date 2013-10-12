CONFIGS := $(shell cat config.h)
FLAGS = --fast-math -Wall -fno-strict-aliasing
DEFS = -ggdb
LIBS = -lm


server: colorgcc configcheck sv.o io.o db.o cmd.o map.o
	$(CC) sv.o io.o db.o cmd.o map.o $(DEFS) -o evserver $(FLAGS) $(LIBS)

sv.o: sv.c svban.c config.h sv.h io.h db.h cmd.h artefact.h global.h
	$(CC) sv.c $(DEFS) -o sv.o -c $(FLAGS)
io.o: io.c config.h sv.h io.h db.h cmd.h artefact.h iohttpvars.c iohttp.c iohttp2.c iohttp3.c iohttpmime.c ioevm.c global.h
	$(CC) io.c $(DEFS) -o io.o -c $(FLAGS)
db.o: db.c sv.h io.h db.h cmd.h config.h global.h
	$(CC) db.c $(DEFS) -o db.o -c $(FLAGS)
cmd.o: cmd.c cmdexec.c cmdtick.c battle.c specop.c sv.h io.h db.h cmd.h artefact.h config.h global.h
	$(CC) cmd.c $(DEFS) -o cmd.o -c $(FLAGS)
map.o: map.c config.h global.h
	$(CC) map.c $(DEFS) -o map.o -c $(FLAGS)

#mysql.o: mysql.c mysql.h
#	$(CC) mysql.c $(DEFS) -o mysql.o $(FLAGS) $(LIBS)

clean:
	rm *.o -rf
	rm *~ -rf

stripclean: clean
	rm core -rf
	rm /tmp/evcore -rf

fresh: stripclean server

colorgcc:
ifeq ($(wildcard /usr/bin/colorgcc),) 
CC = gcc
else
CC = colorgcc
endif

configcheck:
ifneq ($(findstring MYSQLENABLE 1,$(CONFIGS)),)
SQLLIBS := $(shell mysql_config --libs)
SQLFLAG := $(shell mysql_config --cflags)
FLAGS += $(SQLFLAG)
LIBS += $(SQLLIBS)
endif
ifneq ($(wildcard .hidden.and.nogit),) 
FLAGS += -DHAHA_NECRO_GOT_YOU
endif

