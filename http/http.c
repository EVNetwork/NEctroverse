
#define PACKAGE_VERSION "NEctroverse HTTP 0.2"

#define MHD_HTTP_HEADER_CONTENT_DISPOSITION "Content-Disposition"
#define MHD_HTTP_HEADER_REFRESH "Refresh"


#include "../config/global.h"

#include "memorypool.c"
#include "internal.c"
#include "response.c"
#include "reason_phrase.c"
#include "postprocessor.c"
#include "connection.c"
#if HTTPS_SUPPORT
#include "connection_https.c"
#endif

#include "daemon.c"


// Our page list is in a seperate file... since it could eventuly get kinda large.
#include "pagelist.c"


static struct MHD_Daemon *PlainHTTP;
#if HTTPS_SUPPORT
static struct MHD_Daemon *SecureHTTP;
#endif

char *cmdUploadState[4] = {
"No Upload",
"In Progress",
"Completed",
"Failed"
};

//Memory allocation for the internal working of the HTTP/S server
#define SERVERALLOCATION (512 * KB_SIZE)

//Default buffer allocations, no need to have these too large...
//If needed the server will expand them itself to facilitate page size. 
//-- Eventully I'd like to add an average formula into the allocations as at the moment it will only grow, not shrink.
static size_t buf_size_allocation[2] = { 
	( 64 * KB_SIZE ), //Directory
	( 128 * KB_SIZE ), //HTML Page Generation.
};


/**
 * Response returned if the requested file does not exist (or is not accessible).
 */
static struct MHD_Response *file_not_found_response;

/**
 * Response returned for internal errors.
 */
static struct MHD_Response *internal_error_response;

/**
 * Response returned for refused uploads.
 */
static struct MHD_Response *request_refused_response;

/**
 * Response returned for '/files'
 */
static struct MHD_Response *cached_directory_response;

/**
 * Global handle to MAGIC data. -- If avalible, which on my system it is not... as I don't want yet another linked lib.
 */
#if HAVE_MAGIC_H
#define MAGIC_HEADER_SIZE (16 * 1024)
static magic_t magic;
#endif

static void mark_as( struct MHD_Response *response, const char *type ) {

(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, type);

return;
}

/**
 * Head of index page.
 */
#define UPLOAD_DIR_PAGE_HEADER "<html>\n<head><title>Welcome</title></head>\n<body>\n"\
   "<h1>Download</h1>\n"\
   "<ol>\n"

/**
 * Footer of index page.
 */
#define UPLOAD_DIR_PAGE_FOOTER "</ol>\n</body>\n</html>"

#define FILE_EXISTS_ERROR "File already exists"

#define INTERNAL_ERROR_PAGE "<html><head><title>Server Error</title></head><body>An internal error has occured....</body></html>"

/**
 * Invalid method page.
 */
#define METHOD_ERROR "<html><head><title>Illegal request</title></head><body>Bad request.</body></html>"

/**
 * Invalid URL page.
 */
#define NOT_FOUND_ERROR "<html><head><title>Not found</title></head><body>The item you are looking for was not found...</body></html>"

/*
 * OK, so lets start easing the CPU load... We don't need to re-generate this each time
 * As it should not change for the lifetime of the server. (-Unless tick speed is changed-)
 */
char *ServerSessionMD5;

static int GenServerSum() {
	ConfigArrayPtr settings[2];
	size_t size;
	char *buffer;

settings[0] = GetSetting( "Tick Speed" );
settings[1] = GetSetting( "Server Name" );

size = 32 + strlen( settings[1]->string_value );

if( (buffer = malloc( size ) ) == NULL ) {
	critical( "Out of Memory" );
	return NO;
}

if( (ServerSessionMD5 = malloc( MD5_HASHSUM_SIZE ) ) == NULL ) {
	critical( "Out of Memory" );
	return NO;
}


snprintf( buffer, size, "%.0f;%s", settings[0]->num_value, settings[1]->string_value );
md5_string( buffer, ServerSessionMD5 );

free( buffer );

return YES;
}



/**
 * Linked list of all active sessions.  Yes, O(n)
 */
static SessionPtr SessionList;
//static SessionPtr SessionTable[ARRAY_MAX];

/**
 * Return the session handle for this connection, or
 * create one if this is a new user.
 */
SessionPtr get_session( int type, void *cls ) {
	SessionPtr ret;
	int id;
	char buffer[256];
	const char *cookie = NULL;

if( type == SESSION_HTTP ) {
	struct MHD_Connection *connection = cls;
	cookie = MHD_lookup_connection_value (connection, MHD_COOKIE_KIND, ServerSessionMD5 );
	if( cookie == NULL )
		cookie = MHD_lookup_connection_value( connection, MHD_GET_ARGUMENT_KIND, ServerSessionMD5 );
} else if( type == SESSION_IRC ) {
	cookie = cls;
}

if (cookie != NULL) {
	ret = SessionList;
	while (NULL != ret) {
		if( 0 == strcmp( cookie, ret->sid ) )
			break;
		ret = ret->next;
	}
	if (NULL != ret) {
		ret->rc++;
		time(&ret->active);
		return ret;
	}
}

//No Cookie, Make a new one.
if( NULL == ( ret = calloc( 1, sizeof(SessionDef) ) ) ) {
	critical( "HTTP session allocation error!" );
	return NULL;
}
ret->dbuser = NULL;
if( type == SESSION_HTTP ) {
	RANDOMIZE_SEED;
	snprintf(buffer, sizeof(buffer), "%X%X%X%X", (unsigned int)random(), (unsigned int)random(), (unsigned int)random(), (unsigned int)random() );
	snprintf(ret->sid, SESSION_SIZE, "%s", buffer );
} else if( type == SESSION_IRC ) {
	snprintf(ret->sid, SESSION_SIZE, "%s", cookie );
}

if( ( type == SESSION_HTTP ) && ( cookie != NULL ) ) {
	if( ( ( id = dbUserSessionSearch( (char *)cookie ) ) < 0  ) ) {
		goto MAKECOOKIE;
	} else {
		ret->dbuser = dbUserLinkID( id );
		time(&now);
		//
		if( (now - (ret->dbuser)->lasttime) > ( SESSION_TIME ) ) {
			ret->dbuser = NULL;
			goto MAKECOOKIE;
		}
		//This should already be true... but what the heck, lets double check.
		if( strcmp( cookie, ret->dbuser->http_session ) == 0 )
			strcpy(ret->sid,cookie);
	}
}

MAKECOOKIE:
ret->rc++;
ret->postdata = NULL;
time(&ret->active);
time(&ret->start);
ret->next = SessionList;

SessionList = ret;

return ret;
}




/**
 * Add header to response to set a session cookie.
 *
 * @param session session to use
 * @param response response to modify
 */
static void add_session_cookie( SessionPtr session, struct MHD_Response *response ) {
	time_t time_r;
	int offset = 0;
	char buffer[256];
	char timebuf[512];
	ConfigArrayPtr setting;

offset += snprintf( &buffer[offset], ( sizeof(buffer) - offset ), "%s=%s;", ServerSessionMD5, session->sid );

setting = GetSetting( "Cookie Domain" );
if( ( setting->string_value ) && ( strcmp( setting->string_value, "false" ) ) )
	offset += snprintf( &buffer[offset], ( sizeof(buffer) - offset ), " Domain=.%s;", setting->string_value );

time_r = ( time(0) + SESSION_TIME );
strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &time_r ) );

offset += snprintf( &buffer[offset], ( sizeof(buffer) - offset ), " Max-Age=%ld; Expires=%s", SESSION_TIME, timebuf );

if (NO == MHD_add_response_header(response, MHD_HTTP_HEADER_SET_COOKIE, buffer)) {
	error( "Failed to set session cookie header!" );
}


return;
}

static ssize_t file_read( void *cls, uint64_t pos, char *buf, size_t max ){
	FILE *file = cls;

file_s(file, pos);

return fread(buf, 1, max, file);
}

static void file_free_callback( void *cls ) {
	FILE *file = cls;

fclose (file);

}


/**
 * Replace the existing 'cached_directory_response' with the
 * given response.
 *
 * @param response new directory response
 */
static void update_cached_response(struct MHD_Response *response) {

(void) pthread_mutex_lock (&mutex);

if (NULL != cached_directory_response)
	MHD_destroy_response (cached_directory_response);
cached_directory_response = response;

(void) pthread_mutex_unlock (&mutex);

}

void *buffer_realloc( StringBufferPtr buffer, int type, size_t fitsize, int *newsize ) {
	void *r;
	size_t ajust;
//Always add 1k at the end of new buffer, as "slack space"
ajust = ( ( buffer->buf_len + fitsize ) + KB_SIZE );

if( type ) {
	buf_size_allocation[type-1] = ajust;
}

if( ajust < buffer->buf_len ) {
	critical( "Size overflow" );
	Shutdown();
}

buffer->buf_len = ajust;

*newsize = (buffer->buf_len - buffer->off);

if( NULL == ( r = realloc( buffer->buf, buffer->buf_len ) ) ) {
	critical( "Realloc Failed" );
	*newsize = 0;
}

return r;
}

void AddBufferString( StringBufferPtr buffer, char *text ) {
	int buf_max = (buffer->buf_len - buffer->off);
	int buf_len = strlen( text );

if( ( buf_max - buf_len ) < 0 ) {
	buffer->buf = buffer_realloc( buffer, 0, buf_len, &buf_max );
}

buffer->off += snprintf( &buffer->buf[buffer->off], buf_max, "%s", text );

return;
}

void AddBufferPrint( StringBufferPtr buffer, char *fmt, ... ) {
	char text[ARRAY_MAX];
	va_list ap;

va_start( ap, fmt );
vsnprintf( text, ARRAY_MAX, fmt, ap );
va_end( ap );

AddBufferString( buffer, text );

return;
}


void httpString( ReplyDataPtr rd, char *text ) {
	int buf_max = (rd->cache.buf_len - rd->cache.off);
	int buf_len = strlen( text );
	
if( ( buf_max - buf_len ) < 0 ) {
	rd->cache.buf = buffer_realloc( &rd->cache, 2, buf_len, &buf_max );
}

rd->cache.off += snprintf( &rd->cache.buf[rd->cache.off], buf_max, "%s", text );

return;
}

void httpPrintf( ReplyDataPtr rd, char *fmt, ... ) {
	char text[ARRAY_MAX];
	va_list ap;

va_start( ap, fmt );
vsnprintf( text, ARRAY_MAX, fmt, ap );
va_end( ap );

httpString( rd, text );

return;
}

static int list_directory( StringBufferPtr rd, const char *dirname ) {
	int buf_max = (rd->buf_len - rd->off);
	char fullname[PATH_MAX];
	struct stat sbuf;
	struct dirent *de;
	DIR *dir;

if (NULL == (dir = opendir (dirname)))
	return NO;      

while (NULL != (de = readdir (dir))) {
	if ('.' == de->d_name[0])
		continue;
	if( sizeof (fullname) <= snprintf (fullname, sizeof (fullname), "%s/%s", dirname, de->d_name) )
		continue;
	if (0 != stat (fullname, &sbuf))
		continue;
	if (! S_ISREG (sbuf.st_mode))
		continue;
	if (rd->off + 1024 > rd->buf_len) {
		rd->buf = buffer_realloc( rd, 1, DEFAULT_BUFFER, &buf_max );
	}
	rd->off += snprintf (&rd->buf[rd->off], buf_max, "<li><a href=\"/files?type=download&name=%s\">%s</a></li>\n", de->d_name, de->d_name );
}
(void)closedir( dir );

return YES;
}


/**
 * Re-scan our local directory and re-build the index.
 */
static void update_directory( struct MHD_Connection *connection ) {
	struct MHD_Response *response;
	StringBufferDef rd;
	ConfigArrayPtr settings;
	char dir_name[PATH_MAX];
	struct stat sbuf;

rd.buf_len = buf_size_allocation[0]; 
if( NULL == ( rd.buf = malloc(rd.buf_len) ) ) {
	critical( "Malloc Failed" );
	update_cached_response (NULL);
	return; 
}
rd.off = snprintf (rd.buf, rd.buf_len, "%s", UPLOAD_DIR_PAGE_HEADER );
settings = GetSetting( "Directory" );
snprintf( dir_name, sizeof(dir_name), "%s/uploads", settings->string_value );

if( 0 == stat (dir_name, &sbuf) ) {  
	if (NO == list_directory (&rd, dir_name)) {
		free (rd.buf);
		update_cached_response (NULL);
		return;
	}
}

rd.off += snprintf (&rd.buf[rd.off], rd.buf_len - rd.off, "%s", UPLOAD_DIR_PAGE_FOOTER );

response = MHD_create_response_from_buffer (rd.off, rd.buf, MHD_RESPMEM_MUST_FREE );
mark_as( response, "text/html" );
(void)MHD_add_response_header (response, MHD_HTTP_HEADER_CONNECTION, "close");

update_cached_response (response);
}



/**
 * Return the current directory listing.
 * 
 * @param connection connection to return the directory for
 * @return YES on success, NO on error
 */
static int return_directory_response( struct MHD_Connection *connection ) {
	int ret;

(void) pthread_mutex_lock( &mutex );

if (NULL == cached_directory_response)
	ret = MHD_queue_response( connection, MHD_HTTP_INTERNAL_SERVER_ERROR,  internal_error_response );
else
	ret = MHD_queue_response( connection, MHD_HTTP_OK, cached_directory_response );

(void)pthread_mutex_unlock( &mutex );


return ret;
}

/**
 * Handler used to generate a 404 reply.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
int not_found_page ( int id, const void *cls, const char *mime, SessionPtr session, struct MHD_Connection *connection) {
	int ret;

ret = MHD_queue_response (connection, MHD_HTTP_NOT_FOUND, file_not_found_response);

return ret;
}

static FileStoragePtr StoredFiles;

int file_render ( int id, const void *cls, const char *mime, SessionPtr session, struct MHD_Connection *connection) {
	#if HAVE_MAGIC_H
	char file_data[MAGIC_HEADER_SIZE];
	ssize_t got;
	#endif
	int ret;
	char dmsg[PATH_MAX];
	char *type, *fname;
	struct stat buf;
	struct MHD_Response *response;
	FileStoragePtr filelist;
	ConfigArrayPtr settings;
	FILE *file;

memset( &dmsg, 0, PATH_MAX );

type = (char *)MHD_lookup_connection_value( connection, MHD_GET_ARGUMENT_KIND, "type");
fname = (char *)MHD_lookup_connection_value( connection, MHD_GET_ARGUMENT_KIND, "name");

if( ( type ) && ( strcmp( type, "download" ) == 0 ) ) {
	settings = GetSetting( "Directory" );
	snprintf(dmsg, PATH_MAX, "%s/uploads/%s", settings->string_value, fname );
} else if ( ( type ) && ( strcmp( type, "image" ) == 0 ) ) {
	goto IMAGE_BUFFER;
} else if ( ( type ) && ( strcmp( type, "eimage" ) == 0 ) ) {
	goto IMAGE_BUFFER;
} else if ( ( type ) && ( strcmp( type, "server" ) == 0 ) ) {
	settings = GetSetting( "HTTP Files" );
	snprintf(dmsg, PATH_MAX, "%s/%s", settings->string_value, fname );
} else if ( ( type ) && ( strcmp( type, "captcha" ) == 0 ) ) {
	char captcha[256];
	snprintf(captcha, 256, "%s.gif", session->sid );
	fname = captcha;
	snprintf(dmsg, PATH_MAX, "%s/%s", TMPDIR, fname );
} else {
	memset( &dmsg, 0, PATH_MAX );
}

if( (0 == stat (dmsg, &buf)) && (S_ISREG (buf.st_mode)) ) {
	file = fopen (dmsg, "rb");
} else {
	file = NULL;
}

   
if (file == NULL) { 
	update_directory( connection );
	ret = return_directory_response( connection );
} else {
	#if HAVE_MAGIC_H
	got = read (file, file_data, sizeof (file_data));
	if (-1 != got)
		mime = magic_buffer (magic, file_data, got);
	else
	#endif
	mime = NULL;

	response = MHD_create_response_from_callback( buf.st_size, ( SERVERALLOCATION / 8 ), &file_read, file, &file_free_callback );
	if (response == NULL) {
		fclose (file);
		return NO;
	}
	if( strcmp( type, "download" ) == 0 ) {
		snprintf(dmsg, sizeof (dmsg), "filename=\"%s\"", fname );
		(void)MHD_add_response_header( response, MHD_HTTP_HEADER_CONTENT_DISPOSITION, dmsg );
	}
	if (NULL != mime) {
		(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, mime);
	} else {
		(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, iohttpMime[ iohttpMimeFind( fname ) ].def );
	}

	strftime(fname,512,"%a, %d %b %G %T %Z", gmtime(&buf.st_mtime) );
	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_LAST_MODIFIED, fname );
	goto RETURN;
}

return ret;
IMAGE_BUFFER:
(void) pthread_mutex_lock( &mutex );
filelist = StoredFiles;

for( ; filelist ; filelist = filelist->next ) {
	if( strcmp( filelist->name, fname ) == 0 )
		break;
}
if( filelist == NULL ) {
	if( strcmp( type, "eimage" ) == 0 ) {
		settings = GetSetting( "Directory" );
		snprintf(dmsg, sizeof (dmsg), "%s/uploads/%s", settings->string_value, fname );
	} else {
		settings = GetSetting( "HTTP Images" );
		snprintf(dmsg, sizeof (dmsg), "%s/%s", settings->string_value, fname );
	}
	if( (0 == stat( dmsg, &buf)) && (S_ISREG(buf.st_mode)) ) {
		file = fopen( dmsg, "rb" );
	} else {
		(void) pthread_mutex_unlock( &mutex );
		return MHD_queue_response (connection, MHD_HTTP_NOT_FOUND, file_not_found_response);
	}
	if( (filelist = malloc( 1*sizeof(FileStorageDef) ) ) == NULL ) {
		critical( "Image Memory allocation failed" );
		fclose( file );
		(void) pthread_mutex_unlock( &mutex );
		return NO;
	}
	filelist->data = malloc( buf.st_size );
	file_r( filelist->data, 1, buf.st_size, file );
	filelist->name = strdup( fname );
	filelist->mime = strdup( iohttpMime[ iohttpMimeFind( fname ) ].def );
	filelist->size = buf.st_size;
	filelist->modofied = buf.st_mtime;
	filelist->next = StoredFiles;
	StoredFiles = filelist;
	fclose( file );
}

time( &filelist->lastaccess );

(void) pthread_mutex_unlock( &mutex );

response = MHD_create_response_from_buffer( filelist->size, filelist->data, MHD_RESPMEM_PERSISTENT);
if (response == NULL) {
	return NO;
}

(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, filelist->mime);

strftime(fname,512,"%a, %d %b %G %T %Z", gmtime(&filelist->modofied) );
(void)MHD_add_response_header (response, MHD_HTTP_HEADER_LAST_MODIFIED, fname );

RETURN:

settings = GetSetting( "Server Name" );
(void)MHD_add_response_header (response, MHD_HTTP_HEADER_SERVER, settings->string_value );
ret = MHD_queue_response( connection, MHD_HTTP_OK, response );
MHD_destroy_response( response );
return ret;
}

int page_render( int id, const void *cls, const char *mime, SessionPtr session, struct MHD_Connection *connection) {
	int ret;
	char buffer[REDIRECT_MAX];
	struct MHD_Response *response;
	ReplyDataDef rd;

//Lock the mutex while we form pages, this is just safer... since it prevents double access of in-game infomation.
(void)pthread_mutex_lock( &mutex );	
rd.session = session;
rd.connection = connection;
rd.cache.off = 0;
rd.cache.buf_len = buf_size_allocation[1];
if( NULL == ( rd.cache.buf = malloc( rd.cache.buf_len ) ) ) {
	critical( "Malloc Failed" );
	return -1;
}

html_page[id].function( &rd );

response = MHD_create_response_from_buffer( strlen(rd.cache.buf), rd.cache.buf, MHD_RESPMEM_MUST_FREE);
add_session_cookie(rd.session, response);
mark_as( response, mime );

if( strlen(rd.session->redirect) ) {
	snprintf( buffer, REDIRECT_MAX, "1; url=%s", rd.session->redirect );
	memset( &rd.session->redirect, 0, REDIRECT_MAX );
	MHD_add_response_header(response, MHD_HTTP_HEADER_REFRESH, buffer );
}

ret = MHD_queue_response( rd.connection, MHD_HTTP_OK, response );
MHD_destroy_response( response );
(void)pthread_mutex_unlock( &mutex );

return ret;
}

static int postdata_set( SessionPtr session, const char *key, const char *value ) {
	PostDataPtr data;

if( session->postdata != NULL) {
	for(data = session->postdata ; data ; data = data->next ) {
		if( ( strcmp( key, data->key ) == 0 ) ) {
			void *r;
			if( strlen( value ) == 0 ) {
				//No data to add, so we'll pretend we did something and pass an OK result back -- I mean, how can we fail here... there's nothing to do! =D
				return YES;
			}
			int toadd = ( strlen( value ) + 32 ); //Ensure at least 32 bytes of "slack space"
			if( (( data->current - data->offset ) - toadd ) < 0 ) {
				//Buffer is too small, adding post data will over-flow, so need to re-size -- Add size ( data-in + 1kb )
				int ajust = ( ( data->current + toadd ) + KB_SIZE );
				if( ajust < data->current ) {
					critical( "Size Overflow" );
					//Shutdown();
					return NO;
				}
				//And now the actuall ajustment...
				if( ( r = realloc( data->value, ajust ) ) == NULL ) {
					critical( "Out of memory" );
					return NO;
				}
				data->value = r;
				data->current = ajust;
			}
			//And finnaly, if we made it this far... there's data to add and out buffer should have ample capacity.
			//Return add function, this saves us having to re-check for success;
			return ( data->offset += snprintf(&data->value[data->offset], ( data->current - data->offset ), "%s", value ) );
		}
	}
}



if( ( data = malloc( 1*sizeof(PostDataDef) ) ) == NULL ) {
	critical( "Out of memory" )
	return NO;
}

data->key = strdup( key );
data->value = strdup( value );
data->offset = strlen( value );
data->current = data->offset;
data->next = session->postdata;

session->postdata = data;

return YES;
}

static int postdata_remove( SessionPtr session, const char *key ) {
	PostDataPtr pos;
	PostDataPtr prev;
	PostDataPtr next;

prev = NULL;
pos = session->postdata;

while( NULL != pos ) {
	next = pos->next;
	if( 0 == strcmp( key, pos->key ) ) {
		if (NULL == prev) {
			session->postdata = pos->next;
		} else {
			prev->next = next;
		}
		free( pos->key );
		free( pos->value );		
		free( pos );
		return YES;
	} else {
	        prev = pos;
        }
	pos = next;
}


return NO;
}

static int postdata_wipe( SessionPtr session ) {
	PostDataPtr data;
	
if( session->postdata != NULL ) {
	for( data = session->postdata ; data ; data = data->next )
		postdata_remove( session, data->key );
	
}

return YES;
}


/**
 * Iterator over key-value pairs where the value
 * maybe made available in increments and/or may
 * not be zero-terminated.  Used for processing
 * POST data.
 *
 * @param cls user-specified closure
 * @param kind type of the value, always MHD_POSTDATA_KIND when called from MHD
 * @param key 0-terminated key for the value
 * @param filename name of the uploaded file, NULL if not known
 * @param content_type mime-type of the data, NULL if not known
 * @param transfer_encoding encoding of the data, NULL if not known
 * @param data pointer to size bytes of data at the
 *              specified offset
 * @param off offset of data in the overall value
 * @param size number of bytes in data available
 * @return YES to continue iterating,
 *         NO to abort the iteration
 */

static int process_upload_data( void *cls, enum MHD_ValueKind kind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size ) {
	ConfigArrayPtr settings;
	dbMainEmpireDef empired;
	dbUserMainDef maind;
	RequestPtr uc = cls;
	int i;

if( ( !( filename ) ) ) {
	return postdata_set( uc->session, key, data );
}
if (NULL == filename) {
	error( "No filename, aborting upload" );
	(uc->session)->upload = UPLOAD_STATE_FAIL;
	return NO;
}

if (-1 == uc->fd) {
	if ( (NULL != strstr (filename, "..")) || (NULL != strchr (filename, '/')) || (NULL != strchr (filename, '\\')) ) {
		uc->response = request_refused_response;
		return NO;
	}
	//User logged in? If not, reject file upload.
	if( !( ( (uc->session)->dbuser ) ) ) {
		uc->response = request_refused_response;
		(uc->session)->upload = UPLOAD_STATE_FAIL;
		return NO;
	} else if( dbUserMainRetrieve( ((uc->session)->dbuser)->id, &maind ) < 0 ) {
		error( "User Info Lookup Failed" );
		return NO;
	}
	
	if( ( maind.empire != -1 ) && ( dbEmpireGetInfo( maind.empire, &empired ) < 0 ) )
		return NO;
	char fn[PATH_MAX];
	settings = GetSetting( "Directory" );
	if( empired.politics[CMD_POLITICS_LEADER] == ((uc->session)->dbuser)->id ) {
		if( empired.picture > 0 ) {
			snprintf(fn, sizeof (fn),"%s/uploads/empire%d/pic%d", settings->string_value, maind.empire, empired.picture );
			unlink(fn);
		}
		empired.picture = rand();
	} else {
		return NO;
	}
	if ( dbEmpireSetInfo( maind.empire, &empired ) < 0 ) {
		error( "Saving \'%s\' for user #%d", filename, ((uc->session)->dbuser)->id );
		return NO;
	}
	snprintf (fn, sizeof (fn), "%s/uploads/empire%d", settings->string_value, maind.empire );
	(void) dirstructurecheck(fn,false);
	snprintf(fn, sizeof (fn),"%s/uploads/empire%d/pic%d", settings->string_value, maind.empire, empired.picture );
	for (i=strlen (fn)-1;i>=0;i--) {
		if( !isprint( (int)fn[i] ) ){
			fn[i] = '_';
		}
	}
	uc->fd = open (fn, O_CREAT | O_EXCL
#if O_LARGEFILE
		     | O_LARGEFILE
#endif
		     | O_WRONLY,S_IRUSR | S_IWUSR);

	if (-1 == uc->fd) {
		error( "Error opening file for upload: \'%s\'", fn );
		uc->response = request_refused_response;
		(uc->session)->upload = UPLOAD_STATE_FAIL;
		return NO;
	}
	uc->filename = strdup (fn);
}

if ( (0 != size) && (size != write (uc->fd, data, size)) ) {
	error( "Error writing to file: \'%s\'", uc->filename);
	uc->response = internal_error_response;
	(uc->session)->upload = UPLOAD_STATE_FAIL;
	close (uc->fd);
	uc->fd = -1;
	if (NULL != uc->filename) {
		unlink (uc->filename);
		free (uc->filename);
		uc->filename = NULL;
	}
	return NO;
}


return YES;
}

/**
 * Main MHD callback for handling requests.
 *
 *
 * @param cls argument given together with the function
 *        pointer when the handler was registered with MHD
 * @param url the requested url
 * @param method the HTTP method used ("GET", "PUT", etc.)
 * @param version the HTTP version string (i.e. "HTTP/1.1")
 * @param upload_data the data being uploaded (excluding HEADERS,
 *        for a POST that fits into memory and that is encoded
 *        with a supported encoding, the POST data will NOT be
 *        given in upload_data and is instead available as
 *        part of MHD_get_connection_values; very large POST
 *        data *will* be made available incrementally in
 *        upload_data)
 * @param upload_data_size set initially to the size of the
 *        upload_data provided; the method must update this
 *        value to the number of bytes NOT processed;
 * @param con_cls pointer that the callback can set to some
 *        address and that will be preserved by MHD for future
 *        calls for this request; since the access handler may
 *        be called many times (i.e., for a PUT/POST operation
 *        with plenty of upload data) this allows the application
 *        to easily associate some request-specific state.
 *        If necessary, this state can be cleaned up in the
 *        global "MHD_RequestCompleted" callback (which
 *        can be set with the MHD_OPTION_NOTIFY_COMPLETED).
 *        Initially, <tt>*con_cls</tt> will be NULL.
 * @return MHS_YES if the connection was handled successfully,
 *         MHS_NO if the socket must be closed due to a serious
 *         error while handling the request
 */

static int create_response (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data,  size_t *upload_data_size, void **ptr) {
	RequestPtr request;
	SessionPtr session;
	urlinfoPtr urlp;
	int ret;
	unsigned int i;
	const char *temp_x[2];
	bool allowed = false;

/*
 * Ajustement to allowed is to check that post requests only come from permitted sources.
 * Such as localhost (this server), and facebook login requests.
 * Possibly change this to a real list if more support needed.
 *      ---(Flawed, but sufficent for now)---
 */
temp_x[0] = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Referer");
temp_x[1] = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
if( ( temp_x[0] ) && ( temp_x[1] ) ) {
	urlp = parse_url( temp_x[0] );
	if( strstr( temp_x[0], urlp->host ) || strstr( temp_x[0], "facebook.com" ) ) {
		allowed = true;
	}
	urlinfo_free( urlp );
}

request = *ptr;
if( (0 == strcmp( method, MHD_HTTP_METHOD_POST) ) && ( allowed ) ) {
	if (NULL == request) {
		if( NULL == (request = calloc(1, sizeof(RequestDef) ) ) )
			return NO; /* out of memory, close connection */
		request->session = get_session( SESSION_HTTP, connection );
		if (NULL == request->session) {
			error( "Failed to setup session for \'%s\'", url );
			free (request);
			return NO; /* internal error */
		}
		request->session->upload = UPLOAD_STATE_START;
		request->post_url = url;
		request->fd = -1;
		request->connection = connection;
		request->pp = MHD_create_post_processor (connection, ( SERVERALLOCATION / 4 ), &process_upload_data, request);
		if (NULL == request->pp) {
			/* out of memory, close connection */
			free (request);
			return NO;
		}
		*ptr = request;
		return YES;
	}
	if (0 != *upload_data_size) {
		if (NULL == request->response)
			(void)MHD_post_process( request->pp, upload_data, *upload_data_size );
		*upload_data_size = 0;
		return YES;
	}
	/* end of upload, finish it! */
	MHD_destroy_post_processor(request->pp);
	request->pp = NULL;
	if (-1 != request->fd) {
		close (request->fd);
		request->fd = -1;
		request->session->upload = UPLOAD_STATE_DONE;
	}
	if (NULL != request->response) {
		return MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, request->response);
	} else {
	i=0;
	while ( (html_page[i].url != NULL) && (0 != strcmp (html_page[i].url, request->post_url)) )
		i++;
	ret = html_page[i].handler( i, html_page[i].handler_cls, html_page[i].mime, request->session, request->connection );
	if (ret != YES) {
		error( "Failed to create page for \'%s\'", request->post_url);
	}
	return ret;
	}
}

if( ( request ) && ( request->session ) ) {
	session = request->session;
} else {
	session = get_session( SESSION_HTTP, connection );
	session->upload = UPLOAD_STATE_NULL;
}

if ( (0 == strcmp (method, MHD_HTTP_METHOD_GET)) || (0 == strcmp (method, MHD_HTTP_METHOD_HEAD)) ) {
	i=0;
	while ( (html_page[i].url != NULL) && (0 != strcmp (html_page[i].url, url)) )
		i++;
	ret = html_page[i].handler( i, html_page[i].handler_cls, html_page[i].mime, session, connection );
	if (ret != YES) {
		error( "Failed to create page for \'%s\'", url);
	}
	return ret;
}


/* unsupported HTTP method */
ret = MHD_queue_response (connection, MHD_HTTP_METHOD_NOT_ACCEPTABLE, request_refused_response);

return ret;
}

/**
 * Function called whenever a request was completed.
 * Used to clean up 'struct UploadContext' objects.
 *
 * @param cls client-defined closure, NULL
 * @param connection connection handle
 * @param con_cls value as set by the last call to
 *        the MHD_AccessHandlerCallback, points to NULL if this was
 *            not an upload
 * @param toe reason for request termination
 */
static void completed_callback (void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
	RequestPtr request = *con_cls;

if (NULL == request)
	return;
if (NULL != request->session)
	request->session->rc--;
if (NULL != request->pp) {
	MHD_destroy_post_processor(request->pp);
	request->pp = NULL;
}

if (-1 != request->fd) {
	(void) close (request->fd);
	if (NULL != request->filename) {
		request->session->upload = UPLOAD_STATE_FAIL;
		error( "Upload of file `%s' failed (incomplete or aborted), removing file.", request->filename );
		(void) unlink (request->filename);
	}
}

if (NULL != request->filename) {
	free (request->filename);
}

postdata_wipe( request->session );

free(request);
}

static int access_check(void *cls, const struct sockaddr *addr, socklen_t addrlen) {
	int a;

for( a = 0 ; a < sysconfig.banlist.number ; a++ ) {
	if( ioCompareFindWords( inet_ntoa( ((struct sockaddr_in *)addr)->sin_addr ), sysconfig.banlist.ip[a] ) )
		return NO;
}

return YES;
}

void purge_captcha( SessionPtr session ) {
	char captcha[PATH_MAX];
	struct stat buf;

if( session->captcha[0] ) {
	memset( &session->captcha, 0, sizeof( session->captcha ) );
}
snprintf(captcha, PATH_MAX, "%s/%s.gif", TMPDIR, session->sid);
if( (0 == stat( captcha, &buf)) && (S_ISREG(buf.st_mode)) ) {
	unlink( captcha );
}

return;
}


/**
 * Clean up handles of sessions that have been idle for
 * too long.
 */
static void expire_sessions () {
	int id;
	dbUserPtr user;
	SessionPtr pos;
	SessionPtr prev;
	SessionPtr next;

time(&now);
prev = NULL;
pos = SessionList;

while( NULL != pos ) {
	next = pos->next;
	if( (now - pos->active) > ( SESSION_TIME ) ) {
		if (NULL == prev) {
			SessionList = pos->next;
		} else {
			prev->next = next;
		}
		if( ( ( id = dbUserSessionSearch( pos->sid ) ) >= 0 ) ) {
			if( ( user = dbUserLinkID( id ) ) ) {
				memset( user->http_session, 0, sizeof(user->http_session) );
				dbUserSave( id, user );
			}
		}
		postdata_wipe( pos );
		purge_captcha( pos );
		free( pos );
	} else {
	        prev = pos;
        }
	pos = next;
}

}


int remove_session( const char *sid ) {
	int id;
	dbUserPtr user;
	SessionPtr pos;
	SessionPtr prev;
	SessionPtr next;

prev = NULL;
pos = SessionList;

while( NULL != pos ) {
	next = pos->next;
	if( 0 == strcmp( sid, pos->sid ) ) {
		if (NULL == prev) {
			SessionList = pos->next;
		} else {
			prev->next = next;
		}
		if( ( ( id = dbUserSessionSearch( pos->sid ) ) >= 0 ) ) {
			if( ( user = dbUserLinkID( id ) ) ) {
				memset( user->http_session, 0, sizeof(user->http_session) );
				dbUserSave( id, user );
			}
		}
		postdata_wipe( pos );
		purge_captcha( pos );
		free( pos );
		return YES;
	} else {
	        prev = pos;
        }
	pos = next;
}


return NO;
}


static void expire_file_storage () {
	FileStoragePtr pos;
	FileStoragePtr prev;
	FileStoragePtr next;

time(&now);
prev = NULL;
pos = StoredFiles;

while( NULL != pos ) {
	next = pos->next;
	if( (now - pos->lastaccess) > ( day ) ) {
		if (NULL == prev) {
			StoredFiles = pos->next;
		} else {
			prev->next = next;
		}
		free( pos->name );
		free( pos->mime );
		free( pos->data );
		free( pos );
	} else {
	        prev = pos;
        }
	pos = next;
}

}


#if HTTPS_SUPPORT
static char *loadsslfile( char *filename, int required ) {
	struct stat stdata;
	char *data, *ret;
	FILE *file;

ret = NULL;

if( stat( filename, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( filename, "rb" ) ) ) {
			if( ( fread( data, 1, stdata.st_size, file ) < 1 ) && ( stdata.st_size ) ) {
				error( "Failure reading ssl file." );
			} else {
				ret = strdup( data );
			}
			fclose( file );
		}
		free( data );
	}
} else {
	if( ( filename != NULL ) && ( strlen(filename) ) ) {
		error( "Specified HTTPS File does not exist: %s", filename );
	} else if ( required ) {
		error( "Filename Missing for HTTPS Certificate or Key, please ajust" );
	}
}

return ret;
}
#endif

struct MHD_OptionItem ops[] = {
	{ MHD_OPTION_CONNECTION_MEMORY_LIMIT, (size_t)SERVERALLOCATION, NULL },
	{ MHD_OPTION_PER_IP_CONNECTION_LIMIT, (unsigned int)(64), NULL },
	{ MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int)(120), NULL },
	{ MHD_OPTION_END, 0, NULL }
};
#if MULTI_THREAD_SUPPORT
static int THREADS;
static int flags = MHD_USE_SELECT_INTERNALLY /*| MHD_USE_DUAL_STACK*/; //I have no IPv6, so no point dual stacking.
#else
static int flags = 0 /*| MHD_USE_DUAL_STACK*/; //I have no IPv6, so no point dual stacking.
#endif



int http_prep(){
	char md5sum[MD5_HASHSUM_SIZE];

#if MULTI_THREAD_SUPPORT
	cpuInfo cpuinfo;

cpuGetInfo( &cpuinfo );

THREADS = fmax( 1.0, ( cpuinfo.socketphysicalcores / 2 ) );
#endif

#if HAVE_MAGIC_H
magic = magic_open(MAGIC_MIME_TYPE);
(void) magic_load(magic, NULL);
#endif

file_not_found_response = MHD_create_response_from_buffer( strlen( NOT_FOUND_ERROR ), (void *)NOT_FOUND_ERROR, MHD_RESPMEM_PERSISTENT );
request_refused_response = MHD_create_response_from_buffer( strlen( METHOD_ERROR ), (void *)METHOD_ERROR, MHD_RESPMEM_PERSISTENT );
internal_error_response = MHD_create_response_from_buffer( strlen( INTERNAL_ERROR_PAGE ), (void *)INTERNAL_ERROR_PAGE, MHD_RESPMEM_PERSISTENT );

if( ( file_not_found_response == NULL ) || ( request_refused_response == NULL ) || ( internal_error_response == NULL ) ) {
	return NO;
}

md5_string( METHOD_ERROR, md5sum );
(void)MHD_add_response_header(request_refused_response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
(void)MHD_add_response_header(request_refused_response, MHD_HTTP_HEADER_CONNECTION, "close");
mark_as(request_refused_response, "text/html" );

md5_string(NOT_FOUND_ERROR, md5sum );
(void)MHD_add_response_header(file_not_found_response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
(void)MHD_add_response_header(file_not_found_response, MHD_HTTP_HEADER_CONNECTION, "close");
mark_as(file_not_found_response, "text/html" );

md5_string( INTERNAL_ERROR_PAGE, md5sum );
(void)MHD_add_response_header(internal_error_response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
(void)MHD_add_response_header(internal_error_response, MHD_HTTP_HEADER_CONNECTION, "close");
mark_as(internal_error_response, "text/html" );

#if DEBUG_SUPPORT
flags |=  MHD_USE_DEBUG;
#endif

#if EPOLL_SUPPORT
flags |= MHD_USE_EPOLL_LINUX_ONLY;
#if EPOLL_TURBO
flags |= MHD_USE_EPOLL_TURBO;
#endif
#endif

#if HTTPS_SUPPORT
#if MULTI_THREAD_SUPPORT
THREADS = fmax( 1.0, ( THREADS / 2 ) );
#endif
#endif


if( ServerSessionMD5 == NULL ) {
	if( GenServerSum() == NO ) {
		return NO;
	}
}

return YES;
}

#if HTTPS_SUPPORT
static char *ssl_files[3];

int https_start() {
	ConfigArrayPtr settings;

char *list[4] = { "HTTPS Key", "HTTPS Cert"/*, "HTTPS Trust"*/, NULL };
settings = ListSettings( list );

ssl_files[0] = loadsslfile( settings[0].string_value, true );
ssl_files[1] = loadsslfile( settings[1].string_value, true );
/*ssl_files[2] = loadsslfile( settings[2].string_value, false );*/

free( settings );

SecureHTTP = MHD_start_daemon (flags | MHD_USE_SSL,
				options.port[PORT_HTTPS],
				&access_check, NULL,
				&create_response, NULL,
				MHD_OPTION_ARRAY, ops,
				MHD_OPTION_HTTPS_MEM_KEY, ssl_files[0],
				MHD_OPTION_HTTPS_MEM_CERT, ssl_files[1],
				/*MHD_OPTION_HTTPS_MEM_TRUST, ssl_files[2],
				MHD_OPTION_HTTPS_CRED_TYPE, GNUTLS_CRD_CERTIFICATE,
				MHD_OPTION_HTTPS_PRIORITIES, "NORMAL:+COMP-ALL",*/
				#if MULTI_THREAD_SUPPORT
				MHD_OPTION_THREAD_POOL_SIZE, (unsigned int)fmax( 1.0, THREADS ),
				#endif
				MHD_OPTION_NOTIFY_COMPLETED, &completed_callback, NULL,
				MHD_OPTION_END);




if(NULL == SecureHTTP) {
	return NO;
}

#if MULTI_THREAD_SUPPORT
info( "HTTPS Server live with %d thread%s on port: %d", THREADS, ( (THREADS > 1) ? "s" : "" ), options.port[PORT_HTTPS] );
#else
info( "HTTPS Server live on port: %d", options.port[PORT_HTTPS] );
#endif


return YES;
}
#endif

int http_start() {

PlainHTTP = MHD_start_daemon (flags,
				options.port[PORT_HTTP],
				&access_check, NULL,
				&create_response, NULL,
				MHD_OPTION_ARRAY, ops,
				#if MULTI_THREAD_SUPPORT
				#if HTTPS_SUPPORT
				MHD_OPTION_THREAD_POOL_SIZE, (unsigned int)fmax( 1.0, THREADS ),
				#else
				MHD_OPTION_THREAD_POOL_SIZE, (unsigned int)THREADS,
				#endif
				#endif
				MHD_OPTION_NOTIFY_COMPLETED, &completed_callback, NULL,
				MHD_OPTION_END);

if(NULL == PlainHTTP) {
	return NO;
}

#if MULTI_THREAD_SUPPORT
info( "HTTP Server live with %d thread%s on port: %d", THREADS, ( (THREADS > 1) ? "s" : "" ), options.port[PORT_HTTP] );
#else
info( "HTTP Server live on port: %d", options.port[PORT_HTTP] );
#endif

return YES;
}

#if MULTI_THREAD_SUPPORT == 0
fd_set WWWSelectRead;
fd_set WWWSelectWrite;
fd_set WWWSelectError;

#define SERVER_SELECT_MSEC 1000

void WWWSelect( ) {
	int max;
	struct timeval tv;
	MHD_UNSIGNED_LONG_LONG mhd_timeout;

tv.tv_usec = ( SERVER_SELECT_MSEC % 1000 ) * 1000;
tv.tv_sec = SERVER_SELECT_MSEC / 1000;
max = 0;
FD_ZERO(&WWWSelectRead);
FD_ZERO(&WWWSelectWrite);
FD_ZERO(&WWWSelectError);

if (YES != MHD_get_fdset( PlainHTTP, &WWWSelectRead, &WWWSelectWrite, &WWWSelectError, &max)) {
	critical( "WWW Select Get" );
	return; /* fatal internal error */
}
if (MHD_get_timeout( PlainHTTP, &mhd_timeout ) == YES) {
	if (tv.tv_sec * 1000 < mhd_timeout) {
		tv.tv_sec = mhd_timeout / 1000;
		tv.tv_usec = (mhd_timeout - (tv.tv_sec * 1000)) * 1000;
	}
}

#if HTTPS_SUPPORT
if( SecureHTTP != NULL ) {
	max = 2;
	if (YES != MHD_get_fdset( SecureHTTP, &WWWSelectRead, &WWWSelectWrite, &WWWSelectError, &max)) {
		critical( "WWW Select Get HTTPS" );
		return; /* fatal internal error */
	}
	if (MHD_get_timeout( SecureHTTP, &mhd_timeout ) == YES) {
		if (tv.tv_sec * 1000 < mhd_timeout) {
			tv.tv_sec = mhd_timeout / 1000;
			tv.tv_usec = (mhd_timeout - (tv.tv_sec * 1000)) * 1000;
		}
	}
}
#endif

if( ( select(max + 1, &WWWSelectRead, &WWWSelectWrite, &WWWSelectError, &tv) < 0 ) && ( sysconfig.shutdown == false ) ){
	error( "WWW Select Apply" );
}

MHD_run( PlainHTTP );

#if HTTPS_SUPPORT
if( SecureHTTP != NULL ) {
	MHD_run( SecureHTTP );
}
#endif

return;
}
#endif

void Shutdown() {

sysconfig.shutdown = true;

if( PlainHTTP ) {
	MHD_stop_daemon(PlainHTTP);
	info( "HTTP Server has been gracefully shutdown!" );
}
#if HTTPS_SUPPORT
if( SecureHTTP ) {
	MHD_stop_daemon(SecureHTTP);
	info("HTTPS Server has been gracefully shutdown!");
}
if( ssl_files[0] )
	free( ssl_files[0] );
if( ssl_files[1] )
	free( ssl_files[1] );
if( ssl_files[2] )
	free( ssl_files[2] );
#endif

MHD_destroy_response (file_not_found_response);
MHD_destroy_response (request_refused_response);
MHD_destroy_response (internal_error_response);
update_cached_response (NULL);
(void) pthread_mutex_destroy (&mutex);

#if HAVE_MAGIC_H
magic_close (magic);
#endif

info( "Server shutdown complete, now cleaning up!" );

for( ; StoredFiles ; StoredFiles = StoredFiles->next ) {
	free( StoredFiles->name );
	free( StoredFiles->mime );
	free( StoredFiles->data );
	free( StoredFiles );
}
StoredFiles = NULL;

if( ServerSessionMD5 != NULL ) {
	free( ServerSessionMD5 );
	ServerSessionMD5 = NULL;
}

dbFlush();
cleanUp(0);
cleanUp(1);
if( sysconfig.regen == false ) {
	UnloadConfig();
}

return;
}

/*
 *     -----NOT THREAD SAFE, MUST ONLY BE CALLED ONCE PER LINE!!-----
 * If called multiple times in one line, it will default to the first calling.
 *	but return results do not require freeing, as they use "internal memory"
 *
 */

char *URLAppend( ReplyDataPtr cnt, char *url ) {
	char buffer[DEFAULT_BUFFER];
	char *r = buffer;
	int offset;


offset = snprintf( r, DEFAULT_BUFFER, "%s", url );

//Check if session key/id are in URL string, if not add it.
if( strstr( url, (cnt->session)->sid ) == 0 ) {
	if( ServerSessionMD5 == NULL ) {
		if( GenServerSum() == NO ) {
			critical( "This is a no go Jo.." );
		}
	}
	offset += snprintf( &r[offset], DEFAULT_BUFFER - offset, "?%s=%s", ServerSessionMD5, (cnt->session)->sid );
}


#if FACEBOOK_SUPPORT
/*
 * Were we called from inside facebook? If so... preserve those details
 * First just ensures windows doesn't redirect over the top, second contains the login
 */
if( ( iohtmlVarsFind( cnt, "fbapp" ) != NULL ) && ( strstr( url, "fbapp=" ) == 0 ) ) {
	offset += snprintf( &r[offset], DEFAULT_BUFFER - offset, "&fbapp=%s", iohtmlVarsFind( cnt, "fbapp" ) );
}

if( ( iohtmlVarsFind( cnt, "fblogin_token" ) != NULL ) && ( strstr( url, "fblogin_token=" ) == 0 ) ) {
	offset += snprintf( &r[offset], DEFAULT_BUFFER - offset, "&fblogin_token=%s", iohtmlVarsFind( cnt, "fblogin_token" ) );
}
#endif

return r;
}


void URLString( ReplyDataPtr cnt, char *url, char *label ) {
	char buffer[DEFAULT_BUFFER];
	int offset;



offset = snprintf( buffer, DEFAULT_BUFFER, "<a href=\"%s", URLAppend( cnt, url ) );
offset += snprintf( &buffer[offset], DEFAULT_BUFFER - offset, "%s", "\">" );
offset += snprintf( &buffer[offset], DEFAULT_BUFFER - offset, "%s", label );
offset += snprintf( &buffer[offset], DEFAULT_BUFFER - offset, "%s", "</a>" );

httpString( cnt, buffer );

return;
}


void WWWExpire() {

expire_sessions();

expire_file_storage();

return;
}

char *targetframe( ReplyDataPtr cnt ) {
	char buffer[128];
	char *r = buffer;

#if FACEBOOK_SUPPORT
if ( iohtmlVarsFind( cnt, "fbapp" ) != NULL ) 
	snprintf( r, 128, "iframe_canvas%s", ( strcmp( iohtmlVarsFind( cnt, "fbapp" ), "secure" ) == 0 ) ? "_fb_https" : "" );
else
#endif
	snprintf( r, 128, "%s", "_top" );

return r;
}

bool securecnt( ReplyDataPtr cnt ) {
	#if HTTPS_SUPPORT
	urlinfoPtr urlp;
	const char *host, *referer;
	char temp[32];
	#endif
	bool result = false;

#if HTTPS_SUPPORT	
host = MHD_lookup_connection_value( cnt->connection, MHD_HEADER_KIND, "Host" );
referer = MHD_lookup_connection_value( cnt->connection, MHD_HEADER_KIND, "Referer" );

if( host == NULL ) {
	return false;
}

if( referer ) {
	urlp = parse_url( referer );
	if( urlp ) {
		result = ( strcmp( urlp->scheme, "https" ) == 0 ) ? true : false;
	}
	urlinfo_free( urlp );
} else {
	sprintf(temp, "%d", options.port[PORT_HTTPS]);
	if( ( host != NULL ) ) {
		result = strstr( host, temp ) ? true : false;
	}
}
#endif

return result;
}



