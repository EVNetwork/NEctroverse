#define ADMININCLUDES

void iohttpFunc_admin( svConnectionPtr cnt );
void iohttpFunc_admin2( svConnectionPtr cnt );
void iohttpFunc_adminforum( svConnectionPtr cnt );

void iohttpFunc_moderator( svConnectionPtr cnt );

void iohttpAdminForm( svConnectionPtr cnt, char *target );
void iohttpAdminSubmit( svConnectionPtr cnt, char *name );



//Not really meant to be here, but oh well for now...
void InitHTTP();
void EndHTTP();
