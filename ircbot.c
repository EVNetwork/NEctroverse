#ifndef GLOBALINCLUDED
#include "global.h"
#endif

void ircbot_send(char *fmt, ...) {
	va_list ap;
	char sbuf[512];
	char *ender = "\r\n";

va_start(ap, fmt);
vsnprintf(sbuf, 512, fmt, ap);
va_end(ap);
strncat(sbuf, ender, 512 - strlen(sbuf) - 1);
if( !( send(options.botconn, sbuf, strlen(sbuf), 0) ) )
	loghandle(LOG_INFO, false, "Failed Sending to IRC: %s", trimwhitespace(strdup(sbuf))); //We duplicate the output, triming the enders for IRC off, and add the shell end back on... just to stop blank lines.
return;
}


void ircbot_scan() {
	char sbuf[512];
	char *command, *where, *sep;
	int i, j, l, sl, o = -1, start = 0, wordcount;
	char buf[512+1];
	char bottrigger[32];
	ircmessageDef ircmsg;

sprintf(bottrigger, "!%s ", irccfg.botnick);
memset(&sbuf, 0, 512 );
if( ( sl = recv(options.botconn, sbuf, 512, 0) ) ){
	for (i = 0; i < sl; i++) {
		o++;
		buf[o] = sbuf[i];
		if ((i > 0 && sbuf[i] == '\n' && sbuf[i - 1] == '\r') || o == 512) {
			buf[o + 1] = '\0';
			l = o;
			o = -1;
			//if( options.verbose )
			//	loghandle(LOG_INFO, false, ">> %s", buf); //Purely optional, and not really suggested... since it will output EVERYTHING from the IRC network.

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

				if (!strncmp(command, "001", 3) && ( irccfg.channel != NULL ) ) {
					ircbot_send("JOIN %s", irccfg.channel);
					if( irccfg.botpass ){ //We always request OP, we don't care what channel it is... we just request it anyways.
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
				} else if( !strncmp(command, "PART", 4) || !strncmp(command, "JOIN", 4) ) {
					if ((sep = strchr(ircmsg.nick, '!')) != NULL) ircmsg.nick[sep - ircmsg.nick] = '\0';
					if( !strcmp(ircmsg.nick,"NickServ") || !strcmp(ircmsg.nick,"ChanServ") || !strcmp(ircmsg.nick,irccfg.botnick) ) continue;
					//Trigger for when someone leaves/joins the channel -- We only use the one channel, so we won't bother checking where they are.
					loghandle(LOG_INFO, false, "%s from host \'%s\' %s channel.", ircmsg.nick, ircmsg.host, ( !strncmp(command, "JOIN", 4) ? "Joined" : "Left" ) );
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
a = 1;
if( setsockopt( options.botconn, IPPROTO_TCP, TCP_NODELAY, (char *)&a, sizeof(int) ) == -1 ) {
	loghandle(LOG_ERR, errno, "Error %03d, set bot sockopt", errno );
	close( options.botconn );
	options.botconn = -1; irccfg.bot = false;
	return 0;
}
if( ( connect(options.botconn, res->ai_addr, res->ai_addrlen) == -1 ) ) {
	loghandle(LOG_ERR, errno, "Error %03d, ircbot unable to connect", errno );
	options.botconn = -1; irccfg.bot = false;
	return 0;
}
if( fcntl( options.botconn, F_SETFL, O_NONBLOCK ) == -1 ) {
	loghandle(LOG_ERR, errno, "Error %03d, setting non-blocking on port:", errno );
	close( options.botconn );
	options.botconn = -1; irccfg.bot = false;
	return 0;
}

ircbot_send("USER %s 0 0 :%s", irccfg.botnick, sysconfig.servername);
ircbot_send("NICK %s", irccfg.botnick);
if( irccfg.botpass ){
	ircbot_send("PRIVMSG NickServ :identify %s", irccfg.botpass);
}

return 1;
}
#if PIPEFILE
int ircbot_command( char *command ) {
	char *sub = {0}, *chop = {0};

if( ( strlen(command) > 3 ) && ( strchr( command, ' ' ) ) )
sub = ( strchr( command, ' ' ) + 1 );

if( !( sub ) )
	sub = "status";

if( !( strcmp(sub,"status") ) ){
	if( irccfg.bot == false ) {
		svPipeSend(0,"IRC Bot is not curently enabled!");
		return 0;
	} else {
		svPipeSend(0,"IRC Bot is curently enabled with host \'%s\' for channel \'%s\'", irccfg.host, irccfg.channel);
	}
	return 1;
} else if( !( strcmp(sub,"announce") ) ){
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
		ircbot_send("QUIT :Administration has requested IRC Bot shutdown!");
		if( close( options.botconn ) == -1 ) {
			loghandle(LOG_ERR, errno, "Error %03d, closing ircbot socket", errno);
		}
		irccfg.bot = false;
	}
	svPipeSend(0,"%s IRC Bot.", irccfg.bot ? "Starting" : "Stoping" );
	return 1;
} else if( !( strncmp(sub, "hop", 3) ) ){
	if( irccfg.bot == false ) {
		svPipeSend(0,"IRC Bot is not enabled, can't channel hop!");
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
#endif

#define IS_CTRL  (1 << 0)
#define IS_EXT	 (1 << 1)
#define IS_ALPHA (1 << 2)

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
/* Experimental, got the idea from a forum page... not sure how it's going to work. But this will be used in Login.
char *token, *string;

string = strdup(irc->input);
if(string != NULL)
while( (token = strsep(&string, " ") ) != NULL ) {
          printf("%s\n", token);
}
free(string);
*/
if( options.verbose )
	loghandle(LOG_INFO, false, "[from:%s] [host:%s] [reply-to:%s] %s", irc->nick, irc->host, irc->target, irc->input);

if( !strcmp(irc->input,"tick") ){
	if( ticks.status ) {
		ircbot_send("NOTICE %s :Week %d, Year %d (Tick #%d)", irc->target, ticks.number % 52, ticks.number / 52, ticks.number );
	} else {
		ircbot_send("NOTICE %s :Game time is frozen!", irc->target );
	}
} else if( !strncmp(irc->input,"login", 5) ) {
	printf("%s of host:%s attempted to login via IRC bot.\n", irc->nick, irc->host);
	if(irc->target != irc->nick) {
		ircbot_send("NOTICE %s :Login via public is so unsafe... I'm going to deny login.", irc->nick);
		ircbot_send("NOTICE %s :Please only use Private Message or Notice to login.", irc->nick);
		ircbot_send("NOTICE %s :Sorry %s, your login attempt is invalid!", irc->target, irc->nick);
		return;
	}
	ircbot_send("NOTICE %s :Sorry, %s is currently still under construction!", irc->target, irc->input);
} else {
	//Deny ability for any input not listed above.
	ircbot_send("NOTICE %s :<%s> is not currently a command I recognize!", irc->target, irc->input);
}

return;
}


