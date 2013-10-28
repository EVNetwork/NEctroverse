#ifndef GLOBALINCLUDED
#include "global.h"
#endif

char iohttpEnd[2][5] = { "\r\n\r\n", "\n\n" };
int iohttpEndSize[2] = { 4, 2 };

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


iohttpFilePtr iohttpFileAdd( int size )
{
  iohttpFilePtr file;
  char *mem;
  if( !( mem = malloc( sizeof(iohttpFileDef) + size ) ) )
  {
    printf( "Error %03d, malloc\n", errno );
    return 0;
  }
  file = (iohttpFilePtr)mem;
  memset( file, 0, sizeof(iohttpFileDef) );
  file->data = &mem[sizeof(iohttpFileDef)];
  file->size = size;
  file->prev = (void **)&(iohttpFileList);
  file->next = iohttpFileList;
  if( iohttpFileList )
    iohttpFileList->prev = &(file->next);
  iohttpFileList = file;
  return file;
}

int iohttpFileDel( char *path )
{
  iohttpFilePtr file, next;
  for( file = iohttpFileList ; file ; file = file->next )
  {
    if( !( ioCompareExact( file->path, path ) ) )
      continue;
    next = file->next;
    *(file->prev) = (void *)next;
    if( next )
      next->prev = file->prev;
    free( file );
    return 1;
  }
  return 0;
}

iohttpFilePtr iohttpFileFind( char *path )
{
  iohttpFilePtr file;
  for( file = iohttpFileList ; file ; file = file->next )
  {
    if( !( ioCompareExact( file->path, path ) ) )
      continue;
    return file;
  }
  return 0;
}




void InitHTTP()
{
	char COREDIR[1024];
  int a;
  DIR *dirdata;
  struct dirent *direntry;
  struct stat stdata;
  iohttpFilePtr file;
  FILE *fd;
  dbMainEmpireDef empired;


if( chdir( sysconfig.httpimages ) == -1 ) {
	printf( "Error %d, chdir, Dir : %s\n", errno, sysconfig.httpimages );
	return;
}
if( !( dirdata = opendir( sysconfig.httpimages ) ) ) {
	printf( "Error %03d, opendir\n", errno );
	return;
}
while( ( direntry = readdir( dirdata ) ) ) {
	if( stat( direntry->d_name, &stdata ) == -1 )
		continue;
	if( !( S_ISREG( stdata.st_mode ) ) )
		continue;
	if( strlen(direntry->d_name) >= SERVER_PATH_BUFSIZE-1 )
		continue;
	if( !( fd = fopen( direntry->d_name, "rb" ) ) )
		continue;
	if( !( file = iohttpFileAdd( stdata.st_size ) ) ) {
		printf( "Error %03d, malloc\n", errno );
		fclose( fd );
		continue;
	}
	file->type = FILE_IMAGEDIR;
	file->path[0] = '/';
	file->mime = iohttpMimeFind( direntry->d_name );
	memcpy( &file->scurtime, &stdata.st_mtime, sizeof(time_t) );
	sprintf( &file->path[1], "images/%s", direntry->d_name );
	fread( file->data, 1, file->size, fd );
	fclose( fd );
}
closedir( dirdata );

if( chdir( sysconfig.httpfiles ) == -1 ) {
	printf( "Error %d, chdir, Dir : %s\n", errno, sysconfig.httpfiles );
	return;
}
if( !( dirdata = opendir( sysconfig.httpfiles ) ) ) {
	printf( "Error %03d, opendir\n", errno );
	return;
}

sprintf( COREDIR, "%s/data", sysconfig.directory );
if( chdir( COREDIR ) != -1 ) {
	for( file = iohttpFileList ; file ; file = file->next ) {
		if( !( ioCompareFindWords( file->path, "/fampic" ) ) )
			continue;
		if( sscanf( &file->path[7], "%d", &a ) != 1 )
			continue;
		if( dbMapRetrieveEmpire( a, &empired ) < 0 )
			continue;
		if( (unsigned int)empired.picmime > IOHTTP_MIME_TYPES )
			continue;
		file->mime = empired.picmime;
		sprintf( file->path, "/fampic%02d%d", a, empired.pictime );
	}
}

  // ahhaha... yes, we'll make a table

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_front;
  sprintf( file->path, "/" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_faq;
  sprintf( file->path, "/faq" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_gettingstarted;
  sprintf( file->path, "/gettingstarted" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_halloffame;
  sprintf( file->path, "/halloffame" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_status;
  sprintf( file->path, "/status" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_races;
  sprintf( file->path, "/races" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_register;
  sprintf( file->path, "/register" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_register2;
  sprintf( file->path, "/register2" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_register3;
  sprintf( file->path, "/register3" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_login;
  sprintf( file->path, "/login" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_main;
  sprintf( file->path, "/main" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_menu;
  sprintf( file->path, "/menu" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_hq;
  sprintf( file->path, "/hq" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_news;
  sprintf( file->path, "/news" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_council;
  sprintf( file->path, "/council" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_units;
  sprintf( file->path, "/units" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_market;
  sprintf( file->path, "/market" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_planets;
  sprintf( file->path, "/planets" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_pltake;
  sprintf( file->path, "/pltake" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_empire;
  sprintf( file->path, "/empire" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_famaid;
  sprintf( file->path, "/famaid" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_famgetaid;
  sprintf( file->path, "/famgetaid" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_famvote;
  sprintf( file->path, "/famvote" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_famnews;
  sprintf( file->path, "/famnews" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_famrels;
  sprintf( file->path, "/famrels" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_famleader;
  sprintf( file->path, "/famleader" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_map;
  sprintf( file->path, "/map" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_mappick;
  sprintf( file->path, "/mappick" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_mapadv;
  sprintf( file->path, "/mapadv" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_system;
  sprintf( file->path, "/system" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_player;
  sprintf( file->path, "/player" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_playerlist;
  sprintf( file->path, "/playerlist" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_planet;
  sprintf( file->path, "/planet" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_build;
  sprintf( file->path, "/build" );
  
  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_cancelbuild;
  sprintf( file->path, "/cancelbuild" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_massbuild;
  sprintf( file->path, "/massbuild" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_raze;
  sprintf( file->path, "/raze" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_fleets;
  sprintf( file->path, "/fleets" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_fleetssplit;
  sprintf( file->path, "/fleetssplit" );
  
  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_fleetsmerge;
  sprintf( file->path, "/fleetsmerge" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_fleetdisband;
  sprintf( file->path, "/fleetdisband" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_fleetsend;
  sprintf( file->path, "/fleetsend" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_fleetchange;
  sprintf( file->path, "/fleetchange" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_fleetattack;
  sprintf( file->path, "/fleetattack" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_explore;
  sprintf( file->path, "/explore" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_attack;
  sprintf( file->path, "/attack" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_station;
  sprintf( file->path, "/station" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_spec;
  sprintf( file->path, "/spec" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_specinfos;
  sprintf( file->path, "/specinfos" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_operation;
  sprintf( file->path, "/operation" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_spell;
  sprintf( file->path, "/spell" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_incant;
  sprintf( file->path, "/incant" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_operationsend;
  sprintf( file->path, "/operationsend" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_spellsend;
  sprintf( file->path, "/spellsend" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_incantsend;
  sprintf( file->path, "/incantsend" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_research;
  sprintf( file->path, "/research" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_mail;
  sprintf( file->path, "/mail" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_rankings;
  sprintf( file->path, "/rankings" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_famranks;
  sprintf( file->path, "/famranks" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_ptrankings;
  sprintf( file->path, "/ptrankings" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_ptfamranks;
  sprintf( file->path, "/ptfamranks" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_account;
  sprintf( file->path, "/account" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_changepass;
  sprintf( file->path, "/changepass" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_delete;
  sprintf( file->path, "/delete" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_logout;
  sprintf( file->path, "/logout" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_search;
  sprintf( file->path, "/search" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_moderator;
  sprintf( file->path, "/moderator" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_oldadmin;
  sprintf( file->path, "/admin" );
  
  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_adminframe;
  sprintf( file->path, "/administration" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_adminmenu;
  sprintf( file->path, "/adminmenu" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_adminforum;
  sprintf( file->path, "/adminforum" );

  file = iohttpFileAdd(0);
  file->type = FILE_FUNCTION;
  file->function = iohttpForum;
  sprintf(file->path, "/forum");
  
  file = iohttpFileAdd( 0 );
  file->type = FILE_FUNCTION;
  file->function = iohttpFunc_Approve;
  sprintf( file->path, "/approve" );
  
  file = iohttpFileAdd( 0 );
  file->type = FILE_HTML;
  sprintf( file->path, "/stats" );
  sprintf( file->fileread, "stats.html" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_HTML;
  sprintf( file->path, "/chat" );
  sprintf( file->fileread, "chat.html" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_CSS;
  sprintf( file->path, "/style.css" );
  sprintf( file->fileread, "style.css" );

  file = iohttpFileAdd( 0 );
  file->type = FILE_JAVA;
  sprintf( file->path, "/javascript.js" );
  sprintf( file->fileread, "javascript.js" );

return;
}



void EndHTTP()
{
  iohttpFilePtr file, next;
  for( file = iohttpFileList ; file ; file = next )
  {
    next = file->next;
    free( file );
  }
  iohttpFileList = 0;
  return;
}



void inNewHTTP( svConnectionPtr cnt )
{
  int i;
  iohttpDataPtr iohttp;
  if( !( cnt->iodata = malloc( sizeof(iohttpDataDef) ) ) )
  {
    svSendString( cnt, "Internal error, malloc failed\n" );
    cnt->flags |= SV_FLAGS_NEED_WRITE;
    return;
  }
  memset( cnt->iodata, 0, sizeof(iohttpDataDef) );

  iohttp = cnt->iodata;
  for( i = 0 ; i < svBanNum ; i++ )
  {
    if( !( ioCompareFindWords( inet_ntoa( cnt->sockaddr.sin_addr ), svBanList[i] ) ) )
      continue;
    cnt->flags |= SV_FLAGS_NEED_WRITE;
    iohttp->flags = 8 | 16;
    iohttp->code = 200;
    return;
  }

  return;
}



void inNewDataHTTP( svConnectionPtr cnt )
{
  int a, b, c;
  int content_pos;
  iohttpDataPtr iohttp = cnt->iodata;

  if( iohttp->flags & 8 )
    return;

  if( iohttp->flags & 2 )
  {
    content_pos = 0;
    goto inNewDataHTTPL1;
  }
  else if( iohttp->flags & 1 )
  {
    content_pos = iohttp->content - cnt->recv;
    goto inNewDataHTTPL1;
  }
  else if( !( iohttp->content = ioCompareFindWords( cnt->recv, "\r\n\r\n" ) ) )
  {
    if( !( iohttp->content = ioCompareFindWords( cnt->recv, "\n\n" ) ) )
      return;
    content_pos = iohttp->content - cnt->recv;
    a = content_pos - 1;
  }
  else
  {
    content_pos = iohttp->content - cnt->recv;
    a = content_pos - 2;
  }

  for( b = c = 0 ; b < a ; b++ )
  {
    if( cnt->recv[b] == '\r' )
    {
      cnt->recv[b] = 0;
      continue;
    }
    if( cnt->recv[b] != '\n' )
      continue;
    cnt->recv[b] = 0;
    if( iohttpParseHeader( cnt, iohttp, &cnt->recv[c] ) )
      break;
    c = b + 1;
  }
  iohttp->flags |= 1;

  inNewDataHTTPL1:

  if( cnt->recv_pos - content_pos < iohttp->content_length )
    return;

  cnt->recv[ content_pos + iohttp->content_length ] = 0;
  cnt->flags |= SV_FLAGS_NEED_WRITE;
  iohttp->flags |= 8;

  return;
}

void outSendReplyHTTP( svConnectionPtr cnt )
{
  time_t curtime;
  char scurtime[256];
  iohttpDataPtr iohttp = cnt->iodata;
  iohttpFilePtr file;
  FILE *fd;
  struct stat stdata;
  char *data;
  char path[4096];

  file = iohttp->file;
  if( iohttp->code == 200 )
    svSendString( cnt, "HTTP/1.0 200 OK\n" );
  else if( iohttp->code == 201 )
    svSendString( cnt, "HTTP/1.0 201 Created\n" );
  else if( iohttp->code == 400 )
    svSendString( cnt, "HTTP/1.0 400 Bad Request\n" );
  else if( iohttp->code == 403 )
    svSendString( cnt, "HTTP/1.0 403 Permission Denied\n" );
  else if( iohttp->code == 404 )
    svSendString( cnt, "HTTP/1.0 404 Not Found\n" );
  else if( iohttp->code == 501 )
    svSendString( cnt, "HTTP/1.0 501 Method Not Supported\n" );
  else
    svSendString( cnt, "HTTP/1.0 500 Internal Error\n" );

  svSendPrintf( cnt, "Server: %s\nConnection: close\n", sysconfig.servername );
  curtime = time( (time_t*) 0 );
  strftime( scurtime, 256, "%a, %d %b %Y %H:%M:%S GMT", gmtime( &curtime ) );
  svSendPrintf( cnt, "Date: %s\n", scurtime );

  if( iohttp->flags & 16 )
  {
    svSendPrintf( cnt, "Last-Modified: %s\n", scurtime );
    svSendString( cnt, "Content-Type: text/html\n\n" );
    svSendString( cnt, "<html><head><style type=\"text/css\">body,td{font-size:smaller;font-family:verdana,geneva,arial,helvetica,sans-serif;}a:hover{color:#00aaaa}</style></head><body bgcolor=\"#000000\" text=\"#C0D0D8\" link=\"#FFFFFF\" alink=\"#FFFFFF\" vlink=\"#FFFFFF\">" );
    svSendPrintf( cnt, "Your IP has been banned from %s, it is likely that you know why if it went that far.<br><br>Have a nice day!", sysconfig.servername );
    svSendString( cnt, "</body></html>" );
  }
  else if( iohttp->flags & 4 )
  {
    svSendPrintf( cnt, "Last-Modified: %s\n", scurtime );
    svSendString( cnt, "Content-Type: text/html\n\n" );
    svSendString( cnt, "<html><head><style type=\"text/css\">body,td{font-size:smaller;font-family:verdana,geneva,arial,helvetica,sans-serif;}a:hover{color:#00aaaa}</style></head><body bgcolor=\"#000000\" text=\"#C0D0D8\" link=\"#FFFFFF\" alink=\"#FFFFFF\" vlink=\"#FFFFFF\">" );
    svSendPrintf( cnt, "Rejected POST query : content-lenght value too high.<br><br>If you were uploading a empire picture or posting a huge post, please make it smaller than 64kb." );
    svSendString( cnt, "</body></html>" );
  }
  else if( !( file ) )
  {
    svSendPrintf( cnt, "Last-Modified: %s\n", scurtime );
    svSendString( cnt, "Content-Type: text/html\n\n" );
    outSendReplyHTTPL0:
    svSendString( cnt, "<html><head><style type=\"text/css\">body,td{font-size:smaller;font-family:verdana,geneva,arial,helvetica,sans-serif;}a:hover{color:#00aaaa}</style></head><body bgcolor=\"#000000\" text=\"#C0D0D8\" link=\"#FFFFFF\" alink=\"#FFFFFF\" vlink=\"#FFFFFF\">" );
    svSendPrintf( cnt, "HTTP Code : %d<br>Path requested : %s<br>Method : %d<br>Query string : %s<br>Agent : %s<br>Content : %s<br>Host : %s<br>Referer : %s<br>Cookie : %s<br>", iohttp->code, iohttp->path, iohttp->method, iohttp->query_string, iohttp->user_agent, iohttp->content, iohttp->host, iohttp->referer, iohttp->cookie );
    svSendString( cnt, "</body></html>" );
  }
  else if( file->type == FILE_IMAGEDIR )
  {
    strftime( scurtime, 256, "%a, %d %b %Y %H:%M:%S GMT", gmtime( &file->scurtime ) );
    svSendPrintf( cnt, "Last-Modified: %s\n", scurtime );
    svSendString( cnt, "Expires: Thu, 01 Dec 2004 16:00:00 GMT\n" );
    svSendPrintf( cnt, "Content-Length: %d\n", file->size );
    svSendPrintf( cnt, "Content-MD5: %s\n", str2md5(file->data) );
    svSendPrintf( cnt, iohttpMime[file->mime].def );
    svSendStatic( cnt, file->data, file->size );
  }
  else if( file->type == FILE_FUNCTION )
  {
    svSendPrintf( cnt, "Last-Modified: %s\n", scurtime );
    svSendString( cnt, "Cache-control: no-store, no-cache, max-age=0, must-revalidate\n" );
    svSendString( cnt, "Pragma: no-cache\n" );
    fflush( stdout );
    file->function( cnt );
/*
    cmdExecuteFlush();
*/
  }
  else if( ( file->type == FILE_HTML ) || ( file->type == FILE_CSS ) || ( file->type == FILE_JAVA ) )
  {
    sprintf( path, "%s/%s", sysconfig.httpfiles, file->fileread );
    if( stat( path, &stdata ) == -1 )
      goto outSendReplyHTTPL0;
    if( !( data = malloc( stdata.st_size + 1 ) ) )
      goto outSendReplyHTTPL0;
    if( !( fd = fopen( path, "rb" ) ) )
    {
      free( data );
      goto outSendReplyHTTPL0;
    }
    data[stdata.st_size] = 0;
    fread( data, 1, stdata.st_size, fd );

    strftime( scurtime, 256, "%a, %d %b %Y %H:%M:%S GMT", gmtime( &stdata.st_mtime ) );
    svSendPrintf( cnt, "Last-Modified: %s\n", scurtime );
    svSendString( cnt, "Cache-control: no-store, no-cache, max-age=0, must-revalidate\n" );
    svSendPrintf( cnt, "Content-Length: %d\n", (int)stdata.st_size );
    svSendPrintf( cnt, "Content-MD5: %s\n", str2md5(data) );
    svSendString( cnt, "Pragma: no-cache\n" );
    file->mime = iohttpMimeFind( file->fileread );
    svSendPrintf( cnt, iohttpMime[file->mime].def );

    svSendString( cnt, data );
    fclose( fd );
    free( data );
  }

/*
  printf( "Path : %s\n", iohttp->path );
  fflush( stdout );
*/


  return;
}



void inSendCompleteHTTP( svConnectionPtr cnt )
{
  cnt->flags |= SV_FLAGS_TO_CLOSE;
  return;
}



void inClosedHTTP( svConnectionPtr cnt )
{
  iohttpDataPtr iohttp;
  if( cnt->iodata )
  {
    iohttp = cnt->iodata;
    if( iohttp->flags & 2 )
      free( iohttp->content );
    free( iohttp );
  }
  cnt->iodata = 0;
  return;
}



void inErrorHTTP( svConnectionPtr cnt, int type )
{
  int a, content_pos;
  char *content;
  iohttpDataPtr iohttp = cnt->iodata;
  if( type == 0 )
    cnt->flags |= SV_FLAGS_TO_CLOSE;
  else if( type == 1 )
  {
    content = iohttp->content;
    content_pos = iohttp->content - cnt->recv;
    if( ( iohttp->content_length >= 1024+64000 ) || ( iohttp->flags & 2 ) || !( iohttp->content ) || !( iohttp->content = malloc( 1024+iohttp->content_length ) ) )
    {
      iohttp->flags |= 4;
      cnt->flags |= SV_FLAGS_NEED_WRITE;
      iohttp->code = 200;
      return;
    }
    a = cnt->recv_max - content_pos;
    memcpy( iohttp->content, content, a );
    cnt->recv = iohttp->content;
    cnt->recv_pos = a;
    cnt->recv_max = 1024+iohttp->content_length;
    iohttp->flags |= 2;
  }
  return;
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


int iohttpParseHeader( svConnectionPtr cnt, iohttpDataPtr iohttp, char *cmd )
{
  int a;
  char *cmd2;
  iohttpFilePtr file;

  if( !( iohttp->code ) )
  {
    if( ioCompareWords( cmd, "GET " ) )
    {
      cmd += 4;
      iohttp->method = 1;
    }
    else if( ioCompareWords( cmd, "HEAD " ) )
    {
      cmd += 5;
      iohttp->method = 2;
    }
    else if( ioCompareWords( cmd, "POST " ) )
    {
      cmd += 5;
      iohttp->method = 3;
    }
    else
    {
      iohttp->code = 501;
      return 1;
    }

    for( a = 0 ; ( cmd[a] != ' ' ) && ( cmd[a] ) ; a++ );
    cmd[a] = 0;
    if( cmd[0] != '/' )
    {
      iohttp->code = 400;
      return 1;
    }
    for( a = 0 ; *cmd ; a++, cmd++ )
    {
      if( a == SERVER_PATH_BUFSIZE-1 )
      {
        iohttp->code = 400;
        iohttp->path[a] = 0;
        return 1;
      }
      else if( ioCompareWords( cmd, ".." ) )
      {
        iohttp->code = 400;
        iohttp->path[a] = 0;
        return 1;
      }
      else if( *cmd == '?' )
      {
        // FIXME: query string needs %<hex> etc, too!!
        iohttp->query_string = &cmd[1];
        break;
      }
      else if( *cmd == '%' )
      {
        if( !( cmd[1] ) || !( cmd[2] ) )
          break;
        if( !( iohttp->path[a] =  iohttpInputHex( &cmd[1] ) ) )
          break;
        cmd += 2;
      }
      else
        iohttp->path[a] = *cmd;
    }
    iohttp->path[a] = 0;
    for( file = iohttpFileList ; file ; file = file->next )
    {
      if( !( ioCompareExact( file->path, iohttp->path ) ) )
        continue;
      iohttp->code = 200;
      iohttp->file = file;
      return 0;
    }
    iohttp->code = 404;
  }
  else if( ( cmd2 = ioCompareWords( cmd, "Content-length: " ) ) || ( cmd2 = ioCompareWords( cmd, "Content-Length: " ) ) )
  {
    if( sscanf( cmd2, "%d", &a ) <= 0 )
      return 0;
    iohttp->content_length = a;
  }
  else if( ( cmd2 = ioCompareWords( cmd, "Content-type: " ) ) || ( cmd2 = ioCompareWords( cmd, "Content-Type: " ) ) )
    iohttp->content_type = cmd2;
  else if( ( cmd2 = ioCompareWords( cmd, "Cookie: " ) ) )
    iohttp->cookie = cmd2;
  else if( ( cmd2 = ioCompareWords( cmd, "Accept: " ) ) )
    iohttp->accept = cmd2;
  else if( ( cmd2 = ioCompareWords( cmd, "Accept-Language: " ) ) )
    iohttp->accept_language = cmd2;
  else if( ( cmd2 = ioCompareWords( cmd, "Host: " ) ) )
    iohttp->host = cmd2;
  else if( ( cmd2 = ioCompareWords( cmd, "Referer: " ) ) )
    iohttp->referer = cmd2;
  else if( ( cmd2 = ioCompareWords( cmd, "User-Agent: " ) ) )
    iohttp->user_agent = cmd2;

#if SERVER_REPORT_IGNOREDHEADER == 1
  else
    printf( "Ignored http header : %s\n", cmd );
#endif

  return 0;
}




void TickStartHTTP()
{
  return;
}


void TickEndHTTP()
{
  return;
}






