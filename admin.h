#define ADMINHEADERS

void iohttpFunc_admin( svConnectionPtr cnt );
void iohttpFunc_admin2( svConnectionPtr cnt );
void iohttpFunc_adminforum( svConnectionPtr cnt );

void iohttpFunc_moderator( svConnectionPtr cnt );

void iohttpAdminForm( svConnectionPtr cnt, unsigned char *target );
void iohttpAdminSubmit( svConnectionPtr cnt, unsigned char *name );



//Not really meant to be here, but oh well for now...
void InitHTTP();
void EndHTTP();
