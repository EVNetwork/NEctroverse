#ifndef CONFIGINCLUDED
#include "config.h"
#endif
#ifndef GLOBALINCLUDED
#include "global.h"
#endif


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

int iohttpIdentify( svConnectionPtr cnt, int action )
{
 int a, b, id, session[4];
 char *src;
 FILE *file;
 struct stat stdata;
 char *data;
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
 for( a = 0 ; a < 4 ; a++ )
 {
  b = iohttpIdentifyHex( &src[4+(a<<2)] );
  if( session[a] != b )
   goto iohttpIdentifyL0;
 }

 if(( action & 2 )&&(cnt->dbuser))
 {
  if( !( (cnt->dbuser)->flags & CMD_USER_FLAGS_ACTIVATED ) && ( (cnt->dbuser)->level < LEVEL_MODERATOR ) )
  {
   if( action & 1 )
   {
    if( action & 8 )
     iohttpBase( cnt, 1|2 );
    svSendString( cnt, "This account has not been activated yet.</body></html>" );
   }
   return -1;
  }
 }
 if( action & 4 )
 {
  if( (cnt->dbuser)->flags & CMD_USER_FLAGS_ACTIVATED )
  {
   if( action & 1 )
   {
    if( action & 8 )
     iohttpBase( cnt, 1|2 );
    svSendString( cnt, "This account has been activated.</body></html>" );
   }
   return -1;
  }
 }

 return id;

 iohttpIdentifyL0:
 if( action & 1 )
 {
  if( action & 8 )
   iohttpBase( cnt, 1|2 );
  if( stat( IOHTTP_READ_DIRECTORY "/login.html", &stdata ) != -1 )
  {
   if( ( data = malloc( stdata.st_size + 1 ) ) )
   {
    data[stdata.st_size] = 0;
    if( ( file = fopen( IOHTTP_READ_DIRECTORY "/login.html", "rb" ) ) )
    {
     fread( data, 1, stdata.st_size, file );
     svSendString( cnt, data );
     fclose( file );
    }
    free( data );
   }
  }
  else
   svSendString( cnt, "<br><br>You are not logged in!<br><a href=\"login\" target=\"_top\">Log in</a><br><br><br>If you were playing just a few seconds ago, the server program was probably updated and restarted.<br><br>Information about any important update can be found on the <a href=\"/\" target=\"_top\">front page</a> or the <a href=\"/forum?forum=0\">Announcements forum</a></body></html>" );
 }

 return -1;
}

void iohttpBase( svConnectionPtr cnt, int flags )
{
	
	svSendString( cnt, "Content-Type: text/html\n\n" );
 svSendString( cnt, "<html><head>");
 if( flags & 4 )
  svSendString( cnt, "<base target=\"_blank\">" );
 
 svSendString( cnt, "<style type=\"text/css\">body,td{font-size:smaller;font-family:verdana,geneva,arial,helvetica,sans-serif;}a:hover{color:#00aaaa}</style></head><body bgcolor=\"#000000\" text=\"#C0D0D8\" link=\"#FFFFFF\" alink=\"#FFFFFF\" vlink=\"#FFFFFF\"" );
 if( flags & 1 )
 {
  svSendString( cnt, " background=\"mbg.gif\"" );
  if( !( flags & 2 ) )
   svSendString( cnt, " bgproperties=\"fixed\"" );
 }
 svSendString( cnt, " marginheight=\"0\" topmargin=\"0\"><center>" );
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
 svSendString( cnt, "<td width=\"35\" rowspan=\"2\"><img height=\"63\" src=\"i02.jpg\" width=\"35\"></td>" );
 svSendString( cnt, "<td width=\"100%\" height=\"21\" align=\"center\">" );

 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" background=\"i05.jpg\" border=\"0\" align=\"center\">" );
 svSendString( cnt, "<tr>" );
 svSendString( cnt, "<td width=\"41\"><img height=\"21\" src=\"i03.jpg\" width=\"41\"></td>" );
 svSendString( cnt, "<td background=\"i05.jpg\">" );

 if( svTickStatus )
 svSendPrintf( cnt, "<table width=\"100%%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Networth : %lld</b></font></td><td width=\"40%%\" align=\"center\"><font size=\"1\"><b>Tick time : %d seconds left</b></font></td><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Population : %lld0</b></font></td></tr></table>", mainp->networth, (int)( svTickTime - time(0) ), mainp->ressource[CMD_RESSOURCE_POPULATION] );
 else
 svSendPrintf( cnt, "<table width=\"100%%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Networth : %lld</b></font></td><td width=\"40%%\" align=\"center\"><font size=\"1\"><b>Tick time : time frozen</b></font></td><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Population : %lld0</b></font></td></tr></table>", mainp->networth, mainp->ressource[CMD_RESSOURCE_POPULATION] );

 svSendString( cnt, "</td><td width=\"78\"><img height=\"21\" src=\"i07.jpg\" width=\"78\"></td></tr></table>" );
 svSendString( cnt, "</td></tr>" );

 svSendString( cnt, "<tr><td width=\"100%\" align=\"center\">" );
 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" background=\"i15.jpg\" border=\"0\" align=\"center\"><tr>" );

 a = dbUserNewsGetFlags( id );
 svSendString( cnt, "<td width=\"41\"><a href=\"hq\"><img height=\"42\" title=\"mail\" src=\"i09" );
 if( a & CMD_NEWS_FLAGS_MAIL )
  svSendString( cnt, "a" );
 svSendString( cnt, ".jpg\" width=\"41\" border=\"0\"></a></td>" );

 svSendString( cnt, "<td width=\"40\"><a href=\"hq\"><img height=\"42\" title=\"reports\" src=\"i10" );
 if( a & CMD_NEWS_FLAGS_BUILD )
  svSendString( cnt, "a" );
 svSendString( cnt, ".jpg\" width=\"40\" border=\"0\"></a></td>" );

 svSendString( cnt, "<td width=\"39\"><a href=\"hq\"><img height=\"42\" title=\"economy\" src=\"i11" );
 if( a & CMD_NEWS_FLAGS_AID )
  svSendString( cnt, "a" );
 svSendString( cnt, ".jpg\" width=\"39\" border=\"0\"></a></td>" );

 svSendString( cnt, "<td width=\"39\"><a href=\"hq\"><img height=\"42\" title=\"fleets\" src=\"i12" );
 if( a & CMD_NEWS_FLAGS_ATTACK )
  svSendString( cnt, "a" );
 else if( a & CMD_NEWS_FLAGS_FLEET )
  svSendString( cnt, "b" );
 svSendString( cnt, ".jpg\" width=\"39\" border=\"0\"></a></td>" );

 svSendString( cnt, "<td width=\"18\"><img height=\"42\" src=\"i13.jpg\" width=\"18\"></td>" );
 svSendString( cnt, "<td width=\"100%\" background=\"i15.jpg\" align=\"left\">" );

 svSendString( cnt, "<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\"><tr>" );
 svSendString( cnt, "<td width=\"50%\" align=\"center\" nowrap><font size=\"1\"><b>" );
 svSendPrintf( cnt, "Energy: %lld<br>Mineral: %lld</b></font></td><td width=\"50%%\" align=\"center\" nowrap><font size=\"1\"><b>Crystal: %lld<br>Ectrolium: %lld</b></font>", mainp->ressource[0], mainp->ressource[1], mainp->ressource[2], mainp->ressource[3] );
 svSendString( cnt, "</td></tr></table>" );

 svSendString( cnt, "</td><td width=\"49\"><img height=\"42\" src=\"i17.jpg\" width=\"49\"></td></tr></table>" );

 svSendString( cnt, "</td></tr></table><br>" );

 return 1;
}


void iohttpBodyInit( svConnectionPtr cnt, char *title, ... )
{
 char text[4096];
 va_list ap;
 va_start( ap, title );
 vsnprintf( text, 4096, title, ap );
 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\" background=\"i27.jpg\"><tr><td width=\"10%\"><img height=\"24\" src=\"i25.jpg\" width=\"22\"></td><td width=\"80%\" align=\"center\" nowrap><b><font face=\"verdana\" size=\"2\">" );
 svSendString( cnt, text );
 svSendString( cnt, "</font></b></td><td width=\"10%\" align=\"right\"><img height=\"24\" src=\"i30.jpg\" width=\"62\"></td></tr></table>" );
 svSendString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"7\" background=\"i38.jpg\">&nbsp;</td><td bgcolor=\"#0b1119\" align=\"center\"><br>" );
 return;
}

void iohttpBodyEnd( svConnectionPtr cnt )
{
 svSendString( cnt, "<br></td><td width=\"7\" background=\"i43.jpg\">&nbsp;</td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"62\"><img height=\"12\" src=\"i45.jpg\" width=\"62\"></td><td width=\"100%\" background=\"i47.jpg\"><img height=\"12\" src=\"i47.jpg\" width=\"1\"></td><td width=\"62\"><img height=\"12\" src=\"i49.jpg\" width=\"62\"></td></tr></table></center><br><br><br></body></html>" );
 return;
}



void iohttpFunc_races( svConnectionPtr cnt )
{
 iohttpBase( cnt, 0 );
 svSendPrintf( cnt, "<br><b>%s races</b><br><br>", SERVERNAME );

 svSendString( cnt, "<h3>Harks</h3><table width=\"620\" border=\"0\"><tr>" );
 svSendString( cnt, "<td valign=\"top\" width=\"340\"><i>Main bonuses</i><br>-10% Energy production<br>+20% Research production <br>-20% Population upkeep reduction<br>+40% Attack strength<br>-10% Defence strength<br>+40% Travel speed<br><br>" );
 svSendString( cnt, "<i>Special</i><br>Fighters units 20% stronger<br>Higher crystal production by 25%<br>Military Research production limit of 250%<br>Halved culture Research production </td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Operations</i><br>Network virus<br>Infiltration<br>Bio infection<br>Military sabotage<br>Nuke Planet<br><br><i>Spells</i><br>Irradiate Ectrolium<br>Incandescence<br>Black Mist<br>War Illusions</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Incantations</i><br>Portal Force Field<br>Vortex Portal<br>Energy Surge</td>" );
 svSendString( cnt, "</tr></table><br><br>" );

 svSendString( cnt, "<h3>Manticarias</h3><table width=\"620\" border=\"0\"><tr>" );
 svSendString( cnt, "<td valign=\"top\" width=\"340\"><i>Main bonuses</i><br>+40% Energy production<br>-10% Research production<br>-10% Population upkeep reduction<br>+40% Psychics strength<br>+20% Ghost Ships strength<br>-30% Attack strength<br>+10% Defence strength<br><br>" );
 svSendString( cnt, "<i>Special</i><br>Higher solars collectors efficiency by 15%<br>Doubled culture Research production </td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Operations</i><br>Spy Target<br>Observe Planet<br>Energy Transfer<br><br><i>Spells</i><br>Dark Web<br>Black Mist<br>War Illusions<br>Psychic Assault<br>Phantoms</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Incantations</i><br>Planetary Shielding<br>Mind Control</td>" );
 svSendString( cnt, "</tr></table><br><br>" );

 svSendString( cnt, "<h3>Foohons</h3><table width=\"620\" border=\"0\"><tr>" );
 svSendString( cnt, "<td valign=\"top\" width=\"340\"><i>Main bonuses</i><br>-20% Energy production<br>+50% Research production<br>+10% Population upkeep reduction<br>+10% Ghost Ships strength<br>+20% Attack strength<br>+10% Defence strength<br><br>" );
 svSendString( cnt, "<i>Special</i><br>Each 4000 population produces 1 research point each week<br>Ectrolium production increased by 20%</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Operations</i><br>Spy Target<br>Observe Planet<br>Infiltration<br>Military Sabotage<br>High Infiltration<br>Planetary Beacon<br><br><i>Spells</i><br>Irradiate Ectrolium<br>Dark Web<br>Incandescence<br>Psychic Assault</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Incantations</i><br>Sense Artefact<br>Survey System<br>Vortex Portal<br>Mind Control</td>" );
 svSendString( cnt, "</tr></table><br><br>" );

 svSendString( cnt, "<h3>Spacebornes</h3><table width=\"620\" border=\"0\"><tr>" );
 svSendString( cnt, "<td valign=\"top\" width=\"340\"><i>Main bonuses</i><br>+30% Energy production<br>+10% Research production<br>+20% Population upkeep reduction<br>-30% Psychics strength<br>+30% Agents strengths<br>+20% Defence strength<br>+80% Travel speed<br><br>" );
 svSendString( cnt, "<i>Special</i><br>Soldiers and droids units 10% stronger<br>Energy production Research production limit of 250%<br>Halved technology Research production </td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Operations</i><br>Spy Target<br>Observe Planet<br>Network Virus<br>Bio Infection<br>Energy Transfer<br>Nuke Planet<br>Planetary Beacon<br><br><i>Spells</i><br>Irradiate Ectrolium<br>Incandescence<br>Black Mist</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Incantations</i><br>Survey System<br>Planetary Shielding<br></td>" );
 svSendString( cnt, "</tr></table><br><br>" );

 svSendString( cnt, "<h3>Dreamweavers</h3><table width=\"620\" border=\"0\"><tr>" );
 svSendString( cnt, "<td valign=\"top\" width=\"340\"><i>Main bonuses</i><br>-20% Energy production<br>+40% Research production<br>-10% Population upkeep reduction<br>+50% Psychics strength<br>+30% Ghost Ships strength<br>-30% Defence strength<br><br>" );
 svSendString( cnt, "<i>Special</i><br>Doubled technology Research production <br>Higher crystal production by 25%<br>Construction Research production limit of 250%<br>Halved military Research production </td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Operations</i><br>Network Virus<br>Bio Infection<br>Energy Transfer<br>Military Sabotage<br><br><i>Spells</i><br>Irradiate Ectrolium<br>Dark Web<br>Incandescence<br>Black Mist<br>War Illusions<br>Psychic Assault<br>Phantoms</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Incantations</i><br>Sense Artefact<br>Portal Force Field<br>Mind Control<br>Energy Surge</td>" );
 svSendString( cnt, "</tr></table><br><br>" );
/*	
	svSendString( cnt, "<h3>Furtifons</h3><table width=\"620\" border=\"0\"><tr>" );
 svSendString( cnt, "<td valign=\"top\" width=\"340\"><i>Main bonuses</i><br>-10% Attack strength<br>-10% Research production<br>-10% Population upkeep reduction<br>+20% Agents strengths<br>+20% Ghost Ships strength<br>+60% Travel speed<br><br>" );
 svSendString( cnt, "<i>Special</i><br>Culture Research production provides a psychic shield for planets<br>Double Operations research<br>Construction Research production limit of 250%</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Operations</i><br>Spy Target<br>Observe Planet<br>Energy Transfer<br>Infiltration<br>High Infiltration<br>Planetary Beacon<br><br><i>Spells</i><br>Incandescence<br>War Illusions</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Incantations</i><br>Sense Artefact<br>Survey System<br>Planetary Shielding<br>Portal Force Field<br>Vortex Portal</td>" );
	svSendString( cnt, "</tr></table><br><br>" );
	
	svSendString( cnt, "<h3>Samsonites</h3><table width=\"620\" border=\"0\"><tr>" );
 svSendString( cnt, "<td valign=\"top\" width=\"340\"><i>Main bonuses</i><br>+30% Attack strength<br>+20% Defence strength<br>+10% Energy production<br>+10% Research production<br>-10% Agents strength<br>-10% Psychics strength<br>-20% Travel speed<br><br>" );
 svSendString( cnt, "<i>Special</i><br>Ignore Dark Webs effects<br>185% Research production max</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Operations</i><br>Bio Infection<br>Military Sabotage<br>Nuke Planet<br><br><i>Spells</i><br>Black Mist<br>Phantoms</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Incantations</i><br>Survey System<br>Mind Control<br>Energy Surge</td>" );
	svSendString( cnt, "</tr></table><br><br>" );

	svSendString( cnt, "<h3>Ultimums</h3><table width=\"620\" border=\"0\"><tr>" );
 svSendString( cnt, "<td valign=\"top\" width=\"340\"><i>Main bonuses</i><br>+70% stronger for all units<br><br>" );
 svSendString( cnt, "<i>Special</i><br>Double Research production points production <br>All Research production maximum are 250%<br>Each 4000 population produces a Research production point<br>Higher solars collectors efficiency by 15%</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Operations</i><br>Spy Target<br>Observe Planet<br>Network Virus<br>Infiltration<br>Bio Infection<br>Energy Transfer<br>Military Sabotage<br><br>Nuke Planet<br>High Infiltration<br>Planetary Beacon<br><i>Spells</i><br>Irradiate Ectrolium<br>Dark Web<br>Incandescence<br>Black Mist<br>War Illusions<br>Psychic Assault<br>Phantoms</td>" );
 svSendString( cnt, "<td valign=\"top\" width=\"140\"><i>Incantations</i><br>Sense Artefact<br>Survey System<br>Planetary Shielding<br>Portal Force Field<br>Vortex Portal<br>Mind Control<br>Energy Surge</td>" );
 svSendString( cnt, "</tr></table><br><br>" );
*/
 svSendString( cnt, "<br><br><br>" );

/*
Psychic Assault - attemps to kill the psychics of another faction, causing psychics casualities on both sides.

Sense Artefact - Attempts to locate the nearest artefact, from a particular system
Survey System - Attempt to determine the size, resource bonus and the precense of portals for each planet of a solar system
Planetary Shielding - Create temporary shielding protecting defensive units for a specific planet
Portal Force Field - Create a force field around a specific planet, making portal travel very difficult, reducing the owner capabilities to protect it
Vortex Portal - Create a temporary portal in the targeted system from which fleets can attack and return to the main fleet
Mind Control - Take control of an enemy planet with all its infrastructure, most of the population is killed
Energy Surge - Spreads a destructive wave in an faction network, feeding on the faction's energy reserves ; destroying resource reserves, research and buildings
*/

 return;
}

void iohttpFunc_register( svConnectionPtr cnt )
{
 iohttpBase ( cnt, 0 );
  svSendString ( cnt, "<br><br><h3>Register</h3><br>" );
  svSendString( cnt, "Check <a href=\"gettingstarted\">this</a> page to get you started.<br><br>" );
  svSendString( cnt, "<form action=\"register2\" method=\"POST\">User name<br><input type=\"text\" name=\"name\"><br><br>Password<br><input type=\"password\" name=\"pass\"><br><br>Faction name<br><input type=\"text\" name=\"faction\"><br><br><input type=\"submit\" value=\"OK\"></form>" );
  svSendString( cnt, "</center></body></html>" );
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
 name = iohttpVarsFind( "name" );
 pass = iohttpVarsFind( "pass" );
 faction = iohttpVarsFind( "faction" );
 iohttpVarsCut();
 if( ( name ) && ( pass ) && ( faction ) )
 {
  if( ( id = cmdExecNewUser( name, pass, faction ) ) < 0 )
  {
   iohttpBase( cnt, 0 );
   if( cmdErrorString )
    svSendString( cnt, cmdErrorString );
   else
    svSendString( cnt, "Error encountered while registering user" );
   goto iohttpFunc_register2L0;
  }
  	newd[0] = svTickNum;
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
  (maild.mail).tick = svTickNum;
  (maild.mail).flags = 0;
  if( dbMailAdd( id, 0, &maild ) < 0 )
	syslog(LOG_ERR, "Error sending registration email\n" );

  
  if( ( dbUserLinkDatabase( cnt, id ) < 0 ) || ( dbSessionSet( cnt->dbuser, 0, session ) < 0 ) )
  {
   iohttpBase( cnt, 0 );
   svSendString( cnt, "Error encountered while registering session" );
   goto iohttpFunc_register2L0;
  }
  svSendPrintf( cnt, "Set-Cookie: USRID=%04x%04x%04x%04x%04x; path=/\n", id, session[0], session[1], session[2], session[3] );
  iohttpBase( cnt, 0 );

  svSendPrintf( cnt, "New user created<br>User name : %s<br>Password : %s<br>Faction name : %s<br>Account ID : %d<br>", name, pass, faction, id );
/*
  sprintf( COREDIR, "%s/logs/register", COREDIRECTORY );
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
  }*/
 }
 else
 {
  iohttpBase( cnt, 0 );
  if( ( id = iohttpIdentify( cnt, 4|1 ) ) < 0 )
   return;
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
 svSendString( cnt, "</center></body></html>" );

 return;
}

void iohttpFunc_register3( svConnectionPtr cnt )
{
 int a, id, raceid;
 char *empire;
 char *fampass;
 char *race;

 iohttpBase( cnt, 0 );
 if( ( id = iohttpIdentify( cnt, 1|4 ) ) < 0 )
  return;

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
  svSendString( cnt, "<br><br><a href=\"/\">Main page</a><br><br><a href=\"/main\">Log in</a>" );
  svSendString( cnt, "</center></body></html>" );
  return;
 }
 else
  svSendString( cnt, "Incorrect query!" );

 iohttpFunc_register3L0:
 svSendString( cnt, "<br><br><a href=\"/\">Main page</a><br><br><a href=\"/login\">Log in</a>" );
 svSendString( cnt, "</center></body></html>" );
 return;
}


void iohttpFunc_login( svConnectionPtr cnt )
{
 iohttpBase( cnt, 0 );

 svSendString( cnt, "<br><br><h3>Login</h3><br>" );
 svSendString( cnt, "<form action=\"main\" method=\"POST\">Name<br><input type=\"text\" name=\"name\"><br><br>Password<br><input type=\"password\" name=\"pass\"><br><br><input type=\"submit\" value=\"OK\"></form>" );

 svSendString( cnt, "</center></body></html>" );

 return;
}

