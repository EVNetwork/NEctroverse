#ifndef GLOBALINCLUDED
#include "global.h"
#endif


int botconn;
char sbuf[SERVER_RECV_BUFSIZE];

void ircbotsend(char *fmt, ...) {
	va_list ap;
	char *ender = "\r\n";

va_start(ap, fmt);
vsnprintf(sbuf, SERVER_RECV_BUFSIZE, fmt, ap);
va_end(ap);
strcat(sbuf,ender);
if( options.verbose )
	printf("<< %s", sbuf);
send(botconn, sbuf, strlen(sbuf), 0);

return;
}


void scanirc() {
	char *user, *command, *where, *message, *sep, *target;
	int i, j, l, sl, o = -1, start = 0, wordcount;
	char buf[SERVER_RECV_BUFSIZE+1];
	char bottrigger[32];

sprintf(bottrigger, "!%s ", irccfg.botnick);

if( (sl = read(botconn, sbuf, SERVER_RECV_BUFSIZE)) ) {
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
				ircbotsend(buf);
			} else if (buf[0] == ':') {
				wordcount = 0;
				user = command = where = message = NULL;
				for (j = 1; j < l; j++) {
					if (buf[j] == ' ') {
						buf[j] = '\0';
						wordcount++;
						switch(wordcount) {
							case 1: user = buf + 1; break;
							case 2: command = buf + start; break;
							case 3: where = buf + start; break;
						}
						if (j == l - 1) continue;
						start = j + 1;
					} else if (buf[j] == ':' && wordcount == 3) {
						if (j < l - 1) message = buf + j + 1;
						break;
					}
				}
                    
				if (wordcount < 2) continue;
                    
				if (!strncmp(command, "001", 3) && irccfg.channel != NULL) {
					ircbotsend("JOIN %s", irccfg.channel);
					if( irccfg.botpass ){
						ircbotsend("PRIVMSG ChanServ :op %s %s", irccfg.channel, irccfg.botnick);
					}
				} else if (!strncmp(command, "PRIVMSG", 7) || !strncmp(command, "NOTICE", 6)) {
					if (where == NULL || message == NULL) continue;
					if ((sep = strchr(user, '!')) != NULL) user[sep - user] = '\0';
					if (where[0] == '#' || where[0] == '&' || where[0] == '+' || where[0] == '!') target = where; else target = user;
					if( options.verbose )
						printf("[from: %s] [reply-with: %s] [where: %s] [reply-to: %s] %s", user, command, where, target, message);
					if( !strncmp(message, bottrigger, strlen(bottrigger)) ) {
						memmove(message, message+strlen(bottrigger), strlen(message));
						message = trimwhitespace(message);
		        	                ircbotsend("%s %s :%s said %s", command, target, user, message); // Test responce, just echo what whoever says...
					}
				}
			}
		}
	}
        
}

return;
}

int ircbotconnect(){
	int a;
	struct addrinfo hints, *res;
    
memset(&hints, 0, sizeof hints);
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
getaddrinfo(irccfg.host, irccfg.port, &hints, &res);
botconn = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
connect(botconn, res->ai_addr, res->ai_addrlen);

a = 1;
if( setsockopt( botconn, SOL_SOCKET, SO_REUSEADDR, (char *)&a, sizeof(int) ) == -1 ) {
	if( options.verbose )
	printf("Error %03d, set bot sockopt\n", errno );
	syslog(LOG_ERR, "Error %03d, set bot sockopt\n", errno );
	close( botconn );
	botconn = -1; irccfg.bot = false;
	return 0;
}
if( fcntl( botconn, F_SETFL, O_NONBLOCK ) == -1 ) {
	if( options.verbose )
	printf("Error %03d, setting non-blocking on port:\n", errno );
	syslog(LOG_ERR, "Error %03d, setting non-blocking on port:\n", errno );
	close( botconn );
	botconn = -1; irccfg.bot = false;
	return 0;
}    
ircbotsend("USER %s 0 0 :%s", irccfg.botnick, irccfg.botnick);
ircbotsend("NICK %s", irccfg.botnick);
if( irccfg.botpass ){
	ircbotsend("PRIVMSG NickServ :identify %s", irccfg.botpass);
}

return 1;  
}

