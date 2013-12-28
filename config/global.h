#ifndef GLOBALINCLUDED
#define GLOBALINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.
#include "depreciated.h"
#include <crypt.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <inttypes.h>
#include <libgen.h>
#include <limits.h>
#include <math.h>
#include <memory.h>
#if MYSQL_SUPPORT
#include <mysql.h>
#endif
#include <netdb.h>
#include <png.h>
#include <poll.h>
#include <pthread.h>
#include <pwd.h>
#include <search.h>
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
#include <unistd.h>

#include <arpa/inet.h>
#include <asm/param.h>
#include <linux/limits.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#if EPOLL_SUPPORT
#include <sys/epoll.h>
#endif
#include <sys/msg.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>

#if HAVE_MAGIC
#include <magic.h>
#endif

#if HTTPS_SUPPORT
#include <gnutls/gnutls.h>
#include <gcrypt.h>
#endif



// 0 = Off | 1 = On
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef CT_TO_SECS
#define CT_TO_SECS(x) ((x)/HZ)
#endif

#ifndef DEF_PI
#define DEF_PI 3.14159265358979323846
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


#ifndef ANG_SIN
#define ANG_SIN(x) sin((x*2*DEF_PI)/360.0)
#endif

#ifndef ANG_COS
#define ANG_COS(x) cos((x*2*DEF_PI)/360.0)
#endif

#ifndef TMPDIR
#define TMPDIR "/tmp/evcore"
#endif

#ifndef TIMES
#define TIMES
static const long minute = 60;
static const long hour = (60 * 60);
static const long day = ((60*60) * 24);
static const long week = (((60*60) * 24) * 7);
static const double megabyte = (1024 * 1024);
#endif


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


#define info(msg) loghandle(LOG_INFO, false, "%s", msg )
#define error(msg) loghandle(LOG_ERR, errno, "Error \'%s\' in: %s, on line: %d", msg, __FILE__, __LINE__ )
#define critical(msg) loghandle(LOG_CRIT, errno, "Critical Error \'%s\' in: %s, on line: %d", msg, __FILE__, __LINE__ )

#endif

