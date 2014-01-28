

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

if( dbUserSave( id, (cnt->session)->dbuser ) < 0 ) {
	error( "Database UserSave" );
	goto NEGATIVE;
}
if( action & 8 ) {
	goto RETURN;
}

if( ( action & 2 ) ) {
	if( !( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) && ( ((cnt->session)->dbuser)->level < LEVEL_MODERATOR ) ) {
		if( action & 1 ) {
			iohtmlFunc_register( cnt );
		}
		goto NEGATIVE;
	}
}

if( action & 4 ) {
	if( ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) ) {
		if( action & 1 ) {
			iohtmlFunc_register( cnt );
		}
		goto NEGATIVE;
	}
}
RETURN:
purge_captcha( cnt->session );
return id;

iohtmlIdentifyL0:

if( action & 1 ) {
	iohtmlFunc_login( cnt, 1, "Your session has expired, you need to login again.<br>If you were playing just a few seconds ago, your account may have been hijacked." );
}

NEGATIVE:
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
httpPrintf( cnt, "<link href=\"%s&type=server&name=style.css\" rel=\"stylesheet\" type=\"text/css\" media=\"screen\">", URLAppend( cnt, "files" ) );
if( !( flags & 32 ) ) {
	if( flags & 8 )
		httpPrintf( cnt, "<script type=\"text/javascript\" src=\"%s&type=server&name=jquery-1.10.2.min.js\"></script>", URLAppend( cnt, "files" ) );
	httpPrintf( cnt, "<script type=\"text/javascript\" src=\"%s\"></script>", URLAppend( cnt, "ajax.js" ) );
	if( flags & 16 )
		httpPrintf( cnt, "<script type=\"text/javascript\" src=\"%s&type=server&name=status.js\"></script>", URLAppend( cnt, "files" ) );
	httpPrintf( cnt, "<script type=\"text/javascript\" src=\"%s&type=server&name=javascript.min.js\"></script>", URLAppend( cnt, "files" ) );
}
if( flags & 4 )
	httpString( cnt, "<base target=\"_blank\">" );
if( flags & 1 ) {
	httpString( cnt, "<style type=\"text/css\">" );
	httpString( cnt, "body{background-image:url(files?type=image&name=background.gif);" );
	if( !( flags & 2 ) )
		httpString( cnt, "background-attachment:fixed;" );
	httpString( cnt, "}</style>" );
}

httpString( cnt, "</head>" );
httpString( cnt, "<body" );

if( iohtmlVarsFind( cnt, "fbapp" ) == NULL ) {
	if( flags & 8 ) {
		httpString( cnt, " onload=\"if(window != window.top){top.location.href=location.href};" );
	} else {
		httpString( cnt, " onload=\"if(window==window.top){" );
		httpString( cnt, "if(top.location.search ){" );
		httpString( cnt, "var params = parseQueryString();" );
		httpPrintf( cnt, "top.location.href=\'main?%s=\'+params[\'%s\']+\'&page=\'+top.location.pathname+encodeURIComponent(top.location.search);", ServerSessionMD5, ServerSessionMD5 );
		httpString( cnt, "}else{" );
		httpString( cnt, "top.location.href=\'main?page=\'+top.location.pathname;" );
		httpString( cnt, "}" );
		httpString( cnt, "};" );
	}
}

httpString( cnt, "\">" );

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
 httpPrintf( cnt, "<td width=\"41\"><a href=\"%s\"><img id=\"headermail\" height=\"42\" title=\"mail\" src=\"files?type=image&name=i09", URLAppend( cnt, "hq" ) );
 if( a & CMD_NEWS_FLAGS_MAIL )
  httpString( cnt, "a" );
 httpString( cnt, ".jpg\" width=\"41\" border=\"0\"></a></td>" );

 httpPrintf( cnt, "<td width=\"40\"><a href=\"%s\"><img id=\"headerbuild\" height=\"42\" title=\"reports\" src=\"files?type=image&name=i10", URLAppend( cnt, "hq" ) );
 if( a & CMD_NEWS_FLAGS_BUILD )
  httpString( cnt, "a" );
 httpString( cnt, ".jpg\" width=\"40\" border=\"0\"></a></td>" );

 httpPrintf( cnt, "<td width=\"39\"><a href=\"%s\"><img id=\"headeraid\" height=\"42\" title=\"economy\" src=\"files?type=image&name=i11", URLAppend( cnt, "hq" ) );
 if( a & CMD_NEWS_FLAGS_AID )
  httpString( cnt, "a" );
 httpString( cnt, ".jpg\" width=\"39\" border=\"0\"></a></td>" );

 httpPrintf( cnt, "<td width=\"39\"><a href=\"%s\"><img id=\"headerfleet\" height=\"42\" title=\"fleets\" src=\"files?type=image&name=i12", URLAppend( cnt, "hq" ) );
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
/*
httpString( cnt, "<tr><td align=\"center\"><img src=\"files?type=image&name=ectro_03.jpg\" width=\"350\" height=\"80\" alt=\"ect Top\"></td></tr>" );

httpString( cnt, "<tr><td>" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr>" );
httpString( cnt, "<td background=\"files?type=image&name=ectro_12.jpg\" width=\"45%\">&nbsp;</td>" );
httpString( cnt, "<td align=\"center\" width=\"10%\"><img src=\"files?type=image&name=ectro_06.jpg\" width=\"450\" height=\"75\"></td>" );
httpString( cnt, "<td background=\"files?type=image&name=ectro_12.jpg\" width=\"45%\">&nbsp;</td>" );
httpString( cnt, "</tr></table>" );
httpString( cnt, "</tr></td>" );
*/
httpString( cnt, "<tr><td align=\"center\"><img src=\"files?type=image&name=n1.jpg\" ></td></tr>" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_12.jpg\" align=\"center\">" );
httpString( cnt, "<table width=\"90%\" height=\"75\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td background=\"files?type=image&name=ectro_12.jpg\" valign=\"middle\">" );
httpString( cnt, "<table style=\"width:100%;border-width:0;\"><tr>" );
httpPrintf( cnt, "<td align=\"center\"><span id=\"u_online\">%d</span> of <span id=\"u_activated\">%d</span> players online</td>", dbRegisteredInfo[DB_TOTALS_USERS_ONLINE], dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED] );
httpString( cnt, "<td align=\"center\"><b>" );

if( !( flags == FMENU_MAIN ) ) {
	URLString( cnt, "/", "Main" );
}
if( !( flags == FMENU_REGISTER ) ) {
	if( !( flags == FMENU_MAIN ) )
		httpString( cnt, " | " );
	URLString( cnt, ( ( (cnt->session)->dbuser ) ? ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ? "main" : "register" ) : "register" ), ( ( (cnt->session)->dbuser ) ? ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ? "Game" : "Activate" ) : "Register" ) );
}
if( !( flags == FMENU_FORUM ) ) {
	httpString( cnt, " | " );
	URLString( cnt, "forum", "Forums" );
}
if( !( flags == FMENU_FAQ ) ) {
	httpString( cnt, " | " );
	URLString( cnt, "faq", "FAQ" );
}
if( !( flags == FMENU_GSTART ) ) {
	httpString( cnt, " | " );
	URLString( cnt, "gettingstarted", "Getting Started" );
}
if( !( flags == FMENU_RANKS ) ) {
	httpString( cnt, " | " );
	URLString( cnt, "halloffame", "Hall of fame" );
}
if( !( flags == FMENU_SERVER ) ) {
	httpString( cnt, " | " );
	URLString( cnt, "status", "Server Status" );
}

httpString( cnt, "</b></td></tr></table></td></tr>" );

httpString( cnt, "</table>" );
httpString( cnt, "</td></tr></table>" );
httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" );
httpString( cnt, "<br>" );

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

void iohtmlFunc_info( ReplyDataPtr cnt ) {
	int a, b, id, table;
	char *type;
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

type = iohtmlVarsFind( cnt, "type" );

if( type == NULL ) {
	goto INVALID;
} else if( strcmp( type, "races" ) == 0 ) {
	iohtmlBodyInit( cnt, "%s: Races", settings->string_value );
	goto RACES;
} else if( strcmp( type, "units" ) == 0 ) {
	iohtmlBodyInit( cnt, "%s: Units", settings->string_value );
	goto UNITS;
} else if( strcmp( type, "buildings" ) == 0 ) {
	iohtmlBodyInit( cnt, "%s: Buildings", settings->string_value );
	goto BUILDINGS;
} else {
	INVALID:
	iohtmlBodyInit( cnt, "%s: Info", settings->string_value );
	httpString( cnt, "Invalid Request" );
	goto RETURN;
}


RACES:
for( a = 0; a < CMD_RACE_NUMUSED ; a++) {
	httpPrintf( cnt, "<div class=\"genlarge\">%s</div><br>", cmdRaceName[a] );
	if( cmdRace[a].special )
	httpPrintf( cnt, "<i><b>Special bonus.</b></i><br>" );
	if( cmdRace[a].special & CMD_RACE_SPECIAL_POPRESEARCH )
		httpString( cnt, "Each 6000 population produces 1 research point every week!<br>" );
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
	if( (int)((((cmdRace[a].growth)/0.02) - 1 ) * 100 ) != 0 ) {
		httpPrintf( cnt, "<br><i><b>Ressource bonuses</b></i><br>" );
		httpPrintf( cnt, "%+.0f%% Population Growth<br>",  (((cmdRace[a].growth)/0.02) - 1 ) * 100 );
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
goto RETURN;
UNITS:


httpString( cnt, "<table width=\"100%\">" );

for( a = table = 0; a < CMD_UNIT_NUMUSED; a++, table++ ) {
	if( table == 3 ) {
		httpString( cnt, "</tr><tr><td>&nbsp;</td></tr>" );
		table = 0;
	} else {
		httpString( cnt, "<td>&nbsp;&nbsp;&nbsp;</td>" );
	}
	if( table == 0 ) {
		httpString( cnt, "<tr>" );
	}
	httpString( cnt, "<td valign=\"top\" align=\"center\">" );
	httpPrintf( cnt, "<span class=\"genlarge\">%s</span><br>", cmdUnitName[a] );
	httpString( cnt, "<table>" );
	if( a != CMD_UNIT_PHANTOM ) {
		httpString( cnt, "<tr><td><span class=\"genblue\">Base Cost:</span></td><td>" );
		httpString( cnt, "<table>" );
		for( b = 0; b < CMD_RESSOURCE_NUMUSED; b++) {
			if( cmdUnitCost[a][b] > 0 ) {
				httpPrintf( cnt, "<tr><td align=\"right\">%lld</td><td align=\"left\">%s</td></tr>", (long long)cmdUnitCost[a][b], cmdRessourceName[b] );
			}
		}
		httpString( cnt, "</table></td>" );
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Build Time:</span></td><td>%lld Ticks</td></tr>", (long long)cmdUnitCost[a][CMD_RESSOURCE_TIME] );
	} else {
		httpString( cnt, "<tr><td><span class=\"genblue\">Special Unit:</span></td><td>Can not be built.</td></tr>" );
	}
	if( cmdUnitUpkeep[a] > 0 ) {
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Upkeep:</span></td><td>%.2f %s per Tick</td></tr>", cmdUnitUpkeep[a], cmdRessourceName[CMD_RESSOURCE_ENERGY] );
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Travel Upkeep:</span></td><td>%.2f %s per Tick</td></tr>", (cmdUnitUpkeep[a] * cmdUnitPenalty[0]), cmdRessourceName[CMD_RESSOURCE_ENERGY] );
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Station Upkeep:</span></td><td>%.2f %s per Tick</td></tr>", (cmdUnitUpkeep[a] * cmdUnitPenalty[1]), cmdRessourceName[CMD_RESSOURCE_ENERGY] );
	} else {
		httpString( cnt, "<tr><td><span class=\"genblue\">Base Upkeep:</span></td><td>No Upkeep</td></tr>" );
	}
	if( cmdUnitTech[a] > 0 ) {
		httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Tech:</span></td><td>%d%%</td></tr>", cmdUnitTech[a] );
	}
	for( b = 0; b < CMD_UNIT_STATS_NUMUSED; b++ ) {
		if( cmdUnitStats[a][b] > 0 ) {
			httpPrintf( cnt, "<tr><td><span class=\"genblue\">%s:</span></td><td>%d</td></tr>", cmdUnitStatsNames[b], cmdUnitStats[a][b] );
		}
	}
	
	httpString( cnt, "</table>" );
	httpString( cnt, "</td>" );
}

httpString( cnt, "</table>" );


goto RETURN;
BUILDINGS:

httpString( cnt, "<table width=\"100%\">" );

for( a = table = 0; a < CMD_BLDG_NUMUSED+1; a++, table++ ) {
	if( table == 3 ) {
		httpString( cnt, "</tr><tr><td>&nbsp;</td></tr>" );
		table = 0;
	} else {
		httpString( cnt, "<td>&nbsp;&nbsp;&nbsp;</td>" );
	}
	if( table == 0 ) {
		httpString( cnt, "<tr>" );
	}
	httpString( cnt, "<td valign=\"top\" align=\"center\">" );
	httpPrintf( cnt, "<span class=\"genlarge\">%s</span><br>", cmdBuildingName[a] );
	httpString( cnt, "<table>" );
	httpString( cnt, "<tr><td><span class=\"genblue\">Base Cost:</span></td><td>" );
	httpString( cnt, "<table>" );
	for( b = 0; b < CMD_RESSOURCE_NUMUSED; b++) {
		if( cmdBuildingCost[a][b] > 0 ) {
			httpPrintf( cnt, "<tr><td align=\"right\">%lld</td><td align=\"left\">%s</td></tr>", (long long)cmdBuildingCost[a][b], cmdRessourceName[b] );
		}
	}
	httpString( cnt, "</table></td>" );
	httpPrintf( cnt, "<tr><td><span class=\"genblue\">Build Time:</span></td><td>%lld Ticks</td></tr>", (long long)cmdBuildingCost[a][CMD_RESSOURCE_TIME] );
	if( a < CMD_BLDG_NUMUSED ) {
		if( cmdBuildingUpkeep[a] > 0 ) {
			httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Upkeep:</span></td><td>%.2f per Tick</td></tr>", cmdBuildingUpkeep[a] );
		} else {
			httpString( cnt, "<tr><td><span class=\"genblue\">Base Upkeep:</span></td><td>No Upkeep</td></tr>" );
		}
		if( cmdBuildingProduction[a] > 0 ) {
			if( a == CMD_BUILDING_CITIES ) {
				httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Capacity:</span></td><td>%.0f Population</td></tr>", cmdBuildingProduction[a] );
			} else {
				httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Output:</span></td><td>%.2f per Tick</td></tr>", cmdBuildingProduction[a] );
			}
		}
		if( cmdBuildingTech[a] > 0 ) {
			httpPrintf( cnt, "<tr><td><span class=\"genblue\">Base Tech:</span></td><td>%d%%</td></tr>", cmdBuildingTech[a] );
		}
	}
	httpPrintf( cnt, "<tr><td><span class=\"genblue\">Networth:</span></td><td>%d</td></tr>", cmdBuildingNetworth[a] );
	httpString( cnt, "</table>" );
	httpString( cnt, "</td>" );
}

httpString( cnt, "</table>" );

RETURN:
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
	char DATAPOOL[USER_DESC_MAX];
	FILE *file;
	int id, len, notices = 0;
	va_list ap;

va_start( ap, text );
len = vsnprintf( DATAPOOL, USER_DESC_MAX, text, ap );
va_end( ap );


iohtmlBase( cnt, 8 );

if( ( id = iohtmlIdentify( cnt, 0 ) ) >= 0 ) {
	if( dbUserMainRetrieve( id, &maind ) < 0 )
	return;
}

#if FACEBOOK_SUPPORT
iohtmlFBSDK( cnt );
#endif

iohtmlFunc_frontmenu( cnt, FMENU_MAIN );

if( len > 0 )
	httpPrintf( cnt, "<b>%s</b><br><br>", DATAPOOL );

httpString( cnt, "<td width=\"40%\" valign=\"top\">" );

//read notices from notices.txt and format for display. -- If this file is missing, or empty it is skipped.
settings[0] = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/notices.txt", settings[0]->string_value );
settings[1] = GetSetting( "Display Notices" );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( !(boxopen) && ( strlen( trimwhitespace(data) ) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_MAX );
						iohtmlFunc_boxstart( cnt, DATAPOOL );
						boxopen = true;
						notices++;
					} else if ( strlen( trimwhitespace(data) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_MAX );
						httpPrintf( cnt, "&nbsp;&nbsp;%s<br>", DATAPOOL );
					}
					if( (boxopen) && ( strlen( trimwhitespace(data) ) == false ) ) {
						iohtmlFunc_boxend( cnt );
						boxopen = false;
						httpString( cnt, "<br><br>" );
						if( notices == (int)settings[1]->num_value ) {
							httpString( cnt, "<table align=\"right\">" );
							httpPrintf( cnt, "<tr><td width=\"40%\" valign=\"top\"><a href=\"%s\">See full list...</a></td></tr>", URLAppend( cnt, "notices" ) );
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
	httpPrintf( cnt, "<font size=\"2\"><form action=\"%s\" method=\"POST\">", URLAppend( cnt, "login" ) );
	httpString( cnt, "Name<br><input type=\"text\" name=\"name\" size=\"24\"><br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "Password<br><input type=\"password\" name=\"pass\" size=\"24\"><br>" );
	httpString( cnt, "<br>" );
	httpString( cnt, "<input type=\"submit\" value=\"Log in\"></form>" );
} else {
	#if FACEBOOK_SUPPORT
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBMADE ) ) {
		dbUserInfoRetrieve( id, &infod );
		httpPrintf( cnt, "<br><b>You are already logged in as <i>%s</i></b><br>", infod.fbinfo.full_name );
	} else
	#endif
	httpPrintf( cnt, "<br><b>You are already logged in as <i>%s</i></b><br>", ((cnt->session)->dbuser)->name );
	httpString( cnt, "<br>" );
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) {
		httpPrintf( cnt, "<a href=\"%s\" target=\"%s\">Proceed to game</a><br>", URLAppend( cnt, "main" ), targetframe( cnt ) );
	} else {
		httpPrintf( cnt, "<a href=\"%s\" target=\"%s\">Activate Now!</a><br>", URLAppend( cnt, "register" ), targetframe( cnt ) );
	}
	httpString( cnt, "<br>" );
	httpPrintf( cnt, "<a href=\"%s\" target=\"%s\">Log out</a><br>", URLAppend( cnt, "logout" ), targetframe( cnt ) );
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
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_MAX );
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

#if FACEBOOK_SUPPORT
httpString( cnt, "<br><div class=\"fb-like\" data-href=\"https://www.facebook.com/nectroverse\" data-layout=\"standard\" data-action=\"like\" data-show-faces=\"true\" data-share=\"true\" data-colorscheme=\"dark\"></div>" );
#endif

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
httpPrintf( cnt, "If you want to start an Empire of your own, look at the <a href=\"%s&typ=1\">Empire rankings</a> and pick a number not yet in the list!", URLAppend( cnt, "rankings" ) );
httpString( cnt, "<br>Want to team up with random players? Leave blank. But remember, be a teamplayer and you'll earn a rank in the Empire.<br>" );
httpString( cnt, "<br>" );

httpString( cnt, "<a name=\"a3\"><b><i>3. Choosing your race.</i></b></a><br>" );
httpString( cnt, "You are well on your way to making a name for yourself.<br>" );
httpString( cnt, "But how will people remember you? As an aggressive attacker? A proud and rich Energy provider? A self made and self sufficient powerhouse?<br>" );
httpString( cnt, "<br>" );
httpString( cnt, "Your race will decide which path you will walk.<br>" );
httpPrintf( cnt, "<a href=\"%s&type=races\">View the stats for each race here</a>.<br><br>", URLAppend( cnt, "info" ) );

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

httpPrintf( cnt, "<tr><td><br>Round %d - Current round<br><a href=\"%s&typ=1\">Empires</a> - <a href=\"%s\">Players</a></td></tr>", ticks.round, URLAppend( cnt, "rankings" ), URLAppend( cnt, "rankings" ) );

for( a = ( ticks.round - 1 ); a > -1; a--) {

httpPrintf( cnt, "<tr><td><br>Round %d<br>", a );
settings = GetSetting( "Directory" );
sprintf( DIRCHECKER, "%s/rankings/round%dfamranks.txt", settings->string_value, a );
sprintf( LINKSTRING, "<a href=\"%s&rnd=%d&typ=1\">", URLAppend( cnt, "rankings" ), a );
httpPrintf( cnt, "%sEmpires%s - ", ((stat( DIRCHECKER, &stdata ) != -1) ? LINKSTRING : ""), ((stat( DIRCHECKER, &stdata ) != -1) ? "</a>" : "") );

sprintf( DIRCHECKER, "%s/rankings/round%dranks.txt", settings->string_value, a );
sprintf( LINKSTRING, "<a href=\"%s&rnd=%d\">", URLAppend( cnt, "rankings" ), a );
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
	char DATAPOOL[USER_DESC_MAX];
	char DIRCHECKER[PATH_MAX];
	FILE *file;
	int id;

if( iohtmlVarsFind( cnt, "request" ) ) {
	iohtmlBase( cnt, 1 );
} else {
	iohtmlBase( cnt, 8 );

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	if( dbUserMainRetrieve( id, &maind ) < 0 )
		return;
}

iohtmlFunc_frontmenu( cnt, FMENU_NOTICES );
}

httpString( cnt, "<tr><td width=\"7%\">&nbsp;</td><td width=\"86%\" valign=\"top\"><table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">" );
httpString( cnt, "<tr><td class=\"center\"><div class=\"genlarge\"><b>Listing of all Server Notices</b></span></td></tr>" );
httpString( cnt, "<tr><td></td></tr>" );
httpString( cnt, "<tr><td>" );

//read notices from notices.txt and format for display. -- If this file is missing, or empty it is skipped.
settings = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/notices.txt", settings->string_value );
if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					if( !(boxopen) && ( strlen( trimwhitespace(data) ) ) ) {
						httpString( cnt, "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\"><tbody><tr>" );
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_MAX );
						httpPrintf( cnt, "<td background=\"files?type=image&name=ectro_16.jpg\" height=\"15\"><font color=\"#FFFFFF\" size=\"2\"><b>%s</b></font></td>", DATAPOOL );
						httpString( cnt, "</tr><tr><td><font size=\"2\">" );
						boxopen = true;
					} else if ( strlen( trimwhitespace(data) ) ) {
						iohttpForumFilter2( DATAPOOL, trimwhitespace(data), USER_DESC_MAX );
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


void iohtmlFunc_login( ReplyDataPtr cnt, int flag, char *text, ... ) {
	int a, i, id, num;
	char rtpass[USER_PASS_MAX];
	int64_t *newsp, *newsd;
	dbUserInfoDef infod;
	ConfigArrayPtr settings;
	struct stat stdata;
	char *data, *name, *pass;
	#if FACEBOOK_SUPPORT
	char *token = NULL;
	#endif
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
		fprintf( file, "Time: %s\n", timebuf );
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
		if( file ) {
			fprintf( file, "Ban Match\n" );
		}
		info( "ban match" );
		goto LOGIN_FAIL;
	}
	if( ( id = dbUserSearch( name ) ) < 0 ) {
		if( file ) {
			fprintf( file, "No User named: %s\n", name );
		}
		goto LOGIN_FAIL;
	}
	if( dbUserRetrievePassword( id, rtpass ) < 0 ) {
		if( file ) {
			fprintf( file, "Error Getting Password.\n" );
		}
		error( "Getting Password for User: %d", id );
		goto LOGIN_FAIL;
	}
	if( !( checkencrypt( pass, rtpass ) ) ) {
		if( file ) {
			fprintf( file, "Password Missmatch.\n" );
		}
		goto LOGIN_FAIL;
	}
	if( dbUserLinkDatabase( cnt, id ) < 0 ) {
		if( file ) {
			fprintf( file, "Unable to link user -> database\n" );
		}
		error( "Getting linking User: %d", id );
		goto LOGIN_FAIL;
	}

	if( dbSessionSet( (cnt->session)->dbuser, (cnt->session)->sid ) < 0 ) {
		if( file ) {
			fprintf( file, "Setting session failed.\n" );
		}
		error( "Getting setting session for User: %d", id );
		goto LOGIN_FAIL;
	}
	dbRegisteredInfo[DB_TOTALS_USERS_ONLINE]++;
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
		httpString( cnt, "Your Home Planet has been conquered and whiped out, your faction has been destroyed!<br><br>" );
		URLString( cnt, "register", "Rejoin the Galaxy" );
		httpString( cnt, "<br><br>" );
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
		httpString( cnt, "<br>Your account have been deleted by an administrator, most likely for not respecting a rule of the game.<br><br>" );
		URLString( cnt, "register", "Register this account again" );
		httpString( cnt, "<br><br>" );
		goto iohtmlFunc_mainL1;
	}
	if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_NEWROUND ) ) {
		httpString( cnt, "<br>The account has been deactivated for the new round, starting soon!<br>You'll be asked to join an empire of your choice again.<br><br>" );
		URLString( cnt, "register", "Complete account registration" );
		httpString( cnt, "<br><br>" );
		goto iohtmlFunc_mainL1;
	}

	if( !( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) ) {
		httpString( cnt, "<br>The activation of this account was not completed.<br><br>" );
		URLString( cnt, "register", "Continue Registration" );
		httpString( cnt, "<br><br>" );
		iohtmlFunc_mainL1:
		URLString( cnt, "forum", "Public Forums" );
		if((cnt->session)->dbuser) {
			if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) {
				httpString( cnt, "<br><br>" );
				URLString( cnt, "moderator", "Moderator panel" );
			} if( ((cnt->session)->dbuser)->level >= LEVEL_ADMINISTRATOR ) {
				httpString( cnt, "<br>" );
				URLString( cnt, "administration", "Admin panel" );
			}
		}
	iohtmlFunc_endhtml( cnt );
	return;
	}
	goto LOGIN_SUCESS;
} else {
	goto LOGIN_RAW;
}
LOGIN_FAIL:
if( file ) {
	fprintf( file, "Failed!\n\n\n" );
	fclose( file );
}
httpString( cnt, "<span class=\"genlargered\">Login Failed</span><br>" );
LOGIN_RAW:

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

httpPrintf( cnt, "<form action=\"/%s\" method=\"POST\">", URLAppend( cnt, "login" ) );

httpString( cnt, "Name<br><input type=\"text\" name=\"name\"><br>" );
httpString( cnt, "<br>Password<br><input type=\"password\" name=\"pass\"><br>" );
httpString( cnt, "<br><input type=\"submit\" value=\"OK\"></form>" );


goto LOGIN_END;

LOGIN_SUCESS:
#if FACEBOOK_SUPPORT
if( token ) {
	redirect( cnt, "/%s", URLAppend( cnt, "facebook") );
} else
#endif
if( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) )
redirect( cnt, "/%s", URLAppend( cnt, "main") );
else
redirect( cnt, "/%s", URLAppend( cnt, "register") );

httpString( cnt, "<b>Login sucess, you should be redirected shortly...</b><br>" );
httpString( cnt, "<br>" );
URLString( cnt, bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ? "main" : "register", "Click here if it takes too long." );
httpString( cnt, "<br>" );
LOGIN_END:
iohtmlFunc_endhtml( cnt );
return;
}

#define REGISTER_DISABLE 0

void iohtmlFunc_register( ReplyDataPtr cnt ) {
	int a, i, id, raceid;
	FILE *file;
	char timebuf[256];
	char COREDIR[PATH_MAX];
	char *name, *pass, *faction, *race, *empire, *fampass;
	char *token = NULL;
	#if FACEBOOK_SUPPORT
	FBUserDef fbdata;
	FBTokenDef token_post;
	dbUserInfoDef infod;
	char fbtemp[2][USER_NAME_MAX];
	#endif
	int64_t newd[DB_USER_NEWS_BASE];
	dbMailDef maild;
	ConfigArrayPtr settings;
	char Message[] = "Congratulations! You have successfully registered your account!<br>Good luck and have fun,<br><br>- Administration";


race = iohtmlVarsFind( cnt, "race" );
empire = iohtmlVarsFind( cnt, "empire" );
fampass = iohtmlVarsFind( cnt, "fampass" );
name = iohtmlVarsFind( cnt, "name" );
pass = iohtmlVarsFind( cnt, "pass" );
faction = iohtmlVarsFind( cnt, "faction" );
#if FACEBOOK_SUPPORT
token = iohtmlVarsFind( cnt, "fblogin_token" );
#endif

id = iohtmlIdentify( cnt, 8 );

iohtmlBase( cnt, 8 );
iohtmlFunc_frontmenu( cnt, FMENU_REGISTER );

httpPrintf( cnt, "<h3>%s</h3><br>", ( ( (cnt->session)->dbuser ) ? "Activation" : "Registration" ) );

//Used to skip/disable registration.
#if REGISTER_DISABLE
goto DISABLE;
#endif

if( race ) {
	if( id < 0 ) {
		httpString( cnt, "Error encountered while validating user registration..." );
		goto END;
	}
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
 		httpString( cnt, "<br><br>" );
 		URLString( cnt, "register", "Try Again." );
 		goto END;
  	}
	httpPrintf( cnt, "<b>Account activated!</b><br>" );
	httpString( cnt, "<br><br><br>" );
	URLString( cnt, "main", "Click here if not redirected." );
	redirect( cnt, "%s", URLAppend( cnt, "main" ) );
	goto END;
} else if( ( ( token != NULL ) && ( ( faction != NULL ) && ( strlen(faction) > 0 ) ) ) || ( ( name != NULL ) && ( pass != NULL ) && ( faction != NULL ) ) ) {
	if( ( name != NULL ) && ( strncmp( name, "FBUSER", 6 ) == 0 ) ) {
		httpPrintf( cnt, "Username format prohibited<br>%s is blacklisted due to FBUSER*", name );
		goto END;
	}
	#if FACEBOOK_SUPPORT
	if( ( name != NULL ) && ( pass != NULL ) ) {
		snprintf( fbtemp[0], USER_NAME_MAX-1, "%s", name );
		snprintf( fbtemp[1], USER_NAME_MAX-1, "%s", pass );
	} else if ( token != NULL ) {
		strncpy( token_post.val, token, sizeof( token_post.val ) );
		facebook_getdata_token( &fbdata, token_post );
		if( strlen( fbdata.id ) == 0 ) {
			httpString( cnt, "Invalid Token Detected... Aborting!" );
			redirect( cnt, "%s", URLAppend( cnt, "main" ) );
			goto END;
		}
		snprintf( fbtemp[0], USER_NAME_MAX-1, "FBUSER%s", fbdata.id );
		RANDOMIZE_SEED;
		snprintf( fbtemp[1], USER_NAME_MAX-1, "P%X%X%X%X", (unsigned int)random(), (unsigned int)random(), (unsigned int)random(), (unsigned int)random() );
	}
	if( ( id = cmdExecNewUser( fbtemp[0], fbtemp[1], faction ) ) < 0 ) {
	#else
	if( ( id = cmdExecNewUser( name, pass, faction ) ) < 0 ) {
	#endif

		if( cmdErrorString )
			httpString( cnt, cmdErrorString );
		else
			httpString( cnt, "Error encountered while registering user" );

		httpString( cnt, "<br><br>" );
		URLString( cnt, "register", "Try Again." );
		goto END;
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
		error( "Error sending registration mail" );

	if( ( dbUserLinkDatabase( cnt, id ) < 0 ) || ( dbSessionSet( (cnt->session)->dbuser, (cnt->session)->sid ) < 0 ) ) {
		httpString( cnt, "Error encountered while registering session" );
		goto END;
	}
	httpString( cnt, "New user created<br>" );
	#if FACEBOOK_SUPPORT
	if( ( token != NULL ) && ( strlen( fbdata.id ) ) ) {
		dbUserInfoRetrieve( id, &infod );
		snprintf( fbdata.token.val, sizeof(fbdata.token.val), "%s", token );
		infod.fbinfo = fbdata;
		dbUserInfoSet( id, &infod );
		bitflag_add( &((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBMADE );
		bitflag_add( &((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBLINK );
		snprintf( ((cnt->session)->dbuser)->fbid, sizeof( ((cnt->session)->dbuser)->fbid ), "%s", fbdata.id );
		dbUserSave( id, (cnt->session)->dbuser );
	} else
	#endif	
	httpPrintf( cnt, "User name : %s<br>Password : %s<br>", ((cnt->session)->dbuser)->name, pass );
	httpPrintf( cnt, "Faction name : %s<br>Account ID : %d<br>", ((cnt->session)->dbuser)->faction, id );
	settings = GetSetting( "Directory" );
	sprintf( COREDIR, "%s/logs/register.log", settings->string_value );
	if( ( file = fopen( COREDIR, "a" ) ) ) {
		fprintf( file, "Register ID %d ( %x )\n", id, id );
		a = time(0);
		strftime( timebuf, 256, "%T, %b %d %Y;", localtime( (time_t *)&a ) );
		fprintf( file, "Time %s\n", timebuf );
		fprintf( file, "Name %s;\n", ((cnt->session)->dbuser)->name );
		fprintf( file, "Faction %s;\n", ((cnt->session)->dbuser)->faction );
		if( (cnt->connection)->addr->sa_family == AF_INET )
			fprintf( file, "IP %s;\n", inet_ntoa( ((struct sockaddr_in *)(cnt->connection)->addr)->sin_addr ) );
		strcpy(timebuf, iohtmlHeaderFind( cnt, "User-Agent" ) );
		for(i=0;i<strlen(timebuf);i++) {
			if(timebuf[i] == ';')
				timebuf[i] = ',';
		}
		fprintf( file, "User Agent: %s;\n", timebuf );
		fprintf(file, "ID : %d ( %X );\n\n\n", id, id);
		fclose( file );
	}



} else if ( ( id < 0 ) ) {
	httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "register" ) );
	if( token == NULL ) {
		httpString( cnt, "User name<br><input type=\"text\" name=\"name\"><br>" );
		httpString( cnt, "<br>Password<br><input type=\"password\" name=\"pass\"><br>" );
	} else {
		httpPrintf( cnt, "<input type=\"hidden\" name=\"fblogin_token\" value=\"%s\"><br>", token );
	}
	httpString( cnt, "<br>Faction name<br><input type=\"text\" name=\"faction\"><br>" );
	httpString( cnt, "<br><input type=\"submit\" value=\"OK\"></form>" );
	goto END;
}

if( ( ((cnt->session)->dbuser) ) && ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) ) {
	redirect( cnt, "/%s", URLAppend( cnt, "main" ) );
	httpString( cnt, "This account has already been activated, you will now be redirected into the game...<br>" );
	httpString( cnt, "<br>" );
	httpPrintf( cnt, "<a href=\"%s\">Click here if it takes too long<a>", URLAppend( cnt, "/main" ) );
} else {
	httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><br><br>Empire number<br><i>Leave blank to join a random empire</i><br><input type=\"text\" name=\"empire\"><br><br>", URLAppend( cnt, "register" ) );
	httpString( cnt, "Empire password<br><i>Only required if defined by the leader of the empire to join.</i><br><input type=\"text\" name=\"fampass\"><br><br>" );
	httpString( cnt, "Faction race<br><i>The race of your people define many characteristics affecting different aspects of your faction.</i> - " );
	httpPrintf( cnt, "<a href=\"%s&type=races\" target=\"_blank\">See races</a><br><select name=\"race\">", URLAppend( cnt, "info" ) );
	for( a = 0 ; a < CMD_RACE_NUMUSED ; a++ ) {
		httpPrintf( cnt, "<option value=\"%d\">%s</option>", a, cmdRaceName[a] );
	}
	httpString( cnt, "</select><br><br>" );
	httpString( cnt, "<input type=\"submit\" value=\"OK\"></form>" );
}

httpPrintf( cnt, "<br><br><a href=\"%s&typ=1\" target=\"_blank\">See empire rankings</a>", URLAppend( cnt, "rankings" ) );
httpPrintf( cnt, "<br><a href=\"%s\" target=\"_blank\">See faction rankings</a>", URLAppend( cnt, "rankings" ) );

//End New functions
#if REGISTER_DISABLE
goto END;
DISABLE:
httpString ( cnt, "<b>Sorry... Currently disabled</b>" );
#endif

END:
iohtmlFunc_endhtml( cnt );


return;
}



