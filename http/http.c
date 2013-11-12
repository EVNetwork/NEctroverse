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
#include "../global.h"
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


MHD_DaemonPtr server_http;
MHD_DaemonPtr server_https;

size_t initial_allocation = 256 * 1024; //Yer, its huge... so sue me!


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
//static MHD_ResponsePtr cached_directory_response;

/**
 * Response returned for refused uploads.
 */
static MHD_ResponsePtr request_refused_response;

/**
 * Mutex used when we update the cached directory response object.
 */
static pthread_mutex_t mutex;

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
 * Linked list of all active sessions.  Yes, O(n) but a
 * hash table would be overkill for a simple example...
 */
static SessionPtr sessions;




/**
 * Return the session handle for this connection, or 
 * create one if this is a new user.
 */
static SessionPtr get_session( MHD_ConnectionPtr connection )
{
  SessionPtr ret;
  const char *cookie;

  cookie = MHD_lookup_connection_value (connection, MHD_COOKIE_KIND, COOKIE_NAME);
  if (cookie != NULL)
    {
      /* find existing session */
      ret = sessions;
      while (NULL != ret)
	{
	  if (0 == strcmp (cookie, ret->sid))
	    break;
	  ret = ret->next;
	}
      if (NULL != ret)
	{
	  ret->rc++;
	  return ret;
	}
    }
  /* create fresh session */
  ret = calloc (1, sizeof (SessionDef));
  if (NULL == ret)
    {						
      fprintf (stderr, "calloc error: %s\n", strerror (errno));
      return NULL; 
    }
  /* not a super-secure way to generate a random session ID,
     but should do for a simple example... */
  snprintf (ret->sid,
	    sizeof (ret->sid),
	    "%X%X%X%X",
	    (unsigned int) random (),
	    (unsigned int) random (),
	    (unsigned int) random (),
	    (unsigned int) random ());
  ret->rc++;  
  ret->start = time (NULL);
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
static void add_session_cookie (SessionPtr session, MHD_ResponsePtr response) {
	char cstr[256];

snprintf (cstr, sizeof (cstr), "%s=%s", COOKIE_NAME, session->sid);

if (MHD_NO == MHD_add_response_header(response, MHD_HTTP_HEADER_SET_COOKIE, cstr)) {
	fprintf (stderr, "Failed to set session cookie header!\n");
}

}

/**
 * Handler used to generate a 404 reply.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle 
 * @param connection connection to use
 */
static int
not_found_page ( int id, const void *cls,
		const char *mime,
		SessionPtr session,
		MHD_ConnectionPtr connection)
{
  int ret;
  MHD_ResponsePtr response;

  /* unsupported HTTP method */
  response = MHD_create_response_from_buffer (strlen (NOT_FOUND_ERROR),
					      (void *) NOT_FOUND_ERROR,
					      MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, 
			    MHD_HTTP_NOT_FOUND, 
			    response);
  MHD_add_response_header (response,
			   MHD_HTTP_HEADER_CONTENT_ENCODING,
			   mime);
  MHD_destroy_response (response);
  return ret;
}

static int key_page( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection) {
	const char *pname = cls;
	int ret;
	MHD_ResponsePtr response;
	ReplyDataDef rd;

rd.session = session;
rd.connection = connection;
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
ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
mark_as( response, mime );
MHD_destroy_response (response);
  
return ret;
}


static int page_render( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection) {
	int ret;
	MHD_ResponsePtr response;
	ReplyDataDef rd;

rd.session = session;
rd.connection = connection;
  /* unsupported HTTP method */
rd.response.buf_len = initial_allocation; 
if (NULL == (rd.response.buf = malloc (rd.response.buf_len))) {
	return -1;
}
rd.response.off = 0;

pages[id].function( &rd );

response = MHD_create_response_from_buffer (strlen (rd.response.buf), (void *)rd.response.buf, MHD_RESPMEM_MUST_FREE);
add_session_cookie(session, response);
ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);

MHD_destroy_response (response);
  
return ret;
}

static int file_page( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection) {
	int ret, fd;
	struct stat buf;
	MHD_ResponsePtr response;
	const char *fname = cls;
	char filename[512];

	strcpy(filename,sysconfig.httpfiles);
	strcat(filename,fname);

	if ( (0 == stat (filename, &buf)) && (NULL == strstr (filename, "..")) && ('/' != filename[1]) )
		fd = open (filename, O_RDONLY);
	else
		fd = -1;
	if (-1 == fd) {
		response = MHD_create_response_from_buffer (strlen (NOT_FOUND_ERROR), (void *) NOT_FOUND_ERROR, MHD_RESPMEM_MUST_FREE);
		add_session_cookie(session, response);
		ret = MHD_queue_response (connection, MHD_HTTP_NOT_FOUND, response);
		MHD_destroy_response (response);
		return ret;
	}

	if (NULL == (response = MHD_create_response_from_fd(buf.st_size,fd)) ) {
		/* internal error (i.e. out of memory) */
		(void) close (fd);
		return MHD_NO; 
	}

      /* add mime type if we had one */
	if (NULL != mime)
		(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, mime);

	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_MD5, md5file(filename));
	strftime(filename,512,"%a, %d %b %G %T %Z", gmtime(&buf.st_mtime) );
	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_LAST_MODIFIED, filename );

	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_SERVER, sysconfig.servername );
	add_session_cookie(session, response);
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);

	return ret;
}

/**
 * List of all pages served by this HTTP server.
 */
PageDef pages[] = 
  {
    { "/", "text/html",  &key_page, NULL, NULL },
    { "/login", "text/html",  &key_page, NULL, "login" },
    { "/halloffame", "text/html",  &page_render, iohtmlFunc_halloffame, NULL },
    { "/gettingstarted", "text/html",  &page_render, iohtmlFunc_gettingstarted, NULL },
    { "/faq", "text/html",  &page_render, iohtmlFunc_faq, NULL },
    { "/status", "text/html",  &page_render, iohtmlFunc_status, NULL },
    { "/races", "text/html",  &page_render, iohtmlFunc_races, NULL },
    { "/register", "text/html",  &page_render, iohtmlFunc_register, NULL },
    { "/register2", "text/html",  &page_render, iohtmlFunc_register2, NULL },
    
    { "/style.css", "text/css",  &file_page, NULL, "/style.css" },
    { "/ajax", "text/xml",  &page_render, iohtmlFunc_ajax, NULL },    
    { "/ajax.js", "text/javascript",  &page_render, iohtmlFunc_javaforajax, NULL },    
    { "/javascript.js", "text/javascript",  &file_page, NULL, "/javascript.js" },  

    { NULL, NULL, &not_found_page, NULL, NULL } /* 404 */
  };



/**
 * Iterator over key-value pairs where the value
 * maybe made available in increments and/or may
 * not be zero-terminated.  Used for processing
 * POST data.
 *
 * @param cls user-specified closure
 * @param kind type of the value
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
static int
post_iterator (void *cls,
	       enum MHD_ValueKind kind,
	       const char *key,
	       const char *filename,
	       const char *content_type,
	       const char *transfer_encoding,
	       const char *data, uint64_t off, size_t size)
{
  RequestPtr request = cls;
  SessionPtr session = request->session;

  if (0 == strcmp ("name", key))
    {
      if (size + off > sizeof(session->name))
	size = sizeof (session->name) - off;
      memcpy (&session->name[off],
	      data,
	      size);
      if (size + off < sizeof (session->name))
	session->name[size+off] = '\0';
      return MHD_YES;
    }
  if (0 == strcmp ("pass", key))
    {
      if (size + off > sizeof(session->pass))
	size = sizeof (session->pass) - off;
      memcpy (&session->pass[off],
	      data,
	      size);
      if (size + off < sizeof (session->pass))
	session->pass[size+off] = '\0';
      return MHD_YES;
    }
  if (0 == strcmp ("faction", key))
    {
      if (size + off > sizeof(session->faction))
	size = sizeof (session->faction) - off;
      memcpy (&session->faction[off],
	      data,
	      size);
      if (size + off < sizeof (session->faction))
	session->faction[size+off] = '\0';
      return MHD_YES;
    }
  fprintf (stderr, "Unsupported form value `%s'\n", key);
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
	char *filename, filebuffer[512];

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
		response = MHD_create_response_from_buffer (strlen (NOT_FOUND_ERROR), (void *) NOT_FOUND_ERROR, MHD_RESPMEM_PERSISTENT);
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
		(void) MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, trimwhitespace( strrchr(iohttpMime[ iohttpMimeFind( filename ) ].def, ' ')+1 ) );

	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_MD5, md5file(filename));
	strftime(filebuffer,512,"%a, %d %b %G %T %Z", gmtime(&buf.st_mtime) );
	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_LAST_MODIFIED, filebuffer );

	(void)MHD_add_response_header (response, MHD_HTTP_HEADER_SERVER, sysconfig.servername );
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);

	return ret;
}
//end images
   
  request = *ptr;
  if (NULL == request)
    {
      request = calloc (1, sizeof (RequestDef));
      if (NULL == request)
	{
	  fprintf (stderr, "calloc error: %s\n", strerror (errno));
	  return MHD_NO;
	}
      *ptr = request;
      if( (0 == strcmp (method, MHD_HTTP_METHOD_POST)) && ( local ) )
	{
	
	  request->pp = MHD_create_post_processor (connection, 1024, &post_iterator, request);
	  if (NULL == request->pp)
	    {
	      fprintf (stderr, "Failed to setup post processor for `%s'\n",
		       url);
	      return MHD_NO;
	    }
	} else if ( (0 == strcmp (method, MHD_HTTP_METHOD_POST)) ) {
	return MHD_NO;
	}
      return MHD_YES;
    }
  if (NULL == request->session)
    {
      request->session = get_session (connection);
      if (NULL == request->session)
	{
	  fprintf (stderr, "Failed to setup session for `%s'\n",
		   url);
	  return MHD_NO; 
	}
    }
  session = request->session;
  session->start = time (NULL);
  if( (0 == strcmp (method, MHD_HTTP_METHOD_POST)) && ( local ) )
    {      
      
      MHD_post_process (request->pp,
			upload_data,
			*upload_data_size);
      if (0 != *upload_data_size)
	{
	  *upload_data_size = 0;
	  return MHD_YES;
	}
   
      MHD_destroy_post_processor (request->pp);
      request->pp = NULL;
      method = MHD_HTTP_METHOD_GET;
      if (NULL != request->post_url)
	url = request->post_url;
    }else if ( (0 == strcmp (method, MHD_HTTP_METHOD_POST)) ) {
	return MHD_NO;
	}

  if ( (0 == strcmp (method, MHD_HTTP_METHOD_GET)) || (0 == strcmp (method, MHD_HTTP_METHOD_HEAD)) ) {
      /* find out which page to serve */
      i=0;
      while ( (pages[i].url != NULL) && (0 != strcmp (pages[i].url, url)) )
	i++;
      ret = pages[i].handler( i, pages[i].handler_cls, pages[i].mime, session, connection );
      if (ret != MHD_YES)
	fprintf (stderr, "Failed to create page for `%s'\n",
		 url);
      return ret;
    }
  /* unsupported HTTP method */
  response = MHD_create_response_from_buffer (strlen (METHOD_ERROR),
					      (void *) METHOD_ERROR,
					      MHD_RESPMEM_PERSISTENT);
  (void)MHD_add_response_header (response, MHD_HTTP_HEADER_SERVER, sysconfig.servername );					      
  ret = MHD_queue_response (connection, 
			    MHD_HTTP_METHOD_NOT_ACCEPTABLE, 
			    response);
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
static int
do_append (char **ret,
	   const char *data,
	   size_t size)
{
  char *buf;
  size_t old_len;
  
  if (NULL == *ret)
    old_len = 0;
  else
    old_len = strlen (*ret);
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
static int
process_upload_data (void *cls,
		     enum MHD_ValueKind kind,
		     const char *key,
		     const char *filename,
		     const char *content_type,
		     const char *transfer_encoding,
		     const char *data, 
		     uint64_t off, 
		     size_t size)
{
  RequestPtr uc = cls;
  int i;

  if (0 == strcmp (key, "category"))
    return do_append (&uc->category, data, size);
  if (0 == strcmp (key, "language"))
    return do_append (&uc->language, data, size);
  if (0 != strcmp (key, "upload"))
    {
      fprintf (stderr, 
	       "Ignoring unexpected form value `%s'\n",
	       key);
      return MHD_YES; /* ignore */  
    }
  if (NULL == filename)
    {
      fprintf (stderr, "No filename, aborting upload\n");
      return MHD_NO; /* no filename, error */
    }
  if ( (NULL == uc->category) ||
       (NULL == uc->language) )
    {
      fprintf (stderr, 
	       "Missing form data for upload `%s'\n",
	       filename);
      uc->response = request_refused_response;
      return MHD_NO;
    }
  if (-1 == uc->fd)
    {
      char fn[PATH_MAX];

      if ( (NULL != strstr (filename, "..")) ||
	   (NULL != strchr (filename, '/')) ||
	   (NULL != strchr (filename, '\\')) )
	{
	  uc->response = request_refused_response;
	  return MHD_NO;
	}
      /* create directories -- if they don't exist already */
#ifdef WINDOWS
      (void) mkdir (uc->language);
#else
      (void) mkdir (uc->language, S_IRWXU);
#endif
      snprintf (fn, sizeof (fn),
		"%s/%s",
		uc->language,
		uc->category);  
#ifdef WINDOWS    
      (void) mkdir (fn);
#else
      (void) mkdir (fn, S_IRWXU);
#endif
      /* open file */
      snprintf (fn, sizeof (fn),
		"%s/%s/%s",
		uc->language,
		uc->category,
		filename); 
      for (i=strlen (fn)-1;i>=0;i--)
	if (! isprint ((int) fn[i]))
	  fn[i] = '_';
      uc->fd = open (fn, 
		     O_CREAT | O_EXCL 
#if O_LARGEFILE
		     | O_LARGEFILE
#endif
		     | O_WRONLY,
		     S_IRUSR | S_IWUSR);
      if (-1 == uc->fd)
	{
	  fprintf (stderr, 
		   "Error opening file `%s' for upload: %s\n",
		   fn,
		   strerror (errno));
	  uc->response = request_refused_response;
	  return MHD_NO;
	}      
      uc->filename = strdup (fn);
    }
  if ( (0 != size) &&
       (size != write (uc->fd, data, size)) )    
    {
      /* write failed; likely: disk full */
      fprintf (stderr, 
	       "Error writing to file `%s': %s\n",
	       uc->filename,
	       strerror (errno));
      uc->response = internal_error_response;
      close (uc->fd);
      uc->fd = -1;
      if (NULL != uc->filename)
	{
	  unlink (uc->filename);
	  free (uc->filename);
	  uc->filename = NULL;
	}
      return MHD_NO; 
    }
  return MHD_YES;
}

/**
 * Callback called upon completion of a request.
 * Decrements session reference counter.
 *
 * @param cls not used
 * @param connection connection that completed
 * @param con_cls session handle
 * @param toe status code
 */
void
request_completed_callback (void *cls,
			    MHD_ConnectionPtr connection,
			    void **con_cls,
			    enum MHD_RequestTerminationCode toe)
{
  RequestPtr request = *con_cls;

  if (NULL == request)
    return;
  if (NULL != request->session)
    request->session->rc--;
  if (NULL != request->pp)
    MHD_destroy_post_processor (request->pp);
  free (request);
}


/**
 * Clean up handles of sessions that have been idle for
 * too long.
 */
void
expire_sessions ()
{
  SessionPtr pos;
  SessionPtr prev;
  SessionPtr next;
  time_t now;

  now = time (NULL);
  prev = NULL;
  pos = sessions;
  while (NULL != pos)
    {
      next = pos->next;
      if (now - pos->start > 60 * 60)
	{
	  /* expire sessions after 1h */
	  if (NULL == prev)
	    sessions = pos->next;
	  else
	    prev->next = next;
	  free (pos);
	}
      else
        prev = pos;
      pos = next;
    }      
}

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
				loghandle(LOG_ERR, errno, "%s", "Failure reading ssl file." );
			} else {
				ret = data;
			}
			fclose( file );
		}
		free( data );
	}
}

return strdup( ret );
}

struct MHD_OptionItem ops[] = {
	{ MHD_OPTION_CONNECTION_MEMORY_LIMIT, (size_t)(256 * 1024), NULL },
#if PRODUCTION
	{ MHD_OPTION_PER_IP_CONNECTION_LIMIT, (unsigned int)(64), NULL },
#endif
	{ MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int)(120), NULL },
	{ MHD_OPTION_END, 0, NULL }
};

/**
 * Call with the port number as the only argument.
 * Never terminates (other than by signals, such as CTRL-C).
 */
int
main_clone ()
{
  int THREADS;
  cpuInfo cpuinfo;
  #if HTTPS_SUPPORT 
  unsigned int sslport = 8881;
  #endif
  unsigned int port = 8880;
  cpuGetInfo( &cpuinfo );

  THREADS = fmax( 1.0, ( cpuinfo.socketphysicalcores / 2 ) );
#if HAVE_MAGIC_H
  magic = magic_open (MAGIC_MIME_TYPE);
  (void) magic_load (magic, NULL);
#endif
  (void) pthread_mutex_init (&mutex, NULL);
  file_not_found_response = MHD_create_response_from_buffer (strlen (NOT_FOUND_ERROR),
							     (void *) NOT_FOUND_ERROR,
							     MHD_RESPMEM_PERSISTENT);
  mark_as(file_not_found_response, "text/html" );
  request_refused_response = MHD_create_response_from_buffer (strlen (METHOD_ERROR),
							     (void *) METHOD_ERROR,
							     MHD_RESPMEM_PERSISTENT);
  mark_as(request_refused_response, "text/html" );
  internal_error_response = MHD_create_response_from_buffer (strlen (INTERNAL_ERROR_PAGE),
							     (void *) INTERNAL_ERROR_PAGE,
							     MHD_RESPMEM_PERSISTENT);
  mark_as(internal_error_response, "text/html" );
#if HTTPS_SUPPORT  
  
  server_https = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_SSL
#if EPOLL_SUPPORT 
			| MHD_USE_EPOLL_LINUX_ONLY
#endif
			,
                        sslport,
                        NULL, NULL, 
			&create_response, NULL, 
			MHD_OPTION_ARRAY, ops,
			MHD_OPTION_HTTPS_MEM_KEY, loadsslfile( "/home/stephen/.ssl/certificate.key" ),
			MHD_OPTION_HTTPS_MEM_CERT, loadsslfile("/home/stephen/.ssl/4f2815aefe61ae.crt"),
			MHD_OPTION_HTTPS_MEM_TRUST, loadsslfile("/home/stephen/.ssl/sf_bundle-g2.crt"),
                        MHD_OPTION_THREAD_POOL_SIZE, (unsigned int)THREADS,
			MHD_OPTION_NOTIFY_COMPLETED, &request_completed_callback, NULL,
			MHD_OPTION_END);

#endif  
  
  server_http = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG
#if EPOLL_SUPPORT 
			| MHD_USE_EPOLL_LINUX_ONLY
#endif
			,
                        port,
                        NULL, NULL, 
			&create_response, NULL, 
			MHD_OPTION_ARRAY, ops,
                        MHD_OPTION_THREAD_POOL_SIZE, (unsigned int)THREADS,
			MHD_OPTION_NOTIFY_COMPLETED, &request_completed_callback, NULL,
			MHD_OPTION_END);

 if(NULL == server_http)
    return 1;
  loghandle(LOG_INFO, false, "HTTP  1.1 Server live with %d thread(s) on port: %d", THREADS, port);

#if HTTPS_SUPPORT
if(NULL == server_https)
    return 1;
  loghandle(LOG_INFO, false, "HTTPS 1.1 Server live with %d thread(s) on port: %d", THREADS, sslport);
#endif
  return 0;
}


void call_clean(){

#if HTTPS_SUPPORT
MHD_stop_daemon(server_https);
#endif
MHD_stop_daemon(server_http);
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


