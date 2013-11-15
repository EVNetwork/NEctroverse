#ifndef ADMININCLUDES
#define ADMININCLUDES


typedef struct
{
	int size;
	char *type;
	char *name;
	char *value;
} adminFormInput, *adminFormInputPtr;



void iohtmlFunc_oldadmin( ReplyDataPtr cnt );
void iohtmlFunc_moderator( ReplyDataPtr cnt );



int iohtmlAdminAction( int act );


void iohtmlFunc_oldadmin( ReplyDataPtr cnt );
void iohtmlFunc_moderator( ReplyDataPtr cnt );

void iohtmlFunc_adminframe( ReplyDataPtr cnt );
void iohtmlFunc_adminmenu( ReplyDataPtr cnt );
void iohtmlFunc_adminforum( ReplyDataPtr cnt );

void iohtmlAdminForm( ReplyDataPtr cnt, char *target );
void iohtmlAdminSubmit( ReplyDataPtr cnt, char *name );
void iohtmlAdminInput( ReplyDataPtr cnt, adminFormInputPtr inputs );

#endif
