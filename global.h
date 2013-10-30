#define GLOBALINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.
#include <arpa/inet.h>
#include <asm/param.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <openssl/md5.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>

#include <crypt.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <math.h>
#include <mysql.h>
#include <png.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

// 0 = Off | 1 = On
#ifndef SERVER_REPORT_CONNECT
#define SERVER_REPORT_CONNECT 0
#endif


#ifndef SERVER_REPORT_CLOSE
#define SERVER_REPORT_CLOSE 0
#endif

#ifndef SERVER_REPORT_IGNOREDHEADER
#define SERVER_REPORT_IGNOREDHEADER 0
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

#ifndef SERVER_TIME_ZONE
#define SERVER_TIME_ZONE 0
#endif

#ifndef CT_TO_SECS
#define CT_TO_SECS(x) ((x)/HZ)
#endif

#ifndef DEF_PI
#define DEF_PI 3.14159265358979323846
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

#ifndef ENUMINCLUDES
#include "enum.h"
#endif

#ifndef HASHINCLUDES
#include "md5.h"
#endif

#ifndef ARTEFACTINCLUDES
#include "artefact.h"
#endif

#ifndef DATABASEINCLUDES
#include "db.h"
#endif

#ifndef SERVERINCLUDES
#include "sv.h"
#endif

#ifndef IOINCLUDES
#include "io.h"
#endif

#ifndef IOVARSINCLUDES
#include "iohttpvars.h"
#endif

#ifndef IOMIMEINCLUDES
#include "iohttpmime.h"
#endif

#ifndef IOHTTPINCLUDES
#include "iohttp.h"
#endif

#ifndef HTMLINCLUDES
#include "html.h"
#endif

#ifndef HTMLMAININCLUDES
#include "html_main.h"
#endif

#ifndef GAMEPLAYINCLUDES
#include "html_gameplay.h"
#endif

#ifndef ADMININCLUDES
#include "html_admin.h"
#endif

#ifndef USERINCLUDES
#include "html_user.h"
#endif

#ifndef IOSINCLUDES
#include "html_status.h"
#endif

#ifndef FORUMINCLUDES
#include "html_forum.h"
#endif

#ifndef COMMANDINCLUDES
#include "cmd.h"
#endif

#ifndef MAPINCLUDES
#include "map.h"
#endif

#ifndef INIINCLUDES
#include "ini.h"
#endif

