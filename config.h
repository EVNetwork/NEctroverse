#define CONFIGINCLUDED
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.
#define SERVERNAME "NEctroverse"
//Do not change this "on the fly", you will not beable to login. (Unless you use the md5-sum of your password)
#define HASHENCRYPTION 1
#define FORKING 0

#define HTTP_PORT 9122

#define MYSQLENABLE 1

#if MYSQLENABLE == 1
#define SQL_SRVR "localhost"
#define SQL_USER "defaultu"
#define SQL_PASS "password"
#define SQL_BASE "evcoredb"
#endif

// OK, so can you see what I've done here? Sneaky eh? Hehe =P
#ifdef HAHA_MY_INFO_IS_HIDDEN
#include ".hidden.and.nogit"
#else
#define CMD_ADMIN_NUM 1
static char cmdAdminName[CMD_ADMIN_NUM][32] = { "admin" };
static char cmdAdminPass[CMD_ADMIN_NUM][32] = { "password" };
static char cmdAdminFaction[CMD_ADMIN_NUM][32] = { "Administrator" };
static char cmdAdminForumTag[CMD_ADMIN_NUM][32] = { "Administrator" };
static int cmdAdminLevel[CMD_ADMIN_NUM] = { 3 };
static int cmdAdminRace[CMD_ADMIN_NUM] = { 0 };

static char cmdAdminEmpirePass[32] = { "password" };
static int cmdAdminEmpire = 0;
#endif

#define COREDIRECTORY "/media/stephen/mini/git/NEctroverse/core"
static char WORKDIRECTORY[32] = "%s/git/NEctroverse/core";
static char HTMLDIRECTORY[32] = "%s/git/NEctroverse/html";
#define TMPDIR "/tmp/evcore"

//IOHTTP_*_DIRECTORY MUST exist with content... since they have files we need.
#define IOHTTP_FILES_DIRECTORY "/media/stephen/mini/git/NEctroverse/html/images"
#define IOHTTP_READ_DIRECTORY "/media/stephen/mini/git/NEctroverse/html/docs"

//we want to change this latter... no real "Public" forums.
#define PUBLIC_FORUM_DIRECTORY "/media/stephen/mini/git/NEctroverse/core/data"

// Umm, IRC... old skool...
#define IRC_SERVER "irc.freenode.net"

#define ROUND_ID (8)
#define AUTOVICTORYIN (16)
#define DELAY_TIME (00)
// 1 * 60 seconds... so every 1 min. But we should change this from fixed.
//#define SV_TICK_TIME (1*60)
#define SV_TICK_TIME (1*10)

#define START_TIME " Apr 16 20:00"		//Yer, this ain't good...
#define STOP_TIME "May 14 16:00"		//Ahh crap, there's two of em...

// 0 = Off | 1 = On
#define SERVER_REPORT_CONNECT 0
#define SERVER_REPORT_CLOSE 0
#define SERVER_REPORT_IGNOREDHEADER 0

#define LOGTAG "EVServer"
#define LOGFAC LOG_LOCAL6 //<-- Possible to also just use LOG_SYSLOG for conveniance.

// 0 = Off | 1 = On -- Since the source for the map program is lost, we'll just drop support.
#define EVMAPENABLE 0

#if EVMAPENABLE == 1
#define EVMP_PORT 9135
#endif


static const long minute = 60;
static const long hour = (60 * 60);
static const long day = ((60*60) * 24);
static const double megabyte = (1024 * 1024);
/*
<<<<<<<<<<<<<END USER VARIABLES >>>>>>>>>>>>>>>>>>>








































^^^ You don't want to change anything down here... it's all up there! ^^^ =) 
*/

#define PIPEFILE "/tmp/evcore/evserver.pipe"
#define SERVER_RECV_BUFSIZE (32768)
#define SERVER_PATH_BUFSIZE (512)
#define SERVER_SELECT_MSEC (1000)
#define SERVER_NAGLE_BUFFERING 0
// Servers use GMT/UTC as standard prehaps we should add timezone to user info?
#define SERVER_TIME_ZONE 0




