#ifndef IOVARSINCLUDES
#define IOVARSINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

char *iohttpVarsFind( char *id );

void iohttpVarsCut();

int iohttpVarsInit( svConnectionPtr cnt );

int iohttpVarsMapcoords( svConnectionPtr cnt, int *coords );

char *iohttpVarsUpload( svConnectionPtr cnt, char **filename, int *filesize );

#endif
