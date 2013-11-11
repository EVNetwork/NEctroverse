#ifndef IOMIMEINCLUDES
#define IOMIMEINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

typedef struct
{
  char def[128];
  char **exts;
  int num;
} iohttpMimeDef, *iohttpMimePtr;

extern char *iohttpMimeHeaderExts0[];

extern char *iohttpMimeHeaderExts2[];

extern char *iohttpMimeHeaderExts3[];

extern char *iohttpMimeHeaderExts4[];

extern char *iohttpMimeHeaderExts5[];

extern char *iohttpMimeHeaderExts6[];

extern char *iohttpMimeHeaderExts7[];

extern iohttpMimeDef iohttpMime[];

#endif
