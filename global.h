#define GLOBALINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.
#include <arpa/inet.h>
#include <asm/param.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#if HASHENCRYPTION == 1
#include <openssl/md5.h>
#endif
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>

#if HASHENCRYPTION == 1
#include <crypt.h>
#endif
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#if MYSQLENABLE == 1
#include <mysql.h>
#endif
#include <png.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

//double option here.
#if HASHENCRYPTION == 1
#ifndef HASHINCLUDES
#include "optional/md5.h"
#endif
#endif
//The order of these is somewhat importatant, as each can call from another...
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



#ifndef CT_TO_SECS
#define CT_TO_SECS(x) ((x)/HZ)
#endif
