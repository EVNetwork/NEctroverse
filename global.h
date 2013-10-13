#define CONFIGINCLUDED
//The above line MUST STAY HERE! -- This prevents double calling.
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#if HASHPASSWORD == 1
#include <openssl/md5.h>
#endif
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#if MYSQLENABLE == 1
#include <mysql.h>
#endif
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
