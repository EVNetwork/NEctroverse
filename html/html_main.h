#ifndef HTMLMAININCLUDES
#define HTMLMAININCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

void iohtmlBase( ReplyDataPtr cnt, int flags );
int iohtmlIdentify( ReplyDataPtr cnt, int action );
int iohtmlHeader( ReplyDataPtr cnt, int id, dbUserMainPtr mainp );

void html_boxstart( ReplyDataPtr cnt, bool type, char *title, ... );
#define iohtmlBodyInit( cnt, title, ... ) html_boxstart( cnt, false, title, ##__VA_ARGS__ )

void iohtmlBodyEnd( ReplyDataPtr cnt );

void iohtmlFunc_frontmenu( ReplyDataPtr cnt, int flags );
void iohtmlFunc_front( ReplyDataPtr cnt, char *text, ... );

void iohtmlFunc_faq( ReplyDataPtr cnt );
void iohtmlFunc_notices( ReplyDataPtr cnt );
void iohtmlFunc_gettingstarted( ReplyDataPtr cnt );
void iohtmlFunc_halloffame( ReplyDataPtr cnt );


void iohtmlFunc_endhtml( ReplyDataPtr cnt );


void iohtmlFunc_register( ReplyDataPtr cnt );

void iohtmlFunc_login( ReplyDataPtr cnt, int flag, char *text, ... );

#endif
