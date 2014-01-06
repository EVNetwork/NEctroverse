#ifndef IOVARSINCLUDES
#define IOVARSINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

int iohtmlVarsMapcoords( ReplyDataPtr cnt, int *coords );

char *iohtmlVarsFind( ReplyDataPtr cnt, char *id );

char *iohtmlHeaderFind( ReplyDataPtr cnt, char *id );

char *iohtmlCookieFind( ReplyDataPtr cnt, char *id );

#endif
