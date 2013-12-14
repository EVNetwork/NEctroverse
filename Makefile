#Check for colorgcc installation, use it if found.
ifeq ($(wildcard /usr/bin/colorgcc),) 
CC = gcc
else
CC = colorgcc
endif
CONFIGS := $(shell cat config/config.h)
SQLLIBS := $(shell mysql_config --libs)
SQLFLAG := $(shell mysql_config --cflags)

MODS = 
#The standard config needed to compile basic server, withought these it won't work.
FLAGS = --fast-math -Wall -fno-strict-aliasing -O2 -O3
LIBS = -lcrypt -lpng

ifneq ($(findstring HTTPS_SUPPORT 1,$(CONFIGS)),)
LIBS += -lgcrypt -lgnutls
endif

ifneq ($(findstring DEBUG_SUPPORT 1,$(CONFIGS)),)
DEFS = -ggdb -rdynamic
endif

ifneq ($(findstring IRCBOT_SUPPORT 1,$(CONFIGS)),)
MODS = ircbot.o
endif


ifneq ($(findstring MYSQL_SUPPORT 1,$(CONFIGS)),)
FLAGS += $(SQLFLAG)
LIBS += $(SQLLIBS)
MODS = mysql.o
else
LIBS += -lm -pthread
endif

HEAD = *.h http/*.h extras/*.h

# Right then, now we know all of that... lets build something!
server: main.o io.o http.o db.o cmd.o html.o map.o encrypt.o $(MODS)
	$(CC) main.o io.o http.o db.o cmd.o html.o map.o encrypt.o $(MODS) $(DEFS) -o evserver $(FLAGS) $(LIBS)

run:	server
	sudo service evserver start

stop:	
	sudo service evserver stop
	
restart: stop run
	
main.o: $(HEAD) main.c extras/iniparser.c
	$(CC) main.c $(DEFS) -o main.o -c $(FLAGS)

io.o: $(HEAD) io.c iohttpvars.c iohttp.c iohttpmime.c extras/url_parser.c
	$(CC) io.c $(DEFS) -o io.o -c $(FLAGS)

http.o: $(HEAD) http/connection.c http/connection_https.c http/daemon.c http/http.c http/internal.c http/memorypool.c http/postprocessor.c http/reason_phrase.c http/response.c http/pagelist.c 
	$(CC) http/http.c $(DEFS) -o http.o -c $(FLAGS)

db.o: $(HEAD) db.c
	$(CC) db.c $(DEFS) -o db.o -c $(FLAGS)

cmd.o: $(HEAD) cmd.c cmdexec.c cmdtick.c battle.c specop.c artefact.c
	$(CC) cmd.c $(DEFS) -o cmd.o -c $(FLAGS)

map.o: $(HEAD) map.c extras/imgpng.c
	$(CC) map.c $(DEFS) -o map.o -c $(FLAGS)

html.o: $(HEAD) html.c html_main.c html_ajax.c html_admin.c html_gameplay.c html_user.c html_forum.c html_status.c extras/cpuinfo.c
	$(CC) html.c $(DEFS) -o html.o -c $(FLAGS)

encrypt.o: $(HEAD) extras/*.h extras/encrypt.c extras/md5.c extras/base64.c
	$(CC) extras/encrypt.c $(DEFS) -o encrypt.o -c $(FLAGS)

ircbot.o: $(HEAD) ircbot/ircbot.c ircbot/libircclient.c
	$(CC) ircbot/ircbot.c $(DEFS) -o ircbot.o -c $(FLAGS)

#I hate to point out the ovbious, but these are just used for cleaning things up a bit.
clean:
	rm *~ -rf
	rm *.o -rf
	rm *.raw -rf
	rm test_modules/*.o -rf
	rm test_modules/*~ -rf

#Dangerous, will wipe everything... whole database the works. Nothing is spared.
#But I do a lot of clean testing, so I like to have this one handy. It doesn't build anything.
#Can also be used in unison such as "make blank server", same for above "make clean server" works.
blank: clean
	rm core -rf
	rm /tmp/evcore -rf

#Not yet in deployment. This is just my testing section.
mysql.o: test_modules/mysql.c test_modules/mysql.h *.h
	$(CC) test_modules/mysql.c $(DEFS) -o mysql.o -c $(FLAGS) $(LIBS)

mysqltest: mysql.o md5.o
	$(CC) mysql.o md5.o $(DEFS) -o mysqltest $(FLAGS) $(LIBS)

