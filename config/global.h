#ifndef GLOBALINCLUDED
#define GLOBALINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.
#include "buildflags.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#if MYSQL_SUPPORT
#include <mysql.h>
#endif
#include <png.h>
#include <poll.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <time.h>

#include <arpa/inet.h>
#include <asm/param.h>
#if EPOLL_SUPPORT
#include <sys/epoll.h>
#endif
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/sendfile.h>
//#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
//#include <sys/time.h>
//#include <sys/times.h>
//#include <sys/types.h>
//#include <sys/un.h>
#include <sys/utsname.h>
//#include <sys/wait.h>

#if HAVE_MAGIC
#include <magic.h>
#endif

#if HTTPS_SUPPORT
#include <gnutls/gnutls.h>
#include <gcrypt.h>
#endif

#if FACEBOOK_SUPPORT
#include <curl/curl.h>
#endif

#ifndef YES
#define YES true
#endif
#ifndef NO
#define NO false
#endif
#ifndef TRUE
#define TRUE true
#endif
#ifndef FALSE
#define FALSE false
#endif
#ifndef null
#define null NULL
#endif


static const long minute = 60;
static const long hour = (60 * 60);
static const long day = ((60*60) * 24);
static const long week = (((60*60) * 24) * 7);

#define KB_SIZE 1024
static const double megabyte = (KB_SIZE * KB_SIZE);

#define SERVER_ARTIFACT_MAX 30

#define DEFAULT_BUFFER 4*KB_SIZE

#ifndef PATH_MAX
#define PATH_MAX 4*KB_SIZE
#endif

#define ARRAY_MAX 64*KB_SIZE

#define MAIL_MAX 64*KB_SIZE
#define FORUM_MAX 64*KB_SIZE

#define INI_MAX 1*KB_SIZE

#define USER_DESC_MAX 4*KB_SIZE
#define USER_PASS_MAX 129
#define USER_NAME_MAX 129
#define USER_FTAG_MAX 256

#define SESSION_SIZE 129
#define SESSION_TIME ( 15 * minute )


#define CT_TO_SECS(x) ((x)/HZ)
#define DEF_PI 3.14159265358979323846
#define ANG_SIN(x) sin((x*2*DEF_PI)/360.0)
#define ANG_COS(x) cos((x*2*DEF_PI)/360.0)


#ifndef TMPDIR
#define TMPDIR "/tmp/evcore"
#endif

#define RANDOMIZE_SEED srand( ( ANG_SIN( time(NULL) ) * ( time(NULL) / DEF_PI ) ) )

#define file_r( data, size, count, file ) { if( fread( data, size, count, file ) < 1 ) loghandle(LOG_ERR, errno, "File Read error in: %s, on line: %d", __FILE__, __LINE__ ); }
#define file_w( data, size, count, file ) { if( fwrite( data, size, count, file ) < 1 ) loghandle(LOG_ERR, errno, "File Write error in: %s, on line: %d", __FILE__, __LINE__ ); }
#define file_s( file, offset ) { if( fseek( file, offset, SEEK_SET ) ) loghandle(LOG_ERR, errno, "File Seek error in: %s, on line: %d", __FILE__, __LINE__ ); }


#define info(msg, ...) loghandle(LOG_INFO, false, msg, ##__VA_ARGS__);

#define error(msg, ... ) { \
StringBufferPtr error_buffer;\
error_buffer = calloc( 1, sizeof(StringBufferDef) ); \
AddBufferPrint( error_buffer, msg, ##__VA_ARGS__ ); \
loghandle(LOG_ERR, errno, "%s (file: %s, line: %d)", error_buffer->buf, __FILE__, __LINE__ ); \
free( error_buffer->buf );\
free( error_buffer );\
}

#define critical(msg, ... ) { \
StringBufferPtr critical_buffer;\
critical_buffer = calloc( 1, sizeof(StringBufferDef) ); \
AddBufferPrint( critical_buffer, msg, ##__VA_ARGS__ ); \
loghandle(LOG_CRIT, errno, "%s (file: %s, line: %d)", critical_buffer->buf, __FILE__, __LINE__ ); \
free( critical_buffer->buf );\
free( critical_buffer );\
ticks.status = false; \
ticks.locked = true; \
}

#define REDIRECT_MAX 1*KB_SIZE
#define redirect(cnt, url, ...) snprintf( (cnt->session)->redirect, REDIRECT_MAX, url, ##__VA_ARGS__ )

#define GetSetting( name ) loadfromconfig( name, __FILE__, __LINE__ )
#define UnloadSetting( name ) unloadfromconfig( name, __FILE__, __LINE__ )
#define ListSettings( list ) makelistfromconfig( list, __FILE__, __LINE__ )



#include "../enum.h"
#include "../artefact.h"
#include "../db.h"

#include "../http/platform.h"
#include "../extras/extras.h"

#if IRCBOT_SUPPORT
#include "../ircbot/ircbot.h"
#endif

#include "../battle.h"
#include "../main.h"
#include "../io.h"
#include "../iohttpvars.h"
#include "../iohttpmime.h"
#include "../iohttp.h"
#include "../html/html.h"
#include "../cmd.h"
#include "../map.h"


#endif //END OF FILE
