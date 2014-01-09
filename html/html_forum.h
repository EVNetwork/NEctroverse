#ifndef FORUMINCLUDES
#define FORUMINCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.


////////



typedef struct
{
  char title[DB_FORUM_NAME_SIZE];
  int threads;
  int rperms;
  int wperms;
  time_t time;
  int tick;
  int flags;
  int reserved[2];
} dbForumForumDef, *dbForumForumPtr;

typedef struct
{
  char topic[DB_FORUM_NAME_SIZE];
  int posts;
  int authorid;
  char authorname[USER_NAME_MAX];
  time_t time;
  int tick;
  int flags;

  int id;


/*
  int reserved;
*/
  struct in_addr sin_addr;
  char reserved[ 4 - sizeof(struct in_addr) ];


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


/*
  int reserved[2];
*/
  struct in_addr sin_addr;
  char reserved[ 4 - sizeof(struct in_addr) ];


} dbForumPostInDef, *dbForumPostInPtr;

typedef struct
{
  char *text;
  dbForumPostInDef post;
} dbForumPostDef, *dbForumPostPtr;



int dbForumListForums( int perms, dbForumForumPtr *forums );
int dbForumListThreads( int forum, int base, int end, dbForumForumPtr forumd, dbForumThreadPtr *threads );
int dbForumListPosts( int forum, int thread, int base, int end, dbForumThreadPtr threadd, dbForumPostPtr *posts );

int dbForumRetrieveForum( int forum, dbForumForumPtr forumd );

int dbForumAddForum( dbForumForumPtr forumd, int type, int nid );
int dbForumRemoveForum( int forum );

int dbForumAddThread( int forum, dbForumThreadPtr threadd );
int dbForumRemoveThread( int forum, int thread );

int dbForumAddPost( int forum, int thread, dbForumPostPtr postd );
int dbForumRemovePost( int forum, int thread, int post );
int dbForumEditPost( int forum, int thread, int post, dbForumPostPtr postd );


//////////


int iohttpForumFilter( char *dest, char *string, int size, int html );

int iohttpForumFilter2( char *dest, char *string, int size );

int iohttpForumFilter3( char *dest, char *string, int size );

int iohttpForumPerms( int id, int forum, ReplyDataPtr cnt, dbUserMainPtr maind, int perms );

int iohttpForumCleanAuthor( char *string );

int iohttpForumCleanIP( char *ipstring );

void iohtmlFunc_forum( ReplyDataPtr cnt );

#endif
