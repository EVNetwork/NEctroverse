#define IRCINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.

int ircbotconnect();

void ircbotsend(char *fmt, ...);
void scanirc();


extern int botconn;
