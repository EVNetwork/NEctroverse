#ifndef IRCINCLUDED
#define IRCINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

//OK, ... time to covert it to a struct for proper phrasing.
typedef struct {
	char *nick;
	char *host;
	char *input;
	char *target;
	char *how;
} ircmessageDef;

int ircbot_connect();

void ircbot_send(char *fmt, ...);

void ircbot_messagephrase(ircmessageDef *irc);

void ircbot_scan();

#if PIPEFILE_SUPPORT
int ircbot_command( char *command );
#endif

#endif
