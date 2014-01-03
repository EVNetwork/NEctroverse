#ifndef MAININCLUDES
#define MAININCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

/**
 * Mutex used when we update stuff.
 */
extern pthread_mutex_t mutex;


extern char *cmdSignalNames[SIGNALS_NUMUSED];

//config struct
typedef struct {
	//server
	char *servername;
	char *cookdomain;
	char *directory;
	char *downfrom;
	char *httpimages;
	char *httpfiles;
	char *httpread;
	char *pubforum;
	int notices;
	int httpport;
	#if HTTPS_SUPPORT
	int httpsport;
	#endif
	int stockpile;
	int warend;
	int victory;
	int ticktime;
	int round;
	//syslog
	char* syslog_tagname;
	char* syslog_facility;
	//emergency breaker
	bool shutdown;
	//Auto Toggles
	bool autostart;
	bool autostop;
	struct tm start;
	struct tm stop;
} configDef, *configPtr;

extern configDef sysconfig;

typedef struct {
	//admin
	int empire;
	int numadmins;
	int numfakes;
	bool rankommit;
	int *race;
	int *level;
	char *ename;
	char *epassword;
	char **name;
	char **faction;
	char **password;
	char **forumtag;
} adminDef, *adminPtr;

extern adminDef admincfg;

#if FACEBOOK_SUPPORT

typedef struct {
	//Facebook UserData
	char *app_id;
	char *app_secret;
	char *app_token;
} FBCfgDef, *FBCfgPtr;

extern FBCfgDef fbcfg;

#endif

#if IRCBOT_SUPPORT
typedef struct {
	//admin
	bool bot;
	char *host;
	unsigned short port;
	char *botnick;
	char *botpass;
	char *channel;
	bool announcetick;
	irc_session_t *session;
	pthread_t thread;
} ircDef, *ircPtr;

extern ircDef irccfg;
#endif

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
} optionsDef, *optionsPtr;

extern optionsDef options;

typedef struct {
	//mysql
	bool enable;
	char *host;
	int port;
	char *user;
	char *password;
	char *database;
} mySqlDef, *mySqlPtr;

extern mySqlDef mysqlcfg;


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
} tickDef, *tickPtr;

extern tickDef ticks;
extern int savetickconfig();


int file_exist(char *filename);
int loadconfig( char *file, int type );

char *trimwhitespace(char *str);
char** str_split(char* str, char delim, int* numSplits );
void dirstructurecheck(char *directory);

void cleanUp(int type);

void svPipeScan(int pipefileid);
int svPipeSend(int pipedirection, char *message, ...);

void httpString( ReplyDataPtr rd, char *string );
void httpPrintf( ReplyDataPtr rd, char *string, ... );


typedef struct {
	//new ban listing
	int number;
	char **ip;

} svbanDef, *svbanPtr;

extern svbanDef banlist;

int bitflag( int dest, int flag );
void bitflag_add( int *dest, int flag );
void bitflag_remove( int *dest, int flag );
void bitflag_toggle( int *dest, int flag );

#define MAXLOGSTRING 1024
void loghandle( int flag, int error, char *fmt, ... );
extern char logString[MAXLOGSTRING];

extern bool firstload;
char *itoa(int i);

void svSignal( int signal );

#endif
