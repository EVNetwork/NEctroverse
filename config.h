
#define SERVER_SOFTWARE "Ectroverse"

//NEctroverse Additions
#define ADMINDEFAULTNAME "admin"
#define ADMINDEFAULTPASS "password"

#define PIPEFILE "/tmp/evserver.pipe" //No need to change... you'll never need this anywhere else.
//End Additions

#define IRC_SERVER "irc.staticbox.net" // Umm, IRC... old skool...

#define ROUND_ID (8) //Yep, another fixed number that needs releasing to variable...
#define DELAY_TIME (00) //Erm, do something with this latter too? Hahah, naa... purge this crap.
#define SV_TICK_TIME (10*60) // 10 * 60 seconds... so every 10 min. But we should change this from fixed.

#define START_TIME " Apr 16 20:00" //Yer, this ain't good...
#define STOP_TIME "May 14 16:00" //Ahh crap, there's two of em...

#define SERVER_REPORT_CONNECT 0
#define SERVER_REPORT_CLOSE 0
#define SERVER_REPORT_IGNOREDHEADER 0


// Ok, so this needs cleaning....

#define COREDIRECTORY "/home/stephen/git/NEctroverse/core"

#define DB_DIRECTORY "/home/stephen/git/NEctroverse/core/data"
#define USER_DIRECTORY "/home/stephen/git/NEctroverse/core/users"
#define PUBLIC_FORUM_DIRECTORY "/home/stephen/git/NEctroverse/core/data" //we want to change this latter...
#define LOGS_DIRECTORY "/home/stephen/git/NEctroverse/core/logs"

#define SV_TICK_FILE "/home/stephen/git/NEctroverse/core/ticks"


//These MUST exist... since they have files we need.
#define IOHTTP_FILES_DIRECTORY "/home/stephen/git/NEctroverse/html/images"
#define IOHTTP_READ_DIRECTORY "/home/stephen/git/NEctroverse/html/docs"


// You don't want to change anything down here... it's all up there! =)
#define SERVER_RECV_BUFSIZE (32768)
#define SERVER_PATH_BUFSIZE (512)
#define SERVER_SELECT_MSEC (1000)
#define SERVER_NAGLE_BUFFERING 0
#define SERVER_TIME_ZONE 0 // Servers use GMT/UTC prehaps we should add timezone to user info?




