
typedef struct {
	size_t len;
	char *ptr;
} FBStringDef, *FBStringPtr;

void init_string( FBStringPtr s ) {

s->len = 0;
s->ptr = malloc(s->len+1);

if (s->ptr == NULL) {
	critical( "malloc failure" );
}

s->ptr[0] = '\0';

return;
}

size_t writefunc( void *ptr, size_t size, size_t nmemb, FBStringPtr s ) {
	size_t new_len = ( s->len + size*nmemb );

s->ptr = realloc(s->ptr, new_len+1);
if (s->ptr == NULL) {
	critical( "realloc failed" );
	return -3;
}
memcpy(s->ptr+s->len, ptr, size*nmemb);
s->ptr[new_len] = '\0';
s->len = new_len;

return size*nmemb;
}


void facebook_apptoken( char **token ) {
	int offset = 0, maxlen = 1024;
	char post[1024];
	CURL *curl;
	CURLcode res;

offset += snprintf( &post[offset], (maxlen - offset), "client_id=%s", sysconfig.fb_appid );
offset += snprintf( &post[offset], (maxlen - offset), "&client_secret=%s", sysconfig.fb_secret );
offset += snprintf( &post[offset], (maxlen - offset), "%s", "&grant_type=client_credentials" );

curl = curl_easy_init();
if( curl ) {
	FBStringDef s;
	init_string(&s);
	curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );
	/* always cleanup */
	curl_easy_cleanup(curl);
	*token = strdup(s.ptr);
	free(s.ptr);
}


return;
}

void facebook_usertoken( char **dest, char *code ) {
	int offset = 0, maxlen = 1024;
	int i, num;
	char **split;
	char post[1024];
	CURL *curl;
	CURLcode res;

offset = 0;
offset += snprintf( &post[offset], (maxlen - offset), "client_id=%s", sysconfig.fb_appid );
offset += snprintf( &post[offset], (maxlen - offset), "&client_secret=%s", sysconfig.fb_secret );
offset += snprintf( &post[offset], (maxlen - offset), "&redirect_uri=%s", "http://localhost:8880/facebook" );
offset += snprintf( &post[offset], (maxlen - offset), "&code=%s", code );

curl = curl_easy_init();
if( curl ) {
	FBStringDef s;
	init_string(&s);
	curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );

	/* always cleanup */
	curl_easy_cleanup(curl);
	split = str_split( s.ptr, '&', &num );
		
	if ( split == NULL ) {
		info( "str_split returned NULL" );
	} else {
		for ( i = 0; i < num; i++ ) {
			if( strncmp( split[i], "access_token", strlen("access_token") ) == 0 ) {
				*dest = strdup( (strstr( split[i], "=" )+1) );
			}
		}
	}
	free( split );
	free(s.ptr);
}

return;
}


void iohtmlFunc_facebook( ReplyDataPtr cnt ) {
	int offset = 0, maxlen = 1024;
	char *code, *apptoken = NULL;
	char *token = NULL;
	char post[1024];
	CURL *curl;
	CURLcode res;

facebook_apptoken( &apptoken );

code = iohtmlVarsFind( cnt, "code" );

if( code ) {
	facebook_usertoken( &token, code );

	if( token ) {	
		offset = 0;
		offset += snprintf( &post[offset], (maxlen - offset), "%s", "grant_type=fb_exchange_token" );
		offset += snprintf( &post[offset], (maxlen - offset), "&client_id=%s", sysconfig.fb_appid );
		offset += snprintf( &post[offset], (maxlen - offset), "&client_secret=%s", sysconfig.fb_secret );
		offset += snprintf( &post[offset], (maxlen - offset), "&fb_exchange_token=%s", token );
		offset += snprintf( &post[offset], (maxlen - offset), "&redirect_uri=%s", "http://localhost:8880/facebook" );
		
		offset = 0;
		offset += snprintf( &post[offset], (maxlen - offset), "%s", "https://graph.facebook.com/me" );
		offset += snprintf( &post[offset], (maxlen - offset), "?access_token=%s", token );
	

	curl = curl_easy_init();
	if( curl ) {
		FBStringDef s;
    		init_string(&s);
    		curl_easy_setopt(curl, CURLOPT_URL, post );
		//curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/access_token" );
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );

		/* always cleanup */
		curl_easy_cleanup(curl);

		cJSON *root = cJSON_Parse(s.ptr);
		if( root ) {
			cJSON *message = cJSON_GetObjectItem(root,"id");
			if( ( message ) ) {
				info( itoa(message->type) );
				info( ( ( message->type == cJSON_Number ) ? itoa(message->valueint) : message->valuestring ) );
			}
			message = cJSON_GetObjectItem(root,"first_name");
			if( ( message ) ) {
				info( itoa(message->type) );
				info( ( ( message->type == cJSON_Number ) ? itoa(message->valueint) : message->valuestring ) );
			}
			message = cJSON_GetObjectItem(root,"last_name");
			if( ( message ) ) {
				info( itoa(message->type) );
				info( ( ( message->type == cJSON_Number ) ? itoa(message->valueint) : message->valuestring ) );
			}
			message = cJSON_GetObjectItem(root,"timezone");
			if( ( message ) ) {
				info( itoa(message->type) );
				info( ( ( message->type == cJSON_Number ) ? itoa(message->valuedouble) : message->valuestring ) );
				printf( "%f\n", message->valuedouble );
			}
			message = cJSON_GetObjectItem(root,"name");
			if( ( message ) ) {
				info( itoa(message->type) );
				info( ( ( message->type == cJSON_Number ) ? itoa(message->valueint) : message->valuestring ) );
				httpPrintf( cnt, "Welcome %s", message->valuestring );
			}
		} else {
			error( "Bad Responce" );
		}
		cJSON_Delete(root);
		//printf( s.ptr );
    		free(s.ptr);
	}
	
	}
} else {
	httpString( cnt, "Invalid" );
}


if( apptoken )
	free( apptoken );
if( token )
	free( token );

return;
}

