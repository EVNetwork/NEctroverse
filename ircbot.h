#define IRCINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.

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
