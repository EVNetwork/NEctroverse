
#define IOHTTP_MIME_HEADERS0 7
char *iohttpMimeHeaderExts0[IOHTTP_MIME_HEADERS0] =
{ "zip", "bin", "gz", "z", "tar", "rar", "ace" };

#define IOHTTP_MIME_HEADERS1 6
char *iohttpMimeHeaderExts1[IOHTTP_MIME_HEADERS1] =
{ "txt", "c", "h", "cpp", "hpp", "cc" };

#define IOHTTP_MIME_HEADERS2 3
char *iohttpMimeHeaderExts2[IOHTTP_MIME_HEADERS2] =
{ "html", "htm", "shtml" };

#define IOHTTP_MIME_HEADERS3 1
char *iohttpMimeHeaderExts3[IOHTTP_MIME_HEADERS3] =
{ "css" };

#define IOHTTP_MIME_HEADERS4 1
char *iohttpMimeHeaderExts4[IOHTTP_MIME_HEADERS4] =
{ "js" };

#define IOHTTP_MIME_HEADERS5 1
char *iohttpMimeHeaderExts5[IOHTTP_MIME_HEADERS5] =
{ "gif" };

#define IOHTTP_MIME_HEADERS6 1
char *iohttpMimeHeaderExts6[IOHTTP_MIME_HEADERS6] =
{ "jpg" };

#define IOHTTP_MIME_HEADERS7 1
char *iohttpMimeHeaderExts7[IOHTTP_MIME_HEADERS7] =
{ "png" };

#define IOHTTP_MIME_HEADERS8 1
char *iohttpMimeHeaderExts8[IOHTTP_MIME_HEADERS8] =
{ "ico" };

#define IOHTTP_MIME_HEADERS9 1
char *iohttpMimeHeaderExts9[IOHTTP_MIME_HEADERS9] =
{ "cgi" };



#define IOHTTP_MIME_TYPES 10

iohttpMimeDef iohttpMime[IOHTTP_MIME_TYPES] =
{
{ "application/octet-stream", iohttpMimeHeaderExts0, IOHTTP_MIME_HEADERS0 },
{ "text/plain", iohttpMimeHeaderExts1, IOHTTP_MIME_HEADERS1 },
{ "text/html", iohttpMimeHeaderExts2, IOHTTP_MIME_HEADERS2 },
{ "text/css", iohttpMimeHeaderExts3, IOHTTP_MIME_HEADERS3 },
{ "text/javascript", iohttpMimeHeaderExts4, IOHTTP_MIME_HEADERS4 },
{ "image/gif", iohttpMimeHeaderExts5, IOHTTP_MIME_HEADERS5 },
{ "image/jpg", iohttpMimeHeaderExts6, IOHTTP_MIME_HEADERS6 },
{ "image/x-png", iohttpMimeHeaderExts7, IOHTTP_MIME_HEADERS7 },
{ "image/vnd.microsoft.icon", iohttpMimeHeaderExts8, IOHTTP_MIME_HEADERS8 },
{ "application/octet-stream", iohttpMimeHeaderExts9, IOHTTP_MIME_HEADERS9 },
};

