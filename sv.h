#define SERVERINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

//config struct
typedef struct {
	//server
	char* servername;
	char* directory;
	char* downfrom;
	char* httpimages;
	char* httpfiles;
	char* httpread;
	char* pubforum;
	int httpport;
	int stockpile;
	int warend;
	int victory;
	int ticktime;
	int autoresume;
	int round;
	//evmap
	bool evmpactv;
	int evmpport;
	//syslog
	char* syslog_tagname;
	char* syslog_facility;
} configDef, *configPtr;

extern configDef sysconfig;

typedef struct {
	//admin
	char* name;
	char* faction;
	char* password;
	char* forumtag;
	int empire_number;
	char* empire_name;
	char* empire_password;
	int level;
	int race;
} adminDef, *adminPtr;

extern adminDef admincfg;

enum 
{
PORT_HTTP,
PORT_EVMP,
PORT_TOTAL
};

enum 
{
MODE_DAEMON,
MODE_FORKED
};

typedef struct {
	//startup options
	int mode;
	int port[PORT_TOTAL];
	int interfaces;
	int serverpipe;
	int clientpipe;
	bool verbose;
	char sysini[512];
	char banini[512];
	char mapini[512];
	char pipefile[512];
	char pipestring[128];
} optionsDef, *optionsPtr;

extern optionsDef options;

typedef struct {
	//mysql
	bool active;
	char* host;
	int port;
	char* user;
	char* password;
	char* database;
} mySqlDef, *mySqlPtr;

extern mySqlDef mysqlcfg;


typedef struct {
	bool status;
	int number;
	int next;
	int debug_id;
	int debug_pass;
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
int svInit();
int svListen();
void svEnd();
void svSelect();
void svRecv();

int file_exist(char *filename);
int loadconfig( char *file, int type );

char *trimwhitespace(char *str);
char** str_split(char* str, char delim, int* numSplits );
void dirstructurecheck(char *directory);

void svShutdown( svConnectionPtr cnt );
void svClose( svConnectionPtr cnt );
int svFree( svConnectionPtr cnt );

void cleanUp(int pipefileid, int type);

void svPipeScan(int pipefileid);
int svPipeSend(int pipedirection, char *message);

void svSendEnd( svConnectionPtr cnt );
void svSendInit( svConnectionPtr cnt, int size );
int svSendFlush( svConnectionPtr cnt );
void svSendStaticFlush( svConnectionPtr cnt );


// Functions used by the Output interface
void svSend( svConnectionPtr cnt, void *data, int size );
void svSendString( svConnectionPtr cnt, char *string );

__attribute__ ((format (printf, 2, 3))) void svSendPrintf( svConnectionPtr cnt, char *string, ... );

void svSendStatic( svConnectionPtr cnt, void *data, int size );


#define SV_FLAGS_NEED_WRITE (0x1)
#define SV_FLAGS_WRITE_BUFFERED (0x2)
#define SV_FLAGS_WRITE_STATIC (0x4)
#define SV_FLAGS_TO_CLOSE (0x8)
#define SV_FLAGS_WAIT_CLOSE (0x10)
#define SV_FLAGS_TIMEOUT (0x20)


typedef struct {
	//new ban listing
	int number;
	char (*ip)[32];
} svbanDef, *svbanPtr;

extern svbanDef banlist;

