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

/*
 * We store data in IRC session context.
 */
typedef struct {
	char 	* channel;
	char 	* nick;
	//std::map <std::string, unsigned int> insolents;
} irc_ctx_t;



int ircbot_connect();

void ircbot_select();

#if PIPEFILE_SUPPORT
int ircbot_command( char *command );
#endif

#endif
