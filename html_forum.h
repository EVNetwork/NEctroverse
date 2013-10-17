#define FORUMINCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

int iohttpForumFilter( char *dest, char *string, int size, int html );

int iohttpForumFilter2( char *dest, char *string, int size );

int iohttpForumFilter3( char *dest, char *string, int size );

int iohttpForumPerms( int id, int forum, svConnectionPtr cnt, dbUserMainPtr maind, int perms );

int iohttpForumCleanAuthor( char *string );

int iohttpForumCleanIP( char *ipstring );

void iohttpForum( svConnectionPtr cnt );


