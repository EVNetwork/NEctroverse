
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
				token->expire = atoi( (strstr( split[i], "=" )+1) );
			}
		}
	}
	free( split );
	free(curl_str.ptr);
}

return;
}

void facebook_getdata( FBUserPtr data, FBTokenDef token ) {
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
			sscanf( message->valuestring, "%lld", &data->id );
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



void iohtmlFunc_facebook( ReplyDataPtr cnt ) {
	char *code, *host;
	FBUserDef data;
	FBTokenDef token;

host = (char *)MHD_lookup_connection_value( cnt->connection, MHD_HEADER_KIND, "Host" );

#if HTTPS_SUPPORT
is_http = strstr( host, itoa(options.port[PORT_HTTPS]) ) ? true : ( strstr( host, itoa(options.port[PORT_HTTP]) ) ? false : true );
#endif

snprintf( do_redi, sizeof( do_redi ), "%s://%s/facebook", (is_http ? "https" : "http"), host  );


code = iohtmlVarsFind( cnt, "code" );

if( code ) {
	facebook_usertoken( &token, code );
	memset( &data, 0, sizeof(FBUserDef) );
	if( token.val ) {
		facebook_getdata( &data, token );
		if( !( data.id ) ) {
			error( "No ID in Responce" );
			goto BAILOUT;
		}
	} else {
		httpString( cnt, "Invalid Login, Did you just reload this page?" );
		goto BAILOUT;
	}
} else {
	httpString( cnt, "Invalid" );
	goto BAILOUT;
}


httpPrintf( cnt, "Welcome %s, %lld, %.01f", data.full_name, data.id, data.timezone );
httpPrintf( cnt, "<br> Token Expires in: %s", TimeToString(token.expire) );


BAILOUT:

if( token.val )
	free( token.val );

return;
}

