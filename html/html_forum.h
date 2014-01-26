#ifndef FORUMINCLUDES
#define FORUMINCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.


////////



typedef struct
{
  char title[DB_FORUM_NAME_SIZE];
  char lastpost[USER_NAME_MAX];
  int lastid;
  int threads;
  int rperms;
  int wperms;
  time_t time;
  int tick;
  int flags;
  int id;
  struct in_addr sin_addr;
} dbForumForumDef, *dbForumForumPtr;

typedef struct
{
  char topic[DB_FORUM_NAME_SIZE];
  int posts;
  int authorid;
  int lastid;
  char authorname[USER_NAME_MAX];
  char lastpost[USER_NAME_MAX];  
  time_t time;
  int tick;
  int flags;
  int id;
  struct in_addr sin_addr;
} dbForumThreadDef, *dbForumThreadPtr;

typedef struct
{
  int length;
  int authorid;
  char authorname[USER_NAME_MAX];
  char authortag[USER_FTAG_MAX];
  time_t time;
  int tick;
  int flags;
  struct in_addr sin_addr;
} dbForumPostInDef, *dbForumPostInPtr;

typedef struct
{
  char *text;
  dbForumPostInDef post;
} dbForumPostDef, *dbForumPostPtr;



int dbForumListForums( int flags, int perms, dbForumForumPtr *forums );
int dbForumListThreads( int flags, int forum, int base, int end, dbForumForumPtr forumd, dbForumThreadPtr *threads );
int dbForumListPosts( int flags, int forum, int thread, int base, int end, dbForumThreadPtr threadd, dbForumPostPtr *posts );

int dbForumRetrieveForum( int flags, int forum, dbForumForumPtr forumd );

int dbForumAddForum( int flags, dbForumForumPtr forumd );
int dbForumRemoveForum( int flags, int forum );

int dbForumAddThread( int flags, int forum, dbForumThreadPtr threadd );
int dbForumRemoveThread( int flags, int forum, int thread );

int dbForumAddPost( int flags, int forum, int thread, dbForumPostPtr postd );
int dbForumRemovePost( int flags, int forum, int thread, int post );
int dbForumEditPost( int flags, int forum, int thread, int post, dbForumPostPtr postd );


//////////


int iohttpForumFilter( char *dest, char *string, int size, int html );

int iohttpForumFilter2( char *dest, char *string, int size );

int iohttpForumFilter3( char *dest, char *string, int size );

int iohttpForumPerms( int flags, int id, int forum, ReplyDataPtr cnt, dbUserMainPtr maind, int perms );

int iohttpForumCleanAuthor( int flags, char *string );

int iohttpForumCleanIP( int flags, char *ipstring );

void iohtmlFunc_forum( ReplyDataPtr cnt );

#endif
