
static bool is_http;
static char do_redi[1024];

void init_string( CurlStringPtr curl_str ) {

curl_str->len = 0;
curl_str->ptr = malloc(curl_str->len+1);

if (curl_str->ptr == NULL) {
	critical( "malloc failure" );
}

curl_str->ptr[0] = '\0';

return;
}

size_t writefunc( void *ptr, size_t size, size_t nmemb, CurlStringPtr curl_str ) {
	size_t new_len = ( curl_str->len + size*nmemb );

curl_str->ptr = realloc(curl_str->ptr, new_len+1);
if (curl_str->ptr == NULL) {
	critical( "realloc failed" );
	return -3;
}
memcpy(curl_str->ptr+curl_str->len, ptr, size*nmemb);
curl_str->ptr[new_len] = '\0';
curl_str->len = new_len;

return size*nmemb;
}


void facebook_apptoken( char **token ) {
	int offset = 0, maxlen = 1024;
	char post[1024];
	CURL *curl;
	CURLcode res;

offset += snprintf( &post[offset], (maxlen - offset), "client_id=%s", fbcfg.app_id );
offset += snprintf( &post[offset], (maxlen - offset), "&client_secret=%s", fbcfg.app_secret );
offset += snprintf( &post[offset], (maxlen - offset), "%s", "&grant_type=client_credentials" );

curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );
	/* always cleanup */
	curl_easy_cleanup(curl);
	*token = strdup(curl_str.ptr);
	free(curl_str.ptr);
}


return;
}

void facebook_usertoken( FBTokenPtr token, char *code ) {
	int offset = 0, maxlen = 1024;
	int i, num;
	char **split;
	char post[1024];
	CURL *curl;
	CURLcode res;

offset = 0;
offset += snprintf( &post[offset], (maxlen - offset), "client_id=%s", fbcfg.app_id );
offset += snprintf( &post[offset], (maxlen - offset), "&client_secret=%s", fbcfg.app_secret );
offset += snprintf( &post[offset], (maxlen - offset), "&redirect_uri=%s", do_redi );
offset += snprintf( &post[offset], (maxlen - offset), "&code=%s", code );

curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );

	/* always cleanup */
	curl_easy_cleanup(curl);
	split = str_split( curl_str.ptr, '&', &num );
		
	if ( split == NULL ) {
		info( "str_split returned NULL" );
	} else {
		for ( i = 0; i < num; i++ ) {
			if( strncmp( split[i], "access_token", strlen("access_token") ) == 0 ) {
				token->val = strdup( (strstr( split[i], "=" )+1) );
			} else if( strncmp( split[i], "expires", strlen("expires") ) == 0 ) {
				sscanf( (strstr( split[i], "=" )+1), "%d", &token->expire );
			}
		}
	}
	free( split );
	free(curl_str.ptr);
}

return;
}

void facebook_getdata_token( FBUserPtr data, FBTokenDef token ) {
	int offset = 0, maxlen = 1024;
	char post[1024];
	CURL *curl;
	CURLcode res;

		/*
		offset = 0;
		offset += snprintf( &post[offset], (maxlen - offset), "%s", "grant_type=fb_exchange_token" );
		offset += snprintf( &post[offset], (maxlen - offset), "&client_id=%s", fbcfg.app_id );
		offset += snprintf( &post[offset], (maxlen - offset), "&client_secret=%s", fbcfg.app_secret );
		offset += snprintf( &post[offset], (maxlen - offset), "&fb_exchange_token=%s", token );
		offset += snprintf( &post[offset], (maxlen - offset), "&redirect_uri=%s", do_redi );
		*/


offset = 0;
offset += snprintf( &post[offset], (maxlen - offset), "%s", "https://graph.facebook.com/me" );
offset += snprintf( &post[offset], (maxlen - offset), "?access_token=%s", token.val );


curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, post );
	//curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token" );
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );

	/* always cleanup */
	curl_easy_cleanup(curl);

	cJSON *root = cJSON_Parse(curl_str.ptr);
	if( root ) {
		cJSON *message = cJSON_GetObjectItem(root,"id");
		if( ( message ) ) {
			strncpy( data->id, message->valuestring, sizeof( data->id ) );
		}
		message = cJSON_GetObjectItem(root,"first_name");
		if( ( message ) ) {
			strncpy(data->first_name, message->valuestring, sizeof( data->first_name ) );
		}
		message = cJSON_GetObjectItem(root,"last_name");
		if( ( message ) ) {
			strncpy(data->last_name, message->valuestring, sizeof( data->last_name ) );
		}
		message = cJSON_GetObjectItem(root,"timezone");
		if( ( message ) ) {
			data->timezone = message->valuedouble;
		}
		message = cJSON_GetObjectItem(root,"name");
		if( ( message ) ) {
			strncpy(data->full_name, message->valuestring, sizeof( data->full_name ) );
		}
	} else {
		error( "Bad Responce" );
	}
	cJSON_Delete(root);
	//printf( curl_str.ptr );
	free(curl_str.ptr);
}

	
return;
}	


void facebook_getdata_id( FBUserPtr data, char *userid ) {
	int offset = 0, maxlen = 1024;
	char post[1024];
	CURL *curl;
	CURLcode res;

		/*
		offset = 0;
		offset += snprintf( &post[offset], (maxlen - offset), "%s", "grant_type=fb_exchange_token" );
		offset += snprintf( &post[offset], (maxlen - offset), "&client_id=%s", fbcfg.app_id );
		offset += snprintf( &post[offset], (maxlen - offset), "&client_secret=%s", fbcfg.app_secret );
		offset += snprintf( &post[offset], (maxlen - offset), "&fb_exchange_token=%s", token );
		offset += snprintf( &post[offset], (maxlen - offset), "&redirect_uri=%s", do_redi );
		*/


offset = 0;
offset += snprintf( &post[offset], (maxlen - offset), "https://graph.facebook.com/%s", userid  );
offset += snprintf( &post[offset], (maxlen - offset), "?%s", fbcfg.app_token );
//offset += snprintf( &post[offset], (maxlen - offset), "&fields=%s", "name,id" );


curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, post );
	//curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token" );
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );

	/* always cleanup */
	curl_easy_cleanup(curl);

	cJSON *root = cJSON_Parse(curl_str.ptr);
	if( root ) {
		cJSON *message = cJSON_GetObjectItem(root,"id");
		if( ( message ) ) {
			strncpy( data->id, message->valuestring, sizeof( data->id ) );
		}
		message = cJSON_GetObjectItem(root,"first_name");
		if( ( message ) ) {
			strncpy( data->first_name, message->valuestring, sizeof( data->first_name ) );
		}
		message = cJSON_GetObjectItem(root,"last_name");
		if( ( message ) ) {
			strncpy( data->last_name, message->valuestring, sizeof( data->last_name ) );
		}
		message = cJSON_GetObjectItem(root,"languages");
		data->connected = ( message ) ? true : false;
	} else {
		error( "Bad Responce" );
	}
	cJSON_Delete(root);
	//printf( curl_str.ptr );
	free(curl_str.ptr);
}

	
return;
}


int facebook_unlink_id( char *userid ) {
	bool result = false;
	int offset = 0, maxlen = 1024;
	char post[1024];
	CURL *curl;
	CURLcode res;

offset = 0;
offset += snprintf( &post[offset], (maxlen - offset), "https://graph.facebook.com/%s/permissions", userid  );
offset += snprintf( &post[offset], (maxlen - offset), "?%s", fbcfg.app_token );

curl = curl_easy_init();
if( curl ) {
	CurlStringDef curl_str;
	init_string(&curl_str);
	curl_easy_setopt(curl, CURLOPT_URL, post );
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_str);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );

	/* always cleanup */
	curl_easy_cleanup(curl);
	result = ( strcmp( curl_str.ptr, "true" ) == 0 ) ? true : false;//printf( curl_str.ptr );
	free(curl_str.ptr);
}

	
return result;
}

void iohtmlFunc_facebook( ReplyDataPtr cnt ) {
	int a, id, offset = 0;
	char *error, *code, *host, *fbtoke;
	char buffer[1024];
	dbUserPtr user;
	dbUserInfoDef infod;
	FBUserDef data;
	FBTokenDef token;

if( !(strlen(fbcfg.app_id)) || !(strlen(fbcfg.app_secret)) )
	goto BAILOUT;

host = (char *)MHD_lookup_connection_value( cnt->connection, MHD_HEADER_KIND, "Host" );

#if HTTPS_SUPPORT
is_http = strstr( host, itoa(options.port[PORT_HTTPS]) ) ? true : ( strstr( host, itoa(options.port[PORT_HTTP]) ) ? false : true );
#endif

snprintf( do_redi, sizeof( do_redi ), "%s://%s/facebook", (is_http ? "https" : "http"), host  );

memset( &data, 0, sizeof(FBUserDef) );
memset( &token, 0, sizeof(FBTokenDef) );
memset( &buffer, 0, sizeof(buffer) );

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_FACEBOOK );

error = iohtmlVarsFind( cnt, "error" );
code = iohtmlVarsFind( cnt, "code" );
fbtoke = iohtmlVarsFind( cnt, "fblogin_token" );

if( ( code ) || ( fbtoke ) ){
	if( code )
		facebook_usertoken( &token, code );
	else
		token.val = strdup( fbtoke );
	if( token.val ) {
		facebook_getdata_token( &data, token );
		if( !( data.id ) ) {
			error( "No ID in Responce" );
			goto BAILOUT;
		}
	} else {
		offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s", "Invalid Login, Did you just reload this page?" );
	}
} else if( error) {
	offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s", "Error Detected<br>" );
	offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s<br>", error );
	offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s<br>", iohtmlVarsFind( cnt, "error_reason" ) );
	offset += snprintf( &buffer[offset], (sizeof(buffer) - offset), "%s<br>", iohtmlVarsFind( cnt, "error_description" ) );
} else {
	httpString( cnt, "Invalid" );
	goto BAILOUT;
}


id = dbUserFBSearch( data.id );

if( buffer[0] )
	httpString( cnt, buffer );

if( id >= 0 ) {
	user = dbUserLinkID( id );
	if( dbUserLinkDatabase( cnt, id ) < 0 )
		goto BAILOUT;

	if( dbSessionSet( (cnt->session)->dbuser, (cnt->session)->sid ) < 0 )
		goto BAILOUT;

	dbUserInfoRetrieve( id, &infod );
	infod.lasttime = time( 0 );
	if( (cnt->connection)->addr->sa_family == AF_INET )
	for( a = (MAXIPRECORD-2); a >= 0 ; a-- ) {
		if( strcmp(inet_ntoa( infod.sin_addr[a] ),"0.0.0.0") ) {
			memcpy( &(infod.sin_addr[a+1]), &(infod.sin_addr[a]), sizeof(struct in_addr) );
		}
	}
	memcpy( &(infod.sin_addr[0]), &(((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr), sizeof(struct in_addr) );
	dbUserInfoSet( id, &infod );
	redirect( cnt, "/" );
	httpPrintf( cnt, "<b>Welcome <i>%s</i></b><br><br>", user->faction );
	httpString( cnt, "You should be redirected back to the main screen shortly<br>" );
	httpString( cnt, "<a href=\"/\">Click here if it takes too long</a><br>" );
} else if( data.id[0] ) {
	goto LINKWITHFB;
} else {
	httpString( cnt, "Invalid Request" );
}

goto BAILOUT;

LINKWITHFB:

if( ( (cnt->session)->dbuser ) && ( user = (cnt->session)->dbuser ) ) {
	bitflag_add( &user->flags, cmdUserFlags[CMD_USER_FLAGS_FACEBOOK] );
	strcpy( user->fbid, data.id );
	dbUserSave( user->id, user );
	httpPrintf( cnt, "<b>Facebook ID %s now linked with User %s</b><br>", user->fbid, user->name );
	
} else {




	httpPrintf( cnt, "<br>Valid, but no linked account: %s", data.id );
}

BAILOUT:

iohtmlFunc_endhtml( cnt );

if( token.val )
	free( token.val );

return;
}


void iohtmlFBSDK( ReplyDataPtr cnt ) {

httpString( cnt, "<div id=\"fb-root\"></div>\n" );
httpString( cnt, "<script>\n" );
httpString( cnt, "  window.fbAsyncInit = function() {\n" );
httpString( cnt, "    // init the FB JS SDK\n" );
httpString( cnt, "    FB.init({\n" );
httpString( cnt, "      appId      : '110861965600284',                        // App ID from the app dashboard\n" );
httpString( cnt, "      status     : true,                                 // Check Facebook Login status\n" );
httpString( cnt, "      cookie     : true, // enable cookies to allow the server to access the session\n" );
httpString( cnt, "      xfbml      : true                                  // Look for social plugins on the page\n" );
httpString( cnt, "    });\n" );
httpString( cnt, "\n" );
httpString( cnt, " FB.Event.subscribe('auth.authResponseChange', function(response) {\n" );
    // Here we specify what we do with the response anytime this event occurs. 
httpString( cnt, "    if (response.status === 'connected') {\n" );
      // The response object is returned with a status field that lets the app know the current
      // login status of the person. In this case, we're handling the situation where they 
      // have logged in to the app.
httpString( cnt, "      testAPI();\n" );
httpString( cnt, "    } else if (response.status === 'not_authorized') {\n" );
      // In this case, the person is logged into Facebook, but not into the app, so we call
      // FB.login() to prompt them to do so. 
      // In real-life usage, you wouldn't want to immediately prompt someone to login 
      // like this, for two reasons:
      // (1) JavaScript created popup windows are blocked by most browsers unless they 
      // result from direct interaction from people using the app (such as a mouse click)
      // (2) it is a bad experience to be continually prompted to login upon page load.
httpString( cnt, "      FB.login();\n" );
httpString( cnt, "    } else {\n" );
      // In this case, the person is not logged into Facebook, so we call the login() 
      // function to prompt them to do so. Note that at this stage there is no indication
      // of whether they are logged into the app. If they aren't then they'll see the Login
      // dialog right after they log in to Facebook. 
      // The same caveats as above apply to the FB.login() call here.
httpString( cnt, "      FB.login();\n" );
httpString( cnt, "    }\n" );
httpString( cnt, "  });\n" );
httpString( cnt, "  };\n" );
httpString( cnt, "\n" );
// Load the SDK asynchronously

httpString( cnt, "  (function(d){\n" );
httpString( cnt, "   var js, id = 'facebook-jssdk', ref = d.getElementsByTagName('script')[0];\n" );
httpString( cnt, "   if (d.getElementById(id)) {return;}\n" );
httpString( cnt, "   js = d.createElement('script'); js.id = id; js.async = true;\n" );
httpString( cnt, "   js.src = \"//connect.facebook.net/en_US/all.js\";\n" );
httpString( cnt, "   ref.parentNode.insertBefore(js, ref);\n" );
httpString( cnt, "  }(document));\n" );

httpString( cnt, "function testAPI() {\n" );
httpString( cnt, "	FB.api('/me', function(response) {\n" );
httpString( cnt, "	var access_token =   FB.getAuthResponse()['accessToken'];\n" );
httpString( cnt, "      location.href = \"/facebook?fblogin_id=\"+response.id+\"&fblogin_token=\"+access_token;\n" );
httpString( cnt, "	});\n" );
httpString( cnt, "}\n" );
httpString( cnt, "</script>\n" );

return;
}


