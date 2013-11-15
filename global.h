#ifndef GLOBALINCLUDED
#define GLOBALINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.
#include <crypt.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <inttypes.h>
#include <libgen.h>
#include <math.h>
#include <mysql.h>
#include <netdb.h>
#include <png.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <asm/param.h>
#include <linux/limits.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>

// 0 = Off | 1 = On
#ifndef SERVER_REPORT_CONNECT
#define SERVER_REPORT_CONNECT 1
#endif


#ifndef SERVER_REPORT_CLOSE
#define SERVER_REPORT_CLOSE 1
#endif

#ifndef SERVER_REPORT_IGNOREDHEADER
#define SERVER_REPORT_IGNOREDHEADER 1
#endif

#ifndef SERVER_RECV_BUFSIZE
#define SERVER_RECV_BUFSIZE (32768)
#endif

#ifndef SERVER_PATH_BUFSIZE
#define SERVER_PATH_BUFSIZE (512)
#endif

#ifndef SERVER_SELECT_MSEC
#define SERVER_SELECT_MSEC (1000)
#endif

#ifndef SERVER_NAGLE_BUFFERING
#define SERVER_NAGLE_BUFFERING 0
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
static const double megabyte = (1024 * 1024);
#endif


#include "enum.h"
#include "artefact.h"
#include "db.h"


#include "http/config.h"
#include "http/platform.h"

#include "extras/cpuinfo.h"

#include "extras/dictionary.h"
#include "extras/iniparser.h"

#include "extras/md5.h"
#include "extras/base64.h"
#include "extras/encrypt.h"

#include "battle.h"
#include "main.h"
#include "io.h"
#include "iohttpvars.h"
#include "iohttpmime.h"
#include "iohttp.h"
#include "html.h"
#include "html_main.h"
#include "html_gameplay.h"
#include "html_admin.h"
#include "html_user.h"
#include "html_status.h"
#include "html_forum.h"
#include "cmd.h"
#include "map.h"
#include "ircbot.h"
#include "html_ajax.h"


#include "extras/url_parser.h"

#endif

