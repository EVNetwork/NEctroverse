

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

int iohtmlIdentify( ReplyDataPtr cnt, int action ) {
	int id;
	char sess[SESSION_SIZE];

if( NULL == (cnt->session)->dbuser )
	goto iohtmlIdentifyL0;

if( dbSessionRetrieve( (cnt->session)->dbuser, sess ) < 0 )
	goto iohtmlIdentifyL0;

if( strcmp( (cnt->session)->sid, sess )  )
	goto iohtmlIdentifyL0;

id = ((cnt->session)->dbuser)->id;
((cnt->session)->dbuser)->lasttime = time(NULL);

if( dbUserSave( id, (cnt->session)->dbuser ) < 0 )
	error( "Database UserSave" );

if(( action & 2 )&&((cnt->session)->dbuser)) {
	if( !( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) && ( ((cnt->session)->dbuser)->level < LEVEL_MODERATOR ) ) {
		if( action & 1 ) {
			if( action & 8 )
			iohtmlBase( cnt, 1|2 );
			httpString( cnt, "This account has not been activated yet.</body></html>" );
		}
		return -1;
	}
}

if( action & 4 ) {
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) {
		if( action & 1 ) {
			if( action & 8 )
				iohtmlBase( cnt, 1|2 );
			httpString( cnt, "This account has been activated.</body></html>" );
		}
		return -1;
	}
}


return id;

iohtmlIdentifyL0:

if( action & 1 ) {

	iohtmlFunc_login( cnt, 1, "Your session has expired, you need to login again.<br>If you were playing just a few seconds ago, the server program was probably updated and restarted." );

}


return -1;
}

void iohtmlBase( ReplyDataPtr cnt, int flags ) {
	ConfigArrayPtr settings;

settings = GetSetting( "Server Name" );
httpString( cnt, "<!DOCTYPE xhtml>");
httpString( cnt, "<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en\" xml:lang=\"en\"><head>");
httpString( cnt, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" );
httpString( cnt, "<meta http-equiv=\"Content-Style-Type\" content=\"text/css\">" );
httpString( cnt, "<meta http-equiv=\"Content-Language\" content=\"en\">" );
httpString( cnt, "<meta http-equiv=\"imagetoolbar\" content=\"no\">" );
httpPrintf( cnt, "<title>%s</title>", settings->string_value );
httpString( cnt, "<link rel=\"icon\" href=\"files?type=image&name=favicon.ico\">" );
httpString( cnt, "<link href=\"files?type=server&name=style.css\" rel=\"stylesheet\" type=\"text/css\" media=\"screen\">" );
httpString( cnt, "<script type=\"text/javascript\" src=\"files?type=server&name=javascript.min.js\"></script>" );
httpString( cnt, "<script type=\"text/javascript\" src=\"files?type=server&name=jquery-1.10.2.min.js\"></script>" );
httpString( cnt, "<script type=\"text/javascript\" src=\"ajax.js\"></script>" );

if( flags & 16 )
	httpString( cnt, "<script type=\"text/javascript\" src=\"files?type=server&name=status.js\"></script>" );
if( flags & 4 )
	httpString( cnt, "<base target=\"_blank\">" );
if( flags & 1 ) {
	httpString( cnt, "<style type=\"text/css\">" );
	httpString( cnt, "body{background-image:url(files?type=image&name=mbg.gif);" );
	if( !( flags & 2 ) )
		httpString( cnt, "background-attachment:fixed;" );
	httpString( cnt, "}</style>" );
}

httpString( cnt, "</head>" );
httpString( cnt, "<body" );

if( flags & 8 )
	httpString( cnt, " onload=\"if (window != window.top) { top.location.href=location.href }; countDown();\" " );

httpString( cnt, ">" );

httpString( cnt, "<center>" );

return;
}

int iohtmlHeader( ReplyDataPtr cnt, int id, dbUserMainPtr mainp )
{
 int a;
 if( dbUserMainRetrieve( id, mainp ) < 0 )
 {
  httpString( cnt, "Error while retriving user's main data</center></body></html>" );
  return 0;
 }

 httpString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\" align=\"center\">" );

 httpString( cnt, "<tr>" );
 httpString( cnt, "<td width=\"35\" rowspan=\"2\"><img height=\"63\" src=\"files?type=image&name=i02.jpg\" width=\"35\"></td>" );
 httpString( cnt, "<td width=\"100%\" height=\"21\" align=\"center\">" );

 httpString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" background=\"files?type=image&name=i05.jpg\" border=\"0\" align=\"center\">" );
 httpString( cnt, "<tr>" );
 httpString( cnt, "<td width=\"41\"><img height=\"21\" src=\"files?type=image&name=i03.jpg\" width=\"41\"></td>" );
 httpString( cnt, "<td background=\"files?type=image&name=i05.jpg\">" );

 if( ticks.status )
 httpPrintf( cnt, "<table width=\"100%%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Networth : <span id=\"headernetworth\">%lld</span></b></font></td><td width=\"40%%\" align=\"center\"><font size=\"1\"><b>Next tick : <span id=\"headerTime\">%d seconds</b></span></font></td><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Population : <span id=\"headerpopulation\">%lld</span>0</b></font></td></tr></table>", (long long)mainp->networth, (int)( ticks.next - time(0) ), (long long)mainp->ressource[CMD_RESSOURCE_POPULATION] );
 else
 httpPrintf( cnt, "<table width=\"100%%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Networth : %lld</b></font></td><td width=\"40%%\" align=\"center\"><font size=\"1\"><b>Tick time : time frozen</b></font></td><td width=\"30%%\" align=\"center\"><font size=\"1\"><b>Population : %lld0</b></font></td></tr></table>", (long long)mainp->networth, (long long)mainp->ressource[CMD_RESSOURCE_POPULATION] );

 httpString( cnt, "</td><td width=\"78\"><img height=\"21\" src=\"files?type=image&name=i07.jpg\" width=\"78\"></td></tr></table>" );
 httpString( cnt, "</td></tr>" );

 httpString( cnt, "<tr><td width=\"100%\" align=\"center\">" );
 httpString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" background=\"files?type=image&name=i15.jpg\" border=\"0\" align=\"center\"><tr>" );

 a = dbUserNewsGetFlags( id );
 httpString( cnt, "<td width=\"41\"><a href=\"hq\"><img id=\"headermail\" height=\"42\" title=\"mail\" src=\"files?type=image&name=i09" );
 if( a & CMD_NEWS_FLAGS_MAIL )
  httpString( cnt, "a" );
 httpString( cnt, ".jpg\" width=\"41\" border=\"0\"></a></td>" );

 httpString( cnt, "<td width=\"40\"><a href=\"hq\"><img id=\"headerbuild\" height=\"42\" title=\"reports\" src=\"files?type=image&name=i10" );
 if( a & CMD_NEWS_FLAGS_BUILD )
  httpString( cnt, "a" );
 httpString( cnt, ".jpg\" width=\"40\" border=\"0\"></a></td>" );

 httpString( cnt, "<td width=\"39\"><a href=\"hq\"><img id=\"headeraid\" height=\"42\" title=\"economy\" src=\"files?type=image&name=i11" );
 if( a & CMD_NEWS_FLAGS_AID )
  httpString( cnt, "a" );
 httpString( cnt, ".jpg\" width=\"39\" border=\"0\"></a></td>" );

 httpString( cnt, "<td width=\"39\"><a href=\"hq\"><img id=\"headerfleet\" height=\"42\" title=\"fleets\" src=\"files?type=image&name=i12" );
 if( a & CMD_NEWS_FLAGS_ATTACK )
  httpString( cnt, "a" );
 else if( a & CMD_NEWS_FLAGS_FLEET )
  httpString( cnt, "b" );
 httpString( cnt, ".jpg\" width=\"39\" border=\"0\"></a></td>" );

 httpString( cnt, "<td width=\"18\"><img height=\"42\" src=\"files?type=image&name=i13.jpg\" width=\"18\"></td>" );
 httpString( cnt, "<td width=\"100%\" background=\"files?type=image&name=i15.jpg\" align=\"left\">" );

 httpString( cnt, "<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\"><tr>" );
 httpString( cnt, "<td width=\"50%\" align=\"center\" nowrap><font size=\"1\"><b>" );
 httpPrintf( cnt, "Energy: <span id=\"headerenergy\">%lld</span><br>Mineral: <span id=\"headermineral\">%lld</span></b></font></td><td width=\"50%%\" align=\"center\" nowrap><font size=\"1\"><b>Crystal: <span id=\"headercrystal\">%lld</span><br>Ectrolium: <span id=\"headerectrolium\">%lld</span></b></font>", (long long)mainp->ressource[CMD_RESSOURCE_ENERGY], (long long)mainp->ressource[CMD_RESSOURCE_MINERAL], (long long)mainp->ressource[CMD_RESSOURCE_CRYSTAL], (long long)mainp->ressource[CMD_RESSOURCE_ECTROLIUM] );
 httpString( cnt, "</td></tr></table>" );

 httpString( cnt, "</td><td width=\"49\"><img height=\"42\" src=\"files?type=image&name=i17.jpg\" width=\"49\"></td></tr></table>" );

 httpString( cnt, "</td></tr></table><br>" );

 return 1;
}

void iohtmlFunc_frontmenu( ReplyDataPtr cnt, int flags ) {

httpString( cnt, "<table class=\"center\" cellspacing=\"0\" cellpadding=\"10\" style=\"width:100%;border-width:0;\"><tbody><tr><td class=\"bodyline\">" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td align=\"center\"><img src=\"files?type=image&name=ectro_03.jpg\" width=\"350\" height=\"80\" alt=\"ect Top\"></td></tr>" );

httpString( cnt, "<tr><td><table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr>" );
httpString( cnt, "<td background=\"files?type=image&name=ectro_12.jpg\" width=\"45%\">&nbsp;</td>" );
httpString( cnt, "<td align=\"center\" width=\"10%\"><img src=\"files?type=image&name=ectro_06.jpg\" width=\"450\" height=\"75\"></td>" );
httpString( cnt, "<td background=\"files?type=image&name=ectro_12.jpg\" width=\"45%\">&nbsp;</td>" );
httpString( cnt, "</tr></table></tr></td>" );

httpString( cnt, "<tr><td align=\"center\"><img src=\"files?type=image&name=ectro_09.jpg\" width=\"660\" height=\"100\"></td></tr>" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_12.jpg\" align=\"center\"><table width=\"660\" height=\"75\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_13.jpg\" valign=\"middle\"><table style=\"width:100%;border-width:0;\"><tr>" );
httpPrintf( cnt, "<td align=\"left\">%d of %d players online</td>", dbRegisteredInfo[DB_TOTALS_USERS_ONLINE], dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED] );
httpString( cnt, "<td align=\"right\"><b>" );

if( !( flags == FMENU_MAIN ) ) {
	httpString( cnt, "<a href=\"/\">Main</a>" );
}
if( !( (cnt->session)->dbuser ) && !( flags == FMENU_REGISTER ) ) {
	if( !( flags == FMENU_MAIN ) )
		httpString( cnt, " | " );
	httpString( cnt, "<a href=\"register\">Register</a>" );
}
//if( !( flags == FMENU_FORUM ) ) {
	//httpString( cnt, " | " );
	//httpString( cnt, "<a href=\"forum\">Forums</a>" );
//}
if( !( flags == FMENU_FAQ ) ) {
	if( !( (cnt->session)->dbuser ) || ( ( (cnt->session)->dbuser ) && !( flags == FMENU_MAIN ) ) )
		httpString( cnt, " | " );
	httpString( cnt, "<a href=\"faq\">FAQ</a>" );
}
if( !( flags == FMENU_GSTART ) ) {
	httpString( cnt, " | " );
	httpString( cnt, "<a href=\"gettingstarted\">Getting Started</a>" );
}
if( !( flags == FMENU_RANKS ) ) {
	httpString( cnt, " | " );
	httpString( cnt, "<a href=\"halloffame\">Hall of fame</a> " );
}
if( !( flags == FMENU_SERVER ) ) {
	httpString( cnt, " | " );
	httpString( cnt, "<a href=\"status\">Server Status</a>" );
}

httpString( cnt, "</b></td></tr></table></td></tr>" );

httpString( cnt, "</table>" );
httpString( cnt, "</td></tr></table>" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );

return;
}

void iohtmlBodyInit( ReplyDataPtr cnt, char *title, ... )
{
 char text[ARRAY_MAX];
 va_list ap;
 va_start( ap, title );
 vsnprintf( text, ARRAY_MAX, title, ap );
 httpString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\" background=\"files?type=image&name=i27.jpg\"><tr><td width=\"10%\"><img height=\"24\" src=\"files?type=image&name=i25.jpg\" width=\"22\"></td><td width=\"80%\" align=\"center\" nowrap><b><font face=\"verdana\" size=\"2\">" );
 httpString( cnt, text );
 httpString( cnt, "</font></b></td><td width=\"10%\" align=\"right\"><img height=\"24\" src=\"files?type=image&name=i30.jpg\" width=\"62\"></td></tr></table>" );
 httpString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"7\" background=\"files?type=image&name=i38.jpg\">&nbsp;</td><td bgcolor=\"#0b1119\" align=\"center\"><br>" );
 return;
}

void iohtmlBodyEnd( ReplyDataPtr cnt )
{
 httpString( cnt, "<br></td><td width=\"7\" background=\"files?type=image&name=i43.jpg\">&nbsp;</td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"62\"><img height=\"12\" src=\"files?type=image&name=i45.jpg\" width=\"62\"></td><td width=\"100%\" background=\"files?type=image&name=i47.jpg\"><img height=\"12\" src=\"files?type=image&name=i47.jpg\" width=\"1\"></td><td width=\"62\"><img height=\"12\" src=\"files?type=image&name=i49.jpg\" width=\"62\"></td></tr></table></center><br><br><br></body></html>" );
 return;
}

void iohtmlFunc_races( ReplyDataPtr cnt ) {
	int a, b, id;
	bool c;
	dbUserMainDef maind;
	ConfigArrayPtr settings;

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	iohtmlBase( cnt, 1 );
	if( !( iohtmlHeader( cnt, id, &maind ) ) )
		return;
} else {
	iohtmlBase( cnt, 8 );
	iohtmlFunc_frontmenu( cnt, FMENU_NONE );
}

settings = GetSetting( "Server Name" );
iohtmlBodyInit( cnt, "%s: Races", settings->string_value );

for( a = 0; a < CMD_RACE_NUMUSED ; a++) {
	httpPrintf( cnt, "<div class=\"genlarge\">%s</div><br>", cmdRaceName[a] );
	if( cmdRace[a].special )
	httpPrintf( cnt, "<i><b>Special bonus.</b></i><br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_POPRESEARCH )
		httpString( cnt, "Each 4000 population produces 1 research point every week!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_SOLARP15 )
		httpString( cnt, "Solar Production increased by 15%!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_CULPROTECT )
		httpString( cnt, "Culture Research production provides a psychic shield for planets!<br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_IDW )
		httpString( cnt, "Imune to Dark Web Effects!<br>" );

	httpString( cnt, "<table width=\"*\" border=\"0\"><tr>" );
	httpString( cnt, "<td valign=\"top\" width=\"250\"><i><b>Main bonuses</b></i><br>" );
	if( (cmdRace[a].attack - 1) )
	httpPrintf( cnt, "%+.0f%% Attack.<br>", ( cmdRace[a].attack - 1 ) * 100 );
	if( cmdRace[a].defense - 1 )
	httpPrintf( cnt, "%+.0f%% Defence.<br>", ( cmdRace[a].defense - 1 ) * 100 );
	if( ( ( cmdRace[a].speed / 2 ) - 1 ) )
		httpPrintf( cnt, "%+.0f%% Travel Speed<br>", ( ( cmdRace[a].speed / 2 ) - 1 ) *100 );
	for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ ) {
		if( (cmdRace[a].unit[b] - 1) )
			httpPrintf( cnt, " %+.0f%% %s strength.<br>", ( cmdRace[a].unit[b] - 1 ) * 100, cmdUnitName[b] );
	}
	c = false;
	if( (int)((((cmdRace[a].growth-1)/0.02) - 1 ) * 100 ) != 0 ) {
		httpPrintf( cnt, "<br><i><b>Ressource bonuses</b></i><br>" );
		httpPrintf( cnt, "%+.0f%% Population Upkeep Reduction<br>", (((cmdRace[a].growth-1)/0.02) - 1 ) * 100 );
		c = true;
	}
	for( b = 0; b < CMD_RESSOURCE_NUMUSED ; b++) {
		if( (cmdRace[a].resource[b] - 1) ) {
			if( c == false ) {
				httpPrintf( cnt, "<br><i><b>Ressource bonuses</b></i><br>" );
				c = true;
			}
			httpPrintf( cnt, "%+.0f%% %s production<br>", ( cmdRace[a].resource[b] - 1 ) * 100, cmdRessourceName[b] );
		}
	}
	httpString( cnt, "</td><td valign=\"top\" width=\"225\">" );
	httpPrintf( cnt, "<i><b>Research bonuses</b></i><br>" );
	for( b = 0 ; b < CMD_RESEARCH_NUMUSED ; b++ ) {
		if( cmdRace[a].researchmax[b] != 200 )
			httpPrintf( cnt, "%.0f%% %s limit.<br>", cmdRace[a].researchmax[b], cmdResearchName[b] );
	}
	for( b = 0 ; b < CMD_RESEARCH_NUMUSED ; b++ ) {
		if( ( cmdRace[a].researchpoints[b] - 1 ) )
			httpPrintf( cnt, "%+.0f%% %s<br>", ( cmdRace[a].researchpoints[b] - 1 ) * 100, cmdResearchName[b] );
	}
	httpString( cnt, "</td><td valign=\"top\" width=\"140\"><i><b>Operations</b></i>" );
	for( b = 0 ; b < CMD_AGENTOP_NUMUSED ; b++ ) {
		if( specopAgentsAllowed( b, a ) )
			httpPrintf( cnt, "<br>%s", cmdAgentopName[b] );
	}
	httpString( cnt, "<br><br><i><b>Spells</b></i>" );
	for( b = 0 ; b < CMD_PSYCHICOP_NUMUSED ; b++ ) {
		if( specopPsychicsAllowed( b, a ) )
			httpPrintf( cnt, "<br>%s", cmdPsychicopName[b] );
	}
	httpString( cnt, "</td>" );
	httpString( cnt, "<td valign=\"top\" width=\"140\"><i><b>Incantations</b></i>" );
	for( b = 0 ; b < CMD_GHOSTOP_NUMUSED ; b++ ) {
		if( specopGhostsAllowed( b, a ) )
			httpPrintf( cnt, "<br>%s", cmdGhostopName[b] );
	}
	httpString( cnt, "</td></tr></table>" );
	httpString( cnt, "<br><br>" );
}
iohtmlBodyEnd( cnt );


 return;
}


void iohtmlFunc_rules( ReplyDataPtr cnt ) {
	struct stat stdata;
	char *data;
	char DIRCHECKER[PATH_MAX];
	ConfigArrayPtr settings[2];
	FILE *file;

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_NONE );

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\">" );

settings[0] = GetSetting( "Server Name" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpPrintf( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>General Rules for %s.</b></font></td></tr>", settings[0]->string_value );
httpString( cnt, "<tr><td><font size=\"2\">" );

settings[1] = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/rules.txt", settings[1]->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
			httpString( cnt, "<ul>" );
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( strlen(data) > 1 )
						httpPrintf( cnt, "<li>%s</li>", trimwhitespace(data) );
				}
			httpString( cnt, "</ul>" );
			}
			fclose( file );
		}
		free( data );
	}
}


httpString( cnt, "<br>" );
httpPrintf( cnt, "Rules are subject to change at any time and applicable to every instance of %s.<br>", settings[0]->string_value );
httpString( cnt, "<br>" );
httpString( cnt, "Administration is open to discussion regarding these rules. In all cases the Administration's decision is final.<br>" );
httpString( cnt, "While we are open to discuss these rules, they always apply unless specificly waived/altered by Administration. In which case you will be notified.<br>" );
httpString( cnt, "<br>" );
httpString( cnt, "Players breaking the rules will get a warning, an account reset/deletion or a permanent ban.<br>" );
httpString( cnt, "When a player gets warned, his player tag will be changed to “Warned” for a minimum of 4 days.<br>" );
httpString( cnt, "You do not get 2 warnings. A second violation is an account reset (your records will be kept).<br>" );
httpString( cnt, "A third violation is an account deletion and a fourth violation will require me to go all out, and find a way to ban you.<br>" );

httpString( cnt, "</td></tr></table><br><br>" );

iohtmlFunc_endhtml( cnt );

return;
}


void iohtmlFunc_register( ReplyDataPtr cnt ) {

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );

httpString ( cnt, "<br><br><h3>Register</h3><br>" );
httpString ( cnt, "<b>Currently disabled, due to pre-alpha stage.</b>" );

//httpString( cnt, "<form action=\"register2\" method=\"POST\">User name<br><input type=\"text\" name=\"name\"><br><br>Password<br><input type=\"password\" name=\"pass\"><br><br>Faction name<br><input type=\"text\" name=\"faction\"><br><br><input type=\"submit\" value=\"OK\"></form>" );

iohtmlFunc_endhtml( cnt );


return;
}

void iohtmlFunc_register2( ReplyDataPtr cnt )
{
 int a, i, id;
 FILE *file;
 char timebuf[256];
 char COREDIR[PATH_MAX];
 char *name, *pass, *faction;
 int64_t newd[DB_USER_NEWS_BASE];
 dbMailDef maild;
 ConfigArrayPtr settings;
 char Message[] = "Congratulations! You have successfully registered your account!<br>Good luck and have fun,<br><br>- Administration";

 name = NULL;//iohtmlVarsFind( cnt, "name" );
 pass = NULL;//iohtmlVarsFind( cnt, "pass" );
 faction = NULL;//iohtmlVarsFind( cnt, "faction" );

if( ( name != NULL ) && ( pass != NULL ) && ( faction != NULL ) ) {
	if( strncmp( name, "FBUSER", 6 ) == 0 ) {
		iohtmlBase( cnt, 8 );
		iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );
		httpPrintf( cnt, "Username format prohibited<br>%s is blacklisted due to FBUSER*", name );
		goto iohtmlFunc_register2L0;
	}
	
	if( ( id = cmdExecNewUser( name, pass, faction ) ) < 0 ) {
		iohtmlBase( cnt, 8 );
		iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );

		if( cmdErrorString )
			httpString( cnt, cmdErrorString );
		else
			httpString( cnt, "Error encountered while registering user" );

		httpString( cnt, "<br><br><a href=\"register\">Try again?</a>" );
		goto iohtmlFunc_register2L0;
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
	(maild.mail).time = time( 0 );
	(maild.mail).tick = ticks.number;
	(maild.mail).flags = 0;
	if( dbMailAdd( id, 0, &maild ) < 0 )
		error( "Error sending registration email" );


	if( ( dbUserLinkDatabase( cnt, id ) < 0 ) || ( dbSessionSet( (cnt->session)->dbuser, (cnt->session)->sid ) < 0 ) ) {
		iohtmlBase( cnt, 8 );
		iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );
		httpString( cnt, "Error encountered while registering session" );
		goto iohtmlFunc_register2L0;
	}

	iohtmlBase( cnt, 8 );
	iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );

	httpPrintf( cnt, "New user created<br>User name : %s<br>Password : %s<br>Faction name : %s<br>Account ID : %d<br>", name, pass, faction, id );
	settings = GetSetting( "Directory" );
	sprintf( COREDIR, "%s/logs/register.log", settings->string_value );
	if( ( file = fopen( COREDIR, "a" ) ) ) {
		fprintf( file, "Register ID %d ( %x )\n", id, id );
		a = time(0);
		strftime( timebuf, 256, "%T, %b %d %Y;", localtime( (time_t *)&a ) );
		fprintf( file, "Time %s\n", timebuf );
		fprintf( file, "Name %s;\n", name );
		//fprintf( file, "Password %s;\n", pass );
		fprintf( file, "Faction %s;\n", faction );
		if( (cnt->connection)->addr->sa_family == AF_INET )
			fprintf( file, "IP %s;\n", inet_ntoa( ((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr ) );
		strcpy(timebuf, iohtmlHeaderFind( cnt, "User-Agent" ) );
		for(i=0;i<strlen(timebuf);i++) {
			if(timebuf[i] == ';')
				timebuf[i] = ',';
		}
		fprintf( file, "User Agent: %s;\n", timebuf );
		//fprintf( file, "Cookie %s;;\n", iohttp->cookie );
		fprintf(file, "ID : %d ( %X );\n\n\n", id, id);
		fclose( file );
	}

} else if(  ( id = iohtmlIdentify( cnt, 0 ) ) < 0  ) {
	iohtmlBase( cnt, 8 );
	iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );
	httpString( cnt, "Bad input detected... Strange, this shoulden't be able to happen!" );
	goto iohtmlFunc_register2L0;
} else {
	iohtmlBase( cnt, 8 );
	iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );
}

httpString( cnt, "<form action=\"register3\" method=\"POST\"><br><br>Empire number<br><i>Leave blank to join a random empire</i><br><input type=\"text\" name=\"empire\"><br><br>" );
httpString( cnt, "Empire password<br><i>Only required if defined by the leader of the empire to join.</i><br><input type=\"text\" name=\"fampass\"><br><br>" );
httpString( cnt, "Faction race<br><i>The race of your people define many characteristics affecting different aspects of your faction.</i> - <a href=\"races\" target=\"_blank\">See races</a><br><select name=\"race\">" );
	for( a = 0 ; a < CMD_RACE_NUMUSED-1 ; a++ )
		httpPrintf( cnt, "<option value=\"%d\">%s", a, cmdRaceName[a] );
httpString( cnt, "</select><br><br>" );

httpString( cnt, "<input type=\"submit\" value=\"OK\"></form>" );

httpString( cnt, "<br><br><a href=\"rankings?typ=1\" target=\"_blank\">See empire rankings</a>" );
httpString( cnt, "<br><a href=\"rankings\" target=\"_blank\">See faction rankings</a>" );

iohtmlFunc_register2L0:
iohtmlFunc_endhtml( cnt );


return;
}



void iohtmlFunc_register3( ReplyDataPtr cnt )
{
 int a, id, raceid;
 char *empire;
 char *fampass;
 char *race;

 iohtmlBase( cnt, 8 );
 if( ( id = iohtmlIdentify( cnt, 1|4 ) ) < 0 )
  return;
iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );

 race = iohtmlVarsFind( cnt, "race" );
 empire = iohtmlVarsFind( cnt, "empire" );
 fampass = iohtmlVarsFind( cnt, "fampass" );

if( race ) {
	if( fampass ) {
		for( a = 0 ; a < 31 ; a++ ) {
			if( ( fampass[a] == 10 ) || ( fampass[a] == 13 ) )
				break;
		}
	}
	if( !( empire ) || ( empire[0] == 0 ) )
		a = -1;
	else if( empire[0] == '#' )
		sscanf( &empire[1], "%d", &a );
	else
		sscanf( empire, "%d", &a );

	sscanf( race, "%d", &raceid );

	if( cmdExecNewUserEmpire( id, a, fampass, raceid, ((cnt->session)->dbuser)->level ) < 0 ) {
 		if( cmdErrorString )
 			httpString( cnt, cmdErrorString );
 		else
  			httpString( cnt, "Error encountered while registering user" );
 		httpString( cnt, "<br><br><a href=\"/register2\">Try again</a>" );
 		goto iohtmlFunc_register3L0;
  	}
	httpPrintf( cnt, "<b>Account activated!</b><br>" );
	httpString( cnt, "<br><br><br><a href=\"/main\">Click here if not redirected</a>" );
	redirect( cnt, "/main" );
	iohtmlFunc_endhtml( cnt );
	return;
} else {
	httpString( cnt, "Incorrect query, strange... this really shoulden't happen!" );
}

iohtmlFunc_register3L0:
httpString( cnt, "<br><br><a href=\"/\">Main page</a><br><br><a href=\"/login\">Log in</a>" );
iohtmlFunc_endhtml( cnt );


return;
}


void iohtmlFunc_login( ReplyDataPtr cnt, int flag, char *text, ... ) {
	int a, i, id, num;
	char rtpass[USER_PASS_MAX];
	int64_t *newsp, *newsd;
	dbUserInfoDef infod;
	ConfigArrayPtr settings;
	struct stat stdata;
	char *data, *name, *pass;
	char *token = NULL;
	char DIRCHECKER[PATH_MAX];
	char timebuf[512];
	time_t tint;
	FILE *file = NULL;

name = iohtmlVarsFind( cnt, "name" );
pass = iohtmlVarsFind( cnt, "pass" );

#if FACEBOOK_SUPPORT
token = iohtmlVarsFind( cnt, "fblogin_token" );
#endif

iohtmlBase( cnt, 8 );
#if FACEBOOK_SUPPORT
if( ( !(name) && !(pass) ) && !(token) )
iohtmlFBSDK( cnt );
#endif
iohtmlFunc_frontmenu( cnt, FMENU_NONE );

id = iohtmlIdentify( cnt, 0 );

if( ( id >= 0 ) )
	goto LOGIN_SUCESS;

if( ( name ) && ( pass ) ) {
	settings = GetSetting( "Directory" );
	sprintf( DIRCHECKER, "%s/logs/login.log", settings->string_value );
	if( ( file = fopen( DIRCHECKER, "a" ) ) ) {
		time( &tint );
		strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &tint ) );
		fprintf( file, "Time: %s", timebuf );
		fprintf( file, "Name: %s;\n", name );
		if( (cnt->connection)->addr->sa_family == AF_INET )
			fprintf( file, "IP %s;\n", inet_ntoa( ((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr ) );
		strcpy(DIRCHECKER, iohtmlHeaderFind( cnt, "User-Agent" ) );
		for(i=0;i<strlen(DIRCHECKER);i++) {
			if(DIRCHECKER[i] == ';')
				DIRCHECKER[i] = ',';
		}
		fprintf( file, "User Agent: %s;\n", DIRCHECKER );
	}
 
	for( a = 0 ; name[a] ; a++ ) {
		if( name[a] == '+' )
			name[a] = ' ';
		else if( ( name[a] == 10 ) || ( name[a] == 13 ) )
			name[a] = 0;
	}
	for( a = 0 ; pass[a] ; a++ ) {
		if( pass[a] == '+' )
			pass[a] = ' ';
		else if( ( pass[a] == 10 ) || ( pass[a] == 13 ) )
    			pass[a] = 0;
	}
	if( strncmp( name, "FBUSER", 6 ) == 0 ) {
		info( "ban match" );
		goto LOGIN_FAIL;
	}
	if( ( id = dbUserSearch( name ) ) < 0 )
		goto LOGIN_FAIL;
	if( dbUserRetrievePassword( id, rtpass ) < 0 )
		goto LOGIN_FAIL;
	if( !( checkencrypt( pass, rtpass ) ) )
		goto LOGIN_FAIL;
	if( dbUserLinkDatabase( cnt, id ) < 0 )
		goto LOGIN_FAIL;

	if( dbSessionSet( (cnt->session)->dbuser, (cnt->session)->sid ) < 0 )
		goto LOGIN_FAIL;

	dbUserInfoRetrieve( id, &infod );
	infod.lasttime = time( 0 );
	if( (cnt->connection)->addr->sa_family == AF_INET )
	for( a = (MAXIPRECORD-2); a >= 0 ; a-- ) {
		if( strcmp(inet_ntoa( infod.sin_addr[a] ),"0.0.0.0") ) {
			memcpy( &(infod.sin_addr[a+1]), &(infod.sin_addr[a]), sizeof(struct in_addr) );
		}
	}
	memcpy( &(infod.sin_addr[0]), &(((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr), sizeof(struct in_addr) );
	dbUserInfoSet( id, &infod );

	if( ( file ) ) {
		fprintf( file, "ID : %d ( %x ) %s\n\n\n", id, id, ( bitflag( ((cnt->session)->dbuser)->flags, ( CMD_USER_FLAGS_KILLED | CMD_USER_FLAGS_DELETED | CMD_USER_FLAGS_NEWROUND ) ) ? "Deactivated" : "Active") );
		fclose( file );
	}

	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_KILLED ) ) {
		httpString( cnt, "Your Home Planet has been conquered and whiped out, your faction has been destroyed!<br><br><a href=\"register2\">Rejoin the Galaxy</a><br><br>" );
		num = dbUserNewsList( id, &newsp );
		newsd = newsp;
		if( !( num ) )
			httpString( cnt, "<br><b>There are no news reports to display.</b><br>" );
		for( a = 0 ; a < num ; a++, newsd += DB_USER_NEWS_BASE ) {
			iohtmlNewsString( cnt, newsd );
		}
		if( newsp )
			free( newsp );
		goto iohtmlFunc_mainL1;
	}
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_DELETED ) ) {
		httpString( cnt, "<br>Your account have been deleted by an administrator, most likely for not respecting a rule of the game.<br><br><a href=\"register2\">Register this account again</a><br><br>" );
		goto iohtmlFunc_mainL1;
	}
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_NEWROUND ) ) {
		httpString( cnt, "<br>The account has been deactivated for the new round, starting soon!<br>You'll be asked to join an empire of your choice again.<br><br><a href=\"register2\">Complete account registration</a><br><br>" );
		goto iohtmlFunc_mainL1;
	}

	if( !( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) ) {
		httpString( cnt, "<br>The activation of this account was not completed.<br><br><a href=\"register2\">Continue registration</a><br><br>" );
		iohtmlFunc_mainL1:
		httpString( cnt, "<a href=\"forum\">Public Forums</a>" );
		if((cnt->session)->dbuser) {
			if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR )
				httpString( cnt, "<br><br><a href=\"moderator\">Moderator panel</a>" );
			if( ((cnt->session)->dbuser)->level >= LEVEL_ADMINISTRATOR )
				httpString( cnt, "<br><a href=\"administration\">Admin panel</a>" );
		}
	iohtmlFunc_endhtml( cnt );
	return;
	}
	goto LOGIN_SUCESS;
}
LOGIN_FAIL:

if( file ) {
	fprintf( file, "Failed!\n\n\n" );
	fclose( file );
}

if( text ) {
	httpPrintf( cnt, "<br>%s", text );
	if( flag ) {
		settings = GetSetting( "HTTP Text" );
		sprintf( DIRCHECKER, "%s/login.txt", settings->string_value );
		if( stat( DIRCHECKER, &stdata ) != -1 ) {
			if( ( data = malloc( stdata.st_size + 1 ) ) ) {
				data[stdata.st_size] = 0;
				if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
					if( stdata.st_size > 0 ) {
						httpString( cnt, "<br>" );
						while( fgets( data, stdata.st_size, file ) != NULL ) {
							httpPrintf( cnt, "%s<br>", trimwhitespace(data) );
						}
					}
					fclose( file );
				}
				free( data );
			}
		}
	}
	httpString( cnt, "<br><br>" );
} else {
	httpString( cnt, "<br><h3>Login</h3><br>" );
}

httpPrintf( cnt, "<form action=\"/login%s%s\" method=\"POST\">", ( token ? "?fblogin_token=" : "" ), ( token ? token : "" ) );

httpString( cnt, "Name<br><input type=\"text\" name=\"name\"><br>" );
httpString( cnt, "<br>Password<br><input type=\"password\" name=\"pass\"><br>" );
httpString( cnt, "<br><input type=\"submit\" value=\"OK\"></form>" );


goto LOGIN_END;

LOGIN_SUCESS:
#if FACEBOOK_SUPPORT
if( token ) {
	redirect( cnt, "/facebook?fblogin_token=%s", token );
} else
#endif
redirect( cnt, "/main" );


httpString( cnt, "<b>Login sucess, you should be redirected to the game shortly...</b><br>" );
httpString( cnt, "<br>" );
httpString( cnt, "<a href=\"/main\">Click here if it takes too long.</a><br>" );

LOGIN_END:
iohtmlFunc_endhtml( cnt );
return;
}


void iohtmlFunc_endhtml( ReplyDataPtr cnt ) {


httpString( cnt, "</td><td width=\"7%\">&nbsp;</td></tr></table>" );
httpString( cnt, "</td></tr></tbody></table>" );
httpString( cnt, "</body></html>" );

return;
}


void iohtmlFunc_boxstart( ReplyDataPtr cnt, char *title ) {

httpString( cnt, "<table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\" background=\"files?type=image&name=i27.jpg\"><tr><td width=\"10%\"><img height=\"24\" src=\"files?type=image&name=i25.jpg\" width=\"22\"></td><td width=\"80%\" align=\"center\" nowrap><b><font face=\"verdana\" size=\"2\">" );
httpPrintf( cnt, "%s", title);
httpString( cnt, "</font></b></td><td width=\"10%\" align=\"right\"><img height=\"24\" src=\"files?type=image&name=i30.jpg\" width=\"62\"></td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"7\" background=\"files?type=image&name=i38.jpg\">&nbsp;</td><td bgcolor=\"#0b1119\"><br>" );

return;
}

void iohtmlFunc_boxend( ReplyDataPtr cnt ) {

httpString( cnt, "<br><br></td><td width=\"7\" background=\"files?type=image&name=i43.jpg\">&nbsp;</td></tr></table><table cellspacing=\"0\" cellpadding=\"0\" width=\"90%\" border=\"0\" align=\"center\"><tr><td width=\"62\"><img height=\"12\" src=\"files?type=image&name=i45.jpg\" width=\"62\"></td><td width=\"100%\" background=\"files?type=image&name=i47.jpg\"><img height=\"12\" src=\"files?type=image&name=i47.jpg\" width=\"1\"></td><td width=\"62\"><img height=\"12\" src=\"files?type=image&name=i49.jpg\" width=\"62\"></td></tr></table>" );

return;
}

void iohtmlFunc_front( ReplyDataPtr cnt, char *text, ...  ) {
	ConfigArrayPtr settings[2];
	dbUserMainDef maind;
	#if FACEBOOK_SUPPORT
	dbUserInfoDef infod;
	#endif
	struct stat stdata;
	bool boxopen = false;
	char *data;
	char DIRCHECKER[PATH_MAX];
	char DATAPOOL[USER_DESC_SIZE];
	FILE *file;
	int id, len, notices = 0;
	va_list ap;

va_start( ap, text );
len = vsnprintf( DATAPOOL, USER_DESC_SIZE, text, ap );
va_end( ap );


iohtmlBase( cnt, 8 );

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	if( dbUserMainRetrieve( id, &maind ) < 0 )
	return;
}

#if FACEBOOK_SUPPORT
if( (id < 0) && !( len > 0 ) )
	iohtmlFBSDK( cnt );
#endif

iohtmlFunc_frontmenu( cnt, FMENU_MAIN );

if( len > 0 )
	httpPrintf( cnt, "<b>%s</b><br><br>", DATAPOOL );
else
httpString( cnt, "<br>" );

httpString( cnt, "<td width=\"40%\" valign=\"top\">" );

//read notices from updates.txt and format for display. -- If this file is missing, or empty it is skipped.
settings[0] = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/updates.txt", settings[0]->string_value );
settings[1] = GetSetting( "Display Notices" );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( !(boxopen) && ( strlen( trimwhitespace(data) ) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_SIZE );
						iohtmlFunc_boxstart( cnt, DATAPOOL );
						boxopen = true;
						notices++;
					} else if ( strlen( trimwhitespace(data) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_SIZE );
						httpPrintf( cnt, "&nbsp;&nbsp;%s<br>", DATAPOOL );
					}
					if( (boxopen) && ( strlen( trimwhitespace(data) ) == false ) ) {
						iohtmlFunc_boxend( cnt );
						boxopen = false;
						httpString( cnt, "<br><br>" );
						if( notices == (int)settings[1]->num_value ) {
							httpString( cnt, "<table align=\"right\">" );
							httpString( cnt, "<tr><td width=\"40%\" valign=\"top\"><a href=\"/notices\">See full list...</a></td></tr>" );
							httpString( cnt, "</table>" );
							break;
						}
					}
				}
			if(boxopen)
				iohtmlFunc_boxend( cnt );
			httpString( cnt, "<br>" );
			httpString( cnt, "<br>" );
			}
			fclose( file );
		}
	free( data );
	}
}
//end notices

httpString( cnt, "</td><td width=\"6%\">" );
httpString( cnt, "&nbsp;" );
httpString( cnt, "</td><td width=\"40%\" valign=\"top\">" );

httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Log in</b></font></td></tr>" );
httpString( cnt, "<tr><td>" );
httpString( cnt, "<table cellspacing=\"8\"><tr><td>" );

if( (id < 0) ) {
	httpString( cnt, "<font size=\"2\"><form action=\"login\" method=\"POST\">" );
	httpString( cnt, "Name<br><input type=\"text\" name=\"name\" size=\"24\"><br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "Password<br><input type=\"password\" name=\"pass\" size=\"24\"><br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "<input type=\"submit\" value=\"Log in\"></form>" );
} else {
	httpPrintf( cnt, "<br><b>You are already loged in as <i>%s</i></b><br>", (cnt->session)->dbuser->name );
	httpString( cnt, "<br>" );
	httpString( cnt, "<a href=\"/main\" target=\"_top\">Proceed to game</a><br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "<a href=\"/logout\" target=\"_top\">Log out</a><br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "<br>" );
}

#if FACEBOOK_SUPPORT
httpString( cnt, "</td><td>&nbsp;</td><td valign=\"bottom\">" );
if( ((cnt->session)->dbuser) ) {
	dbUserInfoRetrieve( ((cnt->session)->dbuser)->id, &infod );
	if( -timediff( *localtime( &infod.fbinfo.updated ) ) >= day ) {
		facebook_update_user( (cnt->session)->dbuser );
	}
}
iohtmlFBConnect( cnt );
if( (id >= 0) ) {
	httpString( cnt, "<br>" );
}
#endif

httpString( cnt, "</td></table></td></tr>" );

//read the todo list from todo.txt and format for display. -- If this file is missing, or empty it is skipped.
sprintf( DIRCHECKER, "%s/todo.txt", settings[0]->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><i>Items on the admins to-do list :</i></font></td></tr>" );
				httpString( cnt, "<tr><td>" );
				httpString( cnt, "<table cellspacing=\"8\"><tr><td>" );
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( strlen(data) > 1 )
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_SIZE );
						httpPrintf( cnt, "&nbsp;&#9734;&nbsp;&nbsp;%s<br>", DATAPOOL );
				}
				httpString( cnt, "</td></tr></table></td></tr>" );
			}
			fclose( file );
		}
		free( data );
	}
}
//end todo list

httpString( cnt, "</table>" );

iohtmlFunc_endhtml( cnt );
return;
}


void iohtmlFunc_faq( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings;
	struct stat stdata;
	char *data;
	char DIRCHECKER[PATH_MAX];
	FILE *file;

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_FAQ );

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\">" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );

httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Frequently Asked Question</b></font></td></tr>" );
httpString( cnt, "<tr><td>" );
settings = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/faq.html", settings->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( ( fread( data, 1, stdata.st_size, file ) < 1 ) && ( stdata.st_size ) ) {
				error( "Failure reading faq file." );
			} else {
				httpString( cnt, trimwhitespace(data) );
			}
			fclose( file );
		}
		free( data );
	}
}
httpString( cnt, "</td></tr></table><br><br>" );

iohtmlFunc_endhtml( cnt );
return;
}


void iohtmlFunc_gettingstarted( ReplyDataPtr cnt ) {

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_GSTART );


httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\">" );


httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Getting started in the galaxy of Ectroverse</b></font></td></tr>" );
httpString( cnt, "<tr><td><font size=\"2\">" );
httpString( cnt, "<center>This page is a basic guide to new players on how to get started.<br>It is important to read this to get the best possible experience when first playing!</center>" );

httpString( cnt, "<br><b>Creating an account:</b><br>" );
httpString( cnt, "<a href=\"#a0\">0. Registering the account.</a><br>" );
httpString( cnt, "<a href=\"#a1\">1. The user name and faction name.</a><br>" );
httpString( cnt, "<a href=\"#a2\">2. Joining an empire.</a><br>" );
httpString( cnt, "<a href=\"#a3\">3. Choosing your race.</a><br>" );
httpString( cnt, "<a href=\"#a4\">4. Completion and logging in.</a><br>" );
httpString( cnt, "</a><br>" );

httpString( cnt, "<br><b>Playing the game:</b><br>" );
httpString( cnt, "<a href=\"#b0\">0. Resources & Buildings.</a><br>" );
httpString( cnt, "<a href=\"#b1\">1. Planets.</a><br>" );
httpString( cnt, "<a href=\"#b2\">2. Research.</a><br>" );
httpString( cnt, "<a href=\"#b3\">3. Military.</a><br>" );
httpString( cnt, "</a><br>" );

httpString( cnt, "<br><b>Tips:</b><br>" );
httpString( cnt, "<a href=\"#c0\">0. Map generation.</a><br>" );
httpString( cnt, "<a href=\"#c1\">1. Fleet page.</a><br>" );
httpString( cnt, "<a href=\"#c2\">2. Account page.</a><br>" );
httpString( cnt, "</a><br>" );

httpString( cnt, "</td></tr></table><br><br>" );


httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Creating an account:</b></font></td></tr>" );
httpString( cnt, "<tr><td><font size=\"2\"><br>" );

httpString( cnt, "<a name=\"a0\"><b><i>0. Registering the account.</i></b></a><br>" );
httpString( cnt, "The great thing about Ectroverse is that the signup is simple. You won't need to input your e-mail address and no other activation is required." );
httpString( cnt, "<br><br>" );

httpString( cnt, "<a name=\"a1\"><b><i>1. The user name and faction name.</i></b></a><br>" );
httpString( cnt, "Your user name is something personal. Other players won't see this.<br>" );
httpString( cnt, "Your Faction name however, is your handle in the game. Choose carefully.<br>" );
httpString( cnt, "<br>" );

httpString( cnt, "<a name=\"a2\"><b><i>2. Joining an Empire.</i></b></a><br>" );
httpString( cnt, "Being part of an Empire is one of the most important aspects of the game.<br>" );
httpString( cnt, "You start out small and will need other players to grow.<br>" );
httpString( cnt, "Know anyone in the game? Ask them for their empire number and password.<br>" );
httpString( cnt, "<br>" );
httpString( cnt, "If you want to start an Empire of your own, look at the <a href=\"rankings?typ=1\">Empire rankings</a> and pick a number not yet in the list!" );
httpString( cnt, "<br>Want to team up with random players? Leave blank. But remember, be a teamplayer and you'll earn a rank in the Empire.<br>" );
httpString( cnt, "<br>" );

httpString( cnt, "<a name=\"a3\"><b><i>3. Choosing your race.</i></b></a><br>" );
httpString( cnt, "You are well on your way to making a name for yourself.<br>" );
httpString( cnt, "But how will people remember you? As an aggressive attacker? A proud and rich Energy provider? A self made and self sufficient powerhouse?<br>" );
httpString( cnt, "<br>" );
httpString( cnt, "Your race will decide which path you will walk.<br>" );
httpString( cnt, "<a href=\"races\">View the stats for each race here</a>.<br><br>" );

httpString( cnt, "<a name=\"a4\"><b><i>4. Completion and logging in.</i></b></a><br>" );
httpString( cnt, "Congratulations. You have created an account, chosen an Empire to fight for and selected your race.<br>" );
httpString( cnt, "You are now ready to explore, build and fight your way to the top.<br>" );
httpString( cnt, "<br>" );

httpString( cnt, "</td></tr></table><br><br>" );

httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Playing the game:</b></font></td></tr>" );
httpString( cnt, "<tr><td><font size=\"2\"><br>" );

httpString( cnt, "<a name=\"b0\"><b><i>0. Resources & Buildings.</i></b></a><br>" );
httpString( cnt, "Your main concern is growth. You need to spend your resources by making resources.<br>" );
httpString( cnt, "How do you do that? Simple. Build buildings on your planets.<br>" );
httpString( cnt, "There are 4 resources in the galaxy. Energy, Minerals, Crystal and Ectrolium. <br>" );
httpString( cnt, "Most buildings need 3 of the 4 resources to be built, so you need to plan your building a bit at the start.<br><br>" );

httpString( cnt, "<a name=\"b1\"><b><i>1. Planets.</i></b></a><br>" );
httpString( cnt, "Exploring and conquering planets is your way of getting to the top.<br>" );
httpString( cnt, "You start out with 4 exploration ships. Send these out into the galaxy to colonise planets.<br>" );
httpString( cnt, "Don't worry, you'll be building a lot of them soon enough. Afterall, who is pleased with <i>just</i> 4 planets....<br><br>" );

httpString( cnt, "<a name=\"b2\"><b><i>2. Research.</i></b></a><br>" );
httpString( cnt, "Get smarter and do it smart.<br>" );
httpString( cnt, "Research is a very important aspect of the game.<br>" );
httpString( cnt, "Head over to your Research tab and distribute points wisely!<br><br>" );

httpString( cnt, "<a name=\"b3\"><b><i>3. Military.</i></b></a><br>" );
httpString( cnt, "A big fleet makes you strong, but they also increase your Networth.<br>" );
httpString( cnt, "A higher networth makes it harder to grow.<br>" );
httpString( cnt, "Build up your fleet steadily, don't go overboard.<br>" );
httpString( cnt, "And remember, you need to pay upkeep for your little Cruisers too!<br><br>" );

httpString( cnt, "</td></tr></table><br><br>" );



httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Tips:</b></font></td></tr>" );
httpString( cnt, "<tr><td><font size=\"2\"><br>" );

httpString( cnt, "<a name=\"c0\"><b><i>0. Map generation</i></b></a><br>" );
httpString( cnt, "The galaxy map is the place where you'll find planets to explore and conquer... But it is easy to lose your way and location." );
httpString( cnt, "<br>Luckily you can generate a map using the Generation page." );
httpString( cnt, "<br>You can filter away planets that are already owned, that are of your Empire or are that of enemies!" );
httpString( cnt, "<br>A usefull tool to say the least.<br><br>" );

httpString( cnt, "<a name=\"c1\"><b><i>1. Fleet page.</i></b></a><br>" );
httpString( cnt, "Check your fleet page. In the top right you see <b>\"Fleet Orders\"</b>, select the dropdown and chose <b>\"Wait in System\"</b> instead of the default one." );
httpString( cnt, "<br>It is easy to forget this option, and spend 10 minutes wondering where your mighty fleet has gone!.<br><br>" );

httpString( cnt, "<a name=\"c2\"><b><i>2. Account page.</i></b></a><br>" );
httpString( cnt, "This will here page allows you to alter your Faction name when the game hasn't started yet." );
httpString( cnt, "<br>Once the time starts flowing, you won't be able to change your name until the next round has started.<br><br>" );


httpString( cnt, "</td></tr></table><br><br>" );

iohtmlFunc_endhtml( cnt );

return;
}


void iohtmlFunc_halloffame( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings;
	int a;
	struct stat stdata;
	char DIRCHECKER[PATH_MAX];
	char LINKSTRING[256];


iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_RANKS );

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\"><table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>Hall of Fame / Server Rankings</b></font></td></tr>" );
httpString( cnt, "<tr><td>" );

httpPrintf( cnt, "<tr><td><br>Round %d - Current round<br><a href=\"rankings?&typ=1\">Empires</a> - <a href=\"rankings\">Players</a></td></tr>", ticks.round );

for( a = ( ticks.round - 1 ); a > -1; a--) {

httpPrintf( cnt, "<tr><td><br>Round %d<br>", a );
settings = GetSetting( "Directory" );
sprintf( DIRCHECKER, "%s/rankings/round%dfamranks.txt", settings->string_value, a );
sprintf( LINKSTRING, "<a href=\"rankings?rnd=%d&typ=1\">", a );
httpPrintf( cnt, "%sEmpires%s - ", ((stat( DIRCHECKER, &stdata ) != -1) ? LINKSTRING : ""), ((stat( DIRCHECKER, &stdata ) != -1) ? "</a>" : "") );

sprintf( DIRCHECKER, "%s/rankings/round%dranks.txt", settings->string_value, a );
sprintf( LINKSTRING, "<a href=\"rankings?rnd=%d\">", a );
httpPrintf( cnt, "%sPlayers%s", ((stat( DIRCHECKER, &stdata ) != -1) ? LINKSTRING : ""), ((stat( DIRCHECKER, &stdata ) != -1) ? "</a>" : "") );
httpString( cnt, "</td></tr>" );

}


httpString( cnt, "</td></tr></table><br><br><br><br><br><br><br><br></td><td width=\"7%\">&nbsp;</td></tr>" );

iohtmlFunc_endhtml( cnt );
return;
}


void iohtmlFunc_notices( ReplyDataPtr cnt ) {
	ConfigArrayPtr settings;
	dbUserMainDef maind;
	struct stat stdata;
	bool boxopen = false;
	char *data;
	char DATAPOOL[USER_DESC_SIZE];
	char DIRCHECKER[PATH_MAX];
	FILE *file;
	int id;

iohtmlBase( cnt, 8 );

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	if( dbUserMainRetrieve( id, &maind ) < 0 )
		return;
}

iohtmlFunc_frontmenu( cnt, FMENU_NOTICES );

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\"><table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td class=\"center\"><div class=\"genlarge\"><b>Listing of all Server Notices</b></span></td></tr>" );
httpString( cnt, "<tr><td></td></tr>" );
httpString( cnt, "<tr><td>" );

//read notices from updates.txt and format for display. -- If this file is missing, or empty it is skipped.
settings = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/updates.txt", settings->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( !(boxopen) && ( strlen( trimwhitespace(data) ) ) ) {
						httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody><tr>" );
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_SIZE );
						httpPrintf( cnt, "<td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>%s</b></font></td>", DATAPOOL );
						httpString( cnt, "</tr><tr><td><font size=\"2\">" );
						boxopen = true;
					} else if ( strlen( trimwhitespace(data) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_SIZE );
						httpPrintf( cnt, "&nbsp;&nbsp;%s<br>", DATAPOOL );
					}
					if( (boxopen) && ( strlen( trimwhitespace(data) ) == false ) ) {
						httpString( cnt, "</font></td></tr></tbody></table><br>" );
						boxopen = false;
					}
				}
				if(boxopen) {
					httpString( cnt, "</font></td></tr></tbody></table>" );
				}
			}
			fclose( file );
		}
	free( data );
	}
}
//end notices

httpString( cnt, "</td></tr></table></td><td width=\"7%\">&nbsp;</td></tr>" );

iohtmlFunc_endhtml( cnt );
return;
}



