#ifndef GLOBALINCLUDED
#include "global.h"
#endif

 
void ircbot_send(char *fmt, ...) {
	va_list ap;
	char sbuf[SERVER_RECV_BUFSIZE];
	char *ender = "\r\n";

va_start(ap, fmt);
vsnprintf(sbuf, SERVER_RECV_BUFSIZE, fmt, ap);
va_end(ap);
strcat(sbuf,ender);
if( options.verbose )
	printf("<< %s", sbuf);
send(options.botconn, sbuf, strlen(sbuf), 0);

return;
}


void ircbot_scan() {
	char sbuf[SERVER_RECV_BUFSIZE];
	char *command, *where, *sep;
	int i, j, l, sl, o = -1, start = 0, wordcount;
	char buf[SERVER_RECV_BUFSIZE+1];
	char bottrigger[32];
	ircmessageDef ircmsg;

sprintf(bottrigger, "!%s ", irccfg.botnick);
memset(&sbuf, 0, SERVER_RECV_BUFSIZE );
if( (sl = read(options.botconn, sbuf, SERVER_RECV_BUFSIZE)) ) {
	for (i = 0; i < sl; i++) {
		o++;
		buf[o] = sbuf[i];
		if ((i > 0 && sbuf[i] == '\n' && sbuf[i - 1] == '\r') || o == SERVER_RECV_BUFSIZE) {
			buf[o + 1] = '\0';
			l = o;
			o = -1;
			if( options.verbose )
				printf(">> %s", buf);
                
                	if (!strncmp(buf, "PING", 4)) {
				buf[1] = 'O';
				ircbot_send(buf);
			} else if (buf[0] == ':') {
				wordcount = 0;
				ircmsg.nick = command = where = ircmsg.input = ircmsg.host = NULL;
				for (j = 1; j < l; j++) {
					if (buf[j] == ' ') {
						buf[j] = '\0';
						wordcount++;
						switch(wordcount) {
							case 1: ircmsg.nick = buf + 1; ircmsg.host = ( strrchr( buf, '@' ) ? strrchr( buf, '@' )+1 : "local" ); break;
							case 2: command = buf + start; break;
							case 3: where = buf + start; break;
						}
						if (j == l - 1) continue;
						start = j + 1;
					} else if (buf[j] == ':' && wordcount == 3) {
						if (j < l - 1) ircmsg.input = buf + j + 1;
						break;
					}
				}
                    
				if (wordcount < 2) continue;
                    
				if (!strncmp(command, "001", 3) && irccfg.channel != NULL) {
					ircbot_send("JOIN %s", irccfg.channel);
					if( irccfg.botpass ){
						ircbot_send("PRIVMSG ChanServ :op %s %s", irccfg.channel, irccfg.botnick);
					}
				} else if (!strncmp(command, "PRIVMSG", 7) || !strncmp(command, "NOTICE", 6)) {
					if (where == NULL || ircmsg.input == NULL) continue;
					if ((sep = strchr(ircmsg.nick, '!')) != NULL) ircmsg.nick[sep - ircmsg.nick] = '\0';
					if( !strcmp(ircmsg.nick,"NickServ") || !strcmp(ircmsg.nick,"ChanServ") ) continue;
					if (where[0] == '#' || where[0] == '&' || where[0] == '+' || where[0] == '!') ircmsg.target = where; else ircmsg.target = ircmsg.nick;
					if( !strncmp(where, irccfg.botnick, strlen(irccfg.botnick)) ) {
						ircmsg.input = trimwhitespace(ircmsg.input);
						ircbot_messagephrase(&ircmsg);
					} else if( !strncmp(ircmsg.input, bottrigger, strlen(bottrigger)) ) {
						memmove(ircmsg.input, ircmsg.input+strlen(bottrigger), strlen(ircmsg.input));
						ircmsg.input = trimwhitespace(ircmsg.input);
						ircbot_messagephrase(&ircmsg);
					}
				}
			}
		}
	}
        
}

return;
}

int ircbot_connect(){
	int a;
	struct addrinfo hints, *res;
    
memset(&hints, 0, sizeof hints);
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
getaddrinfo(irccfg.host, irccfg.port, &hints, &res);
options.botconn = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
if( connect(options.botconn, res->ai_addr, res->ai_addrlen) == -1) {
	if( options.verbose )
		printf("Error %03d, ircbot unable to connect\n", errno );
	syslog(LOG_ERR, "Error %03d, ircbot unable to connect\n", errno );
	options.botconn = -1; irccfg.bot = false;
	return 0;
}

a = 1;
if( setsockopt( options.botconn, SOL_SOCKET, SO_REUSEADDR, (char *)&a, sizeof(int) ) == -1 ) {
	if( options.verbose )
		printf("Error %03d, set bot sockopt\n", errno );
	syslog(LOG_ERR, "Error %03d, set bot sockopt\n", errno );
	close( options.botconn );
	options.botconn = -1; irccfg.bot = false;
	return 0;
}
if( fcntl( options.botconn, F_SETFL, O_NONBLOCK ) == -1 ) {
	if( options.verbose )
		printf("Error %03d, setting non-blocking on port:\n", errno );
	syslog(LOG_ERR, "Error %03d, setting non-blocking on port:\n", errno );
	close( options.botconn );
	options.botconn = -1; irccfg.bot = false;
	return 0;
}    
ircbot_send("USER %s 0 0 :%sBot", irccfg.botnick, irccfg.botnick);
ircbot_send("NICK %s", irccfg.botnick);
if( irccfg.botpass ){
	ircbot_send("PRIVMSG NickServ :identify %s", irccfg.botpass);
}

return 1;  
}

int ircbot_command( char *command ) {
	char *sub = {0}, *chop = {0};

if( strlen(command) > 3 )
sub = ( strchr( command, ' ' ) + 1 );

if( !( sub ) )
	sub = "status";

if( !( strcmp(sub,"announce") ) ){
	if( irccfg.bot == false ) {
		svPipeSend(0,"Bot is not enabled, can't announce tick!");
		return 0;
	} 
	if( irccfg.announcetick ){
		irccfg.announcetick = false;
		svPipeSend(0,"Bot announce tick is now OFF!");
		ircbot_send("NOTICE %s :Administration has disabled channel notice of game tick!", irccfg.channel);
	} else {
		irccfg.announcetick = true;
		svPipeSend(0,"Bot announce tick is now ON!");
		ircbot_send("NOTICE %s :Administration has enabled channel notice of game tick!", irccfg.channel);
	}
	return 1;
} else if( !( strcmp(sub,"toggle") ) ){
	if( irccfg.bot == false ) {
		irccfg.bot = true;
		ircbot_connect();
	} else {
		ircbot_send("NOTICE %s :Administration has requested IRC Bot shutdown!", irccfg.channel);
		ircbot_send("PRIVMSG %s :Good bye all, untill next time! =)", irccfg.channel);
		ircbot_send("QUIT");
		if( close( options.botconn ) == -1 )
			syslog(LOG_ERR, "Error %03d, closing ircbot socket\n", errno);
		irccfg.bot = false;
	}
	svPipeSend(0,"%s IRC Bot.", irccfg.bot ? "Starting" : "Stoping" );
	return 1;
} else if( !( strncmp(sub, "hop", 3) ) ){
	if( irccfg.bot == false ) {
		svPipeSend(0,"Bot is not enabled, can't channel hop!");
		return 0;
	}
	if( strlen(sub) > 3 )
		chop = ( strrchr( sub, ' ' ) + 1 );
	if( !( chop ) )
		chop = irccfg.channel;
	irccfg.bot = false;
	ircbot_send("NOTICE %s :Administration has requested IRC Bot channel hop to %s!", irccfg.channel, chop);
	ircbot_send("PART %s", irccfg.channel);
	svPipeSend(0,"IRC Bot Left channel %s.", irccfg.channel );
	ircbot_send("JOIN %s", chop);
	strcpy(irccfg.channel,chop);
	if( irccfg.botpass ){
		ircbot_send("PRIVMSG ChanServ :op %s %s", irccfg.channel, irccfg.botnick);
	}
	svPipeSend(0,"IRC Bot joined channel %s.", chop );
	irccfg.bot = true;
	return 1;
}


return 0;
}
 
#define IS_CTRL  (1 << 0)
#define IS_EXT	 (1 << 1)
#define IS_ALPHA (1 << 2)
#define IS_DIGIT (1 << 3)
 
unsigned int char_tbl[256] = {0};
 

void init_table() {
	int i;
 
	for (i = 0; i < 32; i++) char_tbl[i] |= IS_CTRL;
	char_tbl[127] |= IS_CTRL;
 
	for (i = 'A'; i <= 'Z'; i++) {
		char_tbl[i] |= IS_ALPHA;
		char_tbl[i + 0x20] |= IS_ALPHA; /* lower case */
	}
 
	for (i = 128; i < 256; i++) char_tbl[i] |= IS_EXT;
}
 
void strip(char * str, int what) {
	unsigned char *ptr, *s = (void*)str;
	ptr = s;
	while (*s != '\0') {
		if ((char_tbl[(int)*s] & what) == 0)
			*(ptr++) = *s;
		s++;
	}
	*ptr = '\0';
}

void ircbot_messagephrase(ircmessageDef *irc) {


init_table();
strip(irc->input, IS_CTRL | IS_EXT);


if( options.verbose )
	printf("[from:%s] [host:%s] [reply-to:%s] %s\n", irc->nick, irc->host, irc->target, irc->input);


// Test responce, we just deny all ability for now... untill something's actually added.
ircbot_send("NOTICE %s :<%s> is not currently an input I recognize!", irc->target, irc->input); 

return;
}


