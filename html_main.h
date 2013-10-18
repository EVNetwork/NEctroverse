#define HTMLMAININCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

int iohttpIdentifyHex( char *num );

void iohttpBase( svConnectionPtr cnt, int flags );

int iohttpHeader( svConnectionPtr cnt, int id, dbUserMainPtr mainp );

void iohttpFunc_starthtml( svConnectionPtr cnt, int flags );

void iohttpBodyInit( svConnectionPtr cnt, char *title, ... );

void iohttpBodyEnd( svConnectionPtr cnt );

void iohttpFunc_races( svConnectionPtr cnt );

void iohttpFunc_register( svConnectionPtr cnt );

void iohttpFunc_register2( svConnectionPtr cnt );

void iohttpFunc_register3( svConnectionPtr cnt );

void iohttpFunc_login( svConnectionPtr cnt, char *text, ... );

void iohttpFunc_endhtml( svConnectionPtr cnt );

void iohttpFunc_front( svConnectionPtr cnt, char *text, ... );

void iohttpFunc_faq( svConnectionPtr cnt );

void iohttpFunc_gettingstarted( svConnectionPtr cnt );


