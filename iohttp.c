#include "iohttpvars.c"
#include "iohttpmime.c"

int iohttpMimeFind( char *name )
{
  int a, b;
  char *ext;
  for( a = 0 ; name[a] ; a++ );
  for( b = 0 ; ; b++, a-- )
  {
    if( !( a ) || ( b >= 8 ) )
      return 0;
    if( name[a] == '.' )
      break;
  }
  ext = &name[a+1];
  for( a = 0 ; a < IOHTTP_MIME_TYPES ; a++ )
  {
    for( b = 0 ; b < iohttpMime[a].num ; b++ )
    {
      if( iohttpCompareExt( ext, iohttpMime[a].exts[b] ) )
        return a;
    }
  }
  return 0;
}

char iohttpInputHex( char *src )
{
  int a;
  char b = 0;
  for( a = 0 ; ; a++ )
  {
    if( ( src[a] >= '0' ) && ( src[a] <= '9' ) )
      b += src[a] - '0';
    else if( ( src[a] >= 'A' ) && ( src[a] <= 'F' ) )
      b += src[a] + 10 - 'A';
    else if( ( src[a] >= 'a' ) && ( src[a] <= 'f' ) )
      b += src[a] + 10 - 'a';
    else
      return 0;
    if( a == 0 )
      b <<= 4;
    else
      break;
  }
  return b;
}

int io_prep() {

if( http_prep() == NO )  {
	loghandle(LOG_CRIT, false, "%s", "Server Command Initation Failed, now exiting..." );
	return NO;
}

if( LoadForumList() == NO )  {
	loghandle(LOG_CRIT, false, "%s", "Forum Smiley List Initation Failed, now exiting..." );
	return NO;
}

return YES;
}

int io_end() {

if( UnLoadForumList() == NO )  {
	loghandle(LOG_CRIT, false, "%s", "Forum Smiley List Unload Failed, now exiting..." );
	return NO;
}

return YES;
}




