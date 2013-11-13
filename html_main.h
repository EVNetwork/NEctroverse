#ifndef HTMLMAININCLUDES
#define HTMLMAININCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

int iohttpIdentifyHex( char *num ); //no need -- hotlinked

int iohttpIdentify( svConnectionPtr cnt, int action ); //done

void iohttpBase( svConnectionPtr cnt, int flags ); //done

int iohttpHeader( svConnectionPtr cnt, int id, dbUserMainPtr mainp ); //done

void iohttpFunc_frontmenu( svConnectionPtr cnt, int flags ); //done

void iohttpBodyInit( svConnectionPtr cnt, char *title, ... ); //done

void iohttpBodyEnd( svConnectionPtr cnt ); //done

void iohttpFunc_races( svConnectionPtr cnt ); //done

void iohttpFunc_register( svConnectionPtr cnt ); //done

void iohttpFunc_register2( svConnectionPtr cnt ); //done

void iohttpFunc_register3( svConnectionPtr cnt ); // done

void iohttpFunc_login( svConnectionPtr cnt, int flag, char *text, ... ); //done

void iohttpFunc_endhtml( svConnectionPtr cnt ); //done

void iohttpFunc_front( svConnectionPtr cnt, char *text, ... ); //done

void iohttpFunc_faq( svConnectionPtr cnt ); //done

void iohttpFunc_gettingstarted( svConnectionPtr cnt ); //done

void iohttpFunc_halloffame( svConnectionPtr cnt ); //done


int iohtmlIdentify( ReplyDataPtr cnt, int action );

void iohtmlBase( ReplyDataPtr cnt, int flags );

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
void iohtmlFunc_gettingstarted( ReplyDataPtr cnt );
void iohtmlFunc_halloffame( ReplyDataPtr cnt );


void iohtmlFunc_endhtml( ReplyDataPtr cnt );

#endif
