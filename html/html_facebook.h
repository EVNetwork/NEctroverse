#ifndef USER_FB_INCLUDES
#define USER_FB_INCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

typedef struct {
	size_t len;
	char *ptr;
} CurlStringDef, *CurlStringPtr;

void facebook_apptoken( char **token );

void iohtmlFunc_facebook( ReplyDataPtr cnt );

void iohtmlFBSDK( ReplyDataPtr cnt );

void iohtmlFBConnect( ReplyDataPtr cnt );

void facebook_getdata_id( FBUserPtr fbdata, char *userid );

int facebook_unlink_app( char *userid );

void facebook_update_user( dbUserPtr user );

void facebook_getdata_token( FBUserPtr fbdata, FBTokenDef token );

#endif
