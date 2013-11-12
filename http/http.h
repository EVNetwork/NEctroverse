#ifndef HTTP_H
#define HTTP_H

/**
 * State we keep for each user/session/browser.
 */
typedef struct Session
{
  /**
   * We keep all sessions in a linked list.
   */
  struct Session *next;

  /**
   * Unique ID for this session. 
   */
  char sid[33];

  /**
   * Reference counter giving the number of connections
   * currently using this session.
   */
  unsigned int rc;

  /**
   * Time when this session was last active.
   */
  time_t start;

  /**
   * String submitted via form.
   */
  char name[256];

  /**
   * Another value submitted via form.
   */
  char pass[256];
  
  /**
   * Another value submitted via form.
   */
  char faction[256];

} SessionDef, *SessionPtr;


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
   * Language for the upload.
   */
  char *language;

  /**
   * Category for the upload.
   */
  char *category;

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
  MHD_ConnectionPtr connection;

   /**
   * URL to serve in response to this POST (if this request 
   * was a 'POST')
   */
  const char *post_url;
  
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



#define INTERNAL_ERROR_PAGE "<html><head><title>Server Error</title></head><body>An internal error has occured....</body></html>"

/**
 * Invalid method page.
 */
#define METHOD_ERROR "<html><head><title>Illegal request</title></head><body>Go away.</body></html>"

/**
 * Invalid URL page.
 */
#define NOT_FOUND_ERROR "<html><head><title>Not found</title></head><body>Go away.</body></html>"

/**
 * Front page. (/)
 */
#define MAIN_PAGE "<html><head><title>Welcome</title></head><body><form action=\"/2\" method=\"post\">What is your name? <input type=\"text\" name=\"v1\" value=\"%s\" /><input type=\"submit\" value=\"Next\" /></body></html>"

/**
 * Second page. (/2)
 */
#define SECOND_PAGE "<html><head><title>Tell me more</title></head><body><a href=\"/\">previous</a> <form action=\"/S\" method=\"post\">%s, what is your job? <input type=\"text\" name=\"v2\" value=\"%s\" /><input type=\"submit\" value=\"Next\" /></body></html>"

/**
 * Second page (/S)
 */
#define SUBMIT_PAGE "<html><head><title>Ready to submit?</title></head><body><form action=\"/F\" method=\"post\"><a href=\"/2\">previous </a> <input type=\"hidden\" name=\"DONE\" value=\"yes\" /><input type=\"submit\" value=\"Submit\" /></body></html>"

/**
 * Last page.
 */
#define LAST_PAGE "<html><head><title>Thank you</title></head><body>Thank you %s, I hope you enjoyed %s.</body></html>"

/**
 * Name of our cookie.
 */
#define COOKIE_NAME "session"
#endif
