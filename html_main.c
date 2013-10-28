

int iohttpIdentifyHex( char *num )
{
 int a, b, c;
 for( a = b = 0 ; ; b <<= 4 )
 {
  if( ( num[a] >= '0' ) && ( num[a] <= '9' ) )
   b += c = num[a] - '0';
  else if( ( num[a] >= 'a' ) && ( num[a] <= 'z' ) )
   b += c = num[a] - ('a'-10);
  else if( ( num[a] >= 'A' ) && ( num[a] <= 'Z' ) )
   b += c = num[a] - ('A'-10);
  if( ++a == 4 )
   break;
 }
 return b;
}

int iohttpIdentify( svConnectionPtr cnt, int action ) {
	int a, b, id, session[4];
	char *src;
	char *data;
	FILE *file;
	struct stat stdata;


iohttpDataPtr iohttp = cnt->iodata;


if( !( src = iohttp->cookie ) )
	goto iohttpIdentifyL0;

if( !( src = ioCompareFindWords( src, "USRID=" ) ) )
	goto iohttpIdentifyL0;

id = iohttpIdentifyHex( &src[0] );

if( dbUserLinkDatabase( cnt, id ) < 0 )
	goto iohttpIdentifyL0;

if( dbSessionRetrieve( cnt->dbuser, session ) < 0 )
	goto iohttpIdentifyL0;

for( a = 0 ; a < 4 ; a++ ) {
	b = iohttpIdentifyHex( &src[4+(a<<2)] );

	if( session[a] != b )
		goto iohttpIdentifyL0;
}

if(( action & 2 )&&(cnt->dbuser)) {
	if( !( (cnt->dbuser)->flags & CMD_USER_FLAGS_ACTIVATED ) && ( (cnt->dbuser)->level < LEVEL_MODERATOR ) ) {
		if( action & 1 ) {
			if( action & 8 )
			iohttpBase( cnt, 1|2 );
			svSendString( cnt, "This account has not been activated yet.</body></html>" );
		}
		return -1;
	}
}

if( action & 4 ) {
	if( (cnt->dbuser)->flags & CMD_USER_FLAGS_ACTIVATED ) {
		if( action & 1 ) {
			if( action & 8 )
				iohttpBase( cnt, 1|2 );
			svSendString( cnt, "This account has been activated.</body></html>" );
		}
		return -1;
	}
}


return id;

iohttpIdentifyL0:

if( action & 1 ) {

	iohttpFunc_login( cnt, 1, "If you were playing just a few seconds ago, the server program was probably updated and restarted." );

}


return -1;
}

void iohttpBase( svConnectionPtr cnt, int flags ) {
	FILE *file;
	struct stat stdata;
	char *data;
	char DIRCHECKER[256];
	
svSendString( cnt, "Content-Type: text/html\n\n" );
svSendString( cnt, "<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-gb\" xml:lang=\"en-gb\"><head>");
svSendString( cnt, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" );
svSendString( cnt, "<meta http-equiv=\"Content-Style-Type\" content=\"text/css\">" );
svSendString( cnt, "<meta http-equiv=\"Content-Language\" content=\"en-gb\">" );
svSendString( cnt, "<meta http-equiv=\"imagetoolbar\" content=\"no\">" );
svSendPrintf( cnt, "<title>%s</title>", sysconfig.servername );
svSendPrintf( cnt, "<link rel=\"icon\" href=\"images/favicon.ico\">" );
svSendPrintf( cnt, "<link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" media=\"screen\">" );
svSendPrintf( cnt, "<script type=\"text/javascript\" src=\"javascript.js\"></script>" );

if( flags & 4 )
	svSendString( cnt, "<base target=\"_blank\">" );
if( flags & 1 ) { 
svSendString( cnt, "<style type=\"text/css\">" );
	svSendString( cnt, "body{background-image:url(images/mbg.gif);" );
	if( !( flags & 2 ) )
		svSendString( cnt, "background-attachment:fixed;" );
	svSendString( cnt, "}</style>" );
}

svSendString( cnt, "</head>" );
svSendString( cnt, "<body" );

if( flags & 8 )
	svSendString( cnt, " onload=\"if (window != window.top) { top.location.href=location.href }\" " );


svSendString( cnt, "><center>" );

return;
}

int iohttpHeader( svConnectionPtr cnt, int id, dbUserMainPtr mainp )
{
 int a;
 if( dbUserMainRetrieve( id, mainp ) < 0 )
 {
  svSendString( cnt, "Error while retriving user's main data</center></body></html>" );
  return 0;
 }

 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\" align=\"center\">" );

 svSendString( cnt, "<tr>" );
 svSendString( cnt, "<td width=\"35\" rowspan=\"2\"><img height=\"63\" src=\"images/i02.jpg\" width=\"35\"></td>" );
 svSendString( cnt, "<td width=\"100%\" height=\"21\" align=\"center\">" );

 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" background=\"images/i05.jpg\" border=\"0\" align=\"center\">" );
 svSendString( cnt, "<tr>" );
 svSendString( cnt, "<td width=\"41\"><img height=\"21\" src=\"images/i03.jpg\" width=\"41\"></td>" );
 svSendString( cnt, "<td background=\"images/i05.jpg\">" );

 if( ticks.status )
 svSendPrintf( cnt, "<table width=\"100%%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Networth : %lld</b></font></td><td width=\"40%%\" align=\"center\"><font size=\"1\"><b>Tick time : %d seconds left</b></font></td><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Population : %lld0</b></font></td></tr></table>", mainp->networth, (int)( ticks.next - time(0) ), mainp->ressource[CMD_RESSOURCE_POPULATION] );
 else
 svSendPrintf( cnt, "<table width=\"100%%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Networth : %lld</b></font></td><td width=\"40%%\" align=\"center\"><font size=\"1\"><b>Tick time : time frozen</b></font></td><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Population : %lld0</b></font></td></tr></table>", mainp->networth, mainp->ressource[CMD_RESSOURCE_POPULATION] );

 svSendString( cnt, "</td><td width=\"78\"><img height=\"21\" src=\"images/i07.jpg\" width=\"78\"></td></tr></table>" );
 svSendString( cnt, "</td></tr>" );

 svSendString( cnt, "<tr><td width=\"100%\" align=\"center\">" );
 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" background=\"images/i15.jpg\" border=\"0\" align=\"center\"><tr>" );

 a = dbUserNewsGetFlags( id );
 svSendString( cnt, "<td width=\"41\"><a href=\"hq\"><img height=\"42\" title=\"mail\" src=\"images/i09" );
 if( a & CMD_NEWS_FLAGS_MAIL )
  svSendString( cnt, "a" );
 svSendString( cnt, ".jpg\" width=\"41\" border=\"0\"></a></td>" );

 svSendString( cnt, "<td width=\"40\"><a href=\"hq\"><img height=\"42\" title=\"reports\" src=\"images/i10" );
 if( a & CMD_NEWS_FLAGS_BUILD )
  svSendString( cnt, "a" );
 svSendString( cnt, ".jpg\" width=\"40\" border=\"0\"></a></td>" );

 svSendString( cnt, "<td width=\"39\"><a href=\"hq\"><img height=\"42\" title=\"economy\" src=\"images/i11" );
 if( a & CMD_NEWS_FLAGS_AID )
  svSendString( cnt, "a" );
 svSendString( cnt, ".jpg\" width=\"39\" border=\"0\"></a></td>" );

 svSendString( cnt, "<td width=\"39\"><a href=\"hq\"><img height=\"42\" title=\"fleets\" src=\"images/i12" );
 if( a & CMD_NEWS_FLAGS_ATTACK )
  svSendString( cnt, "a" );
 else if( a & CMD_NEWS_FLAGS_FLEET )
  svSendString( cnt, "b" );
 svSendString( cnt, ".jpg\" width=\"39\" border=\"0\"></a></td>" );

 svSendString( cnt, "<td width=\"18\"><img height=\"42\" src=\"images/i13.jpg\" width=\"18\"></td>" );
 svSendString( cnt, "<td width=\"100%\" background=\"images/i15.jpg\" align=\"left\">" );

 svSendString( cnt, "<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\"><tr>" );
 svSendString( cnt, "<td width=\"50%\" align=\"center\" nowrap><font size=\"1\"><b>" );
 svSendPrintf( cnt, "Energy: %lld<br>Mineral: %lld</b></font></td><td width=\"50%%\" align=\"center\" nowrap><font size=\"1\"><b>Crystal: %lld<br>Ectrolium: %lld</b></font>", mainp->ressource[0], mainp->ressource[1], mainp->ressource[2], mainp->ressource[3] );
 svSendString( cnt, "</td></tr></table>" );

 svSendString( cnt, "</td><td width=\"49\"><img height=\"42\" src=\"images/i17.jpg\" width=\"49\"></td></tr></table>" );

 svSendString( cnt, "</td></tr></table><br>" );

 return 1;
}

void iohttpFunc_frontmenu( svConnectionPtr cnt, int flags ) {

svSendString( cnt, "<table class=\"center\" cellspacing=\"0\" cellpadding=\"10\" style=\"width:100%;border-width:0;\"><tbody><tr><td class=\"bodyline\">" );
svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td align=\"center\"><img src=\"images/ectro_03.jpg\" width=350 height=80  alt=\"ect Top\"></td></tr>" );

svSendString( cnt, "<tr><td><table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr>" );
svSendString( cnt, "<td background=\"images/ectro_12.jpg\" width=\"45%\">&nbsp;</td>" );
svSendString( cnt, "<td align=\"center\" width=\"10%\"><img src=\"images/ectro_06.jpg\" width=\"450\" height=\"75\"></td>" );
svSendString( cnt, "<td background=\"images/ectro_12.jpg\" width=\"45%\">&nbsp;</td>" );
svSendString( cnt, "</tr></table></tr></td>" );

svSendString( cnt, "<tr><td align=\"center\"><img src=\"images/ectro_09.jpg\" width=\"660\" height=\"100\"></td></tr>" );
svSendString( cnt, "<tr><td background=\"images/ectro_12.jpg\" align=\"center\"><table width=\"660\" height=\"75\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td background=\"images/ectro_13.jpg\" align=\"right\" valign=\"middle\"><b>" );

if( !( flags == 1 ) ) {
svSendString( cnt, "<a href=\"/\">Main</a>" );
}
if( !( flags == 2 ) ) {
if( !( flags == 1 ) )
svSendString( cnt, " | " );
svSendString( cnt, "<a href=\"register\">Register</a>" );
}
//if( !( flags == 3 ) ) {
//svSendString( cnt, " | " );
//svSendString( cnt, "<a href=\"forum\">Forums</a>" );
//}
if( !( flags == 4 ) ) {
svSendString( cnt, " | " );
svSendString( cnt, "<a href=\"faq\">FAQ</a>" );
}
if( !( flags == 5 ) ) {
svSendString( cnt, " | " );
svSendString( cnt, "<a href=\"gettingstarted\">Getting Started</a>" );
}
if( !( flags == 6 ) ) {
svSendString( cnt, " | " );
svSendString( cnt, "<a href=\"halloffame\">Hall of fame</a> " );
}
if( !( flags == 7 ) ) {
svSendString( cnt, " | " );
svSendString( cnt, "<a href=\"status\">Server Status</a>" );
}

svSendString( cnt, "</b></td></tr>" );

svSendString( cnt, "</table>" );
svSendString( cnt, "</td></tr></table>" );
svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );

return;
}

void iohttpBodyInit( svConnectionPtr cnt, char *title, ... )
{
 char text[4096];
 va_list ap;
 va_start( ap, title );
 vsnprintf( text, 4096, title, ap );
 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\" background=\"images/i27.jpg\"><tr><td width=\"10%\"><img height=\"24\" src=\"images/i25.jpg\" width=\"22\"></td><td width=\"80%\" align=\"center\" nowrap><b><font face=\"verdana\" size=\"2\">" );
 svSendString( cnt, text );
 svSendString( cnt, "</font></b></td><td width=\"10%\" align=\"right\"><img height=\"24\" src=\"images/i30.jpg\" width=\"62\"></td></tr></table>" );
 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"7\" background=\"images/i38.jpg\">&nbsp;</td><td bgcolor=\"#0b1119\" align=\"center\"><br>" );
 return;
}

void iohttpBodyEnd( svConnectionPtr cnt )
{
 svSendString( cnt, "<br></td><td width=\"7\" background=\"images/i43.jpg\">&nbsp;</td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"62\"><img height=\"12\" src=\"images/i45.jpg\" width=\"62\"></td><td width=\"100%\" background=\"images/i47.jpg\"><img height=\"12\" src=\"images/i47.jpg\" width=\"1\"></td><td width=\"62\"><img height=\"12\" src=\"images/i49.jpg\" width=\"62\"></td></tr></table></center><br><br><br></body></html>" );
 return;
}


// Woo, new races page... all auto-generated =D
void iohttpFunc_races( svConnectionPtr cnt ) {
	int a, b, c, id;
	dbUserMainDef maind;

if( ( id = iohttpIdentify( cnt, 2 ) ) >= 0 ) {
	iohttpBase( cnt, 1 );
	if( !( iohttpHeader( cnt, id, &maind ) ) )
		return;
} else {
	iohttpBase( cnt, 8 );
	iohttpFunc_frontmenu( cnt, 0 );
}

iohttpBodyInit( cnt, "%s: Races", sysconfig.servername );

for( a = 0; a < CMD_RACE_NUMUSED ; a++) {
	svSendPrintf( cnt, "<div class=\"genlarge\">%s</div><br>", cmdRaceName[a] );
	if( cmdRace[a].special )
	svSendPrintf( cnt, "<i><b>Special bonus.</b></i><br>", cmdRaceName[a] );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_POPRESEARCH )
		svSendPrintf( cnt, "Each 4000 population produces 1 research point every week!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_SOLARP15 )
		svSendPrintf( cnt, "Solar Production increased by 15%!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_CULPROTECT )
		svSendPrintf( cnt, "Culture Research production provides a psychic shield for planets!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_IDW )
		svSendPrintf( cnt, "Imune to Dark Web Effects!<br>" );

	svSendString( cnt, "<table width=\"*\" border=\"0\"><tr>" );
	svSendString( cnt, "<td valign=\"top\" width=\"250\"><i><b>Main bonuses</b></i><br>" );
	if( (cmdRace[a].attack - 1) )
	svSendPrintf( cnt, "%+.0f%% Attack.<br>", ( cmdRace[a].attack - 1 ) * 100 );
	if( cmdRace[a].defense - 1 )
	svSendPrintf( cnt, "%+.0f%% Defence.<br>", ( cmdRace[a].defense - 1 ) * 100 );
	if( ( ( cmdRace[a].speed / 2 ) - 1 ) )
		svSendPrintf( cnt, "%+.0f%% Travel Speed<br>", ( ( cmdRace[a].speed / 2 ) - 1 ) *100 );
	for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ ) {
		if( (cmdRace[a].unit[b] - 1) )
			svSendPrintf( cnt, " %+.0f%% %s strength.<br>", ( cmdRace[a].unit[b] - 1 ) * 100, cmdUnitName[b] );
	}
	svSendPrintf( cnt, "<br><i><b>Ressource bonuses</b></i><br>" );
	svSendPrintf( cnt, "%+.0f%% Population Upkeep Reduction<br>", (((cmdRace[a].growth-1)/0.02) - 1 ) * 100 );
	for( b = 0; b < CMD_RESSOURCE_NUMUSED ; b++) {
		if( (cmdRace[a].resource[b] - 1) )
			svSendPrintf( cnt, "%+.0f%% %s production<br>", ( cmdRace[a].resource[b] - 1 ) * 100, cmdRessourceName[b] );
	}
	svSendString( cnt, "</td><td valign=\"top\" width=\"225\">" );
	svSendPrintf( cnt, "<i><b>Research bonuses</b></i><br>" );
	for( b = 0 ; b < CMD_RESEARCH_NUMUSED ; b++ ) {
		if( cmdRace[a].researchmax[b] != 200 )
			svSendPrintf( cnt, "%.0f%% %s limit.<br>", cmdRace[a].researchmax[b], cmdResearchName[b] );
	}
	for( b = 0 ; b < CMD_RESEARCH_NUMUSED ; b++ ) {
		if( ( cmdRace[a].researchpoints[b] - 1 ) )
			svSendPrintf( cnt, "%+.0f%% %s<br>", ( cmdRace[a].researchpoints[b] - 1 ) * 100, cmdResearchName[b] );
	}
	svSendString( cnt, "</td><td valign=\"top\" width=\"140\"><i><b>Operations</b></i>" );
	for( b = 0 ; b < CMD_AGENTOP_NUMUSED ; b++ ) {
		if( specopAgentsAllowed( b, a ) )
			svSendPrintf( cnt, "<br>%s", cmdAgentopName[b] );
	}
	svSendString( cnt, "<br><br><i><b>Spells</b></i>" );
	for( b = 0 ; b < CMD_PSYCHICOP_NUMUSED ; b++ ) {
		if( specopPsychicsAllowed( b, a ) )
			svSendPrintf( cnt, "<br>%s", cmdPsychicopName[b] );
	}
	svSendString( cnt, "</td>" );
	svSendString( cnt, "<td valign=\"top\" width=\"140\"><i><b>Incantations</b></i>" );
	for( b = 0 ; b < CMD_GHOSTOP_NUMUSED ; b++ ) {
		if( specopGhostsAllowed( b, a ) )
			svSendPrintf( cnt, "<br>%s", cmdGhostopName[b] );
	}
	svSendString( cnt, "</td></tr></table>" );
	svSendString( cnt, "<br><br>" );
}
iohttpBodyEnd( cnt );


 return;
}

void iohttpFunc_register( svConnectionPtr cnt ) {

iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, 2 );

svSendString ( cnt, "<br><br><h3>Register</h3><br>" );
svSendString ( cnt, "<b>Currently disabled, due to pre-alpha stage.</b>" );

//svSendString( cnt, "<form action=\"register2\" method=\"POST\">User name<br><input type=\"text\" name=\"name\"><br><br>Password<br><input type=\"password\" name=\"pass\"><br><br>Faction name<br><input type=\"text\" name=\"faction\"><br><br><input type=\"submit\" value=\"OK\"></form>" );

iohttpFunc_endhtml( cnt );


return;
}

void iohttpFunc_register2( svConnectionPtr cnt )
{
 int a, i, id;
 int session[4];
 char *name, *pass, *faction;
 FILE *file;
 iohttpDataPtr iohttp;
 char timebuf[256];
 char COREDIR[256];
 long long int newd[DB_USER_NEWS_BASE];
 dbMailDef maild;
 char Message[] = "Congratulations! You have successfully registered your account!<br>Good luck and have fun,<br><br>- Administration";

 iohttpVarsInit( cnt );
/* name = iohttpVarsFind( "name" );
 pass = iohttpVarsFind( "pass" );
 faction = iohttpVarsFind( "faction" );*/
 iohttpVarsCut();
 if( ( name ) && ( pass ) && ( faction ) ) {
	  if( ( id = cmdExecNewUser( name, pass, faction ) ) < 0 ) {
		iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, 2 );

		if( cmdErrorString )
			svSendString( cnt, cmdErrorString );
		else
			svSendString( cnt, "Error encountered while registering user" );

		svSendString( cnt, "<br><br><a href=\"register\">Try again?</a>" );
		goto iohttpFunc_register2L0;
	}
  	newd[0] = ticks.number;
	 	newd[1] = CMD_NEWS_FLAGS_NEW;
  newd[2] = CMD_NEWS_MAIL;
  newd[3] = 0;
  newd[4] = 0; //From the admin
  newd[5] = 0; //From the admin team
  memcpy( &newd[6], "Admin", 6 );
  cmdUserNewsAdd( id, newd, CMD_NEWS_FLAGS_MAIL );
  	
  	
		(maild.mail).length = strlen(Message);
		maild.text = Message;
		(maild.mail).authorid = 0;
  sprintf( (maild.mail).authorname, "Admin" );
  (maild.mail).authorempire = 0;
  (maild.mail).time = time( 0 )-(3600*SERVER_TIME_ZONE);
  (maild.mail).tick = ticks.number;
  (maild.mail).flags = 0;
  if( dbMailAdd( id, 0, &maild ) < 0 )
	syslog(LOG_ERR, "Error sending registration email\n" );

  
  if( ( dbUserLinkDatabase( cnt, id ) < 0 ) || ( dbSessionSet( cnt->dbuser, 0, session ) < 0 ) )
  {
   iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, 2 );
   svSendString( cnt, "Error encountered while registering session" );
   goto iohttpFunc_register2L0;
  }
  svSendPrintf( cnt, "Set-Cookie: USRID=%04x%04x%04x%04x%04x; path=/\n", id, session[0], session[1], session[2], session[3] );
  iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, 2 );

  svSendPrintf( cnt, "New user created<br>User name : %s<br>Password : %s<br>Faction name : %s<br>Account ID : %d<br>", name, pass, faction, id );

  sprintf( COREDIR, "%s/logs/register", sysconfig.directory );
  if( ( file = fopen( COREDIR, "ab" ) ) )
  {
   //fprintf( file, "Register ID %d ( %x )\n", id, id );
   a = time(0);
   strftime( timebuf, 256, "%T, %b %d %Y;", localtime( (time_t *)&a ) );
   fprintf( file, "Time %s", timebuf );
   fprintf( file, "Name %s;", name );
   fprintf( file, "Password %s;", pass );
   fprintf( file, "Faction %s;", faction );
   fprintf( file, "IP %s;", inet_ntoa( cnt->sockaddr.sin_addr ) );
   iohttp = cnt->iodata;
   strcpy(timebuf, iohttp->user_agent);
	  for(i=0;i<strlen(timebuf);i++)
	  {
	  	if(timebuf[i] == ';')
	  		timebuf[i] = ',';
	  }
	  fprintf( file, "%s;", timebuf );
   fprintf( file, "Cookie %s;;", iohttp->cookie );
   fprintf(file, "ID : %d ( %X );\n", id, id);
   fclose( file );
  }
} else {
iohttpBase( cnt, 8 );
if( ( id = iohttpIdentify( cnt, 4|1 ) ) < 0 )
	return;
iohttpFunc_frontmenu( cnt, 2 );
svSendString( cnt, "This account was not activated yet." );
}

 svSendString( cnt, "<form action=\"register3\" method=\"POST\"><br><br>Empire number<br><i>Leave blank to join a random empire</i><br><input type=\"text\" name=\"empire\"><br><br>" );
 svSendString( cnt, "Empire password<br><i>Only required if defined by the leader of the empire to join.</i><br><input type=\"text\" name=\"fampass\"><br><br>" );
 svSendString( cnt, "Faction race<br><i>The race of your people define many characteristics affecting different aspects of your faction.</i> - <a href=\"races\" target=\"_blank\">See races</a><br><select name=\"race\">" );
 for( a = 0 ; a < CMD_RACE_NUMUSED-1 ; a++ )
  svSendPrintf( cnt, "<option value=\"%d\">%s", a, cmdRaceName[a] );
 svSendString( cnt, "</select><br><br>" );

 svSendString( cnt, "<input type=\"submit\" value=\"OK\"></form>" );

 svSendString( cnt, "<br><br><a href=\"famranks\" target=\"_blank\">See empire rankings</a>" );
 svSendString( cnt, "<br><a href=\"rankings\" target=\"_blank\">See faction rankings</a>" );

 iohttpFunc_register2L0:
 iohttpFunc_endhtml( cnt );

 return;
}

void iohttpFunc_register3( svConnectionPtr cnt )
{
 int a, id, raceid;
 char *empire;
 char *fampass;
 char *race;

 iohttpBase( cnt, 8 );
 if( ( id = iohttpIdentify( cnt, 1|4 ) ) < 0 )
  return;
iohttpFunc_frontmenu( cnt, 2 );

 iohttpVarsInit( cnt );
 empire = iohttpVarsFind( "empire" );
 fampass = iohttpVarsFind( "fampass" );
 race = iohttpVarsFind( "race" );
 iohttpVarsCut();

 if( ( empire ) && ( race ) )
 {
  for( a = 0 ; a < 31 ; a++ )
  {
   if( ( fampass[a] == 10 ) || ( fampass[a] == 13 ) )
    break;
  }
		
		if( empire[0] == 0 )
  	a = -1;
  else if( empire[0] == '#' ) 
   sscanf( &empire[1], "%d", &a );
  else
   sscanf( empire, "%d", &a );
  sscanf( race, "%d", &raceid );
  
  if( cmdExecNewUserEmpire( id, a, fampass, raceid, (cnt->dbuser)->level ) < 0 )
  {
   if( cmdErrorString )
    svSendString( cnt, cmdErrorString );
   else
    svSendString( cnt, "Error encountered while registering user" );
   svSendString( cnt, "<br><br><a href=\"/register2\">Try again</a>" );
   goto iohttpFunc_register3L0;
  }
  svSendPrintf( cnt, "<b>Account activated!</b><br>" );
  svSendString( cnt, "<br><br><br><a href=\"/main\">Log in</a>" );
iohttpFunc_endhtml( cnt );
  return;
 }
 else
  svSendString( cnt, "Incorrect query!" );

 iohttpFunc_register3L0:
 svSendString( cnt, "<br><br><a href=\"/\">Main page</a><br><br><a href=\"/login\">Log in</a>" );
iohttpFunc_endhtml( cnt );
 return;
}


void iohttpFunc_login( svConnectionPtr cnt, int flag, char *text, ... ) {
	struct stat stdata;
	char *data;
	char DIRCHECKER[256];
	FILE *file;

iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, 0 );


if( strlen(text) ) {
	svSendPrintf( cnt, "<br>%s", text );
	if( flag ) {
		sprintf( DIRCHECKER, "%s/login.txt", sysconfig.httpread );
		if( stat( DIRCHECKER, &stdata ) != -1 ) {
			if( ( data = malloc( stdata.st_size + 1 ) ) ) {
				data[stdata.st_size] = 0;
				if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
					if( stdata.st_size > 0 ) {
						svSendString( cnt, "<br>" );
						while( fgets( data, stdata.st_size, file ) != NULL ) {
							svSendPrintf( cnt, "%s<br>", trimwhitespace(data) );
						}
					}
					fclose( file );
				}
				free( data );
			}
		}
	}
	svSendString( cnt, "<br><br>" );
} else {
svSendString( cnt, "<br><h3>Login</h3><br>" );
}
svSendString( cnt, "<form action=\"main\" method=\"POST\">Name<br><input type=\"text\" name=\"name\"><br><br>Password<br><input type=\"password\" name=\"pass\"><br><br><input type=\"submit\" value=\"OK\"></form>" );

svSendString( cnt, "</center></body></html>" );

iohttpFunc_endhtml( cnt );

return;
}

void iohttpFunc_endhtml( svConnectionPtr cnt ) {


svSendString( cnt, "</td><td width=\"7%\">&nbsp;</td></tr></table>" );
svSendString( cnt, "</td></tr></tbody></table>" );
svSendString( cnt, "</body></html>" );

return;
}

void iohttpFunc_boxstart( svConnectionPtr cnt, char *title ) {

svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\" background=\"images/i27.jpg\"><tr><td width=\"10%\"><img height=\"24\" src=\"images/i25.jpg\" width=\"22\"></td><td width=\"80%\" align=\"center\" nowrap><b><font face=\"verdana\" size=\"2\">" );
svSendPrintf( cnt, "%s", title);
svSendString( cnt, "</font></b></td><td width=\"10%\" align=\"right\"><img height=\"24\" src=\"images/i30.jpg\" width=\"62\"></td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"7\" background=\"images/i38.jpg\">&nbsp;</td><td bgcolor=\"#0b1119\"><br>" );

return;
}

void iohttpFunc_boxend( svConnectionPtr cnt ) {

svSendString( cnt, "<br><br></td><td width=\"7\" background=\"images/i43.jpg\">&nbsp;</td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"62\"><img height=\"12\" src=\"images/i45.jpg\" width=\"62\"></td><td width=\"100%\" background=\"images/i47.jpg\"><img height=\"12\" src=\"images/i47.jpg\" width=\"1\"></td><td width=\"62\"><img height=\"12\" src=\"images/i49.jpg\" width=\"62\"></td></tr></table>" );

return;
}

void iohttpFunc_front( svConnectionPtr cnt, char *text, ...  ) {
	dbUserMainDef maind;
	struct stat stdata;
	bool boxopen = false;
	char *data;
	char DIRCHECKER[256];
	FILE *file;
	int id;


iohttpBase( cnt, 8 );

if( ( id = iohttpIdentify( cnt, 2 ) ) >= 0 ) {
	if( dbUserMainRetrieve( id, &maind ) < 0 )
	return;
} 

iohttpFunc_frontmenu( cnt, 1 );

if( strlen(text) )
	svSendPrintf( cnt, "<b>%s</b><br><br>", text );
else
svSendString( cnt, "<br>" );

svSendString( cnt, "<td width=\"40%\" valign=\"top\">" );

//read notices from updates.txt and format for display. -- If this file is missing, or empty it is skipped.
sprintf( DIRCHECKER, "%s/updates.txt", sysconfig.httpread );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( !(boxopen) && ( strlen( trimwhitespace(data) ) ) ) {
						iohttpFunc_boxstart( cnt, trimwhitespace(data) );
						boxopen = true;
					} else if ( strlen( trimwhitespace(data) ) ) {
						svSendPrintf( cnt, "&nbsp;&nbsp;%s<br>", trimwhitespace(data) );
					}
					if( (boxopen) && ( strlen( trimwhitespace(data) ) == false ) ) {
						iohttpFunc_boxend( cnt );
						boxopen = false;
						svSendString( cnt, "<br><br>" );
					}
				}
			if(boxopen)
			iohttpFunc_boxend( cnt );
			svSendString( cnt, "<br>" );
			svSendString( cnt, "<br>" );
			}
			fclose( file );
		}
	free( data );
	}
}
//end notices

svSendString( cnt, "</td><td width=\"6%\">" );
svSendString( cnt, "&nbsp;" );
svSendString( cnt, "</td><td width=\"40%\" valign=\"top\">" );

svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td background=\"images/ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Log in</b></font></td></tr>" );
svSendString( cnt, "<tr><td>" );
svSendString( cnt, "<table cellspacing=\"8\"><tr><td>" );

if( (id < 0) || (strlen(text)) ) {
	svSendString( cnt, "<font size=\"2\"><form action=\"main\" method=\"POST\">Name<br><input type=\"text\" name=\"name\" size=\"24\"><br><br>Password<br><input type=\"password\" name=\"pass\" size=\"24\"><br><br><input type=\"submit\" value=\"Log in\"></form>" );
} else {
	svSendPrintf( cnt, "<br><b>You are already loged in as <i>%s</i></b><br>", cnt->dbuser->name );
	svSendString( cnt, "<br>" );
	svSendString( cnt, "<a href=\"/main\" target=\"_top\">Proceed to game</a>" );
	svSendString( cnt, "<br>" );
	svSendString( cnt, "<br>" );
	svSendString( cnt, "<a href=\"/logout\" target=\"_top\">Log out</a>" );
	svSendString( cnt, "<br>" );
	svSendString( cnt, "<br>" );
}
svSendString( cnt, "</td></tr></table>" );

//read the todo list from todo.txt and format for display. -- If this file is missing, or empty it is skipped.
sprintf( DIRCHECKER, "%s/todo.txt", sysconfig.httpread );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
			svSendString( cnt, "<i>Items on the admins to-do list :</i>" );
			svSendString( cnt, "<br>" );
			while( fgets( data, stdata.st_size, file ) != NULL ) {
				if( strlen(data) > 1 )
					svSendPrintf( cnt, "&nbsp;&#9734;&nbsp;&nbsp;%s<br>", trimwhitespace(data) );
			}
			svSendString( cnt, "<br>" );
			}
			fclose( file );
		}
		free( data );
	}
}
//end todo list

/*
svSendString( cnt, "<iframe src=\"images///www.facebook.com/plugins/like.php?href=http%3A%2F%2Fwww.facebook.com%2Fpages%2FEctroverse%2F133044593518078&amp;send=false&amp;layout=box_count&amp;width=450&amp;show_faces=false&amp;font=segoe+ui&amp;colorscheme=dark&amp;action=like&amp;height=90\" scrolling=\"no\" frameborder=\"0\" style=\"border:none; overflow:hidden; width:450px; height:90px;\" allowTransparency=\"true\"></iframe>\n" );

svSendString( cnt, "<br>\n" );
svSendString( cnt, "<br>\n" );
svSendString( cnt, "<script type=\"text/javascript\">\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "  var _gaq = _gaq || [];\n" );
svSendString( cnt, "    _gaq.push(['_setAccount', 'UA-38148306-1']);\n" );
svSendString( cnt, "      _gaq.push(['_trackPageview']);\n" );
svSendString( cnt, "\n" );      
svSendString( cnt, "        (function() {\n" );
svSendString( cnt, "            var ga = document.createElement('script'); ga.type =\n" );
svSendString( cnt, "            'text/javascript'; ga.async = true;\n" );
svSendString( cnt, "                ga.src = ('https:' == document.location.protocol ?\n" );
svSendString( cnt, "                'https://ssl' : 'http://www') +\n" );
svSendString( cnt, "                '.google-analytics.com/ga.js';\n" );
svSendString( cnt, "                    var s = document.getElementsByTagName('script')[0];\n" );
svSendString( cnt, "                    s.parentNode.insertBefore(ga, s);\n" );
svSendString( cnt, "                      })();\n" );
svSendString( cnt, "                      \n" );
svSendString( cnt, "                      </script>\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "<br>\n" );
*/
iohttpFunc_endhtml( cnt );
return;
}


void iohttpFunc_faq( svConnectionPtr cnt ) {
	struct stat stdata;
	char *data;
	char DIRCHECKER[256];
	FILE *file;

iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, 4 );

svSendString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\"><table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td background=\"images/ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Frequently Asked Question</b></font></td></tr>" );
svSendString( cnt, "<tr><td>" );
sprintf( DIRCHECKER, "%s/faq.html", sysconfig.httpread );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			fread( data, 1, stdata.st_size, file );
			svSendString( cnt, trimwhitespace(data) );
			fclose( file );
		}
		free( data );
	}
}

svSendString( cnt, "</td></tr></table></td><td width=\"7%\">&nbsp;</td></tr>" );

iohttpFunc_endhtml( cnt );
return;
}


void iohttpFunc_gettingstarted( svConnectionPtr cnt ) {

iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, 5 );

svSendString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\">" );


svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td background=\"images/ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Getting started in the galaxy of Ectroverse</b></font></td></tr>" );
svSendString( cnt, "<tr><td><font size=\"2\">" );
svSendString( cnt, "<center>This page is a basic guide to new players on how to get started.<br>It is important to read this to get the best possible experience when first playing!</center>" );

svSendString( cnt, "<br><b>Creating an account:</b><br>" );
svSendString( cnt, "<a href=\"#a0\">0. Registering the account.</a><br>" );
svSendString( cnt, "<a href=\"#a1\">1. The user name and faction name.</a><br>" );
svSendString( cnt, "<a href=\"#a2\">2. Joining an empire.</a><br>" );
svSendString( cnt, "<a href=\"#a3\">3. Choosing your race.</a><br>" );
svSendString( cnt, "<a href=\"#a4\">4. Completion and logging in.</a><br>" );
svSendString( cnt, "</a><br>" );

svSendString( cnt, "<br><b>Playing the game:</b><br>" );
svSendString( cnt, "<a href=\"#b0\">0. Resources & Buildings.</a><br>" );
svSendString( cnt, "<a href=\"#b1\">1. Planets.</a><br>" );
svSendString( cnt, "<a href=\"#b2\">2. Research.</a><br>" );
svSendString( cnt, "<a href=\"#b3\">3. Military.</a><br>" );
svSendString( cnt, "</a><br>" );

svSendString( cnt, "<br><b>Tips:</b><br>" );
svSendString( cnt, "<a href=\"#c0\">0. Map generation.</a><br>" );
svSendString( cnt, "<a href=\"#c1\">1. Fleet page.</a><br>" );
svSendString( cnt, "<a href=\"#c2\">2. Account page.</a><br>" );
svSendString( cnt, "</a><br>" );

svSendString( cnt, "</td></tr></table><br><br>" );


svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td background=\"images/ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Creating an account:</b></font></td></tr>" );
svSendString( cnt, "<tr><td><font size=\"2\"><br>" );

svSendString( cnt, "<a name=\"a0\"><b><i>0. Registering the account.</i></b></a><br>" );
svSendString( cnt, "The great thing about Ectroverse is that the signup is simple. You won't need to input your e-mail address and no other activation is required." );
svSendString( cnt, "<br><br>" );

svSendString( cnt, "<a name=\"a1\"><b><i>1. The user name and faction name.</i></b></a><br>" );
svSendString( cnt, "Your user name is something personal. Other players won't see this.<br>" );
svSendString( cnt, "Your Faction name however, is your handle in the game. Choose carefully.<br>" );
svSendString( cnt, "<br>" );

svSendString( cnt, "<a name=\"a2\"><b><i>2. Joining an Empire.</i></b></a><br>" );
svSendString( cnt, "Being part of an Empire is one of the most important aspects of the game.<br>" );
svSendString( cnt, "You start out small and will need other players to grow.<br>" );
svSendString( cnt, "Know anyone in the game? Ask them for their empire number and password.<br>" );
svSendString( cnt, "<br>" );
svSendString( cnt, "If you want to start an Empire of your own, look at the <a href=\"famranks\">Empire</a> rankings and pick a number not yet in the list!" );
svSendString( cnt, "<br>Want to team up with random players? Leave blank. But remember, be a teamplayer and you'll earn a rank in the Empire.<br>" );
svSendString( cnt, "<br>" );

svSendString( cnt, "<a name=\"a3\"><b><i>3. Choosing your race.</i></b></a><br>" );
svSendString( cnt, "You are well on your way to making a name for yourself.<br>" );
svSendString( cnt, "But how will people remember you? As an aggressive attacker? A proud and rich Energy provider? A self made and self sufficient powerhouse?<br>" );
svSendString( cnt, "<br>" );
svSendString( cnt, "Your race will decide which path you will walk.<br>" );
svSendString( cnt, "View the stats for each race <a href=\"races\">here</a>.<br><br>" );

svSendString( cnt, "<a name=\"a4\"><b><i>4. Completion and logging in.</i></b></a><br>" );
svSendString( cnt, "Congratulations. You have created an account, chosen an Empire to fight for and selected your race.<br>" );
svSendString( cnt, "You are now ready to explore, build and fight your way to the top.<br>" );
svSendString( cnt, "<br>" );

svSendString( cnt, "</td></tr></table><br><br>" );

svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td background=\"images/ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Playing the game:</b></font></td></tr>" );
svSendString( cnt, "<tr><td><font size=\"2\"><br>" );

svSendString( cnt, "<a name=\"b0\"><b><i>0. Resources & Buildings.</i></b></a><br>" );
svSendString( cnt, "Your main concern is growth. You need to spend your resources by making resources.<br>" );
svSendString( cnt, "How do you do that? Simple. Build buildings on your planets.<br>" );
svSendString( cnt, "There are 4 resources in the galaxy. Energy, Minerals, Crystal and Ectrolium. <br>" );
svSendString( cnt, "Most buildings need 3 of the 4 resources to be built, so you need to plan your building a bit at the start.<br><br>" );

svSendString( cnt, "<a name=\"b1\"><b><i>1. Planets.</i></b></a><br>" );
svSendString( cnt, "Exploring and conquering planets is your way of getting to the top.<br>" );
svSendString( cnt, "You start out with 4 exploration ships. Send these out into the galaxy to colonise planets.<br>" );
svSendString( cnt, "Don't worry, you'll be building a lot of them soon enough. Afterall, who is pleased with <i>just</i> 4 planets....<br><br>" );

svSendString( cnt, "<a name=\"b2\"><b><i>2. Research.</i></b></a><br>" );
svSendString( cnt, "Get smarter and do it smart.<br>" );
svSendString( cnt, "Research is a very important aspect of the game.<br>" );
svSendString( cnt, "Head over to your Research tab and distribute points wisely!<br><br>" );

svSendString( cnt, "<a name=\"b3\"><b><i>3. Military.</i></b></a><br>" );
svSendString( cnt, "A big fleet makes you strong, but they also increase your Networth.<br>" );
svSendString( cnt, "A higher networth makes it harder to grow.<br>" );
svSendString( cnt, "Build up your fleet steadily, don't go overboard.<br>" );
svSendString( cnt, "And remember, you need to pay upkeep for your little Cruisers too!<br><br>" );

svSendString( cnt, "</td></tr></table><br><br>" );



svSendString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td background=\"images/ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Tips:</b></font></td></tr>" );
svSendString( cnt, "<tr><td><font size=\"2\"><br>" );

svSendString( cnt, "<a name=\"c0\"><b><i>0. Map generation</i></b></a><br>" );
svSendString( cnt, "The galaxy map is the place where you'll find planets to explore and conquer... But it is easy to lose your way and location." );
svSendString( cnt, "<br>Luckily you can generate a map using the Generation page." );
svSendString( cnt, "<br>You can filter away planets that are already owned, that are of your Empire or are that of enemies!" );
svSendString( cnt, "<br>A usefull tool to say the least.<br><br>" );

svSendString( cnt, "<a name=\"c1\"><b><i>1. Fleet page.</i></b></a><br>" );
svSendString( cnt, "Check your fleet page. In the top right you see <b>\"Fleet Orders\"</b>, select the dropdown and chose <b>\"Wait in System\"</b> instead of the default one." );
svSendString( cnt, "<br>It is easy to forget this option, and spend 10 minutes wondering where your mighty fleet has gone!.<br><br>" );

svSendString( cnt, "<a name=\"c2\"><b><i>2. Account page.</i></b></a><br>" );
svSendString( cnt, "This will here page allows you to alter your Faction name when the game hasn't started yet." );
svSendString( cnt, "<br>Once the time starts flowing, you won't be able to change your name until the next round has started.<br><br>" );


svSendString( cnt, "</td></tr></table><br><br>" );

iohttpFunc_endhtml( cnt );

return;
}

void iohttpFunc_halloffame( svConnectionPtr cnt ) {
	struct stat stdata;
	char *data;
	char DIRCHECKER[256];
	FILE *file;

iohttpBase( cnt, 8 );
iohttpFunc_frontmenu( cnt, 6 );

svSendString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\"><table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
svSendString( cnt, "<tr><td background=\"images/ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Hall of Fame / Server Rankings</b></font></td></tr>" );
svSendString( cnt, "<tr><td>" );
sprintf( DIRCHECKER, "%s/halloffame.html", sysconfig.httpread );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			fread( data, 1, stdata.st_size, file );
			svSendString( cnt, data );
			fclose( file );
		}
		free( data );
	}
}

svSendString( cnt, "</td></tr></table><br><br><br><br><br><br><br><br></td><td width=\"7%\">&nbsp;</td></tr>" );

iohttpFunc_endhtml( cnt );
return;
}




