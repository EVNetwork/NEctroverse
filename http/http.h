#ifndef HTTP_H
#define HTTP_H

#define SERVERALLOCATION (256 * 1024)


enum UploadState {

    UPLOAD_STATE_NULL = 0,

    UPLOAD_STATE_START = 1,

    UPLOAD_STATE_DONE = 2,

    UPLOAD_STATE_FAIL = 3,

};

extern char *cmdUploadState[]; 
/**
 * State we keep for each user/session/browser.
 */
typedef struct Session
{
  /**
   * Number of keys.
   */
  int posts;

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
  char sid[33];

  /**
   * Pointers for keys.
   */
  char *key[512];

  /**
   * Pointers for values.
   */
  char *value[512];

  /**
   * Time when this session was last active.
   */
  time_t start;

   /**
   * We keep all sessions in a linked list.
   */
  struct Session *next;

} SessionDef, *SessionPtr;

/**
 * State we keep for each page build... in case we need to set cookies.
 */
typedef struct Cookies
{
  /**
   * Number of cookies.
   */
  int num;

  /**
   * Pointers for cookie values.
   */
  char value[4][512];

} CookiesDef, *CookiesPtr;

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
  SessionPtr session;

   /**
   * Post processor we're using to process the upload.
   */
  struct MHD_PostProcessor *pp;

  /**
   * Handle to connection that we're processing the upload for.
   */
  MHD_ConnectionPtr connection;

  /**
   * Response to generate, NULL to use directory.
   */
  MHD_ResponsePtr response;

} RequestDef, *RequestPtr;


/**
 * Data per responce.
 */
typedef struct ReplyData
{
  /**
   * Associated cookies to be set with responce.
   */
  CookiesDef cookies;

  /**
   * Associated session.
   */
  SessionPtr session;

  /**
   * Post processor handling form data (IF this is
   * a POST request).
   */
  ResponseDataDef response;

  /**
   * URL to serve in response to this POST (if this request
   * was a 'POST')
   */
  MHD_ConnectionPtr connection;

  /**
   * Database user Linkage
   */
  dbUserPtr dbuser;

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

typedef int (*PageHandler)( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection );

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

extern PageDef pages[];

extern MHD_DaemonPtr server_http;
extern MHD_DaemonPtr server_https;

void call_clean();

int main_clone();

extern void mark_as( MHD_ResponsePtr response, const char *type );

extern size_t initial_allocation;

extern void expire_sessions ();

extern int create_response (void *cls, MHD_ConnectionPtr connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **ptr);

extern void request_completed_callback (void *cls, MHD_ConnectionPtr connection, void **con_cls, enum MHD_RequestTerminationCode toe);

extern int not_found_page ( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection);
extern int key_page( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection);
extern int page_render( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection);
extern int file_page( int id, const void *cls, const char *mime, SessionPtr session, MHD_ConnectionPtr connection);

#define INTERNAL_ERROR_PAGE "<html><head><title>Server Error</title></head><body>An internal error has occured....</body></html>"

/**
 * Invalid method page.
 */
#define METHOD_ERROR "<html><head><title>Illegal request</title></head><body>Bad request.</body></html>"

/**
 * Invalid URL page.
 */
#define NOT_FOUND_ERROR "<html><head><title>Not found</title></head><body>The item you are looking for was not found...</body></html>"

/**
 * Name of our cookie.
 */
#define COOKIE_NAME "evsid"
#endif
