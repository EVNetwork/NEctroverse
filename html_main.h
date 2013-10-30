#define HTMLMAININCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

int iohttpIdentifyHex( char *num );
int iohttpIdentify( svConnectionPtr cnt, int action );

void iohttpBase( svConnectionPtr cnt, int flags );

int iohttpHeader( svConnectionPtr cnt, int id, dbUserMainPtr mainp );

void iohttpFunc_frontmenu( svConnectionPtr cnt, int flags );

void iohttpBodyInit( svConnectionPtr cnt, char *title, ... );

void iohttpBodyEnd( svConnectionPtr cnt );

void iohttpFunc_races( svConnectionPtr cnt );

void iohttpFunc_register( svConnectionPtr cnt );

void iohttpFunc_register2( svConnectionPtr cnt );

void iohttpFunc_register3( svConnectionPtr cnt );

void iohttpFunc_login( svConnectionPtr cnt, int flag, char *text, ... );

void iohttpFunc_endhtml( svConnectionPtr cnt );

void iohttpFunc_front( svConnectionPtr cnt, char *text, ... );

void iohttpFunc_faq( svConnectionPtr cnt );

void iohttpFunc_gettingstarted( svConnectionPtr cnt );

void iohttpFunc_halloffame( svConnectionPtr cnt );

enum {
FMENU_NONE,
FMENU_MAIN,
FMENU_REGISTER,
FMENU_FORUM,
FMENU_FAQ,
FMENU_GSTART,
FMENU_RANKS,
FMENU_SERVER,
FMENU_TOTAL,
};
