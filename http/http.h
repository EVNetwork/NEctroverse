#ifndef HTTP_H
#define HTTP_H
extern char *ServerSessionMD5;

enum UploadState {

    UPLOAD_STATE_NULL = 0,

    UPLOAD_STATE_START = 1,

    UPLOAD_STATE_DONE = 2,

    UPLOAD_STATE_FAIL = 3,

};

extern char *cmdUploadState[]; 


typedef struct _FILE_DATA_STORAGE {
	size_t size;
	time_t lastaccess;
	time_t modofied;
	char *name;
	char *mime;
	void *data;
	struct _FILE_DATA_STORAGE *next;
} FileStorageDef, *FileStoragePtr;



typedef struct _POST_DATA_STORAGE {
	int offset;
	int current;
	char *key;
	char *value;
	struct _POST_DATA_STORAGE *next;
} PostDataDef, *PostDataPtr;

/**
 * Context keeping the data for the response we're building.
 */
typedef struct StringBufferContext {
  /**
   * Current data string.
   */
  char *buf;
  
  /**
   * Number of bytes allocated for 'buf'.
   */
  size_t buf_len;

  /**
   * Current position where we append to 'buf'. Must be smaller or equal to 'buf_len'.
   */
  size_t off;

} StringBufferDef, *StringBufferPtr;

/**
 * State we keep for each user/session/browser.
 */
typedef struct Session
{
  /**
   * Upload Status key.
   */
  int upload;

  /**
   * Reference counter giving the number of connections
   * currently using this session.
   */
  unsigned int rc;

  /**
   * Unique ID for this session.
   */
  char sid[SESSION_SIZE];

  /**
   * url to redirect if needed.
   */
  char redirect[REDIRECT_MAX];
 
  /**
   * captcha string if needed.
   */
  char captcha[6];
  
  /**
   * Time when this session was last active.
   */
  time_t active;

   /**
   * Time when this session was started.
   */
  time_t start;

   /**
   * We keep all posted key/values in this struct.
   */  
  PostDataPtr postdata;
 
   /**
   * Database user Linkage
   */
  dbUserPtr dbuser;

   /**
   * We keep all sessions in a linked list.
   */
  struct Session *next;
   
} SessionDef, *SessionPtr;


extern SessionPtr get_session( int type, void *cls );

/**
 * Context we keep for an upload.
 */
typedef struct Request
{
  /**
   * Handle where we write the uploaded file to.
   */
  int fd;

  /**
   * Name of the file on disk (used to remove on errors).
   */
  char *filename;

   /**
   * URL to serve in response to this POST (if this request
   * was a 'POST')
   */
  const char *post_url;

  /**
   * Associated session.
   */
  struct Session *session;

   /**
   * Post processor we're using to process the upload.
   */
  struct MHD_PostProcessor *pp;

  /**
   * Handle to connection that we're processing the upload for.
   */
  struct MHD_Connection *connection;

  /**
   * Response to generate, NULL to use directory.
   */
  struct MHD_Response *response;

} RequestDef, *RequestPtr;


/**
 * Data per responce.
 */
typedef struct ReplyData
{
  /**
   * Post processor handling form data (IF this is
   * a POST request).
   */
  StringBufferDef cache;

  /**
   * Associated session.
   */
  struct Session *session;

  /**
   * URL to serve in response to this POST (if this request
   * was a 'POST')
   */
  struct MHD_Connection *connection;

} ReplyDataDef, *ReplyDataPtr;


/**
 * Type of handler that generates a reply.
 *
 * @param cls content for the page (handler-specific)
 * @param mime mime type to use
 * @param session session information
 * @param connection connection to process
 * @param MHD_YES on success, MHD_NO on failure
 */
typedef void (*PageFunction)( ReplyDataPtr cnt );

typedef int (*PageHandler)( int id, const void *cls, const char *mime, struct Session *session, struct MHD_Connection *connection );

/**
 * Entry we generate for each page served.
 */
typedef struct Page
{
  /**
   * Acceptable URL for this page.
   */
  const char *url;

  /**
   * Mime type to set for the page.
   */
  const char *mime;

  /**
   * Handler to call to generate response.
   */
  PageHandler handler;

  /**
   * Handler to call to generate response.
   */
  PageFunction function;
  /**
   * Extra argument to handler.
   */
  const void *handler_cls;
} PageDef, *PagePtr;


int http_prep();
int http_start();
#if HTTPS_SUPPORT
int https_start();
#endif

void purge_captcha( SessionPtr session );

void WWWSelect( );

void Shutdown();

void WWWExpire();

int remove_session( const char *sid );

int not_found_page ( int id, const void *cls, const char *mime, struct Session *session, struct MHD_Connection *connection);
int file_render ( int id, const void *cls, const char *mime, struct Session *session, struct MHD_Connection *connection);
int page_render( int id, const void *cls, const char *mime, struct Session *session, struct MHD_Connection *connection);

void *buffer_realloc( StringBufferPtr buffer, int type, size_t fitsize, int *newsize );

void AddBufferString( StringBufferPtr buffer, char *text );

void AddBufferPrint( StringBufferPtr buffer, char *string, ... );

void httpString( ReplyDataPtr rd, char *string );
void httpPrintf( ReplyDataPtr rd, char *string, ... );

char *URLAppend( ReplyDataPtr cnt, char *url );
void URLString( ReplyDataPtr cnt, char *url, char *label );

char *targetframe( ReplyDataPtr cnt );
bool securecnt( ReplyDataPtr cnt );

#endif
