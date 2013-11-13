#ifndef USERINCLUDES
#define USERINCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

void iohttpFunc_account( svConnectionPtr cnt );

void iohttpFunc_changepass( svConnectionPtr cnt );

void iohttpFunc_delete( svConnectionPtr cnt );

void iohttpFunc_logout( svConnectionPtr cnt );



void iohtmlFunc_account( ReplyDataPtr cnt );

void iohtmlFunc_changepass( ReplyDataPtr cnt );

void iohtmlFunc_delete( ReplyDataPtr cnt );

void iohtmlFunc_logout( ReplyDataPtr cnt );

#endif
