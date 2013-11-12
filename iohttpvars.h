#ifndef IOVARSINCLUDES
#define IOVARSINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

char *iohttpVarsFind( char *id );

void iohttpVarsCut();

int iohttpVarsInit( svConnectionPtr cnt );

int iohttpVarsMapcoords( svConnectionPtr cnt, int *coords );

char *iohttpVarsUpload( svConnectionPtr cnt, char **filename, int *filesize );


char *iohtmlVarsFind( ReplyDataPtr cnt, char *id );

char *iohtmlHeaderFind( ReplyDataPtr cnt, char *id );

char *iohtmlCookieFind( ReplyDataPtr cnt, char *id );

int iohtmlCookieAdd( ReplyDataPtr cnt, char *name, char *value, ... );

#endif
