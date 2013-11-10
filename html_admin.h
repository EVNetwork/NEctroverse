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


void iohttpFunc_adminframe( svConnectionPtr cnt );
void iohttpFunc_adminmenu( svConnectionPtr cnt );
void iohttpFunc_adminforum( svConnectionPtr cnt );




void iohttpAdminForm( svConnectionPtr cnt, char *target );
void iohttpAdminSubmit( svConnectionPtr cnt, char *name );
void iohttpAdminInput( svConnectionPtr cnt, adminFormInputPtr inputs );

int iohttpAdminAction( int act );
