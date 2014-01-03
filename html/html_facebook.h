#ifndef USER_FB_INCLUDES
#define USER_FB_INCLUDES
//The above line MUST STAY HERE! -- This prevents double calling.

typedef struct {
	size_t len;
	char *ptr;
} CurlStringDef, *CurlStringPtr;

typedef struct {
	int expire;
	char *val;
} FBTokenDef, *FBTokenPtr;

void facebook_apptoken( char **token );

void iohtmlFunc_facebook( ReplyDataPtr cnt );

void iohtmlFBSDK( ReplyDataPtr cnt );

#endif
