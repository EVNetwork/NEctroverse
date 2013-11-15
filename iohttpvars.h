#ifndef IOVARSINCLUDES
#define IOVARSINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

int iohttpVarsMapcoords( svConnectionPtr cnt, int *coords );


char *iohtmlVarsFind( ReplyDataPtr cnt, char *id );

char *iohtmlHeaderFind( ReplyDataPtr cnt, char *id );

char *iohtmlCookieFind( ReplyDataPtr cnt, char *id );

int iohtmlCookieAdd( ReplyDataPtr cnt, char *name, char *value, ... );

#endif
