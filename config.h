#define CONFIGINCLUDED
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

#define MYSQLENABLE 1

#define COREDIRECTORY "/home/stephen/git/NEctroverse/core"

//IOHTTP_*_DIRECTORY MUST exist with content... since they have files we need.
#define IOHTTP_FILES_DIRECTORY "/home/stephen/git/NEctroverse/html/images"
#define IOHTTP_READ_DIRECTORY "/home/stephen/git/NEctroverse/html/docs"

//we want to change this latter... no real "Public" forums.
#define PUBLIC_FORUM_DIRECTORY "/home/stephen/git/NEctroverse/core/data"

// 0 = Off | 1 = On
#define SERVER_REPORT_CONNECT 0
#define SERVER_REPORT_CLOSE 0
#define SERVER_REPORT_IGNOREDHEADER 0
