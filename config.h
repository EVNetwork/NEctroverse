#define SERVER_SOFTWARE "NEctroverse"

#define HTTP_PORT 9122

// 0 = Off | 1 = On -- Since the source for the map program is lost, we'll just drop support.
#define EVMAPENABLE 0

#if EVMAPENABLE == 1
#define EVMP_PORT 9135
#endif


#define CMD_ADMIN_NUM 1

static const char cmdAdminName[CMD_ADMIN_NUM] = {"admin"};
static const char cmdAdminPass[CMD_ADMIN_NUM] = {"password"};

#define ADMIN_DEFAULT_NAME "admin"
#define ADMIN_DEFAULT_PASS "password"

#define COREDIRECTORY "/home/stephen/git/NEctroverse/core"
//IOHTTP_*_DIRECTORY MUST exist with content... since they have files we need.
#define IOHTTP_FILES_DIRECTORY "/home/stephen/git/NEctroverse/html/images"
#define IOHTTP_READ_DIRECTORY "/home/stephen/git/NEctroverse/html/docs"

//we want to change this latter... no real "Public" forums.
#define PUBLIC_FORUM_DIRECTORY "/home/stephen/git/NEctroverse/core/data"

// Umm, IRC... old skool...
#define IRC_SERVER "irc.staticbox.net"

#define ROUND_ID (8)
#define DELAY_TIME (00)
// 10 * 60 seconds... so every 10 min. But we should change this from fixed.
#define SV_TICK_TIME (10*60)

#define START_TIME " Apr 16 20:00"		//Yer, this ain't good...
#define STOP_TIME "May 14 16:00"		//Ahh crap, there's two of em...

// 0 = Off | 1 = On
#define SERVER_REPORT_CONNECT 0
#define SERVER_REPORT_CLOSE 0
#define SERVER_REPORT_IGNOREDHEADER 0

#define LOGTAG "EVServer"
#define LOGFAC LOG_LOCAL6



























// You don't want to change anything down here... it's all up there! =)
#define PIPEFILE "/tmp/evserver.pipe"
#define SERVER_RECV_BUFSIZE (32768)
#define SERVER_PATH_BUFSIZE (512)
#define SERVER_SELECT_MSEC (1000)
#define SERVER_NAGLE_BUFFERING 0
// Servers use GMT/UTC as standard prehaps we should add timezone to user info?
#define SERVER_TIME_ZONE 0




