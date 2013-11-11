#ifndef IOINCLUDES
#define IOINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

typedef struct
{
  void (*Init)();
  void (*End)();
  void (*inNew)( svConnectionPtr cnt );
  void (*inNewData)( svConnectionPtr cnt );
  void (*outSendReply)( svConnectionPtr cnt );
  void (*inSendComplete)( svConnectionPtr cnt );
  void (*inClosed)( svConnectionPtr cnt );
  void (*inError)( svConnectionPtr cnt, int type );
  void (*TickStart)();
  void (*TickEnd)();
  int outputsize;
  int timeout;
  int hardtimeout;
} ioInterfaceDef, *ioInterfacePtr;

extern ioInterfaceDef ioInterface[];


char *ioCompareWords( char *string, char *word );
char *ioCompareFindWords( char *string, char *word );
char *ioCompareFindBinwords( char *string, char *word, int size );
int ioCompareExact( char *s1, char *s2 );
int iohttpCompareExt( char *string, char *ext );
int iohttpCaseLow( char *dest, char *string );



typedef struct
{
  char path[SERVER_PATH_BUFSIZE];
  char fileread[SERVER_PATH_BUFSIZE];
  int type; // 0:file, 1:function call

  void *data;
  int size;
  int mime;
  time_t scurtime;
  void (*function)( svConnectionPtr cnt );

  void *next;
  void **prev;
} iohttpFileDef, *iohttpFilePtr;

iohttpFilePtr iohttpFileList;


typedef struct
{
  int flags;

  // http variables
  char path[SERVER_PATH_BUFSIZE];
  int method;
  int content_length;
  char *content;
  char *query_string;
  char *content_type;
  char *cookie;
  char *accept;
  char *accept_language;
  char *host;
  char *referer;
  char *user_agent;

  // reply
  int code;
  iohttpFilePtr file;

} iohttpDataDef, *iohttpDataPtr;






typedef struct
{
  int status;
} ioevmDataDef, *ioevmDataPtr;





#endif
