#define ADMININCLUDES

void iohttpFunc_oldadmin( svConnectionPtr cnt );
void iohttpFunc_moderator( svConnectionPtr cnt );


void iohttpFunc_adminframe( svConnectionPtr cnt );
void iohttpFunc_adminmenu( svConnectionPtr cnt );
void iohttpFunc_adminforum( svConnectionPtr cnt );




void iohttpAdminForm( svConnectionPtr cnt, char *target );
void iohttpAdminSubmit( svConnectionPtr cnt, char *name );

