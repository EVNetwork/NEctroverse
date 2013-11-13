#ifndef ADMININCLUDES
#define ADMININCLUDES


typedef struct
{
	int size;
	char *type;
	char *name;
	char *value;
} adminFormInput, *adminFormInputPtr;



void iohttpFunc_oldadmin( svConnectionPtr cnt );
void iohttpFunc_moderator( svConnectionPtr cnt );


void iohttpFunc_adminframe( svConnectionPtr cnt ); //done
void iohttpFunc_adminmenu( svConnectionPtr cnt ); //done
void iohttpFunc_adminforum( svConnectionPtr cnt ); //done




void iohttpAdminForm( svConnectionPtr cnt, char *target ); //done
void iohttpAdminSubmit( svConnectionPtr cnt, char *name ); //done
void iohttpAdminInput( svConnectionPtr cnt, adminFormInputPtr inputs ); //done

int iohttpAdminAction( int act );




void iohtmlFunc_oldadmin( ReplyDataPtr cnt );
void iohtmlFunc_moderator( ReplyDataPtr cnt );

void iohtmlFunc_adminframe( ReplyDataPtr cnt );
void iohtmlFunc_adminmenu( ReplyDataPtr cnt );
void iohtmlFunc_adminforum( ReplyDataPtr cnt );

void iohtmlAdminForm( ReplyDataPtr cnt, char *target );
void iohtmlAdminSubmit( ReplyDataPtr cnt, char *name );
void iohtmlAdminInput( ReplyDataPtr cnt, adminFormInputPtr inputs );

#endif
