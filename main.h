#ifndef MAININCLUDES
#define MAININCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

#if PIPEFILE_SUPPORT
#include "pipefile.h"
#endif

#include "loadini.h"

/**
 * Mutex used when we update stuff.
 */
extern pthread_mutex_t mutex;


//config struct
typedef struct {
	int number;
	char **ip;
} IPBanDef, *IPBanPtr;

typedef struct {
	//emergency breaker
	bool shutdown;
	//Auto Toggles
	bool autostart;
	bool autostop;
	struct tm start;
	struct tm stop;
	#if IRCBOT_SUPPORT
	bool irc_enabled;
	bool irc_announce;
	#if MULTI_THREAD_SUPPORT
	pthread_t irc_thread;
	#endif
	irc_session_t *irc_session;
	#endif
	#if FACEBOOK_SUPPORT
	char *facebook_token;
	#endif
	IPBanDef banlist;
} SystemCoreDef, *SystemCorePtr;

extern SystemCoreDef sysconfig;

typedef struct {
	//startup options
	int mode;
	int port[PORT_TOTAL];
	int serverpipe;
	int clientpipe;
	int botconn;
	bool verbose;
	char sysini[PATH_MAX];
	char mapini[PATH_MAX];
	char pipefile[PATH_MAX];
	char pipestring[128];
} ServerOptionsDef, *ServerOptionsPtr;

extern ServerOptionsDef options;


typedef struct {
	bool status;
	bool locked;
	int number;
	int round;
	int speed;
	int last;
	int next;
	int debug_id;
	int debug_pass;
	int uonline;
	int uactive;
	int uregist;
} TickInfoDef, *TickInfoPtr;

extern TickInfoDef ticks;
extern int savetickconfig();


int file_exist(char *filename);
int loadconfig( char *file, int type );

char *trimwhitespace(char *str);
char** str_split(char* str, char delim, int* numSplits );
int dirstructurecheck( char *directory, int verbose );

void cleanUp(int type);

void svPipeScan(int pipefileid);
int svPipeSend(int pipedirection, char *message, ...);


int bitflag( int dest, int flag );
void bitflag_add( int *dest, int flag );
void bitflag_remove( int *dest, int flag );
void bitflag_toggle( int *dest, int flag );

#define MAXLOGSTRING 1024
void loghandle( int flag, int error, char *fmt, ... );

extern bool firstload;

extern char *cmdSignalNames[SIGNALS_NUMUSED];

char *str_replace(char *orig, char *rep, char *with);

#endif
