#define IOHTTP_FORUM_THREADSNUM (40)

#define IOHTTP_FORUM_POSTSNUM (40)
#define IOHTTP_FORUM_SMILEBASE (8)

static int IOHTTP_FORUM_SMILETOTAL;

typedef struct
{
 char string[16];
 char file[PATH_MAX];
} iohttpForumSmileysDef;

typedef struct forum_smileys{
 int id;
 char string[128];
 char name[PATH_MAX];
 struct forum_smileys *next;
} ForumSmileysDef, *ForumSmileysPtr;

static ForumSmileysPtr SmileList;
static ForumSmileysPtr *SmileTable;
 
/* qsort C-string comparison function */ 
int cstring_cmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}

int LoadForumList( ) {
	ForumSmileysPtr LoadList;
	ConfigArrayPtr setting;
	char dname[PATH_MAX];
	char fname[PATH_MAX];
	struct stat sbuf;
	struct dirent *de;
	DIR *dir;
	int a, size;

setting = GetSetting( "HTTP Images" );
snprintf(dname, PATH_MAX, "%s/smilies", setting->string_value);

if (NULL == (dir = opendir (dname)))
	return NO;
IOHTTP_FORUM_SMILETOTAL = 0;
while (NULL != (de = readdir (dir))) {
	if ('.' == de->d_name[0])
		continue;
	if( sizeof (fname) <= snprintf (fname, sizeof (fname), "%s/%s", dname, de->d_name) )
		continue;
	if (0 != stat (fname, &sbuf))
		continue;
	if (! S_ISREG (sbuf.st_mode))
		continue;
	if( NULL == ( LoadList = calloc( 1, sizeof(ForumSmileysDef) ) ) ) {
		critical( "memory allocation error!" );
		return NO;
	}
	LoadList->id = IOHTTP_FORUM_SMILETOTAL;
	char *pointer = strrchr( de->d_name, '.' );
	if( pointer ) {
		size = (int)fmin( sizeof(LoadList->string), 2+((strlen(de->d_name) - strlen(pointer))+1) );
		snprintf( LoadList->string, size, "::%s", de->d_name );
	} else {
		snprintf( LoadList->string, sizeof(LoadList->string), "::%s", de->d_name );
	}
	snprintf( LoadList->name, PATH_MAX, "%s", de->d_name );
	LoadList->next = SmileList;
	SmileList = LoadList;
	IOHTTP_FORUM_SMILETOTAL++;
}

char *value[IOHTTP_FORUM_SMILETOTAL];
for( a = 0, LoadList = SmileList; LoadList; LoadList = LoadList->next, a++ ) {
	value[a] = LoadList->string;
}

size_t strings_len = sizeof(value) / sizeof(char *);
qsort(value, strings_len, sizeof(char *), cstring_cmp);

if( NULL == ( SmileTable = malloc( IOHTTP_FORUM_SMILETOTAL*sizeof(ForumSmileysDef) ) ) ) {
	critical( "memory allocation error!" );
	return NO;
}

for( a = 0; a < IOHTTP_FORUM_SMILETOTAL; a++ ) {
	for( LoadList = SmileList; LoadList; LoadList = LoadList->next ) {
		if( strcmp( LoadList->string, value[a] ) == 0 ) {
			SmileTable[a] = LoadList;
		}
	}
}
(void)closedir( dir );


return YES;
}

int UnLoadForumList( ) {
	ForumSmileysPtr FreeList;
	
for( FreeList = SmileList; FreeList; FreeList = FreeList->next  ) {
	free( FreeList );
}

free( SmileTable );

FreeList = NULL;

return YES;
}

iohttpForumSmileysDef iohttpForumSmileys[IOHTTP_FORUM_SMILEBASE] =
{
{ ":)", "smile.gif" },
{ ":D", "smile2.gif" },
{ ";)", "wink.gif" },
{ "x(", "angry.gif" },
{ "X(", "angry.gif" },
{ ":(", "sad.gif" },
{ ":p", "tongue.gif" },
{ ":P", "tongue.gif" },
};


int iohttpForumFilter( char *dest, char *string, int size, int html )
{
 int a, b;
 char c;
 for( a = b = 0 ; string[a] ; a++ )
 {
  if( b >= size-1 )
   break;
  c = string[a];
  /*if( c == '+' )
   c = ' ';
  else if( c == '%' )
  {
   if( !( string[a+1] ) || !( string[a+2] ) )
    break;
   c = iohttpInputHex( &string[a+1] );
   a += 2;
  }*/
  if( c == 13 )
   continue;
  else if( ( ( c < 32 ) && ( c != 10 ) ) )
   dest[b++] = ' ';
  else if( !( html ) && ( ( c == '<' ) || ( c == '>' ) ) )
   dest[b++] = ' ';
  else
   dest[b++] = c;
 }
 dest[b] = 0;
 return b;
}

int iohttpForumFilter2( char *dest, char *string, int size ) {
	int a, b, c;
	char *string2;

for( b = c = 0 ; *string ; ) {
	if( b >= size-20 )
		break;
	if( ( string2 = ioCompareWords( string, "&&**##" ) ) ) {
		string = string2;
		c ^= 1;
	}
	if( !( c ) ) {
		if( string[0] == ':' ) {
			for( a = (IOHTTP_FORUM_SMILETOTAL-1); a > -1; a-- ) {
				if( !( string2 = ioCompareWords( string, SmileTable[a]->string ) ) )
					continue;
				string = string2;
				b += sprintf( &dest[b], "<img src=\"files?type=image&name=smilies/%s\">", SmileTable[a]->name );
				goto iohttpForumFilter2L0;
			}
		}
		for( a = 0 ; a < IOHTTP_FORUM_SMILEBASE ; a++ ) {
			if( !( string2 = ioCompareWords( string, iohttpForumSmileys[a].string ) ) )
				continue;
			string = string2;
			b += sprintf( &dest[b], "<img src=\"files?type=image&name=smilies/%s\">", iohttpForumSmileys[a].file );
			goto iohttpForumFilter2L0;
		}
	}
	if( *string == 10 ) {
		memcpy( &dest[b], "<br>", 4 );
		b += 4;
		string++;
		continue;
	}
	dest[b++] = *string;
	string++;
	iohttpForumFilter2L0:;
}
dest[b] = 0;

return b;
}

int iohttpForumFilter3( char *dest, char *string, int size ) {
	int a, b, c;
	char *string2, *string3;
	
for( b = c = 0 ; *string ; ) {
	dest[b] = 0;
	if( b >= size-16 )
		break;
	if( ( string2 = ioCompareWords( string, "<br><br><font size=\"1\"><i>Edited by" ) ) )
		break;
	if( ( string2 = ioCompareWords( string, "<br>" ) ) ) {
		dest[b++] = 10;
		string = string2;
		continue;
	}
	if( !( string2 = ioCompareWords( string, "<img src=\"files?type=image&name=smilies/" ) ) ) {
		goto iohttpForumFilter3L0;
	}
	if( !( string2[0] ) || !( string2[1] ) ) {
		goto iohttpForumFilter3L0;
	}
	
	for( a = (IOHTTP_FORUM_SMILETOTAL-1); a > -1; a-- ) {
		if( ( string3 = ioCompareWords( string2, &SmileTable[a]->string[2] ) ) )
			goto iohttpForumFilter3L1;
	}
	goto iohttpForumFilter3L0;
	
	iohttpForumFilter3L1:
	if( !( string2 = ioCompareWords( string3, ".gif\">" ) ) ) {
		goto iohttpForumFilter3L0;
	}
	if( SmileTable[a]->id >= IOHTTP_FORUM_SMILETOTAL )
		goto iohttpForumFilter3L0;
	b += sprintf( &dest[b], "%s", SmileTable[a]->string );
	string = string2;
	continue;
	iohttpForumFilter3L0:
	dest[b++] = *string;
	string++;
}
dest[b] = 0;

return b;
}

/*
perms = 0xFFF
Accessible for all

perms = 2
Empire forum

perms = 0
Accessible to user level >= mod

*/

/*
int iohttpForumPerms( int id, int forum, ReplyDataPtr cnt, dbUserMainPtr maind, int perms )
{
	if( perms >= 4 )
	{
  return 1;
 }
 else if((forum < 100) && ((cnt->session)->dbuser) )
 {
 	if(id == -1)
			return 0;
		if((((cnt->session)->dbuser)->level >= LEVEL_MODERATOR)||(((cnt->session)->dbuser)->level >= LEVEL_FORUMMOD))
 		return 1;
 	else
 		return 0;
 }
 else if( perms >= 2 )
 {
 	if(id == -1)
			return 0;
 	if(( id == -1 ) || (maind == 0))
   return 0;
  if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR )
  {
  	return 1;
  }

  if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_INDEPENDENT ) )
   return 0;
  if( maind->empire+100 == forum )
   return 1;
  if( cmdExecFindRelation( maind->empire, forum-100, 0, 0 ) == CMD_RELATION_ALLY )
   return 1;
  return 0;
 }
 else if( perms >= 1 )
 {
 	if( id == -1 )
   return 0;
 }
 else
 {
 	if( id == -1 )
   return 0;
  if( ( forum >= 100 ) && ( maind->empire == forum-100 ) && ( bitflag( ((cnt->session)->dbuser)->flags, ( CMD_USER_FLAGS_LEADER | CMD_USER_FLAGS_COMMINISTER ) ) ) )
   return 1;
  if( ((cnt->session)->dbuser)->level < LEVEL_MODERATOR )
   return 0;
 }
 return 1;
}*/


int iohttpForumPerms( int flags, int id, int forum, ReplyDataPtr cnt, dbUserMainPtr maind, int perms )
{
  if( perms >= 4 )
    return 1;
  else if( perms >= 2 )
  {
    if( id == -1 )
      return 0;
    if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR )
      return 1;
    if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_INDEPENDENT ) )
      return 0;
    if( ( flags ) && ( maind->empire == forum ) )
      return 1;
    if( cmdExecFindRelation( maind->empire, forum, 0, 0 ) == CMD_RELATION_ALLY )
      return 1;
    return 0;
  }
  else if( perms >= 1 )
  {
    if( id == -1 )
      return 0;
  }
  else
  {
    if( id == -1 )
      return 0;
    if( ( flags ) && ( maind->empire == forum ) && ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_LEADER ) || bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_COMMINISTER ) ) )
      return 1;
    if( ((cnt->session)->dbuser)->level < LEVEL_MODERATOR )
      return 0;
  }
  return 1;
}





int iohttpForumCleanAuthor( int flags, char *string )
{
 int a, b, fnum, tnum, count = 0;
 char word[512];
 dbForumForumPtr forums;
 dbForumForumDef forumd;
 dbForumThreadPtr threads;
 fnum = dbForumListForums( flags, 0, &forums );
 if( fnum < 0 ) return -1;
 iohttpForumFilter( word, string, 512, 0 );
 for( a = 0 ; a < fnum ; a++ )
 {
  tnum = dbForumListThreads( flags, a, 0, 0xFFFF, &forumd, &threads );
  if( tnum < 0 )
   continue;
  for( b = tnum-1 ; b >= 0 ; b-- )
  {
   if( !( threads[b].authorname[0] ) && !( threads[b].topic[0] ) )
   {
    dbForumRemoveThread( flags, a, threads[b].id );
    count++;
   }
   else if( ioCompareFindWords( threads[b].authorname, word ) )
   {
    dbForumRemoveThread( flags, a, threads[b].id );
    count++;
   }
  }
 }
 return count;
}


int iohttpForumCleanIP( int flags, char *ipstring )
{
 int a, b, fnum, tnum, count = 0;
 dbForumForumPtr forums;
 dbForumForumDef forumd;
 dbForumThreadPtr threads;
 fnum = dbForumListForums( flags, 0, &forums );
 if( fnum < 0 )
  return -1;
 for( a = 0 ; a < fnum ; a++ )
 {
  tnum = dbForumListThreads( flags, a, 0, 0xFFFF, &forumd, &threads );
  if( tnum < 0 )
   continue;
  for( b = tnum-1 ; b >= 0 ; b-- )
  {
   if( ioCompareExact( inet_ntoa( threads[b].sin_addr ), ipstring ) )
   {
    dbForumRemoveThread( flags, a, threads[b].id );
    count++;
   }
  }
 }
 return count;
}



void iohtmlFunc_forum( ReplyDataPtr cnt )
{
 int a, b, c, d, id, forum, thread, post, action, skip;
 bool flags, badpost = true;
 dbUserMainDef maind;
 char *forumstring, *threadstring, *topicstring, *poststring, *delthreadstring, *delpoststring, *editpoststring, *namestring, *skipstring, *empirestring, *capstring;
 dbForumForumPtr forums;
 dbForumThreadPtr threads;
 dbForumPostPtr posts;
 dbForumForumDef forumd;
 dbForumThreadDef threadd;
 dbForumPostDef postd;
 ConfigArrayPtr settings;
 char timebuf[512];
 char COREDIR[PATH_MAX];
 char timetemp[PATH_MAX];
 char *text = NULL;
 time_t tint;
 FILE *fFile;

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	iohtmlBase( cnt, 1 );
	if( !( iohtmlHeader( cnt, id, &maind ) ) )
		return;
} else {
	iohtmlBase( cnt, 8 );
	iohtmlFunc_frontmenu( cnt, FMENU_FORUM );
	httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td valign=\"top\">" );
}

 capstring = iohtmlVarsFind( cnt, "captcha" );
 empirestring = iohtmlVarsFind( cnt, "empire" );
 forumstring = iohtmlVarsFind( cnt, "forum" );
 threadstring = iohtmlVarsFind( cnt, "thread" );
 topicstring = iohtmlVarsFind( cnt, "topic" );
 poststring = iohtmlVarsFind( cnt, "post" );
 delthreadstring = iohtmlVarsFind( cnt, "delthread" );
 delpoststring = iohtmlVarsFind( cnt, "delpost" );
 editpoststring = iohtmlVarsFind( cnt, "editpost" );
 namestring = iohtmlVarsFind( cnt, "name" );
 skipstring = iohtmlVarsFind( cnt, "skip" );

if( ( empirestring ) && ( strcmp( empirestring, "true" ) == 0 ) ) {
	flags = true;
} else {
	flags = false;
}
action = 0;
if( ( ( forumstring ) && ( sscanf( forumstring, "%d", &forum ) == 1 ) ) ) {
	action = 1;
  if( ( threadstring ) && ( sscanf( threadstring, "%d", &thread ) == 1 ) )
  {
   if( delthreadstring )
    action = 5;
   else if( ( delpoststring ) && ( sscanf( delpoststring, "%d", &post ) == 1 ) )
    action = 6;
   else if( poststring )
   {
    if( ( editpoststring ) && ( sscanf( editpoststring, "%d", &post ) == 1 ) )
     action = 8;
    else
     action = 4;
   }
   else if( ( editpoststring ) && ( sscanf( editpoststring, "%d", &post ) == 1 ) )
    action = 7;
   else
    action = 2;
  }
  else if( ( topicstring ) && ( poststring ) )
  {
  	a = strlen(topicstring);
   if(a)
   	action = 3;
  }
 }

 skip = 0;
 if( skipstring )
  sscanf( skipstring, "%d", &skip );

 if( action == 0 )
 {
 if( ( flags ) && ( !( (cnt->session)->dbuser ) || !( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) ) ) {
   httpString( cnt, "You are not authorised to view this list." );
   goto RETURN;
 }
 
  b = dbForumListForums( flags, 0, &forums );
  if( b < 0 )
  {
   httpString( cnt, "Error while retrieving list of forums" );
   goto RETURN;
  }
  time( &tint );
  strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &tint ) );
  settings = GetSetting( "Server Name" );
  httpPrintf( cnt, "<table cellspacing=\"4\" width=\"%d%%\">", ( id == -1 ) ? 100 : 80 );
  httpPrintf( cnt, "<tr><td><a href=\"%s\" target=\"_top\">%s</a> - %s public forums</td><td align=\"right\">%s", URLAppend( cnt, "/" ), settings->string_value, settings->string_value, timebuf );
if( ( id != -1 ) && ( ( flags == false ) || ( ( flags ) && ( forum != maind.empire ) ) ) && ( maind.empire != -1 ) ) {
	if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) {
		httpPrintf( cnt, " - <a href=\"%s&amp;empire=true\">Empire forums</a>", URLAppend( cnt, "forum" ) );
	} else {
		httpPrintf( cnt, " - <a href=\"%s&amp;empire=true&amp;forum=%d\">Empire forum</a>", URLAppend( cnt, "forum" ), maind.empire );
	}
}
  httpString( cnt, "</td></tr></table>" );

  httpPrintf( cnt, "<table width=\"%d%%\" cellpadding=\"3\" cellspacing=\"3\" bgcolor=\"#000000\">", ( id == -1 ) ? 100 : 80 );
  httpPrintf( cnt, "<tr bgcolor=\"#333333\"><td width=\"70%\">Forums</td><td width=\"10%\">Threads</td><td width=\"20%\">Last post</td></tr>" );
  for( a = 0 ; a < b ; a++ )
  {
  strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &forums[a].time ) );
   httpPrintf( cnt, "<tr bgcolor=\"#111111\"><td><a href=\"%s%s&amp;forum=%d&last=%d\">%s</a></td><td>%d</td><td nowrap>", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), a, forums[a].time, forums[a].title, forums[a].threads );
   httpPrintf( cnt, "%s<br>Week %d, Year %d", timebuf, forums[a].tick % 52, forums[a].tick / 52 );
	if( forums[a].lastid != -1 ) {
		httpPrintf( cnt, "<br>By <a href=\"%s&amp;id=%d\">%s</a>", URLAppend( cnt, "player" ), forums[a].lastid, forums[a].lastpost );
	}
  httpString( cnt, "</td></tr>" );
   }
  httpString( cnt, "</table>" );
if( forums )
  free( forums );
 }
 else if( action == 1 )
 {
  iohttpForumL2:

  b = dbForumListThreads( flags, forum, skip, skip+IOHTTP_FORUM_THREADSNUM, &forumd, &threads );
  if( b < 0 )
  {
   httpString( cnt, "These threads do not exist!" );
   goto RETURN;
  }
  if( !( iohttpForumPerms( flags, id, forum, cnt, &maind, forumd.rperms ) ) )
  {
   if( threads )
    free( threads );
   httpString( cnt, "You are not authorized to view this forum" );
   goto RETURN;
  }
  if((cnt->session)->dbuser)
  {
	  if((cnt->session)->dbuser->level >= LEVEL_MODERATOR)
	  {
	  	if( flags )
	  	{
	  		settings = GetSetting( "Directory" );
			sprintf( COREDIR, "%s/logs/modlog.log", settings->string_value );
	  		if( maind.empire != forum ) {
	  			fFile = fopen( COREDIR, "a+t" );
		  		if( fFile ) {
		 				fprintf( fFile, "%s > view forum of empire %d\n", maind.faction, forum);
		 				fclose(fFile);
		 		}
		 	}
		 	}
	  	}
	 }
	 	settings = GetSetting( "Server Name" );
		httpPrintf( cnt, "<table cellspacing=\"4\" width=\"%d%%\">", ( id == -1 ) ? 100 : 80 );
		httpPrintf( cnt, "<tr><td><a href=\"%s\" target=\"_top\">%s</a> - ", URLAppend( cnt, "/" ), settings->string_value );
		httpPrintf( cnt, "<a href=\"%s\">%s public forums</a> - %s</td><td align=\"right\">", URLAppend( cnt, "forum" ), settings->string_value, forumd.title );
if( flags ) {
	time( &tint );
	strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &tint ) );
	httpString( cnt, timebuf );
	if( ( id != -1 ) && ( ( flags == false ) || ( ( flags ) && ( forum != maind.empire ) ) ) && ( maind.empire != -1 ) ) {
		if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) {
			httpPrintf( cnt, " - <a href=\"%s&amp;empire=true\">Empire forums</a>", URLAppend( cnt, "forum" ) );
		} else {
			httpPrintf( cnt, " - <a href=\"%s&amp;empire=true&amp;forum=%d\">Empire forum</a>", URLAppend( cnt, "forum" ), maind.empire );
		}
	}
	httpString( cnt, "</td></tr></table>" );
} else
	httpPrintf( cnt, "Week %d, Year %d</td></tr></table>", ticks.number % 52, ticks.number / 52 );

  if( forumd.threads > IOHTTP_FORUM_THREADSNUM )
  {
   httpString( cnt, "Pages : " );
   for( a = 0, c = 1 ; a < forumd.threads ; a += IOHTTP_FORUM_THREADSNUM, c++ )
   {
    if( skip != a )
     httpPrintf( cnt, "<a href=\"%s&%s=%d&skip=%d\">%d</a> ", URLAppend( cnt, "forum" ), ( flags ? "empire" : "forum" ), forum, a, c );
    else
     httpPrintf( cnt, "<b>%d</b> ", c );
   }
  }

  httpPrintf( cnt, "<table width=\"%d%%\" cellpadding=\"3\" cellspacing=\"3\" bgcolor=\"#000000\">", ( id == -1 ) ? 100 : 80 );
  httpString( cnt, "<tr bgcolor=\"#333333\"><td width=\"60%\">Topic</td><td width=\"10%\">Posts</td><td width=\"15%\">Author</td><td width=\"15%\">Last post</td></tr>" );
for( a = 0 ; a < b ; a++ ) {
	strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &threads[a].time ) );
	sprintf(timetemp, "<br>Week %d, Year %d", threads[a].tick % 52, threads[a].tick / 52 );
	strcat( timebuf, timetemp);

   httpPrintf( cnt, "<tr bgcolor=\"#111111\"><td><a href=\"%s%s&amp;forum=%d&thread=%d&last=%d\">%s</a>", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, threads[a].id, threads[a].time, threads[a].topic );
   if( threads[a].posts > IOHTTP_FORUM_POSTSNUM )
   {
    httpString( cnt, " - Pages : " );
    for( d = 0, c = 1 ; d < threads[a].posts ; d += IOHTTP_FORUM_POSTSNUM, c++ )
     httpPrintf( cnt, "<a href=\"%s%s&amp;forum=%d&thread=%d&skip=%d&last=%d\">%d</a> ", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, threads[a].id, d, threads[a].time, c );
   }
   httpPrintf( cnt, "</td><td>%d</td><td nowrap>%s</td><td nowrap>%s", threads[a].posts, threads[a].authorname, timebuf );
	if(( iohttpForumPerms( flags, id, forum, cnt, &maind, 0 ) )&&(id != -1)) {
   		httpPrintf( cnt, " <a href=\"%s%s&amp;forum=%d&thread=%d&delthread=1\">Delete</a>", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, threads[a].id );
  	}
	if( threads[a].lastid != -1  ) {
		httpPrintf( cnt, "<br>By <a href=\"%s&amp;id=%d\">%s</a>", URLAppend( cnt, "player" ), threads[a].lastid, threads[a].lastpost );
	}
   
   
 	if((cnt->session)->dbuser)
  	{
	   if(( id != -1 ) && (((((cnt->session)->dbuser)->level >= LEVEL_FORUMMOD))||(((cnt->session)->dbuser)->level >= LEVEL_FORUMMOD)))
	    httpPrintf( cnt, "<br>IP: %s", inet_ntoa( threads[a].sin_addr ) );
	  }
			httpString( cnt, "</td></tr>" );
  }
  httpString( cnt, "</table>" );
  if( forumd.threads > IOHTTP_FORUM_THREADSNUM )
  {
   httpString( cnt, "Pages : " );
   for( a = 0, c = 1 ; a < forumd.threads ; a += IOHTTP_FORUM_THREADSNUM, c++ )
   {
    if( skip != a )
     httpPrintf( cnt, "<a href=\"%s%s&amp;forum=%d&skip=%d\">%d</a> ", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, a, c );
    else
     httpPrintf( cnt, "<b>%d</b> ", c );
   }
  }

  if( iohttpForumPerms( flags, id, forum, cnt, &maind, forumd.wperms ) )
  {
   httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "forum" ) );
   httpPrintf( cnt, "<input type=\"hidden\" name=\"empire\" value=\"%s\">", ( flags ? "true" : "false" ) );
   httpPrintf( cnt, "<input type=\"hidden\" name=\"forum\" value=\"%d\">", forum );
   httpString( cnt, "<table cellspacing=\"3\"><tr><td>Name</td><td>" );
	if( id == -1 ) {
		purge_captcha( cnt->session );
  		httpPrintf( cnt, "<input type=\"text\" name=\"name\" size=\"32\" value=\"%s\">", ( namestring ) ? namestring : "" );
		unsigned char im[70*200];
		unsigned char gif[gifsize];

		captcha(im,(unsigned char *)(cnt->session)->captcha);
		makegif(im,gif);
		snprintf( COREDIR, PATH_MAX, "%s/%s.gif", TMPDIR, (cnt->session)->sid );
		FILE *file = fopen( COREDIR, "wb+" );
		fwrite( gif, 1, gifsize, file );
		fclose( file );
		httpString( cnt, "</td></tr><tr><td>" );
		httpString( cnt, "Captcha" );
		httpString( cnt, "</td><td>" );
		httpPrintf( cnt, "<table><tr><td><input type=\"text\" name=\"captcha\" size=\"7\"></td><td><img src=\"%s&type=captcha\"></td></tr></table>", URLAppend( cnt, "files" ) );
	} else {
    		httpPrintf( cnt, "%s<input type=\"hidden\" name=\"name\" value=\"%s\">", maind.faction, maind.faction );
    }
   if( badpost ) {
   	httpPrintf( cnt, "</td></tr><tr><td>Topic</td><td><input type=\"text\" name=\"topic\" size=\"32\" value=\"%s\"></td></tr>", ( topicstring ) ? topicstring : "" );
   	httpPrintf( cnt, "<tr><td>Post</td><td><textarea name=\"post\" wrap=\"soft\" rows=\"10\" cols=\"60\">%s</textarea></td></tr>", ( poststring ) ? poststring : ""  );
   } else {
   	httpString( cnt, "</td></tr><tr><td>Topic</td><td><input type=\"text\" name=\"topic\" size=\"32\"></td></tr>" );
   	httpString( cnt, "<tr><td>Post</td><td><textarea name=\"post\" wrap=\"soft\" rows=\"10\" cols=\"60\"></textarea></td></tr>" );
   }
   httpString( cnt, "<tr><td>&nbsp;</td><td><input type=\"submit\" value=\"Post\"></td></tr></table></form>" );
  }

  if( threads )
   free( threads );
 }
 else if( action == 2 )
 {
  iohttpForumL1:
  if( dbForumRetrieveForum( flags, forum, &forumd ) < 0 )
  {
   httpString( cnt, "This forum does not exist!" );
   goto RETURN;
  }
  b = dbForumListPosts( flags, forum, thread, skip, skip+IOHTTP_FORUM_POSTSNUM, &threadd, &posts );
  if( b < 0 )
  {
   httpString( cnt, "Error while retrieving list of posts" );
   goto RETURN;
  }

  if( !( iohttpForumPerms( flags, id, forum, cnt, &maind, forumd.rperms ) ) )
  {
   for( a = 0 ; a < b ; a++ )
   {
    if( posts[a].text )
     free( posts[a].text );
   }
   if( posts )
    free( posts );
   httpString( cnt, "You are not authorized to view this forum" );
   goto RETURN;
  }
settings = GetSetting( "Server Name" );
  httpPrintf( cnt, "<table cellspacing=\"4\" width=\"%d%%\">", ( id == -1 ) ? 100 : 80 );
  httpPrintf( cnt, "<tr><td><a href=\"%s\" target=\"_top\">%s</a> - ", URLAppend( cnt, "/" ), settings->string_value );
  httpPrintf( cnt, "<a href=\"%s\">%s public forums</a> - ", URLAppend( cnt, "forum" ), settings->string_value );
  httpPrintf( cnt, "<a href=\"%s%s&amp;forum=%d\">%s</a> - %s</td><td align=\"right\">", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, forumd.title, threadd.topic );
if( flags )  {
	time( &tint );
	strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &tint ) );
	httpString( cnt, timebuf );
	if( ( id != -1 ) && ( ( flags == false ) || ( ( flags ) && ( forum != maind.empire ) ) ) && ( maind.empire != -1 ) ) {
		if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) {
			httpPrintf( cnt, " - <a href=\"%s&amp;empire=true\">Empire forums</a>", URLAppend( cnt, "forum" ) );
		} else {
			httpPrintf( cnt, " - <a href=\"%s&amp;empire=true&amp;forum=%d\">Empire forum</a>", URLAppend( cnt, "forum" ), maind.empire );
		}
	}
	httpString( cnt, "</td></tr></table>" );
} else
	httpPrintf( cnt, "Week %d, Year %d</td></tr></table>", ticks.number % 52, ticks.number / 52 );

  if( threadd.posts > IOHTTP_FORUM_POSTSNUM )
  {
   httpString( cnt, "Pages : " );
   for( a = 0, c = 1 ; a < threadd.posts ; a += IOHTTP_FORUM_POSTSNUM, c++ )
   {
    if( skip != a )
     httpPrintf( cnt, "<a href=\"%s%s&amp;forum=%d&thread=%d&skip=%d\">%d</a> ", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, thread, a, c );
    else
     httpPrintf( cnt, "<b>%d</b> ", c );
   }
  }

  httpPrintf( cnt, "<table width=\"%d%%\" cellpadding=\"3\" cellspacing=\"3\" bgcolor=\"#000000\">", ( id == -1 ) ? 100 : 80 );
  for( a = 0 ; a < b ; a++ )
  {
   if( !( posts[a].text ) )
    continue;
   c = a + skip;
   if( flags ) {
	strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &posts[a].post.time ) );
      httpPrintf( cnt, "<tr><td valign=\"top\" width=\"10%%\" nowrap bgcolor=\"#282828\"><b>%s</b><br><i>%s</i><br>%s", posts[a].post.authorname, posts[a].post.authortag, timebuf );
   } else {
   httpPrintf( cnt, "<tr><td valign=\"top\" width=\"10%%\" nowrap bgcolor=\"#282828\"><b>%s</b><br><i>%s</i><br>Week %d, Year %d", posts[a].post.authorname, posts[a].post.authortag, (posts[a].post).tick % 52, (posts[a].post).tick / 52 );
   }
   if( iohttpForumPerms( flags, id, forum, cnt, &maind, 0 ) || ( ( id != -1 ) && ( posts[a].post.authorid == id ) && ioCompareExact( posts[a].post.authorname, maind.faction ) )) {
    	httpPrintf( cnt, "<br><a href=\"%s%s&amp;forum=%d&thread=%d&editpost=%d\">Edit</a> - ", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, thread, c );
    	httpPrintf( cnt, "<a href=\"%s%s&amp;forum=%d&thread=%d&delpost=%d\">Delete</a>", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, thread, c );
    }
   if((cnt->session)->dbuser)
   {
	   if( ( id != -1 ) && ( ((cnt->session)->dbuser)->level >= LEVEL_FORUMMOD ) )
	   {
	    httpPrintf( cnt, "<br>Account: <a href=\"%s&amp;id=%d\">%d</a>", URLAppend( cnt, "player" ), posts[a].post.authorid, posts[a].post.authorid );
	    httpPrintf( cnt, "<br>IP: %s", inet_ntoa( posts[a].post.sin_addr ) );
	   }
	  }
   httpString( cnt, "</td><td valign=\"top\" width=\"90%\" bgcolor=\"#181818\">" );
   httpString( cnt, posts[a].text );
   httpString( cnt, "</td></tr>" );
   if( posts[a].text )
    free( posts[a].text );
  }
  httpString( cnt, "</table>" );

  if( threadd.posts > IOHTTP_FORUM_POSTSNUM )
  {
   httpString( cnt, "Pages : " );
   for( a = 0, c = 1 ; a < threadd.posts ; a += IOHTTP_FORUM_POSTSNUM, c++ )
   {
    if( skip != a )
     httpPrintf( cnt, "<a href=\"%s%s&amp;forum=%d&thread=%d&skip=%d\">%d</a> ", URLAppend( cnt, "forum" ), ( flags ? "&amp;empire=true" : "" ), forum, thread, a, c );
    else
     httpPrintf( cnt, "<b>%d</b> ", c );
   }
  }

  if( iohttpForumPerms( flags, id, forum, cnt, &maind, forumd.wperms ) )
  {
   httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "forum" ) );
   httpPrintf( cnt, "<input type=\"hidden\" name=\"empire\" value=\"%s\">", ( flags ? "true" : "false" ) );
   httpPrintf( cnt, "<input type=\"hidden\" name=\"forum\" value=\"%d\">", forum );
   httpPrintf( cnt, "<input type=\"hidden\" name=\"thread\" value=\"%d\">", thread );
   httpString( cnt, "<table cellspacing=\"3\"><tr><td>Name</td><td>" );
	if( id == -1 ) {
		purge_captcha( cnt->session );
  		httpPrintf( cnt, "<input type=\"text\" name=\"name\" size=\"32\" value=\"%s\">", ( namestring ) ? namestring : "" );
		unsigned char im[70*200];
		unsigned char gif[gifsize];

		captcha(im,(unsigned char *)(cnt->session)->captcha);
		makegif(im,gif);
		snprintf( COREDIR, PATH_MAX, "%s/%s.gif", TMPDIR, (cnt->session)->sid );
		FILE *file = fopen( COREDIR, "wb+" );
		fwrite( gif, 1, gifsize, file );
		fclose( file );
		httpString( cnt, "</td></tr><tr><td>" );
		httpString( cnt, "Captcha" );
		httpString( cnt, "</td><td>" );
		httpPrintf( cnt, "<table><tr><td><input type=\"text\" name=\"captcha\" size=\"7\"></td><td><img src=\"%s&type=captcha\"></td></tr></table>", URLAppend( cnt, "files" ) );
	} else {
		httpPrintf( cnt, "%s<input type=\"hidden\" name=\"name\" value=\"%s\">", maind.faction, maind.faction );
		httpString( cnt, "</td></tr>" );
	}
      if( badpost ) {
   	httpPrintf( cnt, "<tr><td>Post</td><td><textarea name=\"post\" wrap=\"soft\" rows=\"10\" cols=\"60\">%s</textarea></td></tr>", ( poststring ) ? poststring : ""  );
   } else {
   	httpString( cnt, "<tr><td>Post</td><td><textarea name=\"post\" wrap=\"soft\" rows=\"10\" cols=\"60\"></textarea></td></tr>" );
   }
   httpString( cnt, "<tr><td>&nbsp;</td><td><input type=\"submit\" value=\"Post\"></td></tr>" );
   httpString( cnt, "</table></form>" );
  }

  if( posts )
   free( posts );
 }
 else if( action == 3 )
 {
  if( dbForumRetrieveForum( flags, forum, &forumd ) < 0 )
  {
   httpString( cnt, "This forum does not exist!" );
   goto RETURN;
  }
  if( !( iohttpForumPerms( flags, id, forum, cnt, &maind, forumd.wperms ) ) )
  {
   httpString( cnt, "You are not authorized to post in this forum" );
   goto RETURN;
  }
  iohttpForumFilter( threadd.topic, topicstring, DB_FORUM_NAME_SIZE, 0 );
  threadd.posts = 0;
  threadd.authorid = id;
	if( id != -1 ) {
		sprintf( threadd.authorname, "%s", maind.faction );
	} else {
		if( ( namestring ) && ( strlen( namestring ) ) ) {
			iohttpForumFilter( threadd.authorname, namestring, USER_NAME_MAX, 0 );
		} else {
			sprintf( threadd.authorname, "Anonymous" );
		}
		if( ( capstring == NULL ) || ( ( capstring != NULL ) && ( strlen(capstring) == 0 ) ) ) {
			httpString( cnt, "<span class=\"center\"><b>No Captcha Entered</b></span><br><br>" );
   			goto iohttpForumL2;
		} else if( strcmp( (cnt->session)->captcha, capstring ) ) {
			httpString( cnt, "<span class=\"center\"><b>Invalid Captcha Entered</b></span></br><br>" );
			goto iohttpForumL2;
		}
	}
  threadd.time = time( 0 );
  threadd.tick = ticks.number;
  threadd.flags = 0;
  memcpy( &(threadd.sin_addr), &( ((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr ), sizeof(struct in_addr) );
  thread = dbForumAddThread( flags, forum, &threadd );
  badpost = false;
  goto iohttpForumL0;
 }
 else if( action == 4 )
 {
  if( dbForumRetrieveForum( flags, forum, &forumd ) < 0 )
  {
   httpString( cnt, "This forum does not exist!" );
   goto RETURN;
  }
  if( !( iohttpForumPerms( flags, id, forum, cnt, &maind, forumd.wperms ) ) )
  {
   httpString( cnt, "You are not authorized to post in this forum" );
   goto RETURN;
  }

	/*	b = dbForumListThreads( forum, thread, thread+1, &forumd, &threads );

  if(b)
  {
  	if(threads[thread].flags == DB_FORUM_FLAGS_THREAD_LOCK)
  	{
  		httpString( cnt, "Sorry this topic is <b>lock</b> and nothing can happen here" );
   	return;
  	}
  }*/
if( id < 0 ) {
	if( ( capstring == NULL ) || ( ( capstring != NULL ) && ( strlen(capstring) == 0 ) ) ) {
		httpString( cnt, "<span class=\"center\"><b>No Captcha Entered</b></span><br><br>" );
		goto iohttpForumL1;
	} else if( strcmp( (cnt->session)->captcha, capstring ) ) {
		httpString( cnt, "<span class=\"center\"><b>Invalid Captcha Entered</b></span></br><br>" );
		goto iohttpForumL1;
	}
}
  iohttpForumL0:

	if( id != -1 ) {
		sprintf( postd.post.authorname, "%s", maind.faction );
		sprintf( postd.post.authortag, "%s", ((cnt->session)->dbuser)->forumtag );
	} else {
		postd.post.authortag[0] = 0;
		if( ( namestring ) && ( strlen( namestring ) ) ) {
			iohttpForumFilter( postd.post.authorname, namestring, USER_NAME_MAX, 0 );
		} else {
			sprintf( postd.post.authorname, "Anonymous" );
		}
	}
  if( !( postd.text = malloc( 3 * FORUM_MAX ) ) )
  {
   goto RETURN;
  }
  a = 0;
  if((cnt->session)->dbuser)
  {
	  if( ( id != -1 ) && ( ((cnt->session)->dbuser)->level >= LEVEL_FORUMMOD ) )
	   a = 1;
	 }
  iohttpForumFilter( &postd.text[2*FORUM_MAX], poststring, FORUM_MAX, a );
  postd.post.length = iohttpForumFilter2( postd.text, &postd.text[2*FORUM_MAX], FORUM_MAX );
  postd.post.authorid = id;
  postd.post.time = time( 0 );
  postd.post.tick = ticks.number;
  postd.post.flags = 0;
  memcpy( &(postd.post.sin_addr), &( ((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr ), sizeof(struct in_addr) );
  a = dbForumAddPost( flags, forum, thread, &postd );
  if( a >= 0 )
   httpPrintf( cnt, "Post added!<br><br>" );
  else
   httpPrintf( cnt, "Error while adding post<br><br>" );
   if( postd.text )
  free( postd.text );
  badpost = false;
  goto iohttpForumL1;
 }
 else if( action == 5 )
 {
  if(( !( iohttpForumPerms( flags, id, forum, cnt, &maind, 0 ) ) ) || (id == -1))
  {
   httpString( cnt, "You are not authorized to delete threads here" );
   goto RETURN;
  }
  iohttpForumL3:

  if( dbForumRemoveThread( flags, forum, thread ) >= 0 )
   httpPrintf( cnt, "Thread removed<br><br>" );
  else
   httpPrintf( cnt, "Error while removing thread<br><br>" );
  goto iohttpForumL2;
 }
 else if( action == 6 )
 {
  b = dbForumListPosts( flags, forum, thread, post, post+1, &threadd, &posts );
  if( b < 0 )
  {
   httpString( cnt, "This post doesn't exist" );
   goto RETURN;
  }
  if( !( iohttpForumPerms( flags, id, forum, cnt, &maind, 0 ) ) && ( ( id == -1 ) || ( posts[0].post.authorid != id ) || !( ioCompareExact( posts[0].post.authorname, maind.faction ) ) ) )
  {
   httpString( cnt, "You are not authorized to delete this post" );
   if( posts[0].text )
    free( posts[0].text );
    if( posts )
   free( posts );
   goto RETURN;
  }
  if( posts[0].text )
   free( posts[0].text );
   if( posts )
  free( posts );
  a = dbForumRemovePost( flags, forum, thread, post );
  if( a >= 0 )
  {
   if( !( a ) )
    goto iohttpForumL3;
   httpPrintf( cnt, "Post removed<br><br>" );
  }
  else
   httpPrintf( cnt, "Error while removing post<br><br>" );
  goto iohttpForumL1;
 }
 else if( action == 7 )
 {
  b = dbForumListPosts( flags, forum, thread, post, post+1, &threadd, &posts );
  if( b < 0 )
  {
   httpString( cnt, "This post doesn't exist" );
   goto RETURN;
  }
  if( !( iohttpForumPerms( flags, id, forum, cnt, &maind, 0 ) ) && ( ( id == -1 ) || ( posts[0].post.authorid != id ) || !( ioCompareExact( posts[0].post.authorname, maind.faction ) ) ) )
  {
   httpString( cnt, "You are not authorized to edit this post" );
   if( posts[0].text )
    free( posts[0].text );
    if( posts )
   free( posts );
   goto RETURN;
  }

  httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "forum" ) );
  httpPrintf( cnt, "<input type=\"hidden\" name=\"empire\" value=\"%s\">", ( flags ? "true" : "false" ) );
  httpPrintf( cnt, "<input type=\"hidden\" name=\"forum\" value=\"%d\">", forum ); 
  httpPrintf( cnt, "<input type=\"hidden\" name=\"thread\" value=\"%d\">", thread );
  httpPrintf( cnt, "<input type=\"hidden\" name=\"editpost\" value=\"%d\">", post );
  httpString( cnt, "Edit post<br><br><textarea name=\"post\" wrap=\"soft\" rows=\"10\" cols=\"60\">" );

  if( !( text = malloc( 2 * FORUM_MAX ) ) )
  {
   if( posts[0].text )
    free( posts[0].text );
    if( posts )
   free( posts );
   goto RETURN;
  }
  iohttpForumFilter3( text, posts[0].text, 2*FORUM_MAX - 512 );
  httpString( cnt, text );

  if( posts[0].text )
   free( posts[0].text );
   if( posts )
  free( posts );
  if( text )
  free( text );
  httpString( cnt, "</textarea><br><br><input type=\"submit\" value=\"Post\"></form>" );
 }
 else if( action == 8 )
 {
  b = dbForumListPosts( flags, forum, thread, post, post+1, &threadd, &posts );
  if( b < 0 )
  {
   httpString( cnt, "This post doesn't exist" );
   goto RETURN;
  }
  if( !( iohttpForumPerms( flags, id, forum, cnt, &maind, 0 ) ) && ( ( id == -1 ) || ( posts[0].post.authorid != id ) || !( ioCompareExact( posts[0].post.authorname, maind.faction ) ) ) )
  {
   httpString( cnt, "You are not authorized to edit this post" );
   if( posts[0].text )
    free( posts[0].text );
    if( posts )
   free( posts );
   goto RETURN;
  }
  sprintf( postd.post.authorname, "%s", posts[0].post.authorname );
  sprintf( postd.post.authortag, "%s", posts[0].post.authortag );
  memcpy( &postd.post.sin_addr, &posts[0].post.sin_addr, sizeof(struct in_addr) );
  postd.post.time = posts[0].post.time;
  postd.post.tick = posts[0].post.tick;
  postd.post.authorid = posts[0].post.authorid;
  postd.post.flags = posts[0].post.flags;
  if( posts[0].text )
   free( posts[0].text );
   if( posts )
  free( posts );

  if( !( postd.text = malloc( 3 * FORUM_MAX ) ) )
  {
   goto RETURN;
  }
  a = 0;
  if((cnt->session)->dbuser)
  {
	  if( ( id != -1 ) && ( ((cnt->session)->dbuser)->level >= LEVEL_FORUMMOD ) )
	 	 a = 1;
	 }
  iohttpForumFilter( &postd.text[2*FORUM_MAX], poststring, FORUM_MAX, a );
  postd.post.length = iohttpForumFilter2( postd.text, &postd.text[2*FORUM_MAX], 2*FORUM_MAX - 512 );
  time( &tint );
  strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &tint ) );
  postd.post.length += sprintf( &postd.text[postd.post.length], "<br><br><font size=\"1\"><i>Edited by %s on Week %d, Year %d - %s</i></font>", maind.faction, ticks.number % 52, ticks.number / 52, timebuf );

  a = dbForumEditPost( flags, forum, thread, post, &postd );
  if( a >= 0 ) {
   httpPrintf( cnt, "Post edited!<br><br>" );
   badpost = false;
  } else {
   httpPrintf( cnt, "Error while editing post<br><br>" );
   }
   if( postd.text )
  free( postd.text );
  goto iohttpForumL1;
 }

RETURN:
if( id != -1 ) {
	httpString( cnt, "</center></body></html>" );
} else {
	iohtmlFunc_endhtml( cnt );
}

return;
}
