#Check for colorgcc installation, use it if found.
ifeq ($(wildcard /usr/bin/colorgcc),) 
CC = gcc
else
CC = colorgcc
endif
CONFIGS := $(shell cat config/buildflags.h)
OBJDIR := .libs
OBJS := $(addprefix $(OBJDIR)/,main.o io.o http.o db.o cmd.o html.o map.o artefact.o extras.o )
#The standard config needed to compile basic server, withought these it won't work.
FLAGS = -O2 -O3 --fast-math -Wall #-Wextra
LIBS = -lcrypt -lpng


ifneq ($(findstring HTTPS_SUPPORT 1,$(CONFIGS)),)
LIBS += -lgcrypt -lgnutls
endif

ifneq ($(findstring FACEBOOK_SUPPORT 1,$(CONFIGS)),)
LIBS += -lcurl
endif

ifneq ($(findstring DEBUG_SUPPORT 1,$(CONFIGS)),)

ifneq ($(findstring MYSQL_SUPPORT 1,$(CONFIGS)),)
DEFS = -rdynamic
else
DEFS = -g -rdynamic
endif

endif

ifneq ($(findstring IRCBOT_SUPPORT 1,$(CONFIGS)),)
OBJS += $(addprefix $(OBJDIR)/,ircbot.o )
endif

ifneq ($(findstring MYSQL_SUPPORT 1,$(CONFIGS)),)
SQLFLAG := $(shell mysql_config --cflags)
SQLLIBS := $(shell mysql_config --libs)
FLAGS += $(SQLFLAG)
LIBS += $(SQLLIBS)
OBJS += $(addprefix $(OBJDIR)/,mysql.o )
else
LIBS += -lm -pthread
endif

HEAD = *.h config/*.h

# Right then, now we know all of that... lets build something!
server: $(OBJS)
	$(CC) $(OBJS) $(MODLIBS) $(DEFS) -o evserver $(FLAGS) $(LIBS)

run:	server
	sudo service evserver start

stop:	
	sudo service evserver stop
	
restart: stop run
	
$(OBJDIR)/main.o: $(HEAD) main.c loadini.c
	$(CC) main.c $(DEFS) -o $(OBJDIR)/main.o -c $(FLAGS)

$(OBJDIR)/io.o: $(HEAD) io.c iohttpvars.c iohttp.c iohttpmime.c
	$(CC) io.c $(DEFS) -o $(OBJDIR)/io.o -c $(FLAGS)

$(OBJDIR)/http.o: $(HEAD) http/*.h http/*.c 
	$(CC) http/http.c $(DEFS) -o $(OBJDIR)/http.o -c $(FLAGS)

$(OBJDIR)/db.o: $(HEAD) db.c
	$(CC) db.c $(DEFS) -o $(OBJDIR)/db.o -c $(FLAGS)

$(OBJDIR)/cmd.o: $(HEAD) cmd.c cmdexec.c cmdtick.c battle.c specop.c artefact.c
	$(CC) cmd.c $(DEFS) -o $(OBJDIR)/cmd.o -c $(FLAGS)

$(OBJDIR)/map.o: $(HEAD) map.c
	$(CC) map.c $(DEFS) -o $(OBJDIR)/map.o -c $(FLAGS)

$(OBJDIR)/artefact.o: $(HEAD) artefact.c
	$(CC) artefact.c $(DEFS) -o $(OBJDIR)/artefact.o -c $(FLAGS)

$(OBJDIR)/html.o: $(HEAD) html/*.h html/*.c
	$(CC) html/html.c $(DEFS) -o $(OBJDIR)/html.o -c $(FLAGS)

$(OBJDIR)/extras.o: $(HEAD) extras/*.h extras/*.c
	$(CC) extras/extras.c $(DEFS) -o $(OBJDIR)/extras.o -c $(FLAGS)

$(OBJDIR)/ircbot.o: $(HEAD) ircbot/*.h ircbot/*.c
	$(CC) ircbot/ircbot.c $(DEFS) -o $(OBJDIR)/ircbot.o -c $(FLAGS)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

#I hate to point out the ovbious, but these are just used for cleaning things up a bit.
clean:
	rm *~ -rf
	rm .libs -rf

#Dangerous, will wipe everything... whole database the works. Nothing is spared.
#But I do a lot of clean testing, so I like to have this one handy. It doesn't build anything.
#Can also be used in unison such as "make blank server", same for above "make clean server" works.
blank: clean
	rm data_core -rf
	rm /tmp/evcore -rf

#Not yet in deployment. This is just my testing section.
$(OBJDIR)/mysql.o: test_modules/mysql.c test_modules/*.h *.h
	$(CC) test_modules/mysql.c $(DEFS) -o $(OBJDIR)/mysql.o -c $(FLAGS) $(LIBS)

mysqltest: mysql.o extras.o
	$(CC) $(OBJDIR)/mysql.o $(OBJDIR)/extras.o $(DEFS) -o mysqltest $(FLAGS) $(LIBS)

