#ifndef MAININCLUDES
#define MAININCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

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
	int httpport;
	int stockpile;
	int warend;
	int victory;
	int ticktime;
	int round;
	//evmap
	bool evmpactv;
	int evmpport;
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
	inikey ini;
} configDef, *configPtr;

extern configDef sysconfig;

typedef struct {
	//admin
	int empire;
	int numadmins;
	bool rankommit;
	int *race;
	int *level;
	char *ename;
	char *epassword;
	char **name;
	char **faction;
	char **password;
	char **forumtag;
	inikey ini;
} adminDef, *adminPtr;

extern adminDef admincfg;

typedef struct {
	//admin
	bool bot;
	char *host;
	char *port;
	char *botnick;
	char *botpass;
	char *channel;
	bool announcetick;
	inikey ini;
} ircDef, *ircPtr;

extern ircDef irccfg;

typedef struct {
	//startup options
	int mode;
	int port[PORT_TOTAL];
	int interfaces;
	int serverpipe;
	int clientpipe;
	int botconn;
	bool verbose;
	char sysini[512];
	char mapini[512];
	char pipefile[512];
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
	inikey ini;
} mySqlDef, *mySqlPtr;

extern mySqlDef mysqlcfg;


typedef struct {
	bool status;
	int number;
	int round;
	int speed;
	int next;
	int debug_id;
	int debug_pass;
	inikey ini;
} tickDef, *tickPtr;

extern tickDef ticks;
extern int savetickconfig();


typedef struct
{
  char *data;
  void *next;
  void **prev;
} svBufferDef, *svBufferPtr;


typedef struct
{
// server
  int socket;
  struct sockaddr_in sockaddr;
  void *next;
  void **previous;

  char *recv;
  char recv_buf[SERVER_RECV_BUFSIZE+1];
  int recv_pos;
  int recv_max;
  int time;
  int flags;

  svBufferPtr sendbuf;
  svBufferPtr sendbufpos;
  int sendpos;
  int sendsize;

  svBufferPtr sendflushbuf;
  int sendflushpos;


  char *sendstatic;
  int sendstaticsize;

// ioInterfacePtr
  void *io;

// pointer for the interface to allocate its data
  void *iodata;

// database
  dbUserPtr dbuser;

} svConnectionDef, *svConnectionPtr;


int svTime();

int file_exist(char *filename);
int loadconfig( char *file, int type );

char *trimwhitespace(char *str);
char** str_split(char* str, char delim, int* numSplits );
void dirstructurecheck(char *directory);

void cleanUp(int type);

void svPipeScan(int pipefileid);
int svPipeSend(int pipedirection, char *message, ...);

// Functions used by the Output interface
void svSend( svConnectionPtr cnt, void *data, int size );
//void svSendString( svConnectionPtr cnt, char *string );

void httpString( ReplyDataPtr rd, char *string );
__attribute__ ((format (printf, 2, 3))) void httpPrintf( ReplyDataPtr rd, char *string, ... );


//__attribute__ ((format (printf, 2, 3))) void svSendPrintf( svConnectionPtr cnt, char *string, ... );

//void svSendStatic( svConnectionPtr cnt, void *data, int size );


#define SV_FLAGS_NEED_WRITE (0x1)
#define SV_FLAGS_WRITE_BUFFERED (0x2)
#define SV_FLAGS_WRITE_STATIC (0x4)
#define SV_FLAGS_TO_CLOSE (0x8)
#define SV_FLAGS_WAIT_CLOSE (0x10)
#define SV_FLAGS_TIMEOUT (0x20)


typedef struct {
	//new ban listing
	int number;
	char **ip;
	inikey ini;
} svbanDef, *svbanPtr;

extern svbanDef banlist;

#define MAXLOGSTRING 1024
void loghandle( int flag, int error, char *fmt, ... );

extern bool firstload;
char *itoa(int i);

void svSignal( int signal );

#endif
