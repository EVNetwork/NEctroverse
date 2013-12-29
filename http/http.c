/*
     This file is part of libmicrohttpd
     (C) 2013 Christian Grothoff (and other contributing authors)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
 * @file demo.c
 * @brief complex demonstration site: create directory index, offer
 *        upload via form and HTTP POST, download with mime type detection
 *        and error reporting (403, etc.) --- and all of this with
 *        high-performance settings (large buffers, thread pool).
 *        If you want to benchmark MHD, this code should be used to
 *        run tests against.  Note that the number of threads may need
 *        to be adjusted depending on the number of available cores.
 * @author Christian Grothoff
 */
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

#include "pagelist.c"

MHD_DaemonPtr server_http;
MHD_DaemonPtr server_https;

char *cmdUploadState[4] = {
"No Upload",
"In Progress",
"Completed",
"Failed"
};

#define SESSION_TIME ( 15 * 60 )

size_t initial_allocation = 256 * 1024;



static size_t dir_buf_allocation = 256 * 1024;


/**
 * Response returned if the requested file does not exist (or is not accessible).
 */
static MHD_ResponsePtr file_not_found_response;

/**
 * Response returned for internal errors.
 */
static MHD_ResponsePtr internal_error_response;

/**
 * Response returned for '/' (GET) to list the contents of the directory and allow upload.
 */
static MHD_ResponsePtr cached_directory_response;

/**
 * Response returned for refused uploads.
 */
static MHD_ResponsePtr request_refused_response;

/**
 * Global handle to MAGIC data.
 */
#if HAVE_MAGIC_H
static magic_t magic;
#endif

void mark_as( MHD_ResponsePtr response, const char *type ) {

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




/**
 * Linked list of all active sessions.  Yes, O(n) but a
 * hash table would be overkill for a simple example...
 */
SessionPtr sessions;



/**
 * Return the session handle for this connection, or
 * create one if this is a new user.
 */
SessionPtr get_session( int type, void *cls ) {
	SessionPtr ret;
	int id, now;
	char buffer[129];
	const char *cookie;

if( type == SESSION_HTTP ) {
	MHD_ConnectionPtr connection = cls;
	
cookie = MHD_lookup_connection_value (connection, MHD_COOKIE_KIND, COOKIE_NAME);
} else if( type == SESSION_IRC ) {
	cookie = cls;
}

if (cookie != NULL) {
	ret = sessions;
	while (NULL != ret) {
		if( 0 == strcmp( cookie, ret->sid ) )
			break;
		ret = ret->next;
	}
	if (NULL != ret) {
		ret->rc++;
		ret->active = time(NULL);
		return ret;
	}
}

//No Cookie, Make a new one.
ret = calloc( 1, sizeof(SessionDef) );

if (NULL == ret) {
	critical( "HTTP session allocation error!" );
	return NULL;
}
ret->dbuser = NULL;
if( type == SESSION_HTTP ) {
	snprintf(buffer, sizeof(buffer), "%X%X%X%X", (unsigned int)random(), (unsigned int)random(), (unsigned int)random(), (unsigned int)random() );
	snprintf(ret->sid, sizeof(ret->sid), "%s", hashencrypt( buffer ) );
} else if( type == SESSION_IRC ) {
	snprintf(ret->sid, sizeof(ret->sid), "%s", cookie );
}

if( ( type == SESSION_HTTP ) && ( cookie != NULL ) ) {
	if( ( ( id = dbUserSessionSearch( (char *)cookie ) < 0 ) ) ) {
		goto MAKECOOKIE;
	} else {
		ret->dbuser = dbUserLinkID( id );
		now = time(NULL);
		if( (now - (ret->dbuser)->lasttime) > ( SESSION_TIME ) ) {
			ret->dbuser = NULL;
			goto MAKECOOKIE;
		}
		if( strcmp( cookie, ret->dbuser->http_session ) == 0 )
			strcpy(ret->sid,cookie);
	}
}

MAKECOOKIE:


ret->rc++;
ret->active = time(NULL);
ret->start = time(NULL);
ret->next = sessions;

sessions = ret;

return ret;
}




/**
 * Add header to response to set a session cookie.
 *
 * @param session session to use
 * @param response response to modify
 */
static void add_session_cookie( SessionPtr session, MHD_ResponsePtr response ) {
	char cstr[256];
if( strlen(sysconfig.cookdomain) )
snprintf( cstr, sizeof (cstr), "%s=%s; Path=/; Domain=.%s; max-age=%d", COOKIE_NAME, session->sid, sysconfig.cookdomain, SESSION_TIME );
else
snprintf( cstr, sizeof (cstr), "%s=%s", COOKIE_NAME, session->sid );

if (MHD_NO == MHD_add_response_header(response, MHD_HTTP_HEADER_SET_COOKIE, cstr)) {
	error( "Failed to set session cookie header!" );
}

}

static ssize_t file_read( void *cls, uint64_t pos, char *buf, size_t max ){
	FILE *file = cls;

(void) fseek (file, pos, SEEK_SET);

return fread (buf, 1, max, file);
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
void update_cached_response(MHD_ResponsePtr response) {

(void) pthread_mutex_lock (&mutex);

if (NULL != cached_directory_response)
	MHD_destroy_response (cached_directory_response);
cached_directory_response = response;

(void) pthread_mutex_unlock (&mutex);

}


static int list_directory( ReplyDataPtr rd, const char *dirname ) {
	char fullname[PATH_MAX];
	struct stat sbuf;
	struct dirent *de;
	DIR *dir;

if (NULL == (dir = opendir (dirname)))
	return MHD_NO;      

while (NULL != (de = readdir (dir))) {
	if ('.' == de->d_name[0])
		continue;
	if( sizeof (fullname) <= snprintf (fullname, sizeof (fullname), "%s/%s", dirname, de->d_name) )
		continue;
	if (0 != stat (fullname, &sbuf))
		continue;
	if (! S_ISREG (sbuf.st_mode))
		continue;
	if (rd->response.off + 1024 > rd->response.buf_len) {
			void *r;
		if ( (2 * rd->response.buf_len + 1024) < rd->response.buf_len)
			    break; /* more than SIZE_T _index_ size? Too big for us */
		rd->response.buf_len = 2 * rd->response.buf_len + 1024;
		if( NULL == ( r = realloc(rd->response.buf, rd->response.buf_len) ) )
			break; /* out of memory */
		rd->response.buf = r;
	}
	rd->response.off += snprintf (&rd->response.buf[rd->response.off], rd->response.buf_len - rd->response.off, "<li><a href=\"/files/%s\">%s</a></li>\n", de->d_name, de->d_name );
}
(void)closedir( dir );

return MHD_YES;
}


/**
 * Re-scan our local directory and re-build the index.
 */
static void update_directory( MHD_ConnectionPtr connection ) {
	MHD_ResponsePtr response;
	ReplyDataDef rd;
	char dir_name[PATH_MAX], md5sum[MD5_HASHSUM_SIZE];
	struct stat sbuf;

rd.connection = connection;


rd.response.buf_len = dir_buf_allocation; 
if( NULL == ( rd.response.buf = malloc(rd.response.buf_len) ) ) {
	update_cached_response (NULL);
	return; 
}
rd.response.off = snprintf (rd.response.buf, rd.response.buf_len, "%s", UPLOAD_DIR_PAGE_HEADER );

snprintf (dir_name, sizeof (dir_name), "%s/uploads", sysconfig.directory);
if( 0 == stat (dir_name, &sbuf) ) {  
	if (MHD_NO == list_directory (&rd, dir_name)) {
		free (rd.response.buf);
		update_cached_response (NULL);
		return;
	}
}

rd.response.off += snprintf (&rd.response.buf[rd.response.off], rd.response.buf_len - rd.response.off, "%s", UPLOAD_DIR_PAGE_FOOTER );
dir_buf_allocation = rd.response.buf_len; /* remember for next time */
response = MHD_create_response_from_buffer (rd.response.off, rd.response.buf, MHD_RESPMEM_MUST_FREE );
md5_string( rd.response.buf, md5sum );
(void)MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
mark_as( response, "text/html" );
(void)MHD_add_response_header (response, MHD_HTTP_HEADER_CONNECTION, "close");

update_cached_response (response);
}



/**
 * Return the current directory listing.
 * 
 * @param connection connection to return the directory for
 * @return MHD_YES on success, MHD_NO on error
 */
static int return_directory_response( MHD_ConnectionPtr connection ) {
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
int not_found_page ( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection) {
	int ret;
	char md5sum[MD5_HASHSUM_SIZE];
	MHD_ResponsePtr response;

  /* unsupported HTTP method */
response = MHD_create_response_from_buffer (strlen (NOT_FOUND_ERROR), (void *) NOT_FOUND_ERROR, MHD_RESPMEM_PERSISTENT);
md5_string( NOT_FOUND_ERROR, md5sum );
(void)MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
ret = MHD_queue_response (connection, MHD_HTTP_NOT_FOUND, response);
MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
MHD_destroy_response (response);

return ret;
}

int files_dir_page ( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection) {
	int ret;
	char dmsg[PATH_MAX];
	struct stat buf;
	MHD_ResponsePtr response;
	FILE *file;

snprintf(dmsg, sizeof (dmsg), "%s/uploads/%s", sysconfig.directory, &connection->url[7] );
if( (0 == stat (dmsg, &buf)) && (S_ISREG (buf.st_mode)) ) {
	file = fopen (dmsg, "rb");
} else {
	file = NULL;
}
   
if (file == NULL) { 
	update_directory( connection );
	ret = return_directory_response( connection );
} else {
	response = MHD_create_response_from_callback( buf.st_size, 32 * 1024, &file_read, file, &file_free_callback );
	if (response == NULL) {
		fclose (file);
		return MHD_NO;
	}
	ret = MHD_queue_response( connection, MHD_HTTP_OK, response );
	MHD_destroy_response( response );
}
	
return ret;
}


int key_page( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection) {
	const char *pname = cls;
	char md5sum[MD5_HASHSUM_SIZE];
	int ret, a;
	MHD_ResponsePtr response;
	ReplyDataDef rd;

(void)pthread_mutex_lock( &mutex );

rd.session = session;
rd.connection = connection;
rd.cookies.num = 0;
  /* unsupported HTTP method */
rd.response.buf_len = initial_allocation;
if (NULL == (rd.response.buf = malloc (rd.response.buf_len))) {
	return -1;
}
rd.response.off = 0;

if( ( pname ) && strcmp( pname, "login" ) == false  ) {
	iohtmlFunc_login( &rd, false, NULL );
} else {
	iohtmlFunc_front( &rd, NULL );
}

response = MHD_create_response_from_buffer (strlen (rd.response.buf), (void *)rd.response.buf, MHD_RESPMEM_MUST_FREE);
add_session_cookie(session, response);
for( a = 0; a < rd.cookies.num ; a++  ) {
	if (MHD_NO == MHD_add_response_header(response, MHD_HTTP_HEADER_SET_COOKIE, rd.cookies.value[a])) {
		error( "Failed to set session cookie header!" );
	}
}
md5_string( rd.response.buf, md5sum );
(void)MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
mark_as( response, mime );
MHD_destroy_response (response);

(void)pthread_mutex_unlock( &mutex );

return ret;
}


int page_render( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection) {
	int ret, a;
	char md5sum[MD5_HASHSUM_SIZE];
	MHD_ResponsePtr response;
	ReplyDataDef rd;

(void)pthread_mutex_lock( &mutex );

rd.session = session;
rd.connection = connection;
rd.cookies.num = 0;
rd.response.buf_len = initial_allocation;
if( NULL == ( rd.response.buf = malloc( rd.response.buf_len ) ) ) {
	return -1;
}
rd.response.off = 0;

pages[id].function( &rd );
response = MHD_create_response_from_buffer (strlen (rd.response.buf), (void *)rd.response.buf, MHD_RESPMEM_MUST_FREE);
add_session_cookie(session, response);
for( a = 0; a < rd.cookies.num ; a++  ) {
	if (MHD_NO == MHD_add_response_header(response, MHD_HTTP_HEADER_SET_COOKIE, rd.cookies.value[a])) {
		error( "Failed to set session cookie header!" );
	}
}
md5_string( rd.response.buf, md5sum );
(void)MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
mark_as( response, mime );
MHD_destroy_response (response);

(void)pthread_mutex_unlock( &mutex );

return ret;
}

int file_page( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection) {
	int ret, fd;
	struct stat buf;
	MHD_ResponsePtr response;
	const char *fname = cls;
	char filename[PATH_MAX], md5sum[MD5_HASHSUM_SIZE];

	strcpy(filename,sysconfig.httpfiles);
	strcat(filename,fname);

	if ( (0 == stat (filename, &buf)) && (NULL == strstr (filename, "..")) && ('/' != filename[1]) )
		fd = open (filename, O_RDONLY);
	else
		fd = -1;
	if (-1 == fd) {
		response = MHD_create_response_from_buffer( strlen( NOT_FOUND_ERROR ), (void *) NOT_FOUND_ERROR, MHD_RESPMEM_MUST_FREE );
		md5_string( NOT_FOUND_ERROR, md5sum );
		(void)MHD_add_response_header( response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
		add_session_cookie( session, response );
		ret = MHD_queue_response( connection, MHD_HTTP_NOT_FOUND, response );
		MHD_destroy_response( response );
		return ret;
	}

	if (NULL == ( response = MHD_create_response_from_fd(buf.st_size,fd) ) ) {
		/* internal error (i.e. out of memory) */
		(void) close (fd);
		return MHD_NO;
	}

      /* add mime type if we had one */
	if (NULL != mime)
		(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, mime);

	md5_file( filename, md5sum );
	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
	
	strftime(filename,512,"%a, %d %b %G %T %Z", gmtime(&buf.st_mtime) );
	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_LAST_MODIFIED, filename );

	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_SERVER, sysconfig.servername );
	//add_session_cookie(session, response);
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);

return ret;
}

/**
 * Append the 'size' bytes from 'data' to '*ret', adding
 * 0-termination.  If '*ret' is NULL, allocate an empty string first.
 *
 * @param ret string to update, NULL or 0-terminated
 * @param data data to append
 * @param size number of bytes in 'data'
 * @return MHD_NO on allocation failure, MHD_YES on success
 */
int do_append (char **ret, const char *data, size_t size) {
	char *buf;
	size_t old_len;

if (NULL == *ret) {
	old_len = 0;
} else {
	old_len = strlen (*ret);
}
buf = malloc (old_len + size + 1);
if (NULL == buf)
	return MHD_NO;
memcpy (buf, *ret, old_len);
if (NULL != *ret)
	free (*ret);
memcpy (&buf[old_len], data, size);
buf[old_len + size] = '\0';
*ret = buf;

return MHD_YES;
}

int set_postvalue(char **ret, const char *data, size_t size) {
	char *buf;

if (NULL != *ret) {
	free( *ret );
}
buf = malloc( size );
if (NULL == buf)
	return MHD_NO;
memcpy(&buf[0], data, size);
buf[size] = '\0';
*ret = buf;

return MHD_YES;
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
 * @return MHD_YES to continue iterating,
 *         MHD_NO to abort the iteration
 */

static int process_upload_data( void *cls, enum MHD_ValueKind kind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size ) {
	RequestPtr uc = cls;
	int i;

if( ( !( filename ) ) ) {
	(uc->session)->upload = UPLOAD_STATE_NULL;
	if( ( data ) && ( strlen(data) ) ) {
		//sprintf( logString, "Ignoring unexpected form value \'%s\' - \'%s\'", key, data);
		//info( logString );
		 set_postvalue(&(uc->session)->key[(uc->session)->posts], key, strlen(key) );
		 set_postvalue(&(uc->session)->value[(uc->session)->posts], data, strlen(data) );
		(uc->session)->posts++;
	} else if ( strlen(data) ) {
		sprintf( logString, "Ignoring unexpected form value \'%s\'", key );
		info( logString ); 
	}
	return MHD_YES;
}
if (NULL == filename) {
	error( "No filename, aborting upload" );
	return MHD_NO;
}

if (-1 == uc->fd) {
	if( !( ( (uc->session)->dbuser ) ) ) {
		uc->response = request_refused_response;
		(uc->session)->upload = UPLOAD_STATE_FAIL;
		return MHD_NO;
	}
	char fn[PATH_MAX];
	if ( (NULL != strstr (filename, "..")) || (NULL != strchr (filename, '/')) || (NULL != strchr (filename, '\\')) ) {
		uc->response = request_refused_response;
		return MHD_NO;
	}
	snprintf (fn, sizeof (fn), "%s/uploads", sysconfig.directory );
	(void) mkdir (fn, S_IRWXU);
	snprintf (fn, sizeof (fn),"%s/uploads/%s", sysconfig.directory, filename);
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
		sprintf( logString, "Error opening file for upload: \'%s\'", fn );
		error( logString );
		uc->response = request_refused_response;
		(uc->session)->upload = UPLOAD_STATE_FAIL;
		return MHD_NO;
	}
	uc->filename = strdup (fn);
}

if ( (0 != size) && (size != write (uc->fd, data, size)) ) {
	sprintf( logString, "Error writing to file: \'%s\'", uc->filename);
	error( logString );
	uc->response = internal_error_response;
	(uc->session)->upload = UPLOAD_STATE_FAIL;
	close (uc->fd);
	uc->fd = -1;
	if (NULL != uc->filename) {
		unlink (uc->filename);
		free (uc->filename);
		uc->filename = NULL;
	}
	return MHD_NO;
}


return MHD_YES;
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
 *         MHS_NO if the socket must be closed due to a serios
 *         error while handling the request
 */
#define MAGIC_HEADER_SIZE (16 * 1024)

int create_response (void *cls, MHD_ConnectionPtr connection, const char *url, const char *method, const char *version, const char *upload_data,  size_t *upload_data_size, void **ptr)
{
  MHD_ResponsePtr response;
  RequestPtr request;
  SessionPtr session;
  int ret, fd;
  unsigned int i;
  bool local;
  struct stat buf;
  ReplyDataDef rd;

rd.connection = connection;

    if( ( iohtmlHeaderFind(&rd, "Referer") ) && ( iohtmlHeaderFind(&rd, "Host") ) ) {
      local = strstr( iohtmlHeaderFind(&rd, "Referer"), iohtmlHeaderFind(&rd, "Host") ) ? true : false;
    } else {
    	local = false;
    }

if ( ( strncmp(url,"/images/",8) == false ) && ( strcmp("/",strrchr(url,'/') ) ) ) {
	/* should be file download */
	#if HAVE_MAGIC_H
	char file_data[MAGIC_HEADER_SIZE];
	ssize_t got;
	#endif
	const char *mime;
	char *filename, filebuffer[PATH_MAX], md5sum[MD5_HASHSUM_SIZE];

	filename = strrchr(url,'/');
	strcpy(filebuffer,sysconfig.httpimages);
	strcat(filebuffer,&url[7]);
	filename = filebuffer;

	if (0 != strcmp (method, MHD_HTTP_METHOD_GET))
		return MHD_NO;  /* unexpected method (we're not polite...) */
	if ( (0 == stat (filename, &buf)) && (NULL == strstr (filename, "..")) && ('/' != filename[1]) )
		fd = open (filename, O_RDONLY);
	else
		fd = -1;
	if (-1 == fd) {
		response = MHD_create_response_from_buffer (strlen (NOT_FOUND_ERROR), (void *) NOT_FOUND_ERROR, MHD_RESPMEM_MUST_COPY);
		ret = MHD_queue_response (connection, MHD_HTTP_NOT_FOUND, response);
		MHD_destroy_response (response);
		return ret;
	}

      /* read beginning of the file to determine mime type  */
#if HAVE_MAGIC_H
	got = read (fd, file_data, sizeof (file_data));
	if (-1 != got)
		mime = magic_buffer (magic, file_data, got);
	else
#endif
		mime = NULL;
	(void) lseek (fd, 0, SEEK_SET);

	if (NULL == (response = MHD_create_response_from_fd(buf.st_size,fd)) ) {
		/* internal error (i.e. out of memory) */
		(void) close (fd);
		return MHD_NO;
	}

      /* add mime type if we had one */
	if (NULL != mime)
		(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, mime);
	else
		(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, trimwhitespace( strrchr( strdup(iohttpMime[ iohttpMimeFind( filename ) ].def ), ' ')+1 ) );

	md5_file( filename, md5sum );
	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
	strftime(filebuffer,PATH_MAX,"%a, %d %b %G %T %Z", gmtime(&buf.st_mtime) );
	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_LAST_MODIFIED, filebuffer );

	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_SERVER, sysconfig.servername );
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);

	return ret;
}
//end images
request = *ptr;
if( (0 == strcmp (method, MHD_HTTP_METHOD_POST) ) && ( local ) ) {
	if (NULL == request) {
		if (NULL == (request = malloc (sizeof (RequestDef))))
			return MHD_NO; /* out of memory, close connection */
		memset (request, 0, sizeof (RequestDef));
		request->session = get_session( SESSION_HTTP, connection );
		if (NULL == request->session) {
			sprintf( logString, "Failed to setup session for \'%s\'", url );
			error( logString );
			return MHD_NO; /* internal error */
		}
		request->session->posts = 0;
		request->session->upload = UPLOAD_STATE_START;
		request->post_url = url;
		request->fd = -1;
		request->connection = connection;
		request->pp = MHD_create_post_processor (connection, 64 * 1024 /* buffer size */, &process_upload_data, request);
		if (NULL == request->pp) {
			/* out of memory, close connection */
			free (request);
			return MHD_NO;
		}
		*ptr = request;
		return MHD_YES;
	}
	if (0 != *upload_data_size) {
		if (NULL == request->response)
			(void)MHD_post_process( request->pp, upload_data, *upload_data_size );
		*upload_data_size = 0;
		return MHD_YES;
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
		if ( ( strncmp(request->post_url,"/files",6) == false ) ) {
			return files_dir_page( false, cls, "text/html", request->session, request->connection);
		}
	i=0;
	while ( (pages[i].url != NULL) && (0 != strcmp (pages[i].url, request->post_url)) )
		i++;
	ret = pages[i].handler( i, pages[i].handler_cls, pages[i].mime, request->session, request->connection );
	if (ret != MHD_YES) {
		sprintf( logString, "Failed to create page for \'%s\'", request->post_url);
		error( logString );
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

session->posts = 0;
if ( ( strncmp(url,"/files",6) == false ) ) {
	return files_dir_page( false, cls, "text/html", session, connection);
}

if ( (0 == strcmp (method, MHD_HTTP_METHOD_GET)) || (0 == strcmp (method, MHD_HTTP_METHOD_HEAD)) ) {
	i=0;
	while ( (pages[i].url != NULL) && (0 != strcmp (pages[i].url, url)) )
		i++;
	ret = pages[i].handler( i, pages[i].handler_cls, pages[i].mime, session, connection );
	if (ret != MHD_YES) {
		sprintf( logString, "Failed to create page for \'%s\'", url);
		error( logString );
	}
	return ret;
}

/* unsupported HTTP method */
response = MHD_create_response_from_buffer (strlen (METHOD_ERROR), (void *) METHOD_ERROR, MHD_RESPMEM_PERSISTENT);
(void)MHD_add_response_header (response, MHD_HTTP_HEADER_SERVER, sysconfig.servername );
ret = MHD_queue_response (connection, MHD_HTTP_METHOD_NOT_ACCEPTABLE, response);
MHD_destroy_response (response);

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
void completed_callback (void *cls, MHD_ConnectionPtr connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
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
		sprintf( logString, "Upload of file `%s' failed (incomplete or aborted), removing file.", request->filename );
		error( logString );
		(void) unlink (request->filename);
	}
}

if (NULL != request->filename) {
	free (request->filename);
}


free(request);
}

int access_check(void *cls, const struct sockaddr *addr, socklen_t addrlen) {
	int a;

for( a = 0 ; a < banlist.number ; a++ ) {
	if( ioCompareFindWords( inet_ntoa( ((struct sockaddr_in *)addr)->sin_addr ), banlist.ip[a] ) )
		return MHD_NO;
}

return MHD_YES;
}


/**
 * Clean up handles of sessions that have been idle for
 * too long.
 */
void expire_sessions () {
	int id;
	char buffer[129];
	dbUserPtr user;
	SessionPtr pos;
	SessionPtr prev;
	SessionPtr next;
	time_t now;

now = time(NULL);
prev = NULL;
pos = sessions;

while( NULL != pos ) {
	next = pos->next;
	if( (now - pos->active) > ( SESSION_TIME ) ) {
		if (NULL == prev) {
			sessions = pos->next;
		} else {
			prev->next = next;
		}
		if( ( ( id = dbUserSessionSearch( pos->sid ) >= 0 ) ) ) {
			if( ( user = dbUserLinkID( id ) ) ){
				snprintf(buffer, sizeof(buffer), "%X%X%X%X", (unsigned int)random(), (unsigned int)random(), (unsigned int)random(), (unsigned int)random() );
				snprintf(user->http_session, sizeof(user->http_session), "%s", hashencrypt(buffer) );
				dbUserSave( id, user );
			}
		}
		free( pos );
	} else {
	        prev = pos;
        }
	pos = next;
}

}


int remove_session( const char *sid ) {
	int id;
	char buffer[129];
	bool donenothing = true;
	dbUserPtr user;
	SessionPtr pos;
	SessionPtr prev;
	SessionPtr next;

prev = NULL;
pos = sessions;

while( NULL != pos ) {
	next = pos->next;
	if( 0 == strcmp( sid, pos->sid ) ) {
		if (NULL == prev) {
			sessions = pos->next;
		} else {
			prev->next = next;
		}
		if( ( ( id = dbUserSessionSearch( pos->sid ) >= 0 ) ) ) {
			if( ( user = dbUserLinkID( id ) ) ){
				snprintf(buffer, sizeof(buffer), "%X%X%X%X", (unsigned int)random(), (unsigned int)random(), (unsigned int)random(), (unsigned int)random() );
				snprintf(user->http_session, sizeof(user->http_session), "%s", hashencrypt(buffer) );
				dbUserSave( id, user );
			}
		}
		donenothing = false;
		free( pos );
	} else {
	        prev = pos;
        }
	pos = next;
}


return donenothing;
}

#if HTTPS_SUPPORT
char *loadsslfile( char *filename ) {
	struct stat stdata;
	char *data, *ret;
	FILE *file;

ret = 0;

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

static int THREADS;
static int flags = MHD_USE_SELECT_INTERNALLY /*| MHD_USE_DUAL_STACK*/; //I have no IPv6, so no point dual stacking.
unsigned int ports[2] = { 8880, 8881 };



int http_prep(){
	char md5sum[MD5_HASHSUM_SIZE];
	cpuInfo cpuinfo;

cpuGetInfo( &cpuinfo );

THREADS = fmax( 1.0, ( cpuinfo.socketphysicalcores / 2 ) );
#if HAVE_MAGIC_H
magic = magic_open(MAGIC_MIME_TYPE);
(void) magic_load(magic, NULL);
#endif

file_not_found_response = MHD_create_response_from_buffer( strlen( NOT_FOUND_ERROR ), (void *)NOT_FOUND_ERROR, MHD_RESPMEM_PERSISTENT );
request_refused_response = MHD_create_response_from_buffer( strlen( METHOD_ERROR ), (void *)METHOD_ERROR, MHD_RESPMEM_PERSISTENT );
internal_error_response = MHD_create_response_from_buffer( strlen( INTERNAL_ERROR_PAGE ), (void *)INTERNAL_ERROR_PAGE, MHD_RESPMEM_PERSISTENT );

mark_as(file_not_found_response, "text/html" );
mark_as(request_refused_response, "text/html" );
mark_as(internal_error_response, "text/html" );
md5_string( METHOD_ERROR, md5sum );
(void)MHD_add_response_header(request_refused_response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
md5_string(NOT_FOUND_ERROR, md5sum );
(void)MHD_add_response_header(file_not_found_response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );
md5_string( INTERNAL_ERROR_PAGE, md5sum );
(void)MHD_add_response_header(internal_error_response, MHD_HTTP_HEADER_CONTENT_MD5, md5sum );

if( options.verbose )
	flags |=  MHD_USE_DEBUG;

#if EPOLL_SUPPORT
flags |= MHD_USE_EPOLL_LINUX_ONLY | MHD_USE_EPOLL_TURBO;
#endif

#if HTTPS_SUPPORT
THREADS = fmax( 1.0, ( THREADS / 2 ) );
#endif

return 0;
}

#if HTTPS_SUPPORT
int https_start() {

server_https = MHD_start_daemon (flags | MHD_USE_SSL,
				ports[1],
				&access_check, NULL,
				&create_response, NULL,
				MHD_OPTION_ARRAY, ops,
				MHD_OPTION_HTTPS_MEM_KEY, loadsslfile( "/home/stephen/.ssl/certificate.key" ),
				MHD_OPTION_HTTPS_MEM_CERT, loadsslfile("/home/stephen/.ssl/4f2815aefe61ae.crt"),
				MHD_OPTION_HTTPS_MEM_TRUST, loadsslfile("/home/stephen/.ssl/sf_bundle-g2.crt"),
				MHD_OPTION_THREAD_POOL_SIZE, (unsigned int)THREADS,
				MHD_OPTION_NOTIFY_COMPLETED, &completed_callback, NULL,
				MHD_OPTION_END);
if(NULL == server_https)
	return 1;

sprintf( logString, "HTTPS Server live with %d thread%s on port: %d", THREADS, ( (THREADS > 1) ? "s" : "" ), ports[1] );
info( logString );

return 0;
}
#endif

int http_start() {

server_http = MHD_start_daemon (flags,
				ports[0],
				&access_check, NULL,
				&create_response, NULL,
				MHD_OPTION_ARRAY, ops,
				#if HTTPS_SUPPORT
				MHD_OPTION_THREAD_POOL_SIZE, (unsigned int)fmax( 1.0, (THREADS / 2) ),
				#else
				MHD_OPTION_THREAD_POOL_SIZE, (unsigned int)THREADS,
				#endif
				MHD_OPTION_NOTIFY_COMPLETED, &completed_callback, NULL,
				MHD_OPTION_END);

if(NULL == server_http)
	return 1;

sprintf( logString, "HTTP Server live with %d thread%s on port: %d", THREADS, ( (THREADS > 1) ? "s" : "" ), ports[0] );
info( logString );

return 0;
}


void server_stop( int flag ) {

if( flag == 1 ) {
	if( server_http ) {
		MHD_stop_daemon(server_http);
	info( "HTTP  Server has been gracefully shutdown!" );
	}
} else if( flag == 2 ) {
	if( server_https ) {
		MHD_stop_daemon(server_https);
	info( "HTTPS Server has been gracefully shutdown!" );
	}
} else {
	if( server_http ) {
		MHD_stop_daemon(server_http);
	info( "HTTP Server has been gracefully shutdown!" );
	}
	if( server_https ) {
		MHD_stop_daemon(server_https);
	info("HTTPS Server has been gracefully shutdown!");
	}
}

return;
}


void server_shutdown(){

server_stop(0);

MHD_destroy_response (file_not_found_response);
MHD_destroy_response (request_refused_response);
MHD_destroy_response (internal_error_response);
//update_cached_response (NULL);
(void) pthread_mutex_destroy (&mutex);

#if HAVE_MAGIC_H
magic_close (magic);
#endif

return;
}




