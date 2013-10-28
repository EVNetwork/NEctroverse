
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
{ "Content-Type: application/octet-stream\n\n", iohttpMimeHeaderExts0, IOHTTP_MIME_HEADERS0 },
{ "Content-Type: text/plain\n\n", iohttpMimeHeaderExts1, IOHTTP_MIME_HEADERS1 },
{ "Content-Type: text/html\n\n", iohttpMimeHeaderExts2, IOHTTP_MIME_HEADERS2 },
{ "Content-Type: text/css\n\n", iohttpMimeHeaderExts3, IOHTTP_MIME_HEADERS3	 },
{ "Content-Type: text/javascript\n\n", iohttpMimeHeaderExts4, IOHTTP_MIME_HEADERS4 },
{ "Content-Type: image/gif\n\n", iohttpMimeHeaderExts5, IOHTTP_MIME_HEADERS5 },
{ "Content-Type: image/jpg\n\n", iohttpMimeHeaderExts6, IOHTTP_MIME_HEADERS6 },
{ "Content-Type: image/x-png\n\n", iohttpMimeHeaderExts7, IOHTTP_MIME_HEADERS7 },
{ "Content-Type: image/vnd.microsoft.icon\n\n", iohttpMimeHeaderExts8, IOHTTP_MIME_HEADERS8 },
{ "Content-Type: application/octet-stream\n\n", iohttpMimeHeaderExts9, IOHTTP_MIME_HEADERS9 },
};

