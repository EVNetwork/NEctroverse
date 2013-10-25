#ifndef CONFIGINCLUDED
#include "config.h"
#endif
#ifndef GLOBALINCLUDED
#include "global.h"
#endif




#define IOHTTP_FORUM_BUFFER (65536)

#define IOHTTP_FORUM_THREADSNUM (40)

#define IOHTTP_FORUM_POSTSNUM (40)

char iohttpInputHex( char *src );

#define IOHTTP_FORUM_SMILETOTAL (62)
#define IOHTTP_FORUM_SMILEBASE (8)

typedef struct
{
 int id;
 char string[16];
} iohttpForumSmileysDef;

iohttpForumSmileysDef iohttpForumSmileys[IOHTTP_FORUM_SMILETOTAL] =
{
{ 0, ":)" },
{ 1, ":D" },
{ 2, ";)" },
{ 3, "x(" },
{ 3, "X(" },
{ 4, ":(" },
{ 5, ":p" },
{ 5, ":P" },
{ 0, "::smile" },
{ 1, "::laugh" },
{ 2, "::wink" },
{ 3, "::angry" },
{ 4, "::sad" },
{ 5, "::tongue" },
{ 6, "::good" },
{ 7, "::bad" },
{ 8, "::friends" },
{ 9, "::sigh" },
{ 10, "::funny" },
{ 11, "::toilet" },
{ 12, "::wave" },
{ 13, "::fool" },
{ 14, "::king" },
{ 15, "::hammer" },
{ 16, "::party" },
{ 17, "::mad" },
{ 18, "::sleep" },
{ 19, "::sob" },
{ 20, "::cry" },
{ 21, "::angel" },
{ 22, "::popcorn" },
{ 23, "::hat" },
{ 24, "::spin" },
{ 25, "::nuts" },
{ 26, "::snooze" },
{ 27, "::wall" },
{ 28, "::wtf" },
{ 29, "::10on10" },
{ 30, "::1on10" },
{ 31, "::bacon" },
{ 32, "::beer" },
{ 33, "::bounce" },
{ 34, "::cool" },
{ 35, "::crazy" },
{ 36, "::fu" },
{ 37, "::sadder" },
{ 38, "::blame" },
{ 39, "::haha" },
{ 40, "::scream" },
{ 41, "::sobber" },
{ 42, "::ninja" },
{ 43, "::puke" },
{ 44, "::kill" },
{ 53, "::banana" },
{ 46, "::smoke" },
{ 47, "::eyeroller" },
{ 48, "::wow" },
{ 49, "::omg" },
{ 50, "::dead" },
{ 51, "::discohaan" },
{ 52, "::eendje" },
{ 53, "::stick" }
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
  if( c == '+' )
   c = ' ';
  else if( c == '%' )
  {
   if( !( string[a+1] ) || !( string[a+2] ) )
    break;
   c = iohttpInputHex( &string[a+1] );
   a += 2;
  }
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

int iohttpForumFilter2( char *dest, char *string, int size )
{
 int a, b, c;
 char *string2;
 for( b = c = 0 ; *string ; )
 {
  if( b >= size-20 )
   break;
  if( ( string2 = ioCompareWords( string, "&&**##" ) ) )
  {
   string = string2;
   c ^= 1;
  }
  if( !( c ) )
  {
   for( a = 0 ; a < IOHTTP_FORUM_SMILEBASE ; a++ )
   {
    if( !( string2 = ioCompareWords( string, iohttpForumSmileys[a].string ) ) )
     continue;
    string = string2;
    b += sprintf( &dest[b], "<img src=\"f%02d.gif\">", iohttpForumSmileys[a].id );
    goto iohttpForumFilter2L0;
   }
   if( string[0] == ':' )
   {
    for( ; a < IOHTTP_FORUM_SMILETOTAL ; a++ )
    {
     if( !( string2 = ioCompareWords( string, iohttpForumSmileys[a].string ) ) )
      continue;
     string = string2;
     b += sprintf( &dest[b], "<img src=\"f%02d.gif\">", iohttpForumSmileys[a].id );
     goto iohttpForumFilter2L0;
    }
   }
  }
  if( *string == 10 )
  {
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

int iohttpForumFilter3( char *dest, char *string, int size )
{
 int b, c;
 char *string2;
 for( b = c = 0 ; *string ; )
 {
  dest[b] = 0;
  if( b >= size-16 )
   break;
  if( ( string2 = ioCompareWords( string, "<br><br><font size=\"1\"><i>Edited by" ) ) )
   break;
  if( ( string2 = ioCompareWords( string, "<br>" ) ) )
  {
   dest[b++] = 10;
   string = string2;
   continue;
  }
  if( !( string2 = ioCompareWords( string, "<img src=\"f" ) ) )
   goto iohttpForumFilter3L0;
  if( !( string2[0] ) || !( string2[1] ) )
   goto iohttpForumFilter3L0;
  c = 10 * ( string2[0]-'0' ) + string2[1]-'0';
  string2 += 2;
  if( !( string2 = ioCompareWords( string2, ".gif\">" ) ) )
   goto iohttpForumFilter3L0;
  if( (unsigned int)c >= IOHTTP_FORUM_SMILETOTAL )
   goto iohttpForumFilter3L0;
  b += sprintf( &dest[b], iohttpForumSmileys[IOHTTP_FORUM_SMILEBASE+c].string );
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


int iohttpForumPerms( int id, int forum, svConnectionPtr cnt, dbUserMainPtr maind, int perms )
{
	if( perms >= 4 )
	{
  return 1;
 }
 else if((forum < 100) && (cnt->dbuser) )
 {
 	if(id == -1)
			return 0;
		if(((cnt->dbuser)->level >= LEVEL_MODERATOR)||((cnt->dbuser)->level >= LEVEL_FORUMMOD))
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
  if( (cnt->dbuser)->level >= LEVEL_MODERATOR )
  {
  	return 1;
  }
  
  if( (cnt->dbuser)->flags & cmdUserFlags[CMD_FLAGS_INDEPENDENT] )
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
  if( ( forum >= 100 ) && ( maind->empire == forum-100 ) && ( (cnt->dbuser)->flags & ( cmdUserFlags[CMD_FLAGS_LEADER] | cmdUserFlags[CMD_FLAGS_VICELEADER] | cmdUserFlags[CMD_FLAGS_COMMINISTER] ) ) )
   return 1;
  if( (cnt->dbuser)->level < LEVEL_MODERATOR )
   return 0;
 }
 return 1;
}






int iohttpForumCleanAuthor( char *string )
{
 int a, b, fnum, tnum, count = 0;
 char word[512];
 dbForumForumPtr forums;
 dbForumForumDef forumd;
 dbForumThreadPtr threads;
 fnum = dbForumListForums( 0, &forums );
 if( fnum < 0 ) return -1;
 iohttpForumFilter( word, string, 512, 0 );
 for( a = 0 ; a < fnum ; a++ )
 {
  tnum = dbForumListThreads( a, 0, 0xFFFF, &forumd, &threads );
  if( tnum < 0 )
   continue;
  for( b = tnum-1 ; b >= 0 ; b-- )
  {
   if( !( threads[b].authorname[0] ) && !( threads[b].topic[0] ) )
   {
    dbForumRemoveThread( a, threads[b].id );
    count++;
   }
   else if( ioCompareFindWords( threads[b].authorname, word ) )
   {
    dbForumRemoveThread( a, threads[b].id );
    count++;
   }
  }
 }
 return count;
}


int iohttpForumCleanIP( char *ipstring )
{
 int a, b, fnum, tnum, count = 0;
 dbForumForumPtr forums;
 dbForumForumDef forumd;
 dbForumThreadPtr threads;
 fnum = dbForumListForums( 0, &forums );
 if( fnum < 0 )
  return -1;
 for( a = 0 ; a < fnum ; a++ )
 {
  tnum = dbForumListThreads( a, 0, 0xFFFF, &forumd, &threads );
  if( tnum < 0 )
   continue;
  for( b = tnum-1 ; b >= 0 ; b-- )
  {
   if( ioCompareExact( inet_ntoa( threads[b].sin_addr ), ipstring ) )
   {
    dbForumRemoveThread( a, threads[b].id );
    count++;
   }
  }
 }
 return count;
}



void iohttpForum( svConnectionPtr cnt )
{
 int a, b, c, d, id, forum, thread, post, action, skip;
 dbUserMainDef maind;
 char *forumstring, *threadstring, *topicstring, *poststring, *delthreadstring, *delpoststring, *editpoststring, *namestring, *skipstring;
 dbForumForumPtr forums;
 dbForumThreadPtr threads;
 dbForumPostPtr posts;
 dbForumForumDef forumd;
 dbForumThreadDef threadd;
 dbForumPostDef postd;
 char timebuf[512];
 char COREDIR[512];
 char timetemp[512];
 char *text;
	FILE *fFile;

iohttpBase( cnt, 1 );
	
if( ( id = iohttpIdentify( cnt, 2 ) ) >= 0 ) {
	if( !( iohttpHeader( cnt, id, &maind ) ) )
		return;
} else {
iohttpFunc_frontmenu( cnt, 3 );
}
	
 iohttpVarsInit( cnt );
 forumstring = iohttpVarsFind( "forum" );
 threadstring = iohttpVarsFind( "thread" );
 topicstring = iohttpVarsFind( "topic" );
 poststring = iohttpVarsFind( "post" );
 delthreadstring = iohttpVarsFind( "delthread" );
 delpoststring = iohttpVarsFind( "delpost" );
 editpoststring = iohttpVarsFind( "editpost" );
 namestring = iohttpVarsFind( "name" );
 skipstring = iohttpVarsFind( "skip" );
 iohttpVarsCut();

 action = 0;
 if( ( forumstring ) && ( sscanf( forumstring, "%d", &forum ) == 1 ) )
 {
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
  b = dbForumListForums( 0, &forums );
  if( b < 0 )
  {
   svSendString( cnt, "Error while retrieving list of forums</center></body></html>" );
   return;
  }
  a = time( 0 )-(3600*SERVER_TIME_ZONE);
  strftime( timebuf, 256, "%T, %b %d", localtime( (time_t *)&a ) );
  svSendPrintf( cnt, "<table cellspacing=\"4\" width=\"80%%\"><tr><td><a href=\"/\" target=\"_top\">%s</a> - %s public forums</td><td align=\"right\">%s", sysconfig.servername, sysconfig.servername, timebuf );
  if( ( id != -1 ) && ( forum != maind.empire + 100 ) && ( maind.empire != -1 ) )
   svSendPrintf( cnt, " - <a href=\"forum?forum=%d\">Empire forum</a>", maind.empire + 100 );
  svSendString( cnt, "</td></tr></table>" );

  svSendString( cnt, "<table width=\"80%\" cellpadding=\"3\" cellspacing=\"3\" bgcolor=\"#000000\"><tr bgcolor=\"#333333\"><td width=\"70%\">Forums</td><td width=\"10%\">Threads</td><td width=\"20%\">Last post</td></tr>" );
  for( a = 0 ; a < b ; a++ )
  {
   strftime( timebuf, 256, "%T, %b %d %Y", localtime( (time_t *)&(forums[a].time) ) );
   svSendPrintf( cnt, "<tr bgcolor=\"#111111\"><td><a href=\"forum?forum=%d&last=%d\">%s</a></td><td>%d</td><td nowrap>%s<br>Week %d, %d</td></tr>", a, forums[a].time, forums[a].title, forums[a].threads, timebuf, forums[a].tick % 52, forums[a].tick / 52 );
  }
  svSendString( cnt, "</table>" );

  free( forums );
 }
 else if( action == 1 )
 {
  iohttpForumL2:
	
  b = dbForumListThreads( forum, skip, skip+IOHTTP_FORUM_THREADSNUM, &forumd, &threads );
  if( b < 0 )
  {
   svSendString( cnt, "These threads do not exist!</center></body></html>" );
   return;
  }
  if( !( iohttpForumPerms( id, forum, cnt, &maind, forumd.rperms ) ) )
  {
   if( threads )
    free( threads );
   svSendString( cnt, "You are not authorized to view this forum</center></body></html>" );
   return;
  }
  if(cnt->dbuser)
  {
	  if(cnt->dbuser->level >= LEVEL_MODERATOR)
	  {
	  	if(forum>100)
	  	{
			sprintf( COREDIR, "%s/logs/modlog.txt", COREDIRECTORY );
	  		fFile = fopen( COREDIR, "a+t" );
		  	if( fFile )
		 		{
		 			fprintf( fFile, "%s > view forum of empire %d\n", maind.faction, forum-100);
		 			fclose(fFile);
		 		}
		 	}
	  	}
	 }
		svSendPrintf( cnt, "<table cellspacing=\"4\" width=\"80%%\"><tr><td><a href=\"/\" target=\"_top\">%s</a> - <a href=\"forum\">%s public forums</a> - %s</td><td align=\"right\">", sysconfig.servername, sysconfig.servername, forumd.title );
  if( forum < 100 )
  {
   a = time( 0 )-(3600*SERVER_TIME_ZONE);
   strftime( timebuf, 256, "%T, %b %d", localtime( (time_t *)&a ) );
   svSendString( cnt, timebuf );
   if( ( id != -1 ) && ( forum != maind.empire + 100 ) && ( maind.empire != -1 ) )
    svSendPrintf( cnt, " - <a href=\"forum?forum=%d\">Empire forum</a>", maind.empire + 100 );
   svSendString( cnt, "</td></tr></table>" );
  }
  else
   svSendPrintf( cnt, "Week %d, Year %d</td></tr></table>", svTickNum % 52, svTickNum / 52 );

  if( forumd.threads > IOHTTP_FORUM_THREADSNUM )
  {
   svSendString( cnt, "Pages : " );
   for( a = 0, c = 1 ; a < forumd.threads ; a += IOHTTP_FORUM_THREADSNUM, c++ )
   {
    if( skip != a )
     svSendPrintf( cnt, "<a href=\"forum?forum=%d&skip=%d\">%d</a> ", forum, a, c );
    else
     svSendPrintf( cnt, "<b>%d</b> ", c );
   }
  }

  svSendString( cnt, "<table width=\"80%\" cellpadding=\"3\" cellspacing=\"3\" bgcolor=\"#000000\"><tr bgcolor=\"#333333\"><td width=\"60%\">Topic</td><td width=\"10%\">Posts</td><td width=\"15%\">Author</td><td width=\"15%\">Last post</td></tr>" );
  for( a = 0 ; a < b ; a++ )
  {
  	strftime( timebuf, 256, "%T, %b %d %Y", localtime( (time_t *)&(threads[a].time) ) );
   sprintf(timetemp, "<br>Week %d, Year %d", threads[a].tick % 52, threads[a].tick / 52 );
   strcat( timebuf, timetemp);
   
   svSendPrintf( cnt, "<tr bgcolor=\"#111111\"><td><a href=\"forum?forum=%d&thread=%d&last=%d\">%s</a>", forum, threads[a].id, threads[a].time, threads[a].topic );
   if( threads[a].posts > IOHTTP_FORUM_POSTSNUM )
   {
    svSendString( cnt, " - Pages : " );
    for( d = 0, c = 1 ; d < threads[a].posts ; d += IOHTTP_FORUM_POSTSNUM, c++ )
     svSendPrintf( cnt, "<a href=\"forum?forum=%d&thread=%d&skip=%d&last=%d\">%d</a> ", forum, threads[a].id, d, threads[a].time, c );
   }
   svSendPrintf( cnt, "</td><td>%d</td><td nowrap>%s</td><td nowrap>%s", threads[a].posts, threads[a].authorname, timebuf );
			if(( iohttpForumPerms( id, forum, cnt, &maind, 0 ) )&&(id != -1))
   {
   	svSendPrintf( cnt, " <a href=\"forum?forum=%d&thread=%d&delthread=1\">Delete</a>", forum, threads[a].id );
  	}
  	if(cnt->dbuser)
  	{
	   if(( id != -1 ) && ((((cnt->dbuser)->level >= LEVEL_FORUMMOD))||((cnt->dbuser)->level >= LEVEL_FORUMMOD)))
	    svSendPrintf( cnt, "<br>IP: %s", inet_ntoa( threads[a].sin_addr ) );
	  }
			svSendString( cnt, "</td></tr>" );
  }
  svSendString( cnt, "</table>" );
  if( forumd.threads > IOHTTP_FORUM_THREADSNUM )
  {
   svSendString( cnt, "Pages : " );
   for( a = 0, c = 1 ; a < forumd.threads ; a += IOHTTP_FORUM_THREADSNUM, c++ )
   {
    if( skip != a )
     svSendPrintf( cnt, "<a href=\"forum?forum=%d&skip=%d\">%d</a> ", forum, a, c );
    else
     svSendPrintf( cnt, "<b>%d</b> ", c );
   }
  }

  if( iohttpForumPerms( id, forum, cnt, &maind, forumd.wperms ) )
  {
   svSendPrintf( cnt, "<form action=\"forum\" method=\"POST\"><input type=\"hidden\" name=\"forum\" value=\"%d\"><table cellspacing=\"3\"><tr><td>Name</td><td>", forum );
   if( id == -1 )
//    svSendPrintf( cnt, "<input type=\"text\" name=\"name\" size=\"32\">" );
   return; 
   else
    svSendPrintf( cnt, "%s<input type=\"hidden\" name=\"name\" value=\"%s\">", (cnt->dbuser)->faction, (cnt->dbuser)->faction );
   svSendString( cnt, "</td></tr><tr><td>Topic</td><td><input type=\"text\" name=\"topic\" size=\"32\"></td></tr>" );
   svSendString( cnt, "<tr><td>Post</td><td><textarea name=\"post\" wrap=\"soft\" rows=\"10\" cols=\"60\"></textarea></td></tr><tr><td>&nbsp;</td><td><input type=\"submit\" value=\"Post\"></td></tr></table></form>" );
  }

  if( threads )
   free( threads );
 }
 else if( action == 2 )
 {
  iohttpForumL1:
  if( dbForumRetrieveForum( forum, &forumd ) < 0 )
  {
   svSendString( cnt, "This forum does not exist!</center></body></html>" );
   return;
  }
  b = dbForumListPosts( forum, thread, skip, skip+IOHTTP_FORUM_POSTSNUM, &threadd, &posts );
  if( b < 0 )
  {
   svSendString( cnt, "Error while retrieving list of posts</center></body></html>" );
   return;
  }

  if( !( iohttpForumPerms( id, forum, cnt, &maind, forumd.rperms ) ) )
  {
   for( a = 0 ; a < b ; a++ )
   {
    if( posts[a].text )
     free( posts[a].text );
   }
   if( posts )
    free( posts );
   svSendString( cnt, "You are not authorized to view this forum</center></body></html>" );
   return;
  }

  svSendPrintf( cnt, "<table cellspacing=\"4\" width=\"80%%\"><tr><td><a href=\"/\" target=\"_top\">%s</a> - <a href=\"forum\">%s public forums</a> - <a href=\"forum?forum=%d\">%s</a> - %s</td><td align=\"right\">", sysconfig.servername, sysconfig.servername, forum, forumd.title, threadd.topic );
  if( forum < 100 )
  {
   a = time( 0 )-(3600*SERVER_TIME_ZONE);
   strftime( timebuf, 256, "%T, %b %d", localtime( (time_t *)&a ) );
   svSendString( cnt, timebuf );
   if( ( id != -1 ) && ( forum != maind.empire + 100 ) && ( maind.empire != -1 ) )
    svSendPrintf( cnt, " - <a href=\"forum?forum=%d\">Empire forum</a>", maind.empire + 100 );
   svSendString( cnt, "</td></tr></table>" );
  }
  else
   svSendPrintf( cnt, "Week %d, Year %d</td></tr></table>", svTickNum % 52, svTickNum / 52 );

  if( threadd.posts > IOHTTP_FORUM_POSTSNUM )
  {
   svSendString( cnt, "Pages : " );
   for( a = 0, c = 1 ; a < threadd.posts ; a += IOHTTP_FORUM_POSTSNUM, c++ )
   {
    if( skip != a )
     svSendPrintf( cnt, "<a href=\"forum?forum=%d&thread=%d&skip=%d\">%d</a> ", forum, thread, a, c );
    else
     svSendPrintf( cnt, "<b>%d</b> ", c );
   }
  }

  svSendString( cnt, "<table width=\"80%\" cellpadding=\"3\" cellspacing=\"3\" bgcolor=\"#000000\">" );
  for( a = 0 ; a < b ; a++ )
  {
   if( !( posts[a].text ) )
    continue;
   c = a + skip;
   if( forum < 100 )
    strftime( timebuf, 256, "%T, %b %d %Y", localtime( (time_t *)&(posts[a].post.time) ) );
   else
    sprintf( timebuf, "Week %d, Year %d", (posts[a].post).tick % 52, (posts[a].post).tick / 52 );
   svSendPrintf( cnt, "<tr><td valign=\"top\" width=\"10%%\" nowrap bgcolor=\"#282828\"><b>%s</b><br><i>%s</i><br>%s", posts[a].post.authorname, posts[a].post.authortag, timebuf );

   if( iohttpForumPerms( id, forum, cnt, &maind, 0 ) || ( ( id != -1 ) && ( posts[a].post.authorid == id ) && ioCompareExact( posts[a].post.authorname, (cnt->dbuser)->faction ) ))
    svSendPrintf( cnt, "<br><a href=\"forum?forum=%d&thread=%d&editpost=%d\">Edit</a> - <a href=\"forum?forum=%d&thread=%d&delpost=%d\">Delete</a>", forum, thread, c, forum, thread, c );
   if(cnt->dbuser)
   {
	   if( ( id != -1 ) && ( (cnt->dbuser)->level >= LEVEL_MODERATOR ) )
	   {
	    svSendPrintf( cnt, "<br>Account: <a href=\"player?id=%d\">%d</a>", posts[a].post.authorid, posts[a].post.authorid );
	    svSendPrintf( cnt, "<br>IP: %s", inet_ntoa( posts[a].post.sin_addr ) );
	   }
	  }
   svSendString( cnt, "</td><td valign=\"top\" width=\"90%\" bgcolor=\"#181818\">" );
   svSendString( cnt, posts[a].text );
   svSendString( cnt, "</td></tr>" );
   if( posts[a].text )
    free( posts[a].text );
  }
  svSendString( cnt, "</table>" );

  if( threadd.posts > IOHTTP_FORUM_POSTSNUM )
  {
   svSendString( cnt, "Pages : " );
   for( a = 0, c = 1 ; a < threadd.posts ; a += IOHTTP_FORUM_POSTSNUM, c++ )
   {
    if( skip != a )
     svSendPrintf( cnt, "<a href=\"forum?forum=%d&thread=%d&skip=%d\">%d</a> ", forum, thread, a, c );
    else
     svSendPrintf( cnt, "<b>%d</b> ", c );
   }
  }

  if( iohttpForumPerms( id, forum, cnt, &maind, forumd.wperms ) )
  {
   svSendPrintf( cnt, "<form action=\"forum\" method=\"POST\"><input type=\"hidden\" name=\"forum\" value=\"%d\"><input type=\"hidden\" name=\"thread\" value=\"%d\"><table cellspacing=\"3\"><tr><td>Name</td><td>", forum, thread );
   if( id == -1 )
   return;
  //  svSendPrintf( cnt, "<input type=\"text\" name=\"name\" size=\"32\">" );
   else
    svSendPrintf( cnt, "%s<input type=\"hidden\" name=\"name\" value=\"%s\">", (cnt->dbuser)->faction, (cnt->dbuser)->faction );
   svSendString( cnt, "</td></tr><tr><td>Post</td><td><textarea name=\"post\" wrap=\"soft\" rows=\"10\" cols=\"60\"></textarea></td></tr><tr><td>&nbsp;</td><td><input type=\"submit\" value=\"Post\"></td></tr></table></form>" );
  }

  if( posts )
   free( posts );
 }
 else if( action == 3 )
 {
  if( dbForumRetrieveForum( forum, &forumd ) < 0 )
  {
   svSendString( cnt, "This forum does not exist!</center></body></html>" );
   return;
  }
  if( !( iohttpForumPerms( id, forum, cnt, &maind, forumd.wperms ) ) )
  {
   svSendString( cnt, "You are not authorized to post in this forum</center></body></html>" );
   return;
  }
  iohttpForumFilter( threadd.topic, topicstring, DB_FORUM_NAME_SIZE, 0 );
  threadd.posts = 0;
  threadd.authorid = id;
  if( id != -1 )
   sprintf( threadd.authorname, (cnt->dbuser)->faction );
  else if( namestring )
   iohttpForumFilter( threadd.authorname, namestring, 32, 0 );
  else
   sprintf( threadd.authorname, "Anonymous" );
  threadd.time = time( 0 )-(3600*SERVER_TIME_ZONE);
  threadd.tick = svTickNum;
  threadd.flags = 0;
  memcpy( &(threadd.sin_addr), &(cnt->sockaddr.sin_addr), sizeof(struct in_addr) );
  thread = dbForumAddThread( forum, &threadd );
  goto iohttpForumL0;
 }
 else if( action == 4 )
 {
  if( dbForumRetrieveForum( forum, &forumd ) < 0 )
  {
   svSendString( cnt, "This forum does not exist!</center></body></html>" );
   return;
  }
  if( !( iohttpForumPerms( id, forum, cnt, &maind, forumd.wperms ) ) )
  {
   svSendString( cnt, "You are not authorized to post in this forum</center></body></html>" );
   return;
  }
		
	/*	b = dbForumListThreads( forum, thread, thread+1, &forumd, &threads );
  
  if(b)
  {
  	if(threads[thread].flags == DB_FORUM_FLAGS_THREAD_LOCK)
  	{
  		svSendString( cnt, "Sorry this topic is <b>lock</b> and nothing can happen here" );
   	return;
  	}
  }*/
		
  iohttpForumL0:

  if( !( postd.text = malloc( 3 * IOHTTP_FORUM_BUFFER ) ) )
  {
   svSendString( cnt, "</center></body></html>" );
   return;
  }
  a = 0;
  if(cnt->dbuser)
  {
	  if( ( id != -1 ) && ( (cnt->dbuser)->level >= LEVEL_MODERATOR ) )
	   a = 1;
	 }
  iohttpForumFilter( &postd.text[2*IOHTTP_FORUM_BUFFER], poststring, IOHTTP_FORUM_BUFFER, a );
  postd.post.length = iohttpForumFilter2( postd.text, &postd.text[2*IOHTTP_FORUM_BUFFER], IOHTTP_FORUM_BUFFER );
  postd.post.authorid = id;
  if( id != -1 )
  {
   sprintf( postd.post.authorname, (cnt->dbuser)->faction );
   sprintf( postd.post.authortag, (cnt->dbuser)->forumtag );
  }
  else
  {
   if( namestring )
    iohttpForumFilter( postd.post.authorname, namestring, 32, 0 );
   else
    sprintf( postd.post.authorname, "Anonymous" );
   postd.post.authortag[0] = 0;
  }
  postd.post.time = time( 0 )-(3600*SERVER_TIME_ZONE);  //to be in GMT with the server running anywhere worldwide
  postd.post.tick = svTickNum;
  postd.post.flags = 0;
  memcpy( &(postd.post.sin_addr), &(cnt->sockaddr.sin_addr), sizeof(struct in_addr) );
  a = dbForumAddPost( forum, thread, &postd );
  if( a >= 0 )
   svSendPrintf( cnt, "Post added!<br><br>" );
  else
   svSendPrintf( cnt, "Error while adding post<br><br>" );
  free( postd.text );
  goto iohttpForumL1;
 }
 else if( action == 5 )
 {
  if(( !( iohttpForumPerms( id, forum, cnt, &maind, 0 ) ) ) || (id == -1))
  {
   svSendString( cnt, "You are not authorized to delete threads here</center></body></html>" );
   return;
  }
  iohttpForumL3:

  if( dbForumRemoveThread( forum, thread ) >= 0 )
   svSendPrintf( cnt, "Thread removed<br><br>" );
  else
   svSendPrintf( cnt, "Error while removing thread<br><br>" );
  goto iohttpForumL2;
 }
 else if( action == 6 )
 {
  b = dbForumListPosts( forum, thread, post, post+1, &threadd, &posts );
  if( b < 0 )
  {
   svSendString( cnt, "This post doesn't exist</center></body></html>" );
   return;
  }
  if( !( iohttpForumPerms( id, forum, cnt, &maind, 0 ) ) && ( ( id == -1 ) || ( posts[0].post.authorid != id ) || !( ioCompareExact( posts[0].post.authorname, (cnt->dbuser)->faction ) ) ) )
  {
   svSendString( cnt, "You are not authorized to delete this post</center></body></html>" );
   if( posts[0].text )
    free( posts[0].text );
   free( posts );
   return;
  }
  if( posts[0].text )
   free( posts[0].text );
  free( posts );
  a = dbForumRemovePost( forum, thread, post );
  if( a >= 0 )
  {
   if( !( a ) )
    goto iohttpForumL3;
   svSendPrintf( cnt, "Post removed<br><br>" );
  }
  else
   svSendPrintf( cnt, "Error while removing post<br><br>" );
  goto iohttpForumL1;
 }
 else if( action == 7 )
 {
  b = dbForumListPosts( forum, thread, post, post+1, &threadd, &posts );
  if( b < 0 )
  {
   svSendString( cnt, "This post doesn't exist</center></body></html>" );
   return;
  }
  if( !( iohttpForumPerms( id, forum, cnt, &maind, 0 ) ) && ( ( id == -1 ) || ( posts[0].post.authorid != id ) || !( ioCompareExact( posts[0].post.authorname, (cnt->dbuser)->faction ) ) ) )
  {
   svSendString( cnt, "You are not authorized to edit this post</center></body></html>" );
   if( posts[0].text )
    free( posts[0].text );
   free( posts );
   return;
  }

  svSendPrintf( cnt, "<form action=\"forum\" method=\"POST\"><input type=\"hidden\" name=\"forum\" value=\"%d\"><input type=\"hidden\" name=\"thread\" value=\"%d\"><input type=\"hidden\" name=\"editpost\" value=\"%d\">", forum, thread, post );
  svSendString( cnt, "Edit post<br><br><textarea name=\"post\" wrap=\"soft\" rows=\"10\" cols=\"60\">" );

  if( !( text = malloc( 2 * IOHTTP_FORUM_BUFFER ) ) )
  {
   if( posts[0].text )
    free( posts[0].text );
   free( posts );
   svSendString( cnt, "</center></body></html>" );
   return;
  }
  iohttpForumFilter3( text, posts[0].text, 2*IOHTTP_FORUM_BUFFER - 512 );
  svSendString( cnt, text );

  if( posts[0].text )
   free( posts[0].text );
  free( posts );
  free( text );
  svSendString( cnt, "</textarea><br><br><input type=\"submit\" value=\"Post\"></form>" );
 }
 else if( action == 8 )
 {
  b = dbForumListPosts( forum, thread, post, post+1, &threadd, &posts );
  if( b < 0 )
  {
   svSendString( cnt, "This post doesn't exist</center></body></html>" );
   return;
  }
  if( !( iohttpForumPerms( id, forum, cnt, &maind, 0 ) ) && ( ( id == -1 ) || ( posts[0].post.authorid != id ) || !( ioCompareExact( posts[0].post.authorname, (cnt->dbuser)->faction ) ) ) )
  {
   svSendString( cnt, "You are not authorized to edit this post</center></body></html>" );
   if( posts[0].text )
    free( posts[0].text );
   free( posts );
   return;
  }
  sprintf( postd.post.authorname, posts[0].post.authorname );
  sprintf( postd.post.authortag, posts[0].post.authortag );
  postd.post.time = posts[0].post.time;
  postd.post.tick = posts[0].post.tick;
  postd.post.authorid = posts[0].post.authorid;
  postd.post.flags = posts[0].post.flags;
  if( posts[0].text )
   free( posts[0].text );
  free( posts );

  if( !( postd.text = malloc( 3 * IOHTTP_FORUM_BUFFER ) ) )
  {
   svSendString( cnt, "</center></body></html>" );
   return;
  }
  a = 0;
  if(cnt->dbuser)
  {
	  if( ( id != -1 ) && ( (cnt->dbuser)->level >= LEVEL_MODERATOR ) )
	 	 a = 1;
	 }
  iohttpForumFilter( &postd.text[2*IOHTTP_FORUM_BUFFER], poststring, IOHTTP_FORUM_BUFFER, a );
  postd.post.length = iohttpForumFilter2( postd.text, &postd.text[2*IOHTTP_FORUM_BUFFER], 2*IOHTTP_FORUM_BUFFER - 512 );
  a = time( 0 )-(3600*SERVER_TIME_ZONE);
  strftime( timebuf, 256, "%T, %b %d %Y", localtime( (time_t *)&a ) );
  postd.post.length += sprintf( &postd.text[postd.post.length], "<br><br><font size=\"1\"><i>Edited by %s on Week %d, Year %d - %s</i></font>", maind.faction, svTickNum % 52, svTickNum / 52, timebuf );

  a = dbForumEditPost( forum, thread, post, &postd );
  if( a >= 0 )
   svSendPrintf( cnt, "Post edited!<br><br>" );
  else
   svSendPrintf( cnt, "Error while editing post<br><br>" );
  free( postd.text );
  goto iohttpForumL1;
 }

if( id != -1 ) {
	svSendString( cnt, "</center></body></html>" );
} else {
	iohttpFunc_endhtml( cnt );
}

return;
}
