#Check for colorgcc installation, use it if found.
ifeq ($(wildcard /usr/bin/colorgcc),) 
CC = gcc
else
CC = colorgcc
endif
SQLLIBS := $(shell mysql_config --libs)
SQLFLAG := $(shell mysql_config --cflags)

#The standard config needed to compile basic server, withought these it won't work.
REQUIRED = global.h
FLAGS = $(SQLFLAG) --fast-math -Wall -fno-strict-aliasing -lpng -O3 
LIBS = $(SQLLIBS) -lcrypt -lcrypto -lssl

#Purely optional, you can remove this. It adds extra debugging headers for gdb usage.
DEFS = -ggdb


#This is what enables the compile to read my real login info, I don't like even using a default.
#I have *.nogit ignored by git commits... so I can use my real info withought breach.
#If this file does not exist... it loads the default from config.h
ifneq ($(wildcard svconfig.nogit.ini),) 
FLAGS += -DHAHA_MY_INFO_IS_HIDDEN
endif

# Right then, now we know all of that... lets build something!
server: sv.o io.o db.o cmd.o html.o map.o md5.o $(MODS)
	$(CC) sv.o io.o db.o cmd.o html.o map.o md5.o $(MODS) $(DEFS) -o evserver $(FLAGS) $(LIBS)

sv.o: sv.c svban.c sv.h io.h db.h cmd.h artefact.h $(REQUIRED)
	$(CC) sv.c $(DEFS) -o sv.o -c $(FLAGS)

io.o: io.c sv.h io.h db.h cmd.h artefact.h iohttpvars.c iohttp.c iohttpmime.c ioevm.c $(REQUIRED)
	$(CC) io.c $(DEFS) -o io.o -c $(FLAGS)

db.o: db.c sv.h io.h db.h cmd.h $(REQUIRED)
	$(CC) db.c $(DEFS) -o db.o -c $(FLAGS)

cmd.o: cmd.c cmdexec.c cmdtick.c battle.c specop.c sv.h io.h db.h cmd.h artefact.h $(REQUIRED)
	$(CC) cmd.c $(DEFS) -o cmd.o -c $(FLAGS)

map.o: map.c map.h $(REQUIRED)
	$(CC) map.c $(DEFS) -o map.o -c $(FLAGS)

html.o: html.c html_main.c html_admin.c html_gameplay.c html_user.c html_forum.c html_status.c $(REQUIRED)
	$(CC) html.c $(DEFS) -o html.o -c $(FLAGS)

md5.o: md5.c md5.h $(REQUIRED)
	$(CC) md5.c $(DEFS) -o md5.o -c $(FLAGS)

#I hate to point out the ovbious, but these are just used for cleaning things up a bit.
clean:
	rm *~ -rf
	rm *.o -rf
	rm *.raw -rf
	rm optional/*.o -rf
	rm optional/*~ -rf

#Dangerous, will wipe everything... whole database the works. Nothing is spared.
#But I do a lot of clean testing, so I like to have this one handy. It doesn't build anything.
#Can also be used in unison such as "make blank server", same for above "make clean server" works.
blank: clean
	rm core -rf
	rm /tmp/evcore -rf

#Not yet in deployment. This is just my testing section.
mysql.o: optional/mysql.c optional/mysql.h $(REQUIRED)
	$(CC) optional/mysql.c $(DEFS) -o mysql.o -c $(FLAGS) $(LIBS)

mysqltest: mysql.o md5.o
	$(CC) mysql.o md5.o $(DEFS) -o mysqltest $(FLAGS) $(LIBS)

