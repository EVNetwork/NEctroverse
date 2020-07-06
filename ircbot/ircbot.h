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

extern float ircbot_version;

int ircbot_prepare();

void IRCSelect();

void *ircbot_connect();

#if PIPEFILE_SUPPORT
int ircbot_command( char *command );
#endif

#endif
