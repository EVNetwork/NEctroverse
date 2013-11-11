#ifndef IOHTTPINCLUDES
#define IOHTTPINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

extern char iohttpEnd[2][5];
extern int iohttpEndSize[2];

int iohttpParseHeader( svConnectionPtr cnt, iohttpDataPtr iohttp, char *cmd );

int iohttpMimeFind( char *name );

iohttpFilePtr iohttpFileAdd( int size );

int iohttpFileDel( char *path );

iohttpFilePtr iohttpFileFind( char *path );

void InitHTTP();

void EndHTTP();

void inNewHTTP( svConnectionPtr cnt );

void inNewDataHTTP( svConnectionPtr cnt );

void outSendReplyHTTP( svConnectionPtr cnt );

void inSendCompleteHTTP( svConnectionPtr cnt );

void inClosedHTTP( svConnectionPtr cnt );

void inErrorHTTP( svConnectionPtr cnt, int type );

char iohttpInputHex( char *src );

int iohttpParseHeader( svConnectionPtr cnt, iohttpDataPtr iohttp, char *cmd );

void TickStartHTTP();

void TickEndHTTP();

#endif
