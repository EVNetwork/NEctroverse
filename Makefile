#Check for colorgcc installation, use it if found.
ifeq ($(wildcard /usr/bin/colorgcc),) 
CC = gcc
else
CC = colorgcc
endif
#The standard config needed to compile basic server, withought these it won't work.
REQUIRED = config.h global.h
CONFIGS := $(shell cat config.h)
FLAGS = --fast-math -Wall -fno-strict-aliasing
#The -s flag was removed from the flags above, since it strips excess headers... such as debugging.
#Purely optional, you can remove this. It adds extra debugging headers for gdb usage.
DEFS = -ggdb
#OK, now to scan the config.h file and see what we need to compile.
ifneq ($(findstring MYSQLENABLE 1,$(CONFIGS)),)
SQLLIBS := $(shell mysql_config --libs)
SQLFLAG := $(shell mysql_config --cflags)
FLAGS += $(SQLFLAG)
LIBS += $(SQLLIBS)
#MODS += mysql.o
else
LIBS += -lm
endif
#Incoming MD5 password encryption, disabled for now since it's not really deployed.
ifneq ($(findstring HASHENCRYPTION 1,$(CONFIGS)),)
LIBS += -lcrypto -lssl
MODS += md5.o
endif

#This is what enables the compile to read my real login info, I don't like even using a default.
#I have *.nogit ignored by git commits... so I can use my real info withought breach.
#If this file does not exist... it loads the default from config.h
ifneq ($(wildcard .hidden.and.nogit),) 
FLAGS += -DHAHA_MY_INFO_IS_HIDDEN
endif

# Right then, now we know all of that... lets build something!
server: sv.o io.o db.o cmd.o admin.o map.o $(MODS)
	$(CC) sv.o io.o db.o cmd.o admin.o map.o $(MODS) $(DEFS) -o evserver $(FLAGS) $(LIBS)

sv.o: sv.c svban.c sv.h io.h db.h cmd.h artefact.h $(REQUIRED)
	$(CC) sv.c $(DEFS) -o sv.o -c $(FLAGS)

io.o: io.c sv.h io.h db.h cmd.h artefact.h iohttpvars.c iohttp.c iohttp2.c iohttp3.c iohttpmime.c ioevm.c $(REQUIRED)
	$(CC) io.c $(DEFS) -o io.o -c $(FLAGS)

db.o: db.c sv.h io.h db.h cmd.h $(REQUIRED)
	$(CC) db.c $(DEFS) -o db.o -c $(FLAGS)

cmd.o: cmd.c cmdexec.c cmdtick.c battle.c specop.c sv.h io.h db.h cmd.h artefact.h $(REQUIRED)
	$(CC) cmd.c $(DEFS) -o cmd.o -c $(FLAGS)

map.o: map.c map.h $(REQUIRED)
	$(CC) map.c $(DEFS) -o map.o -c $(FLAGS)

admin.o: admin.c admin.h $(REQUIRED)
	$(CC) admin.c $(DEFS) -o admin.o -c $(FLAGS)

md5.o: optional/md5.c optional/md5.h $(REQUIRED)
	$(CC) optional/md5.c $(DEFS) -o md5.o -c $(FLAGS)

#I hate to point out the ovbious, but these are just used for cleaning things up a bit.
clean:
	rm *.o -rf
	rm *~ -rf
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

