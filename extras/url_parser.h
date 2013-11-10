/*_
 * Copyright 2010-2011 Scyphus Solutions Co. Ltd.  All rights reserved.
 *
 * Authors:
 *      Hirochika Asai
 */
//Adapted by Necrolgan for NEctroverse.
#define URLPINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

typedef struct
{
    char *scheme;               /* mandatory */
    char *host;                 /* mandatory */
    char *port;                 /* optional */
    char *path;                 /* optional */
    char *query;                /* optional */
    char *fragment;             /* optional */
    char *username;             /* optional */
    char *password;             /* optional */
} urlinfoDef, *urlinfoPtr;

//struct urlinfo * parse_url(const char *);
void urlinfo_free( urlinfoPtr purl );

urlinfoPtr parse_url(const char *url);
