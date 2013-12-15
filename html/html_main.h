#ifndef HTMLMAININCLUDES
#define HTMLMAININCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

int iohttpIdentifyHex( char *num ); //no need -- hotlinked


void iohtmlBase( ReplyDataPtr cnt, int flags );
int iohtmlIdentify( ReplyDataPtr cnt, int action );
int iohtmlHeader( ReplyDataPtr cnt, int id, dbUserMainPtr mainp );

void iohtmlBodyInit( ReplyDataPtr cnt, char *title, ... );
void iohtmlBodyEnd( ReplyDataPtr cnt );

void iohtmlFunc_frontmenu( ReplyDataPtr cnt, int flags );
void iohtmlFunc_front( ReplyDataPtr cnt, char *text, ... );

void iohtmlFunc_register( ReplyDataPtr cnt );
void iohtmlFunc_register2( ReplyDataPtr cnt );
void iohtmlFunc_register3( ReplyDataPtr cnt );

void iohtmlFunc_login( ReplyDataPtr cnt, int flag, char *text, ... );

void iohtmlFunc_faq( ReplyDataPtr cnt );
void iohtmlFunc_races( ReplyDataPtr cnt );
void iohtmlFunc_rules( ReplyDataPtr cnt );
void iohtmlFunc_gettingstarted( ReplyDataPtr cnt );
void iohtmlFunc_halloffame( ReplyDataPtr cnt );


void iohtmlFunc_endhtml( ReplyDataPtr cnt );

#endif
