
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
{ "gif" };

#define IOHTTP_MIME_HEADERS4 1
char *iohttpMimeHeaderExts4[IOHTTP_MIME_HEADERS4] =
{ "jpg" };

#define IOHTTP_MIME_HEADERS5 1
char *iohttpMimeHeaderExts5[IOHTTP_MIME_HEADERS5] =
{ "png" };

#define IOHTTP_MIME_HEADERS6 1
char *iohttpMimeHeaderExts6[IOHTTP_MIME_HEADERS6] =
{ "css" };

#define IOHTTP_MIME_HEADERS7 1
char *iohttpMimeHeaderExts7[IOHTTP_MIME_HEADERS7] =
{ "cgi" };



#define IOHTTP_MIME_TYPES 8

iohttpMimeDef iohttpMime[IOHTTP_MIME_TYPES] =
{
{ "Content-Type: application/octet-stream\n\n", iohttpMimeHeaderExts0, IOHTTP_MIME_HEADERS0 },
{ "Content-Type: text/plain\n\n", iohttpMimeHeaderExts1, IOHTTP_MIME_HEADERS1 },
{ "Content-Type: text/html\n\n", iohttpMimeHeaderExts2, IOHTTP_MIME_HEADERS2 },
{ "Content-Type: image/gif\n\n", iohttpMimeHeaderExts3, IOHTTP_MIME_HEADERS3 },
{ "Content-Type: image/jpg\n\n", iohttpMimeHeaderExts4, IOHTTP_MIME_HEADERS4 },
{ "Content-Type: image/x-png\n\n", iohttpMimeHeaderExts5, IOHTTP_MIME_HEADERS5 },
{ "Content-Type: text/css\n\n", iohttpMimeHeaderExts6, IOHTTP_MIME_HEADERS6 },
{ "Content-Type: application/octet-stream\n\n", iohttpMimeHeaderExts7, IOHTTP_MIME_HEADERS7 },
};

