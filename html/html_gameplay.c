
void iohtmlFunc_main( ReplyDataPtr cnt ) {
	int id;
	char *page;
	ConfigArrayPtr settings;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
	return;

page = iohtmlVarsFind( cnt, "page" );

settings = GetSetting( "Server Name" );
httpPrintf( cnt, "<html><head><title>%s</title><link rel=\"icon\" href=\"files?type=image&name=favicon.ico\"></head>", settings->string_value );
httpString( cnt, "<frameset cols=\"155,*\" framespacing=\"0\" border=\"0\" marginwidth=\"0\" marginheight=\"0\" frameborder=\"no\">" );
httpPrintf( cnt, "<frame src=\"%s\" name=\"menu\" marginwidth=\"0\" marginheight=\"0\" scrolling=\"no\" noresize>", URLAppend( cnt, "menu" ) );
httpPrintf( cnt, "<frame src=\"%s\" name=\"main\" marginwidth=\"0\" marginheight=\"0\" noresize>", URLAppend( cnt, ( page ? page : "hq" ) ) );
httpPrintf( cnt, "<noframes>Your browser does not support frames! That's uncommon :).<br><br><a href=\"%s\">Menu</a></noframes>", URLAppend( cnt, "menu" ) );
httpString( cnt, "</frameset></html>" );

return;
}


void iohtmlFunc_menu( ReplyDataPtr cnt )
{
 int id/*, i, j;
 char szFaction[32]*/;
 dbUserMainDef maind;

 iohtmlBase( cnt, 1|2|32 );
 
 if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
if( dbUserMainRetrieve( id, &maind ) < 0 ) {
	maind.empire = -1;
}

 httpString( cnt, "<br><table cellspacing=\"0\" cellpadding=\"0\" width=\"150\" background=\"files?type=image&name=i36.jpg\" border=\"0\" align=\"center\"><tr><td><img height=\"40\" src=\"files?type=image&name=i18.jpg\" width=\"150\"></td></tr><tr><td background=\"files?type=image&name=i23.jpg\" height=\"20\"><b><font face=\"Tahoma\" size=\"2\">" );

 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Headquarters</a>", URLAppend( cnt, "hq" ) );
 httpString( cnt, "</font></b></td></tr><tr><td background=\"files?type=image&name=i36.jpg\"><table width=\"125\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" align=\"left\"><tr><td><b><font face=\"Tahoma\" size=\"2\">" );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Council</a><br>", URLAppend( cnt, "council" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Units</a><br>", URLAppend( cnt, "units" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Market</a><br>", URLAppend( cnt, "market" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Planets</a><br>", URLAppend( cnt, "planets" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Empire</a><br>&nbsp;&nbsp;- ", URLAppend( cnt, "empire" ) );
 httpPrintf( cnt, "<a href=\"%s&forum=%d\" target=\"main\">Forum</a><br>&nbsp;&nbsp;- ", URLAppend( cnt, "forum" ), maind.empire + 100 );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Send aid</a><br>&nbsp;&nbsp;- ", URLAppend( cnt, "famaid" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Receive aid</a><br>&nbsp;&nbsp;- ", URLAppend( cnt, "famgetaid" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">News</a><br>&nbsp;&nbsp;- ", URLAppend( cnt, "famnews" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Relations</a><br>", URLAppend( cnt, "famrels" ) );

 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Fleets</a><br>", URLAppend( cnt, "fleets" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Galaxy map</a><br>&nbsp;&nbsp;- ", URLAppend( cnt, "mappick" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Full map</a><br>&nbsp;&nbsp;- ", URLAppend( cnt, "map" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Map Gen</a><br>", URLAppend( cnt, "mapadv" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Research</a><br>", URLAppend( cnt, "research" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Operations</a><br>", URLAppend( cnt, "spec" ) );

 httpString( cnt, "</font></b></td></tr></table></td></tr><tr><td background=\"files?type=image&name=i36.jpg\"><img height=\"15\" src=\"files?type=image&name=i53.jpg\" width=\"150\"></td></tr><tr><td background=\"files?type=image&name=i36.jpg\"><table width=\"125\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" align=\"left\"><tr><td><b><font face=\"Tahoma\" size=\"2\">" );
 
 httpPrintf( cnt, "<a href=\"%s&type=0\" target=\"main\">Messages</a><br>", URLAppend( cnt, "mail" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Faction rankings</a><br>", URLAppend( cnt, "rankings" ) );
 httpPrintf( cnt, "<a href=\"%s&typ=1\" target=\"main\">Empire rankings</a><br>", URLAppend( cnt, "rankings" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Forums</a><br>", URLAppend( cnt, "forum" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"main\">Account</a><br>", URLAppend( cnt, "account" ) );
 httpPrintf( cnt, "<a href=\"%s\" target=\"_top\">Logout</a><br><br>", URLAppend( cnt, "logout" ) );

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\" target=\"main\"><input type=\"text\" name=\"search\" size=\"8\" value=\"\"><input type=\"submit\" size=\"2\" value=\"OK\"></form>", URLAppend( cnt, "search" ) );
/*
 strcpy(szFaction, maind.faction);
 for(i=0;i<strlen(szFaction);i++)
	{
		if (szFaction[i] == ' ')
		{
			for(j=i;j<(strlen(szFaction)-1);j++)
				szFaction[j] = szFaction[j+1];
			szFaction[j] = '\0';
		}
		if(i == 15)
		{
			szFaction[i-1] = '\0';
			break;
		}
	}
 httpString( cnt, "<a href=\"http://evtools.awardspace.com/starfury\" target=\"blank\">Guide</a><br>" );
 httpString( cnt, "<a href=\"chat\" target=\"blank\">Chat</a><br>" );
*/
if( (cnt->session)->dbuser ) {
	if( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) {
		httpPrintf( cnt, "<br><a href=\"%s\" target=\"main\">Moderator panel</a>", URLAppend( cnt, "moderator" ) );
	} if( ((cnt->session)->dbuser)->level >= LEVEL_ADMINISTRATOR ) {
		httpPrintf( cnt, "<br><a href=\"%s\" target=\"_top\">Admin panel</a>", URLAppend( cnt, "administration" ) );
	}
}

 httpString( cnt, "</font></b></td></tr></table></td></tr><tr><td><img height=\"20\" src=\"files?type=image&name=i55.jpg\" width=\"150\"></td></tr><tr><td><img height=\"75\" src=\"files?type=image&name=i56.jpg\" width=\"150\"></td></tr></table></body></html>" );

 return;
}


void iohtmlNewsString( ReplyDataPtr cnt, int64_t *newsd )
{
 int a, b;
 dbUserMainDef maind, main2d;

 httpPrintf( cnt, "<br><br><i>Week %lld, year %lld</i><br>", (long long)newsd[0] % 52, (long long)newsd[0] / 52 );
 if( (long long)newsd[2] == CMD_NEWS_BUILDING )
  httpPrintf( cnt, "You built %lld %s on the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>", (long long)newsd[4], cmdBuildingName[ (long long)newsd[3] & 0xFFF ], URLAppend( cnt, "planet" ), (long long)newsd[5], ( (long long)newsd[6] >> 8 ) & 0xFFF, (long long)newsd[6] >> 20, (long long)newsd[6] & 0xFF );
 else if( (long long)newsd[2] == CMD_NEWS_UNIT )
  httpPrintf( cnt, "You built %lld %s", (long long)newsd[4], cmdUnitName[ (long long)newsd[3] & 0xFFF ] );
 else if( (long long)newsd[2] == CMD_NEWS_EXPLORE )
 {
  httpPrintf( cnt, "Your exploration ship reached the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> and established a colony.", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
  if( ( (long long)newsd[5] >= 0 ) && ( (long long)newsd[5] < ARTEFACT_NUMUSED ) )
   httpPrintf( cnt, "<br>We discovered an ancient artefact on this planet! <b>%s</b>", artefactName[ (long long)newsd[5] ] );
 }
 else if( (long long)newsd[2] == CMD_NEWS_EXPLORE_FAILED )
  httpPrintf( cnt, "Your exploration ship reached the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>, but the planet was already habited. The ship is now awaiting your orders in this solar system.", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
 else if((long long)newsd[2] == CMD_NEWS_FLEETS_MERGE)
 	httpPrintf(cnt, "Two of your fleets have merge at position %lld,%lld after travelling %lld weeks", ((long long)newsd[3] >> 8 ) & 0xFFF, (long long)newsd[3] >> 20, (long long)newsd[6]);
 else if( (long long)newsd[2] == CMD_NEWS_STATION )
  httpPrintf( cnt, "Your fleet reached the planet <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> and stationned.", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
 else if( (long long)newsd[2] == CMD_NEWS_STATION_FAILED )
  httpPrintf( cnt, "Your fleet heading for the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> could not station as you do not own this planet. Your forces is now awaiting your orders in this solar system.", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
 else if( (long long)newsd[2] == CMD_NEWS_RECALL )
  httpPrintf( cnt, "A fleet travelling for %lld weeks rejoined our main forces", (long long)newsd[3] );
 else if( (long long)newsd[2] == CMD_NEWS_ATTACK )
 {
  if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
   return;
  httpPrintf( cnt, "You lost the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> to ", URLAppend( cnt, "planet" ), (long long)newsd[5], ( (long long)newsd[6] >> 8 ) & 0xFF, (long long)newsd[6] >> 20, (long long)newsd[6] & 0xFF );
  httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], maind.faction );
  httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
  goto iohtmlNewsStringL0;
 }
 else if( (long long)newsd[2] == CMD_NEWS_ATTACK_FAILED )
 {
  if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
   return;
  httpPrintf( cnt, "Your <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> was unsuccessfully attacked by ", URLAppend( cnt, "planet" ), (long long)newsd[5], ( (long long)newsd[6] >> 8 ) & 0xFF, (long long)newsd[6] >> 20, (long long)newsd[6] & 0xFF );
  httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], maind.faction );
  httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
  iohtmlNewsStringL0:
  httpString( cnt, "<br>You lost : " );
  for( a = b = 0 ; a < CMD_UNIT_FLEET ; a++ )
  {
   if( !( (long long)newsd[8+a] ) )
    continue;
   if( b )
    httpString( cnt, ", " );
   httpPrintf( cnt, "%lld %s", (long long)newsd[8+a], cmdUnitName[a] );
   b = 1;
  }
  if( (long long)newsd[8+2*CMD_UNIT_FLEET] )
  {
   if( b )
    httpString( cnt, ", " );
   httpPrintf( cnt, "%lld %s", (long long)newsd[8+2*CMD_UNIT_FLEET], cmdBuildingName[CMD_BUILDING_SATS] );
   b = 1;
  }
  if( !( b ) )
   httpString( cnt, "Nothing" );
  httpString( cnt, "<br>You destroyed : " );
  for( a = b = 0 ; a < CMD_UNIT_FLEET ; a++ )
  {
   if( !( (long long)newsd[8+CMD_UNIT_FLEET+a] ) )
    continue;
   if( b )
    httpString( cnt, ", " );
   httpPrintf( cnt, "%lld %s", (long long)newsd[8+CMD_UNIT_FLEET+a], cmdUnitName[a] );
   b = 1;
  }
  if( !( b ) )
   httpString( cnt, "Nothing" );
  if( ( (long long)newsd[7] & 0xF00 ) )
  {
   b = (long long)newsd[7] & 0xF00;
   httpString( cnt, "<br>Your forces preferred to avoid directly engaging enemy units in " );
   if( b == 0x100 )
    httpString( cnt, "the first phase" );
   else if( b == 0x200 )
    httpString( cnt, "the second phase" );
   else if( b == 0x300 )
    httpString( cnt, "the first and second phases" );
   else if( b == 0x400 )
    httpString( cnt, "the third phase" );
   else if( b == 0x500 )
    httpString( cnt, "the first and third phases" );
   else if( b == 0x600 )
    httpString( cnt, "the second and third phases" );
   else if( b == 0x700 )
    httpString( cnt, "the first, second and third phases" );
   else if( b == 0x800 )
    httpString( cnt, "the fourth phase" );
   else if( b == 0x900 )
    httpString( cnt, "the first and fourth phases" );
   else if( b == 0xA00 )
    httpString( cnt, "the second and fourth phases" );
   else if( b == 0xB00 )
    httpString( cnt, "the first, second and fourth phases" );
   else if( b == 0xC00 )
    httpString( cnt, "the third and fourth phases" );
   else if( b == 0xD00 )
    httpString( cnt, "the first, third and fourth phases" );
   else if( b == 0xE00 )
    httpString( cnt, "the second, third and fourth phases" );
   else if( b == 0xF00 )
    httpString( cnt, "all phases" );
   httpString( cnt, " of the battle" );
  }
  if( ( (long long)newsd[7] & 0xFF ) )
  {
   httpString( cnt, "<br>Overwhelmed by defending enemy forces, the attacking fleet hastidly retreated to minimize losses in the " );
   if( ( (long long)newsd[7] & 0x10 ) )
    httpString( cnt, "first phase of the battle" );
   if( ( (long long)newsd[7] & 0x20 ) )
    httpString( cnt, "second phase of the battle" );
   if( ( (long long)newsd[7] & 0x40 ) )
    httpString( cnt, "third phase of the battle" );
   if( ( (long long)newsd[7] & 0x80 ) )
    httpString( cnt, "fourth phase of the battle" );
  }
 }
 else if( (long long)newsd[2] == CMD_NEWS_MARKET_BOUGHT )
  httpPrintf( cnt, "You bought %lld %s on the market.", (long long)newsd[4], cmdRessourceName[(long long)newsd[3]+1] );
 else if( (long long)newsd[2] == CMD_NEWS_MARKET_SOLD )
  httpPrintf( cnt, "You sold %lld %s on the market.", (long long)newsd[4], cmdRessourceName[(long long)newsd[3]+1] );
 else if( (long long)newsd[2] == CMD_NEWS_AID )
 {
  if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
   return;
  httpPrintf( cnt, "You received an aid shipment from %s!<br>", maind.faction );
  for( a = b = 0 ; a < 4 ; a++ )
  {
   if( !( (long long)newsd[4+a] ) )
    continue;
   if( b )
    httpString( cnt, ", " );
   httpPrintf( cnt, "%lld %s", (long long)newsd[4+a], cmdRessourceName[a] );
   b = 1;
  }
  httpString( cnt, " has been added to the reserves." );
 }
 else if( (long long)newsd[2] == CMD_NEWS_GETAID )
 {
  if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
   return;
  httpPrintf( cnt, "%s requested an aid shipment!<br>", maind.faction );
  for( a = b = 0 ; a < 4 ; a++ )
  {
   if( !( (long long)newsd[4+a] ) )
    continue;
   if( b )
    httpString( cnt, ", " );
   httpPrintf( cnt, "%lld %s", (long long)newsd[4+a], cmdRessourceName[a] );
   b = 1;
  }
  httpString( cnt, " has been taken from the faction reserves." );
 }
 else if( (long long)newsd[2] == CMD_NEWS_MAIL ) {
  httpPrintf( cnt, "You received a <a href=\"%s&type=0\">message</a> from ", URLAppend( cnt, "mail" ) );
  httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[4], (char *)&newsd[6] );
  httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>.", URLAppend( cnt, "empire" ), (long long)newsd[5], (long long)newsd[5] );
 } else if( ( (long long)newsd[2] >= CMD_NEWS_NUMOPBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMOPEND ) )
 {
  httpPrintf( cnt, "Your agents reached their destination, the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
  if( ( dbUserMainRetrieve( (long long)newsd[5], &main2d ) ) ) {
   httpPrintf( cnt, " owned by <a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[5], main2d.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[6], (long long)newsd[6] );
  }
  httpPrintf( cnt, " to perform <b>%s</b>.<br>", cmdAgentopName[(long long)newsd[9]] );
  if( main2d.faction )
  {
   if( (long long)newsd[7] == -1 )
    httpPrintf( cnt, "<i>Your agents successfully stayed undiscovered during the operation.</i><br>" );
   else
    httpPrintf( cnt, "<i>%lld of your agents were caught, but your forces still managed to kill %lld defending agents.</i><br>", (long long)newsd[7], (long long)newsd[8] );
  }

  if( (long long)newsd[2] == CMD_NEWS_OPSPYTARGET )
  {
   httpPrintf( cnt, "Your agents gathered the following information.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] == -1 )
    httpString( cnt, "Fleet readiness : unknown<br>" );
   else
    httpPrintf( cnt, "Fleet readiness : %lld%%<br>", (long long)newsd[10] >> 16 );
   if( (long long)newsd[11] == -1 )
    httpString( cnt, "Psychics readiness : unknown<br>" );
   else
    httpPrintf( cnt, "Psychics readiness : %lld%%<br>", (long long)newsd[11] >> 16 );
   if( (long long)newsd[12] == -1 )
    httpString( cnt, "Agents readiness : unknown<br>" );
   else
    httpPrintf( cnt, "Agents readiness : %lld%%<br>", (long long)newsd[12] >> 16 );
   for( a = 13 ; a < 13+CMD_RESSOURCE_NUMUSED ; a++ )
   {
    httpString( cnt, cmdRessourceName[a-13] );
    if( (long long)newsd[a] == -1 )
     httpString( cnt, " : unknown<br>" );
    else
     httpPrintf( cnt, " : %lld<br>", (long long)newsd[a] );
   }
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPOBSERVEPLANET )
  {
   httpPrintf( cnt, "Your agents gathered the following information.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] == -1 )
    httpString( cnt, "Planet size : unknown<br>" );
   else
    httpPrintf( cnt, "Planet size : %lld<br>", (long long)newsd[10] );
   if( (long long)newsd[11] )
   {
    if( (long long)newsd[11] == -1 )
     httpString( cnt, "Population : unknown<br>" );
    else
     httpPrintf( cnt, "Population : %lld0<br>", (long long)newsd[11] );
   }
   if( (long long)newsd[12] )
   {
    if( (long long)newsd[12] == -1 )
     httpString( cnt, "Maximum population : unknown<br>" );
    else
     httpPrintf( cnt, "Maximum population : %lld0<br>", (long long)newsd[12] );
   }
   if( (long long)newsd[13] == -1 )
    httpString( cnt, "Portals coverage : unknown<br>" );
   else
    httpPrintf( cnt, "Portals coverage : %lld%%<br>", (long long)newsd[13] );
   if( (long long)newsd[14] == -1 )
    httpString( cnt, "Portal : unknown<br>" );
   else
   {
    if( (long long)newsd[14] == CMD_PLANET_FLAGS_PORTAL )
     httpPrintf( cnt, "Portal : Present<br>" );
    else if( (long long)newsd[14] == CMD_PLANET_FLAGS_PORTAL_BUILD )
     httpPrintf( cnt, "Portal : Under construction<br>" );
    else if( (long long)newsd[14] == CMD_PLANET_FLAGS_PORTAL_BUILD )
     httpPrintf( cnt, "Portal : None<br>" );
   }
   for( a = 15 ; a < 15+CMD_BLDG_NUMUSED ; a++ )
   {
    httpString( cnt, cmdBuildingName[a-15] );
    if( (long long)newsd[a] == -1 )
     httpString( cnt, " : unknown<br>" );
    else
     httpPrintf( cnt, " : %lld<br>", (long long)newsd[a] );
   }
   if( (long long)newsd[15+CMD_BLDG_NUMUSED] >= 0 )
    httpPrintf( cnt, "We discovered an ancient artefact on this planet! <b>%s</b><br>", artefactName[ (long long)newsd[15+CMD_BLDG_NUMUSED] ] );
   if( (long long)newsd[15+2+CMD_BLDG_NUMUSED] > 0 )
    httpPrintf( cnt, "%s production : +%lld%%<br>", cmdRessourceName[ (long long)newsd[15+1+CMD_BLDG_NUMUSED] ], (long long)newsd[15+2+CMD_BLDG_NUMUSED] );
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPNETWORKVIRUS )
  {
   httpPrintf( cnt, "These are the results of the operation.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] == -1 )
    httpString( cnt, "Destroying research : failed<br>" );
   else
    httpPrintf( cnt, "Destroying research : %lld %% lost<br>", (long long)newsd[10] );
   if( (long long)newsd[11] == -1 )
    httpString( cnt, "Increasing upkeep : failed<br>" );
   else
    httpPrintf( cnt, "Increasing upkeep : %lld weeks<br>", (long long)newsd[11] );
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPINFILTRATION )
  {
   httpPrintf( cnt, "Your agents gathered the following information.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   for( a = 10 ; a < 10+CMD_RESSOURCE_NUMUSED ; a++ )
   {
    httpString( cnt, cmdRessourceName[a-10] );
    if( (long long)newsd[a] == -1 )
     httpString( cnt, " : unknown<br>" );
    else
     httpPrintf( cnt, " : %lld<br>", (long long)newsd[a] );
   }
   for( a = 14 ; a < 14+CMD_RESEARCH_NUMUSED ; a++ )
   {
    httpPrintf( cnt, "%s research", cmdResearchName[a-14] );
    if( (long long)newsd[a] == -1 )
     httpString( cnt, " : unknown<br>" );
    else
     httpPrintf( cnt, " : %lld %%<br>", (long long)newsd[a] );
   }
   for( a = 14+CMD_RESEARCH_NUMUSED ; a < 14+CMD_RESEARCH_NUMUSED+CMD_BLDG_NUMUSED ; a++ )
   {
    httpString( cnt, cmdBuildingName[a-14-CMD_RESEARCH_NUMUSED] );
    if( (long long)newsd[a] == -1 )
     httpString( cnt, " : unknown<br>" );
    else
     httpPrintf( cnt, " : %lld<br>", (long long)newsd[a] );
   }
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPBIOINFECTION )
  {
   httpPrintf( cnt, "These are the results of the operation.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] == -1 )
    httpString( cnt, "Kill population : failed<br>" );
   else
    httpPrintf( cnt, "Kill population : %lld0<br>", (long long)newsd[10] );
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPENERGYTRANSFER )
  {
   httpPrintf( cnt, "These are the results of the operation.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] == -1 )
    httpString( cnt, "Target's loss of energy : failed<br>" );
   else
    httpPrintf( cnt, "Target's loss of energy : %lld<br>", (long long)newsd[10] );
   if( (long long)newsd[11] == -1 )
    httpString( cnt, "Energy successfully acquired : failed<br>" );
   else
    httpPrintf( cnt, "Energy successfully acquired : %lld<br>", (long long)newsd[11] );
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPMILITARYSAB )
  {
   httpPrintf( cnt, "These are the results of the operation.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] == -1 )
    httpString( cnt, "Destroy units : failed<br>" );
   else if( (long long)newsd[10] == -2 )
    httpString( cnt, "Destroy units : failed<br>It appears there is no portal on the planet to reach the main fleet from!<br>" );
   else
   {
    for( a = 10 ; a < 10+CMD_UNIT_FLEET ; a++ )
    {
     if( (long long)newsd[a] > 0 )
      httpPrintf( cnt, "%s : %lld destroyed<br>", cmdUnitName[a-10], (long long)newsd[a] );
    }
   }
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPNUKEPLANET )
  {
   httpPrintf( cnt, "These are the results of the operation.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] == -1 )
    httpString( cnt, "Place nuclear devices : failed<br>" );
   else
    httpString( cnt, "Place nuclear devices : succeeded, planet now uninhabited<br>" );
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPHIGHINFIL )
  {
   httpPrintf( cnt, "These are the results of the operation.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] == -1 )
    httpString( cnt, "Infiltration : failed<br>" );
   else
   {
    httpString( cnt, "Faction infiltration : success<br>" );
    if( (long long)newsd[10] & 1 )
     httpString( cnt, "Planets network infiltration : success<br>" );
    else
     httpString( cnt, "Planets network infiltration : failed<br>" );
    if( (long long)newsd[10] & 2 )
     httpString( cnt, "Fleets infiltration : success<br>" );
    else
     httpString( cnt, "Fleets infiltration : failed<br>" );
    httpPrintf( cnt, "You can access the faction information in the <a href=\"%s\">list of current operations</a> for the next two years.<br>", URLAppend( cnt, "spec" ) );
   }
  }
	 else if( (long long)newsd[2] == CMD_NEWS_OPPLANETBEACON )
  {
   httpPrintf( cnt, "These are the results of the operation.<br>" );
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] >= 1 )
   {
    httpString( cnt, "Planeteray beacon : success<br>" );
    httpString( cnt, "The planet have 0 dark web effect but 110% of protection by the owner fleet, all this for 24 ticks<br>" );
   }
   else
   {
    httpString( cnt, "Planeteray beacon : fail<br>" );
   }
   httpString( cnt, "</td></tr></table>" );
  }
 }
 else if( ( (long long)newsd[2] >= CMD_NEWS_NUMOPTARGETBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMOPTARGETEND ) )
 {
  if( (long long)newsd[7] != -1 )
  {
   httpString( cnt, "Your forces intercepted some agents from " );
   if( ( dbUserMainRetrieve( (long long)newsd[5], &main2d ) ) ) {
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[5], main2d.faction );
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[6], (long long)newsd[6] );
   } else
    httpString( cnt, "an unknown faction" );
  }
  else
   httpString( cnt, "Your forces found traces of agents from an unknown faction" );

  httpPrintf( cnt, " performing a <b>%s</b> operation on <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>.<br>", cmdAgentopName[(long long)newsd[9]], URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
  if( (long long)newsd[7] != -1 )
   httpPrintf( cnt, "<i>%lld enemy agents have been arrested, %lld of your agents have been killed.</i><br>", (long long)newsd[7], (long long)newsd[8] );

  if( (long long)newsd[2] == CMD_NEWS_OPNETWORKVIRUS_TARGET )
  {
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] != -1 )
    httpPrintf( cnt, "%lld %% of your research has been lost!<br>", (long long)newsd[10] );
   if( (long long)newsd[11] != -1 )
    httpPrintf( cnt, "Your net building upkeep will be 15%% for higher for %lld weeks.<br>", (long long)newsd[11] );
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPBIOINFECTION_TARGET )
  {
   if( (long long)newsd[10] != -1 )
    httpPrintf( cnt, "%lld0 citizens of your faction died as the disease rapidly spreads.<br>", (long long)newsd[10] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPENERGYTRANSFER_TARGET )
  {
   if( (long long)newsd[10] != -1 )
    httpPrintf( cnt, "%lld energy has been reported missing from your reserves.<br>", (long long)newsd[10] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPMILITARYSAB_TARGET )
  {
   httpString( cnt, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td>" );
   if( (long long)newsd[10] >= 0 )
   {
    for( a = 10 ; a < 10+CMD_UNIT_FLEET ; a++ )
    {
     if( (long long)newsd[a] > 0 )
      httpPrintf( cnt, "%s : %lld destroyed<br>", cmdUnitName[a-10], (long long)newsd[a] );
    }
   }
   httpString( cnt, "</td></tr></table>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_OPNUKEPLANET_TARGET )
  {
   if( (long long)newsd[10] != -1 )
    httpString( cnt, "Nuclear devices exploded everywhere on the surface of the planet! All buildings have been destroyed and the planet is now unhabited.<br>" );
  }
 }

 else if( ( (long long)newsd[2] >= CMD_NEWS_NUMSPBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMSPEND ) )
 {
  httpPrintf( cnt, "Your psychics are casting <b>%s</b> on ", cmdPsychicopName[(long long)newsd[7]] );
  if( (long long)newsd[3] == ((cnt->session)->dbuser)->id )
   httpPrintf( cnt, "your faction.<br>" );
  else
  {
   if( ( dbUserMainRetrieve( (long long)newsd[3], &main2d ) ) ) {
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], main2d.faction );
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>.<br>", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
   } else
    httpPrintf( cnt, "an unknown faction.<br>" );
   if( ( (long long)newsd[3] != ((cnt->session)->dbuser)->id ) && ( main2d.faction ) )
   {
    if( (long long)newsd[5] == -1 )
     httpPrintf( cnt, "<i>Your psychics successfully stayed undiscovered while performing the spell.</i><br>" );
    else
     httpPrintf( cnt, "<i>%lld of your psychics went mad, it appears %lld defending psychics also suffered critical brain damages.</i><br>", (long long)newsd[5], (long long)newsd[6] );
   }
  }

  if( (long long)newsd[2] == CMD_NEWS_SPIRRAECTRO )
  {
   if( (long long)newsd[8] != -1 )
    httpPrintf( cnt, "%lld ectrolium has been irradiated and is now unusable!<br>", (long long)newsd[8] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_SPDARKWEB )
  {
   if( (long long)newsd[8] != -1 )
    httpPrintf( cnt, "Your faction is now more difficult to attack by %lld %%.<br>", (long long)newsd[8] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_SPINCANDESCENCE )
  {
   if( (long long)newsd[8] != -1 )
    httpPrintf( cnt, "%lld crystal has been converted in %lld energy.<br>", (long long)newsd[8], (long long)newsd[9] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_SPBLACKMIST )
  {
   if( (long long)newsd[8] != -1 )
    httpPrintf( cnt, "A dense mist is spreading around the target's planets, your psychics estimate solar collectors efficiency has been reduced by %lld %%.<br>", (long long)newsd[8] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_SPWARILLUSIONS )
  {
   if( (long long)newsd[8] != -1 )
    httpPrintf( cnt, "Various illusions will accompany your fleets for %lld weeks, keeping around %lld %% of enemy fire away from friendly units.<br>", (long long)newsd[9], (long long)newsd[8] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_SPPHANTOMS )
  {
   if( (long long)newsd[8] != -1 )
    httpPrintf( cnt, "%lld Phantoms have been conjured to fight for your faction!<br>", (long long)newsd[8] );
  }

 }

 else if( ( (long long)newsd[2] >= CMD_NEWS_NUMSPTARGETBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMSPTARGETEND ) )
 {
  if( (long long)newsd[5] != -1 )
  {
   httpString( cnt, "Your forces felt the influence of psychics from " );
   if( ( dbUserMainRetrieve( (long long)newsd[3], &main2d ) ) ) {
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], main2d.faction );
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
   } else
    httpString( cnt, "an unknown faction" );
  }
  else
   httpString( cnt, "Your forces felt the influence of psychics from an unknown faction" );
  httpPrintf( cnt, " performing a <b>%s</b> spell.<br>", cmdPsychicopName[(long long)newsd[7]] );
  if( (long long)newsd[5] != -1 )
   httpPrintf( cnt, "<i>%lld enemy psychics have went mad, %lld of your psychics also suffered critical brain damages.</i><br>", (long long)newsd[5], (long long)newsd[6] );

  if( (long long)newsd[2] == CMD_NEWS_SPIRRAECTRO_TARGET )
  {
   if( (long long)newsd[8] != -1 )
    httpPrintf( cnt, "%lld ectrolium has been irradiated and is now unusable!<br>", (long long)newsd[8] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_SPBLACKMIST_TARGET )
  {
   if( (long long)newsd[8] != -1 )
    httpPrintf( cnt, "A dense mist is spreading around your planets, your psychics estimate solar collectors efficiency has been reduced by %lld %%.<br>", (long long)newsd[8] );
  }
 }




 else if( ( (long long)newsd[2] >= CMD_NEWS_NUMINBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMINEND ) )
 {
  httpPrintf( cnt, "Your ghost ships casted <b>%s</b> ", cmdGhostopName[(long long)newsd[10]] );
  if( cmdGhostopFlags[ (long long)newsd[10] ] & 4 )
   httpPrintf( cnt, "in the system %lld,%lld<br>", ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20 );
  else
  {
   httpPrintf( cnt, "on the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
   if( ( dbUserMainRetrieve( (long long)newsd[5], &main2d ) ) )
   {
    httpPrintf( cnt, " owned by <a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[5], main2d.faction );
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a><br>", URLAppend( cnt, "empire" ), (long long)newsd[6], (long long)newsd[6] );
    if( (long long)newsd[7] <= 0 )
     httpPrintf( cnt, "<i>Your ghost ships successfully stayed undiscovered during the incantation.</i><br>" );
    else
     httpPrintf( cnt, "<i>%lld of your ghost ships were oblitered by defending forces.</i><br>", (long long)newsd[7] );
    if( (long long)newsd[8] > 0 )
     httpPrintf( cnt, "<i>%lld defending psychics were killed.</i><br>", (long long)newsd[8] );
    if( (long long)newsd[9] > 0 )
     httpPrintf( cnt, "<i>%lld defending ghost ships were destroyed.</i><br>", (long long)newsd[9] );
   }
  }

  if( (long long)newsd[2] == CMD_NEWS_INSENSE )
  {
   if( (long long)newsd[11] & 0x10000 )
   {
    a = (long long)newsd[11] & 0xFFFF;
    if( a >= 4 )
     httpPrintf( cnt, "<i>An artefact has been discovered, at the coordinates %lld,%lld:%lld!</i><br>", ( (long long)newsd[12] >> 8 ) & 0xFF, ((long long)newsd[12] >> 20) & 0xFF, (long long)newsd[12] & 0xFF );
    else if( a == 3 )
     httpPrintf( cnt, "<i>An artefact has been found in a nearby system, at the coordinates %lld,%lld!</i><br>", ( (long long)newsd[12] >> 8 ) & 0xFF, ((long long)newsd[12] >> 20) & 0xFF );
    else if( a == 2 )
     httpPrintf( cnt, "<i>An artefact was detected in the area, but its precise location remains unknown.</i><br>" );
    else
     httpPrintf( cnt, "<i>An artefact was felt somewhere in the area!</i><br>" );
   }
   else
    httpPrintf( cnt, "<i>No artefact was felt in the area.</i><br>" );


   if( dbUserMainRetrieve( ((cnt->session)->dbuser)->id, &maind ) < 0 )
   	return;
  }
  else if( (long long)newsd[2] == CMD_NEWS_INSURVEY )
  {
   if( (long long)newsd[11] )
   {
    httpPrintf( cnt, "Planets surveyed : %lld<br>", (long long)newsd[11] );
    httpPrintf( cnt, "Incantation duration : %lld<br>", (long long)newsd[12] );
    httpPrintf( cnt, "You can access the system information in the <a href=\"%s\">list of current operations</a> for the duration of the incantation.<br>", URLAppend( cnt, "spec" )  );
   }
   else
    httpString( cnt, "Planets surveyed : none<br>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_INSHIELDING )
  {
   httpPrintf( cnt, "Planetary shield strength : %lld<br>", (long long)newsd[11] );
   httpPrintf( cnt, "Incantation duration : %lld<br>", (long long)newsd[12] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_INFORCEFIELD )
  {
   httpPrintf( cnt, "Force field strength : %lld%%<br>", (long long)newsd[11] );
   httpPrintf( cnt, "Incantation duration : %lld<br>", (long long)newsd[12] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_INVORTEX )
  {
   httpString( cnt, "Vortex portal duration : " );
   if( (long long)newsd[11] != -1 )
    httpPrintf( cnt, "%lld<br>", (long long)newsd[11] );
   else
    httpString( cnt, "failed<br>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_INMINDCONTROL )
  {
   httpString( cnt, "Mind control incantation : " );
   if( (long long)newsd[11] == -1 )
    httpString( cnt, "failed<br>" );
   else
    httpString( cnt, "success<br>" );
  }
  else if( (long long)newsd[2] == CMD_NEWS_INENERGYSURGE )
  {
   if( (long long)newsd[17] == -1 )
    httpString( cnt, "Energy surge : failed<br>" );
   else
   {
    if( (long long)newsd[17] > 0 )
     httpPrintf( cnt, "Energy wasted : %lld<br>", (long long)newsd[17] );
    if( (long long)newsd[11] > 0 )
     httpPrintf( cnt, "Mineral lost : %lld<br>", (long long)newsd[11] );
    if( (long long)newsd[12] > 0 )
     httpPrintf( cnt, "Crystal lost : %lld<br>", (long long)newsd[12] );
    if( (long long)newsd[13] > 0 )
     httpPrintf( cnt, "Ectrolium lost : %lld<br>", (long long)newsd[13] );
    if( (long long)newsd[14] > 0 )
     httpPrintf( cnt, "Solar collectors destroyed : %lld<br>", (long long)newsd[14] );
    if( (long long)newsd[15] > 0 )
     httpPrintf( cnt, "Fission reactors destroyed : %lld<br>", (long long)newsd[15] );
    if( (long long)newsd[16] > 0 )
     httpPrintf( cnt, "Research points eliminated : %lld<br>", (long long)newsd[16] );
   }
  }



 }
 else if( ( (long long)newsd[2] >= CMD_NEWS_NUMINTARGETBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMINTARGETEND ) )
 {
  if( (long long)newsd[7] != -1 )
  {
   httpString( cnt, "Your forces were the target of ghost ships from" );
   if( ( dbUserMainRetrieve( (long long)newsd[5], &main2d ) ) ) {
    httpPrintf( cnt, " <a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[5], main2d.faction );
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[6], (long long)newsd[6] );
   } else
    httpString( cnt, " an unknown faction" );
  }
  else
   httpString( cnt, "Your forces were the target of ghost ships from an unknown faction" );
  httpPrintf( cnt, " performing a <b>%s</b> incantation", cmdGhostopName[(long long)newsd[10]] );
  if( (long long)newsd[3] != -1 )
   httpPrintf( cnt, " on <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
  httpString( cnt, ".<br>" );
  if( (long long)newsd[7] > 0 )
   httpPrintf( cnt, "<i>%lld enemy ghost ships were destroyed.</i><br>", (long long)newsd[7] );
  if( (long long)newsd[8] > 0 )
   httpPrintf( cnt, "<i>%lld of your psychics were killed.</i><br>", (long long)newsd[8] );
  if( (long long)newsd[9] > 0 )
   httpPrintf( cnt, "<i>%lld of your ghost ships were destroyed.</i><br>", (long long)newsd[9] );

  if( (long long)newsd[2] == CMD_NEWS_INSHIELDING_TARGET )
  {
   httpPrintf( cnt, "Planetary shield strength : %lld<br>", (long long)newsd[11] );
   httpPrintf( cnt, "Incantation duration : %lld<br>", (long long)newsd[12] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_INFORCEFIELD_TARGET )
  {
   httpPrintf( cnt, "Force field strength : %lld%%<br>", (long long)newsd[11] );
   httpPrintf( cnt, "Incantation duration : %lld<br>", (long long)newsd[12] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_INMINDCONTROL_TARGET )
  {
   httpString( cnt, "Mind control incantation : " );
   if( (long long)newsd[11] == -1 )
    httpString( cnt, "failed<br>" );
   else
    httpString( cnt, "success<br>" );
  }

  else if( (long long)newsd[2] == CMD_NEWS_INENERGYSURGE_TARGET )
  {
   if( (long long)newsd[17] == -1 )
    httpString( cnt, "Energy surge : failed<br>" );
   else
   {
    if( (long long)newsd[17] > 0 )
     httpPrintf( cnt, "Energy wasted : %lld<br>", (long long)newsd[17] );
    if( (long long)newsd[11] > 0 )
     httpPrintf( cnt, "Mineral lost : %lld<br>", (long long)newsd[11] );
    if( (long long)newsd[12] > 0 )
     httpPrintf( cnt, "Crystal lost : %lld<br>", (long long)newsd[12] );
    if( (long long)newsd[13] > 0 )
     httpPrintf( cnt, "Ectrolium lost : %lld<br>", (long long)newsd[13] );
    if( (long long)newsd[14] > 0 )
     httpPrintf( cnt, "Solar collectors destroyed : %lld<br>", (long long)newsd[14] );
    if( (long long)newsd[15] > 0 )
     httpPrintf( cnt, "Fission reactors destroyed : %lld<br>", (long long)newsd[15] );
    if( (long long)newsd[16] > 0 )
     httpPrintf( cnt, "Research points eliminated : %lld<br>", (long long)newsd[16] );
   }
  }


 }



 else if( (long long)newsd[2] == CMD_NEWS_PLANET_OFFER )
 {
  if( ( dbUserMainRetrieve( (long long)newsd[2], &main2d ) ) ) {
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> offered a ", URLAppend( cnt, "player" ), (long long)newsd[3], main2d.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">planet</a> to your faction. ", URLAppend( cnt, "planet" ), (long long)newsd[4] );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">Take it</a><br>", URLAppend( cnt, "pltake" ), (long long)newsd[4] );
   }
 }
 else if( (long long)newsd[2] == CMD_NEWS_PLANET_GIVEN )
 {
  if( ( dbUserMainRetrieve( (long long)newsd[3], &main2d ) ) ) {
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> took control of a ", URLAppend( cnt, "player" ), (long long)newsd[3], main2d.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">planet</a> previously offered.<br>", URLAppend( cnt, "planet" ), (long long)newsd[4] );
   }
 }
 else if( (long long)newsd[2] == CMD_NEWS_PLANET_TAKEN )
 {
  if( ( dbUserMainRetrieve( (long long)newsd[3], &main2d ) ) ) {
   httpPrintf( cnt, "You took control of a <a href=\"%s&id=%lld\">planet</a> offered by ", URLAppend( cnt, "planet" ), (long long)newsd[4] );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a>.<br>", URLAppend( cnt, "player" ), (long long)newsd[3], main2d.faction );
   }
 }



 else if( (long long)newsd[2] == CMD_NEWS_OPCANCEL )
  httpPrintf( cnt, "An agents operation has been cancelled." );
 else if( (long long)newsd[2] == CMD_NEWS_SPCANCEL )
  httpPrintf( cnt, "A psychics spell has been cancelled." );
 else if( (long long)newsd[2] == CMD_NEWS_INCANCEL )
  httpPrintf( cnt, "A ghost ships incantation has been cancelled." );

 else if( (long long)newsd[2] != -1 )
  httpPrintf( cnt, "Unknown report code : %lld ( tell the admin, thanks ;) )", (long long)newsd[2] );

 return;
}

int iohtmlFamNewsEntryCount;

void iohtmlFamNewsEntry( ReplyDataPtr cnt, int picture, int64_t *newsd )
{
 httpPrintf( cnt, "<tr><td" );
 if( !( iohtmlFamNewsEntryCount ) )
  httpPrintf( cnt, " width=\"5%%\"" );
 if( picture >= 0 )
  httpPrintf( cnt, "><img src=\"files?type=image&name=fn%d.gif\"></td><td", picture );
 else
  httpPrintf( cnt, "><br></td><td" );
 if( !( iohtmlFamNewsEntryCount ) )
 {
  httpPrintf( cnt, " width=\"95%%\"" );
  iohtmlFamNewsEntryCount = 1;
 }
 httpPrintf( cnt, "><i>Week %lld, year %lld</i><br>", (long long)newsd[0] % 52, (long long)newsd[0] / 52 );
 return;
}

void iohtmlFamNews( ReplyDataPtr cnt, int num, int64_t *newsd, dbMainEmpirePtr empired )
{
 int a, b, c;
 dbUserMainDef maind;
 dbUserMainDef *mfamd;
 

 httpPrintf( cnt, "Current date : Week %d, year %d<br>", ticks.number % 52, ticks.number / 52 );
 if( ticks.status )
  httpPrintf( cnt, "%d seconds before tick<br>", (int)( ticks.next - time(0) ) );
 else
  httpPrintf( cnt, "Time frozen<br>" );

if( !( num ) )
 {
  httpString( cnt, "<br><b>There are no news reports to display.</b><br>" );
  return;
 }

 mfamd = malloc( empired->numplayers * sizeof(dbUserMainDef) );
 if( mfamd == NULL ) {
 	critical("Family News Malloc");
 	return;
 }

 for( a = 0 ; a < empired->numplayers ; a++ )
 {
  if( dbUserMainRetrieve( empired->player[a], &mfamd[a] ) < 0 )
   continue;
 }
 
 iohtmlFamNewsEntryCount = 0;
 httpString( cnt, "<table cellspacing=\"4\" cellpadding=\"4\">" );
 for( c = 0 ; c < num ; c++, newsd += DB_USER_NEWS_BASE )
 {
  for( b = 0 ; ; b++ )
  {
   if( b == empired->numplayers )
    goto iohtmlFamNewsL0;
   if( (long long)newsd[1] == empired->player[b] )
    break;
  }
  if( (long long)newsd[2] == CMD_NEWS_EXPLORE )
  {
   iohtmlFamNewsEntry( cnt, 0, newsd );
   httpPrintf( cnt, "An exploration ship of %s reached the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> and established a colony.", mfamd[b].faction, URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
   if( ( (long long)newsd[5] >= 0 ) && ( (long long)newsd[5] < ARTEFACT_NUMUSED ) )
    httpPrintf( cnt, "<br>An ancient artefact has been discovered on this planet! <b>%s</b>", artefactName[ (long long)newsd[5] ] );
  }
  else if( (long long)newsd[2] == CMD_NEWS_EXPLORE_FAILED )
  {
   iohtmlFamNewsEntry( cnt, 1, newsd );
   httpPrintf( cnt, "An exploration ship of %s reached the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>, but the planet was already habited.", mfamd[b].faction, URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
  }
  else if( (long long)newsd[2] == CMD_NEWS_FAMATTACK )
  {
   iohtmlFamNewsEntry( cnt, 2, newsd );
   if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
    goto iohtmlFamNewsL0;
   httpPrintf( cnt, "%s attacked and took control of the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> previously owned by ", mfamd[b].faction, URLAppend( cnt, "planet" ), (long long)newsd[5], ( (long long)newsd[6] >> 8 ) & 0xFF, (long long)newsd[6] >> 20, (long long)newsd[6] & 0xFF );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], maind.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
   goto iohtmlFamNewsL1;
  }
  else if( (long long)newsd[2] == CMD_NEWS_FAMATTACK_FAILED )
  {
   iohtmlFamNewsEntry( cnt, 3, newsd );
   if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
    goto iohtmlFamNewsL0;
   httpPrintf( cnt, "%s attacked and failed to capture the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> owned by ", mfamd[b].faction, URLAppend( cnt, "planet" ), (long long)newsd[5], ( (long long)newsd[6] >> 8 ) & 0xFF, (long long)newsd[6] >> 20, (long long)newsd[6] & 0xFF );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], maind.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
   goto iohtmlFamNewsL1;
  }
  else if( (long long)newsd[2] == CMD_NEWS_ATTACK )
  {
   iohtmlFamNewsEntry( cnt, 5, newsd );
   if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
    goto iohtmlFamNewsL0;
   httpPrintf( cnt, "%s lost the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a> to ", mfamd[b].faction, URLAppend( cnt, "planet" ), (long long)newsd[5], ( (long long)newsd[6] >> 8 ) & 0xFF, (long long)newsd[6] >> 20, (long long)newsd[6] & 0xFF );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3] );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), maind.faction, (long long)newsd[4], (long long)newsd[4] );
   goto iohtmlFamNewsL1;
  }
  else if( (long long)newsd[2] == CMD_NEWS_ATTACK_FAILED )
  {
   iohtmlFamNewsEntry( cnt, 4, newsd );
   if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
    goto iohtmlFamNewsL0;
   httpPrintf( cnt, "The <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>, owned by %s, was unsuccessfully attacked by ", URLAppend( cnt, "planet" ), (long long)newsd[5], ( (long long)newsd[6] >> 8 ) & 0xFF, (long long)newsd[6] >> 20, (long long)newsd[6] & 0xFF, mfamd[b].faction );
   httpPrintf( cnt, "<a href=\"%s&?id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], maind.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
   iohtmlFamNewsL1:
   httpString( cnt, "<br>Defender losses : " );
   for( a = b = 0 ; a < CMD_UNIT_FLEET ; a++ )
   {
    if( !( (long long)newsd[8+a] ) )
     continue;
    if( b )
     httpString( cnt, ", " );
    httpPrintf( cnt, "%lld %s", (long long)newsd[8+a], cmdUnitName[a] );
    b = 1;
   }
   if( (long long)newsd[8+2*CMD_UNIT_FLEET] )
   {
    if( b )
     httpString( cnt, ", " );
    httpPrintf( cnt, "%lld %s", (long long)newsd[8+2*CMD_UNIT_FLEET], cmdBuildingName[CMD_BUILDING_SATS] );
    b = 1;
   }
   if( !( b ) )
    httpString( cnt, "Nothing" );
   httpString( cnt, "<br>Attacker losses : " );
   for( a = b = 0 ; a < CMD_UNIT_FLEET ; a++ )
   {
    if( !( (long long)newsd[8+CMD_UNIT_FLEET+a] ) )
     continue;
    if( b )
     httpString( cnt, ", " );
    httpPrintf( cnt, "%lld %s", (long long)newsd[8+CMD_UNIT_FLEET+a], cmdUnitName[a] );
    b = 1;
   }
   if( !( b ) )
    httpString( cnt, "Nothing" );
   if( ( (long long)newsd[7] & 0xF00 ) )
   {
    b = (long long)newsd[7] & 0xF00;
    httpString( cnt, "<br>Defending forces preferred to avoid directly engaging enemy units in " );
    if( b == 0x100 )
     httpString( cnt, "the first phase" );
    else if( b == 0x200 )
     httpString( cnt, "the second phase" );
    else if( b == 0x300 )
     httpString( cnt, "the first and second phases" );
    else if( b == 0x400 )
     httpString( cnt, "the third phase" );
    else if( b == 0x500 )
     httpString( cnt, "the first and third phases" );
    else if( b == 0x600 )
     httpString( cnt, "the second and third phases" );
    else if( b == 0x700 )
     httpString( cnt, "the first, second and third phases" );
    else if( b == 0x800 )
     httpString( cnt, "the fourth phase" );
    else if( b == 0x900 )
     httpString( cnt, "the first and fourth phases" );
    else if( b == 0xA00 )
     httpString( cnt, "the second and fourth phases" );
    else if( b == 0xB00 )
     httpString( cnt, "the first, second and fourth phases" );
    else if( b == 0xC00 )
     httpString( cnt, "the third and fourth phases" );
    else if( b == 0xD00 )
     httpString( cnt, "the first, third and fourth phases" );
    else if( b == 0xE00 )
     httpString( cnt, "the second, third and fourth phases" );
    else if( b == 0xF00 )
     httpString( cnt, "all phases" );
    httpString( cnt, " of the battle" );
   }
   if( ( (long long)newsd[7] & 0xFF ) )
   {
    httpString( cnt, "<br>Overwhelmed by defending enemy forces, the attacking fleet hastidly retreated to minimize losses in the " );
    if( ( (long long)newsd[7] & 0x10 ) )
     httpString( cnt, "first phase of the battle" );
    if( ( (long long)newsd[7] & 0x20 ) )
     httpString( cnt, "second phase of the battle" );
    if( ( (long long)newsd[7] & 0x40 ) )
     httpString( cnt, "third phase of the battle" );
    if( ( (long long)newsd[7] & 0x80 ) )
     httpString( cnt, "fourth phase of the battle" );
   }
  }
  else if( (long long)newsd[2] == CMD_NEWS_AID )
  {
   iohtmlFamNewsEntry( cnt, 6, newsd );
   if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
    goto iohtmlFamNewsL0;
   httpPrintf( cnt, "%s received an aid shipment from %s!<br>", mfamd[b].faction, maind.faction );
   for( a = b = 0 ; a < 4 ; a++ )
   {
    if( !( (long long)newsd[4+a] ) )
     continue;
    if( b )
     httpString( cnt, ", " );
    httpPrintf( cnt, "%lld %s", (long long)newsd[4+a], cmdRessourceName[a] );
    b = 1;
   }
   httpString( cnt, " has been added to the faction reserves." );
  }
  else if( (long long)newsd[2] == CMD_NEWS_GETAID )
  {
   iohtmlFamNewsEntry( cnt, 7, newsd );
   if( dbUserMainRetrieve( (long long)newsd[3], &maind ) < 0 )
    goto iohtmlFamNewsL0;
   httpPrintf( cnt, "%s requested an aid shipment from %s!<br>", maind.faction, mfamd[b].faction );
   for( a = b = 0 ; a < 4 ; a++ )
   {
    if( !( (long long)newsd[4+a] ) )
     continue;
    if( b )
     httpString( cnt, ", " );
    httpPrintf( cnt, "%lld %s", (long long)newsd[4+a], cmdRessourceName[a] );
    b = 1;
   }
   httpString( cnt, " has been added to the faction reserves." );
  }
  else if( ( (long long)newsd[2] >= CMD_NEWS_NUMOPBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMOPEND ) )
  {
   iohtmlFamNewsEntry( cnt, -1, newsd );
   httpPrintf( cnt, "Agents sent by %s reached their destination, the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>", mfamd[b].faction, URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
   if( ( dbUserMainRetrieve( (long long)newsd[5], &maind ) ) ) {
    httpPrintf( cnt, " owned by <a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[5], maind.faction );
    httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[6], (long long)newsd[6] );
   }
   httpPrintf( cnt, " to perform <b>%s</b>.", cmdAgentopName[(long long)newsd[9]] );
   if( maind.faction )
   {
    if( (long long)newsd[7] == -1 )
     httpPrintf( cnt, "<br><i>The agents successfully stayed undiscovered during the operation.</i>" );
    else
     httpPrintf( cnt, "<br><i>%lld of the agents were caught, but still managed to kill %lld defending agents.</i>", (long long)newsd[7], (long long)newsd[8] );
   }
  }
  else if( ( (long long)newsd[2] >= CMD_NEWS_NUMOPTARGETBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMOPTARGETEND ) )
  {
   iohtmlFamNewsEntry( cnt, -1, newsd );
   if( (long long)newsd[7] != -1 )
   {
    httpPrintf( cnt, "The forces of %s intercepted some agents from ", mfamd[b].faction );
    if( ( dbUserMainRetrieve( (long long)newsd[5], &maind ) ) ) {
     httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[5], maind.faction );
     httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[6], (long long)newsd[6] );
    } else
     httpString( cnt, "an unknown faction" );
   }
   else
    httpPrintf( cnt, "The forces of %s found traces of agents from an unknown faction", mfamd[b].faction );
   httpPrintf( cnt, " performing a <b>%s</b> operation on <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>.", cmdAgentopName[(long long)newsd[9]], URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
   if( (long long)newsd[7] != -1 )
    httpPrintf( cnt, "<br><i>%lld enemy agents have been arrested, %lld defending agents have been killed.</i>", (long long)newsd[7], (long long)newsd[8] );
  }
  else if( ( (long long)newsd[2] >= CMD_NEWS_NUMSPBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMSPEND ) )
  {
   iohtmlFamNewsEntry( cnt, -1, newsd );
   httpPrintf( cnt, "Psychics of %s casted <b>%s</b> on ", mfamd[b].faction, cmdPsychicopName[(long long)newsd[7]] );
   if( (long long)newsd[3] == (long long)newsd[1] )
    httpPrintf( cnt, "their faction." );
   else
   {
    if( ( dbUserMainRetrieve( (long long)newsd[3], &maind ) ) ) {
     httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], maind.faction );
     httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>.", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
    } else
     httpPrintf( cnt, "an unknown faction." );
    if( maind.faction )
    {
     if( (long long)newsd[5] == -1 )
      httpPrintf( cnt, "<br><i>The psychics successfully stayed undiscovered while performing the spell.</i>" );
     else
      httpPrintf( cnt, "<br><i>%lld of the psychics went mad, it appears %lld defending psychics also suffered critical brain damages.</i>", (long long)newsd[5], (long long)newsd[6] );
    }
   }
  }
  else if( ( (long long)newsd[2] >= CMD_NEWS_NUMSPTARGETBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMSPTARGETEND ) )
  {
   iohtmlFamNewsEntry( cnt, -1, newsd );
   if( (long long)newsd[5] != -1 )
   {
    httpPrintf( cnt, "The forces of %s felt the influence of psychics from ", mfamd[b].faction );
    if( ( dbUserMainRetrieve( (long long)newsd[3], &maind ) ) ) {
     httpPrintf( cnt, "<a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[3], maind.faction );
     httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[4], (long long)newsd[4] );
    } else
     httpString( cnt, "an unknown faction" );
   }
   else
    httpPrintf( cnt, "The forces of %s felt the influence of psychics from an unknown faction", mfamd[b].faction );
   httpPrintf( cnt, " casting <b>%s</b>.", cmdPsychicopName[(long long)newsd[7]] );
   if( (long long)newsd[5] != -1 )
    httpPrintf( cnt, "<br><i>%lld of the psychics went mad, it appears %lld defending psychics also suffered critical brain damages.</i>", (long long)newsd[5], (long long)newsd[6] );
  }
  else if( ( (long long)newsd[2] >= CMD_NEWS_NUMINBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMINEND ) )
  {
   iohtmlFamNewsEntry( cnt, -1, newsd );
   httpPrintf( cnt, "Ghost ships of %s casted <b>%s</b> ", mfamd[b].faction, cmdGhostopName[(long long)newsd[10]] );
   if( cmdGhostopFlags[ (long long)newsd[10] ] & 4 )
    httpPrintf( cnt, "in the system %lld,%lld<br>", ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20 );
   else
   {
    httpPrintf( cnt, "on the <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
    if( ( dbUserMainRetrieve( (long long)newsd[5], &maind ) ) )
    {
     httpPrintf( cnt, " owned by <a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[5], maind.faction );
     httpPrintf( cnt, "<a href=\"%s&id=%lld\">empire #%lld</a><br>", URLAppend( cnt, "empire" ), (long long)newsd[6], (long long)newsd[6] );
     if( (long long)newsd[7] <= 0 )
      httpPrintf( cnt, "<i>The ghost ships successfully stayed undiscovered during the incantation.</i><br>" );
     else
      httpPrintf( cnt, "<i>%lld ghost ships were oblitered by defending forces.</i><br>", (long long)newsd[7] );
     if( (long long)newsd[8] > 0 )
      httpPrintf( cnt, "<i>%lld defending psychics were killed.</i><br>", (long long)newsd[8] );
     if( (long long)newsd[9] > 0 )
      httpPrintf( cnt, "<i>%lld defending ghost ships were destroyed.</i><br>", (long long)newsd[9] );
    }
   }
  }
  else if( ( (long long)newsd[2] >= CMD_NEWS_NUMINTARGETBEGIN ) && ( (long long)newsd[2] <= CMD_NEWS_NUMINTARGETEND ) )
  {
   iohtmlFamNewsEntry( cnt, -1, newsd );
   if( (long long)newsd[7] != -1 )
   {
    httpPrintf( cnt, "The forces of %s were the target of ghost ships from", mfamd[b].faction );
    if( ( dbUserMainRetrieve( (long long)newsd[5], &maind ) ) ) {
     httpPrintf( cnt, " <a href=\"%s&id=%lld\">%s</a> of ", URLAppend( cnt, "player" ), (long long)newsd[5], maind.faction );
     httpPrintf( cnt, "<a href=\"%s&?id=%lld\">empire #%lld</a>", URLAppend( cnt, "empire" ), (long long)newsd[6], (long long)newsd[6] );
    } else
     httpString( cnt, " an unknown faction" );
   }
   else
    httpPrintf( cnt, "The forces of %s were the target of ghost ships from an unknown faction", mfamd[b].faction );
   httpPrintf( cnt, " performing a <b>%s</b> incantation", cmdGhostopName[(long long)newsd[10]] );
   if( (long long)newsd[3] != -1 )
    httpPrintf( cnt, " on <a href=\"%s&id=%lld\">planet %lld,%lld:%lld</a>", URLAppend( cnt, "planet" ), (long long)newsd[3], ( (long long)newsd[4] >> 8 ) & 0xFFF, (long long)newsd[4] >> 20, (long long)newsd[4] & 0xFF );
   httpString( cnt, ".<br>" );
   if( (long long)newsd[7] > 0 )
    httpPrintf( cnt, "<i>%lld enemy ghost ships were destroyed.</i><br>", (long long)newsd[7] );
   if( (long long)newsd[8] > 0 )
    httpPrintf( cnt, "<i>%lld defending psychics were killed.</i><br>", (long long)newsd[8] );
   if( (long long)newsd[9] > 0 )
    httpPrintf( cnt, "<i>%lld defending ghost ships were destroyed.</i><br>", (long long)newsd[9] );
  }



  else if( (long long)newsd[2] == CMD_NEWS_PLANET_TAKEN )
  {
   iohtmlFamNewsEntry( cnt, -1, newsd );
   if( ( dbUserMainRetrieve( (long long)newsd[3], &maind ) ) )
    httpPrintf( cnt, "The forces of %s took control of a <a href=\"%s&id=%lld\">planet</a> offered by %s.<br>", URLAppend( cnt, "planet" ), mfamd[b].faction, (long long)newsd[4], maind.faction );
  }




  else if( (long long)newsd[2] != -1 )
  {
   iohtmlFamNewsEntry( cnt, -1, newsd );
   httpPrintf( cnt, "Unknown report code : %lld ( tell the admin, thanks ;) )", (long long)newsd[2] );
  }
  iohtmlFamNewsL0:
  httpString( cnt, "</td></tr>" );
 }
 httpString( cnt, "</table>" );

 free(mfamd);

 return;
}



void iohtmlFunc_hq( ReplyDataPtr cnt ) {
	int id, a, num;
	dbUserMainDef maind;
	dbMainEmpireDef empired;
	ConfigArrayPtr settings;
	#if FACEBOOK_SUPPORT
	dbUserInfoDef infod;
	#endif
	int64_t *newsp, *newsd;
	FILE *file;
	struct stat stdata;
	char *data;
	char DIRCHECKER[PATH_MAX];

 if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );
 num = dbUserNewsListUpdate( id, &newsp, ticks.number );
 if( !( iohtmlHeader( cnt, id, &maind ) ) )
 {
  free( newsp );
  return;
 }

if( dbMapRetrieveEmpire( maind.empire, &empired ) < 0 ) {
	httpString( cnt, "Error retriving Empire details!" );
	if( (cnt->session)->dbuser->level < LEVEL_MODERATOR )
		return;
}

 iohtmlBodyInit( cnt, "Headquarters" );

 httpString( cnt, "<table>" );
 httpPrintf( cnt, "<tr><td align=\"right\">Current date</td><td>&nbsp;:&nbsp;</td><td>Week <span id=\"hqweeks\">%d</span>, year <span id=\"hqyears\">%d</span></td></td>", ticks.number % 52, ticks.number / 52 );

if( ticks.status ) {
	httpPrintf( cnt, "<tr><td align=\"right\">Next tick</td><td>&nbsp;:&nbsp;</td><td id=\"hqTime\" align=\"center\">%d seconds</td></tr>", (int)( ticks.next - time(0) ) );
	httpString( cnt, "</table>" );
} else {
	httpString( cnt, "</table>" );
	httpString( cnt, "Time frozen<br>" );
}
#if FACEBOOK_SUPPORT
dbUserInfoRetrieve( id, &infod );
httpPrintf( cnt, "<br>User <b>%s</b>", bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_FBMADE ) ? infod.fbinfo.full_name : ((cnt->session)->dbuser)->name );
#else
 httpPrintf( cnt, "<br>User <b>%s</b>", ((cnt->session)->dbuser)->name );
#endif
 httpPrintf( cnt, "<br>Faction <b>%s</b><br><br>", maind.faction );

 httpPrintf( cnt, "<table width=\"400\" border=\"0\"><tr><td align=\"center\">Empire : #%d<br>Planets : <span id=\"hqplanets\">%d</span><br>Population : <span id=\"hqpopulation\">%lld</span>0<br>Networth : <span id=\"hqnetworth\">%lld</span></td>", maind.empire, maind.planets, (long long)maind.ressource[CMD_RESSOURCE_POPULATION], (long long)maind.networth );
 httpString( cnt, "<td align=\"center\">" );


for( a = 0 ; a < CMD_READY_NUMUSED ; a++ ) {
	httpPrintf( cnt, "%s readiness : <span id=\"hq%sready\">%d</span>%%<br>", cmdReadyName[a], cmdReadyName[a], maind.readiness[a] >> 16 );
}




 httpPrintf( cnt, "Home planet : %d,%d:%d</td></tr></table><br>", ( maind.home >> 8 ) & 0xFFF, maind.home >> 20, maind.home & 0xFF );
//read hq message from hq.txt and format for display. -- If this file is missing, or empty it is skipped.
settings = GetSetting( "HTTP Text" );
sprintf( DIRCHECKER, "%s/hq.txt", settings->string_value );
 if( stat( DIRCHECKER, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( DIRCHECKER, "rb" ) ) ) {
			if( stdata.st_size > 0 ) {
				httpString( cnt, "<table width=\"80%\"><tr><td align=\"center\">" );
				httpString( cnt, "<div class=\"genwhite\"><i>Message from Administration:</i></div>" );
				httpString( cnt, "<div class=\"quote\"><br>" );
				while( fgets( data, stdata.st_size, file ) != NULL ) {
					httpPrintf( cnt, "%s<br>", trimwhitespace(data) );
				}
				httpString( cnt, "<br></div></td></tr></table><br>" );
			}
			fclose( file );
		}
	free( data );
	}
}
//end hq message
if( ( strlen(empired.message[0]) ) ) {
	httpString( cnt, "<b>Message from your leader</b><br>" );
	httpString( cnt, empired.message[0] );
	httpString( cnt, "<br><br>" );
}

 newsd = newsp;
 if( num )
 {
  httpString( cnt, "<b>New reports</b>" );
  httpPrintf( cnt, "<br><a href=\"%s\">See older reports</a><br>", URLAppend( cnt, "news" ) );
  httpString( cnt, "<table><tr><td>" );
  for( a = 0 ; a < num ; a++, newsd += DB_USER_NEWS_BASE )
  {
   iohtmlNewsString( cnt, newsd );
  }
  httpString( cnt, "</td></tr></table>" );
 }
 else
 {
  httpString( cnt, "<b>No new reports</b>" );
  httpPrintf( cnt, "<br><a href=\"%s\">See older reports</a>", URLAppend( cnt, "news" ) );
 }
 if( newsp )
  free( newsp );

 iohtmlBodyEnd( cnt );

 return;
}

void iohtmlFunc_news( ReplyDataPtr cnt )
{
 volatile int id, a, num;
 dbUserMainDef maind;
 int64_t *newsp, *newsd;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Older reports" );

 httpPrintf( cnt, "Current date : Week %d, year %d<br>", ticks.number % 52, ticks.number / 52 );
 if( ticks.status )
  httpPrintf( cnt, "%d seconds before tick<br>", (int)( ticks.next - time(0) ) );
 else
  httpPrintf( cnt, "Time frozen<br>" );

 num = dbUserNewsList( id, &newsp );
 newsd = newsp;
 if( !( num ) )
  httpString( cnt, "<br><b>There are no news reports to display.</b><br>" );
 else
 {
  httpString( cnt, "<table><tr><td>" );
  for( a = 0 ; a < num ; a++, newsd += DB_USER_NEWS_BASE )
   iohtmlNewsString( cnt, newsd );
  httpString( cnt, "</td></tr></table>" );
 }
 if( newsp )
  free( newsp );

 iohtmlBodyEnd( cnt );

 return;
}


void iohtmlFunc_council( ReplyDataPtr cnt ) {
	int a, b, c, id, numbuild;
	int bsums[CMD_BLDG_NUMUSED+1];
	int usums[CMD_UNIT_NUMUSED];
	dbMainEmpireDef empired;
	dbUserBuildPtr build;
	dbUserMainDef maind;


if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
	return;

iohtmlBase( cnt, 1 );

if( !( iohtmlHeader( cnt, id, &maind ) ) )
	return;

if( dbMapRetrieveEmpire( maind.empire, &empired ) < 0 ) {
        httpString( cnt, "Error retriving Empire details!" );
	return;
}

iohtmlBodyInit( cnt, "Council" );

if( ( numbuild = dbUserBuildList( id, &build ) ) < 0 ) {
	httpString( cnt, "Error while retriving user build list</body></html>" );
	return;
}

if(empired.taxation)
	httpPrintf( cnt, "<i>Empire leaders have set a taxation level of <span id=\"counciltax\">%.02f</span>%%, this is automaticly deducted from your production.</i>", ( empired.taxation * 100.0 ) );

httpString( cnt, "<table width=\"95%\"><tr><td width=\"48%%\" align=\"center\" valign=\"top\"><table>" );
httpString( cnt, "<tr><td><b>Energy</b></td><td>&nbsp;</td></tr>" );
httpPrintf( cnt, "<tr><td>Production</td><td align=\"right\" id=\"energyproduction\">+%lld</td></tr>", (long long)maind.infos[INFOS_ENERGY_PRODUCTION] );

if( maind.infos[INFOS_ENERGY_TAX] )
	httpPrintf( cnt, "<tr><td>Taxation</td><td align=\"right\" id=\"energytax\">-%lld</td></tr>", (long long)maind.infos[INFOS_ENERGY_TAX] );

httpPrintf( cnt, "<tr><td>Decay</td><td align=\"right\" id=\"energydecay\">-%lld</td></tr>", (long long)maind.infos[INFOS_ENERGY_DECAY] );
httpPrintf( cnt, "<tr><td>Buildings upkeep</td><td align=\"right\" id=\"buildingupkeep\">-%lld</td></tr>", (long long)maind.infos[INFOS_BUILDING_UPKEEP] );
httpPrintf( cnt, "<tr><td>Units upkeep</td><td align=\"right\" id=\"unitupkeep\">-%lld</td></tr>", (long long)maind.infos[INFOS_UNITS_UPKEEP] );
httpPrintf( cnt, "<tr><td>Portals upkeep</td><td align=\"right\" id=\"portalsupkeep\">-%lld</td></tr>", (long long)maind.infos[INFOS_PORTALS_UPKEEP] );
httpPrintf( cnt, "<tr><td>Population reduction</td><td align=\"right\" id=\"populationreduction\">+%lld</td></tr>", (long long)maind.infos[INFOS_POPULATION_REDUCTION] );
httpPrintf( cnt, "<tr><td>Energy income</td><td align=\"right\" id=\"energyincome\">%s%+lld%s</td></tr>", ( ( maind.infos[CMD_RESSOURCE_ENERGY] < 0 ) ? "<span class=\"genred\">" : "" ), (long long)maind.infos[CMD_RESSOURCE_ENERGY], ( ( maind.infos[CMD_RESSOURCE_ENERGY] < 0 ) ? "</span>" : "" ) );
httpString( cnt, "</table><br></td><td width=\"45%%\" align=\"center\" valign=\"top\"><table>" );
httpString( cnt, "<tr><td><b>Resources</b></td><td>&nbsp;</td></tr>" );

if( maind.infos[INFOS_MINERAL_TAX] ) {
	httpPrintf( cnt, "<tr><td>Mineral produced</td><td align=\"right\" id=\"mineralproduction\">+%lld</td></tr>", (long long)maind.infos[INFOS_MINERAL_PRODUCTION] );
	httpPrintf( cnt, "<tr><td>Mineral taxation</td><td align=\"right\" id=\"mineraltax\">-%lld</td></tr>", (long long)maind.infos[INFOS_MINERAL_TAX] );
}

httpPrintf( cnt, "<tr><td>Mineral income</td><td align=\"right\" id=\"mineralincome\">+%lld</td></tr>", (long long)maind.infos[CMD_RESSOURCE_MINERAL] );
httpPrintf( cnt, "<tr><td>Crystal production</td><td align=\"right\" id=\"crystalproduction\">+%lld</td></tr>", (long long)maind.infos[INFOS_CRYSTAL_PRODUCTION] );

if( maind.infos[INFOS_CRYSTAL_TAX] )
	httpPrintf( cnt, "<tr><td>Crystal taxation</td><td align=\"right\" id=\"crystaltax\">-%lld</td></tr>", (long long)maind.infos[INFOS_CRYSTAL_TAX] );

httpPrintf( cnt, "<tr><td>Crystal decay</td><td align=\"right\" id=\"crystaldecay\">-%lld</td></tr>", (long long)maind.infos[INFOS_CRYSTAL_DECAY] );
httpPrintf( cnt, "<tr><td>Crystal income</td><td align=\"right\" id=\"crystalincome\">%s%+lld%s</td></tr>", ( ( maind.infos[CMD_RESSOURCE_CRYSTAL] < 0 ) ? "<span class=\"genred\">" : "" ), (long long)maind.infos[CMD_RESSOURCE_CRYSTAL], ( ( maind.infos[CMD_RESSOURCE_CRYSTAL] < 0 ) ? "</span>" : "" ) );

if( maind.infos[INFOS_ECTROLIUM_TAX] ) {
	httpPrintf( cnt, "<tr><td>Ectrolium produced</td><td align=\"right\" id=\"ectroliumproduction\">+%lld</td></tr>", (long long)maind.infos[INFOS_ECTROLIUM_PRODUCTION] );
	httpPrintf( cnt, "<tr><td>Ectrolium taxation</td><td align=\"right\" id=\"ectroliumtax\">-%lld</td></tr>", (long long)maind.infos[INFOS_ECTROLIUM_TAX] );
}

httpPrintf( cnt, "<tr><td>Ectrolium income</td><td align=\"right\" id=\"ectroliumincome\">+%lld</td></tr>", (long long)maind.infos[CMD_RESSOURCE_ECTROLIUM] );
httpString( cnt, "</table><br></td></tr><tr><td align=\"center\" valign=\"top\">" );

httpString( cnt, "<b>Buildings</b><br><table>" );
for( a = b = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ ) {
	httpPrintf( cnt, "<tr><td>%s</td><td>&nbsp;</td><td align=\"right\" id=\"bld%d\">%lld</td></tr>", cmdBuildingName[a], a, (long long)maind.totalbuilding[a] );
	b += (int)maind.totalbuilding[a];
}

httpPrintf( cnt, "<tr><td>Total</td><td>&nbsp;</td><td id=\"bldnum\">%d</td></tr></table><br><br>", b );
httpPrintf( cnt, "<b>Buildings under construction</b><br><table><form name=\"cancelbuild\" action=\"%s\">", URLAppend( cnt, "cancelbuild" ) );
memset( bsums, 0, (CMD_BLDG_NUMUSED+1)*sizeof(int) );

for( a = c = 0 ; a < numbuild ; a++ ) {
	if( build[a].type >> 16 )
		continue;
	httpPrintf( cnt, "<tr><td>%d %s in %d weeks at <a href=\"%s&id=%d\">%d,%d:%d</a></td><td><input type=\"checkbox\" name=\"b%d\"></td></tr>", build[a].quantity, cmdBuildingName[ build[a].type & 0xFFFF ], build[a].time, URLAppend( cnt, "planet" ), build[a].plnid, ( build[a].plnpos >> 8 ) & 0xFFF, build[a].plnpos >> 20, build[a].plnpos & 0xFF , a);
	bsums[ build[a].type & 0xFFFF ] += build[a].quantity;
	c++;
}

if( !( c ) ) {
	httpString( cnt, "</form></table>None<br>" );
} else {
	httpString(cnt, "<tr><td></td><td><a href=\"#\" onclick=\"javascript:togglemb(0);return false;\">Toggle</font></a></td></tr>");
 	httpString(cnt, "<tr><td></td><td><input type=\"submit\" value=\"Cancel\"></td></tr></form></table>");
	httpString( cnt, "<br><i>Summary</i><br>" );

	for( a = b = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ ) {
		if( !( bsums[a] ) )
			continue;
		httpPrintf( cnt, "%d %s<br>", bsums[a], cmdBuildingName[a] );
		b += bsums[a];
	}
	httpPrintf( cnt, "<i>Total of %d buildings under construction</i><br>", b );
}

httpString( cnt, "</td><td align=\"center\" valign=\"top\">" );

httpString( cnt, "<b>Units</b><br><table>" );

for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
	httpPrintf( cnt, "<tr><td>%s</td><td>&nbsp;</td><td align=\"right\" id=\"unt%d\">%lld</td></tr>", cmdUnitName[a], a, (long long)maind.totalunit[a] );
	b += (int)maind.totalunit[a];
}

httpPrintf( cnt, "<tr><td>Total</td><td>&nbsp;</td><td id=\"untnum\">%d</td></tr></table><br><br>", b );
httpPrintf( cnt, "<b>Units under construction</b><br><table><form name=\"cancelunit\" action=\"%s\">", URLAppend( cnt, "cancelbuild" ) );

memset( usums, 0, CMD_UNIT_NUMUSED*sizeof(int) );
for( a = c = 0 ; a < numbuild ; a++ ) {
	if( !( build[a].type >> 16 ) )
		continue;
	httpPrintf( cnt, "<tr><td>%d %s in %d weeks</td><td><input type=\"checkbox\" name=\"b%d\"></td></tr>", build[a].quantity, cmdUnitName[ build[a].type & 0xFFFF ], build[a].time, a);
	usums[ build[a].type & 0xFFFF ] += build[a].quantity;
	c++;
}

if( !( c ) ) {
  httpString( cnt, "</form></table>None<br>" );
} else {
	httpString(cnt, "<tr><td></td><td><a href=\"#\" onclick=\"javascript:togglemb(1);return false;\">Toggle</font></a></td></tr>");
	httpString( cnt, "<tr><td></td><td><input type=\"submit\" value=\"Cancel\"></td></tr></form></table><br><i>Summary</i><br>" );
for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
	if( !( usums[a] ) )
		continue;
	httpPrintf( cnt, "%d %s<br>", usums[a], cmdUnitName[a] );
	b += usums[a];
  	}
	httpPrintf( cnt, "<i>Total of %d units under construction</i><br>", b );
}

httpString( cnt, "</td></tr></table>" );

free( build );
iohtmlBodyEnd( cnt );

return;
}



void iohtmlFunc_units( ReplyDataPtr cnt )
{
 int a, b, c, id;
 int64_t resbuild[CMD_RESSOURCE_NUMUSED+2];
 dbUserMainDef maind;
 char *buildunit[CMD_UNIT_NUMUSED];
 char buildname[16];
 char buildstring[CMD_UNIT_NUMUSED][128];

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );


for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
	sprintf( buildname, "unit%dbuild", a );
	buildunit[a] = iohtmlVarsFind( cnt, buildname );
}

 for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
 {
  buildstring[a][0] = 0;
  if( !( buildunit[a] ) )
   continue;
  if( sscanf( buildunit[a], "%d", &b ) <= 0 )
   continue;
  if( b <= 0 )
   continue;
  c = cmdExecAddBuild( id, 0x10000 | a, b, -1, 0 );
  if( c >= 0 )
   sprintf( buildstring[a], "<i>You started building %d %s</i><br><br>", b, cmdUnitName[a] );
  else if( cmdErrorString )
   sprintf( buildstring[a], "<i>%s</i><br><br>", cmdErrorString );
  else
   sprintf( buildstring[a], "<i>Error while adding build item</i><br>" );
 }

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Units" );

 for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
 {
  if( buildstring[a][0] )
   httpString( cnt, buildstring[a] );
 }

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "units" ) );
 httpString( cnt, "<table cellspacing=\"6\" border=\"0\"><tr><td><b>Unit</b></td><td><b>Cost</b></td><td><b>Owned</b></td><td><b>Build</b></td></tr>" );
 for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
 {
  cmdGetBuildCosts( &maind, 0x10000 | a, resbuild );
  if( !( maind.totalunit[a] ) && ( resbuild[0] == -2 ) )
   continue;
  httpPrintf( cnt, "<tr><td valign=\"top\"><font color=\"#FFFFFF\">%s</font><br>", cmdUnitName[a] );

   if( ( a < CMD_UNIT_SOLDIER ) || ( a == CMD_UNIT_GHOST ) )
      httpPrintf( cnt, "<img src=\"files?type=image&name=units/u%d.jpg\">", a );


  httpString( cnt, "</td><td valign=\"top\" nowrap>" );

  if( resbuild[0] < 0 )
  {
   if( resbuild[0] == -1 )
    httpPrintf( cnt, "<font color=\"#FF0000\">Unavailable<br>Tech too low</font></td><td valign=\"top\" align=\"center\">%lld</td><td>&nbsp;</td></tr>", (long long)maind.totalunit[a] );
   else if( resbuild[0] == -2 )
    httpPrintf( cnt, "<font color=\"#FF0000\">Unavailable<br>Can not be built</font></td><td valign=\"top\" align=\"center\">%lld</td><td>&nbsp;</td></tr>", (long long)maind.totalunit[a] );
   continue;
  }
  for( b = 0 ; b < CMD_RESSOURCE_NUMUSED+1 ; b++ )
  {
   if( resbuild[b] )
    httpPrintf( cnt, "%lld %s<br>", (long long)resbuild[b], cmdRessourceName[b] );
  }
  if( resbuild[b] )
   httpPrintf( cnt, "<font color=\"#FF2020\">Tech : +%lld%% costs</font><br>", (long long)resbuild[b] );
  httpPrintf( cnt, "</td><td valign=\"top\" align=\"center\">%lld</td><td valign=\"top\"><input type=\"text\" name=\"unit%dbuild\" size=\"10\"></td></tr>", (long long)maind.totalunit[a], a );
 }
 httpString( cnt, "</table><input type=\"submit\" value=\"Build\"></form>" );

 iohtmlBodyEnd( cnt );
 return;
}



void iohtmlFunc_market( ReplyDataPtr cnt )
{
 int a, b, c, d, id, fullmarket[6*DB_MARKET_RANGE], rmarket[12];
 int action, resource, price, quantity;
 int *buffer;
 dbUserMainDef maind;
 char *pricestring, *quantitystring, *resstring, *actionstring, *viewstring, *fullstring, *rmbidstring;
 char *marketstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );


 pricestring = iohtmlVarsFind( cnt, "price" );
 quantitystring = iohtmlVarsFind( cnt, "quantity" );
 actionstring = iohtmlVarsFind( cnt, "action" );
 resstring = iohtmlVarsFind( cnt, "res" );
 viewstring = iohtmlVarsFind( cnt, "view" );
 fullstring = iohtmlVarsFind( cnt, "full" );
 rmbidstring = iohtmlVarsFind( cnt, "rmbid" );

	/*
	httpString(cnt, "Market is close<br>But you can have ally");
	iohtmlBodyEnd( cnt );
	return;
	*/

 marketstring = 0;
 if( resstring )
 {
  if( !( actionstring ) )
   marketstring = "You must select an action to place a bid, either buy or sell.";
  else if( ( sscanf( actionstring, "%d", &action ) == 1 ) && ( resstring ) && ( sscanf( resstring, "%d", &resource ) == 1 ) && ( pricestring ) && ( sscanf( pricestring, "%d", &price ) == 1 ) && ( quantitystring ) && ( sscanf( quantitystring, "%d", &quantity ) == 1 ) )
  {
   if( cmdExecAddBid( id, action, resource, price, quantity ) < 0 )
   {
    if( cmdErrorString )
     marketstring = cmdErrorString;
    else
     marketstring = "Error while adding build item";
   }
   else
    marketstring = "Bid placed";
  }
  else
   marketstring = "Incorrect request.";
 }

 if( rmbidstring )
 {
  if( sscanf( rmbidstring, "%d", &a ) == 1 )
  {
   if( cmdExecRemoveBid( id, a ) < 0 )
    marketstring = "Error while removing bid";
   else
    marketstring = "Bid withdrawn";
   viewstring = (void *)1;
  }
 }

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Market" );
 if( marketstring )
  httpPrintf( cnt, "<i>%s</i><br><br>", marketstring );

 if( viewstring )
 {
  b = dbUserMarketList( id, &buffer );
  httpString( cnt, "<b>Your bids</b><br>" );
  if( b <= 0 )
   httpString( cnt, "None<br>" );
  for( a = c = 0 ; a < b ; a++, c += 5 )
  {
   if( !( buffer[c+DB_MARKETBID_ACTION] ) )
    httpString( cnt, "Buying" );
   else
    httpString( cnt, "Selling" );
   httpPrintf( cnt, " %d %s at %d - <a href=\"%s&rmbid=%d\">Withdraw bid</a><br>", URLAppend( cnt, "market" ), buffer[c+DB_MARKETBID_QUANTITY], cmdRessourceName[buffer[c+DB_MARKETBID_RESSOURCE]+1], buffer[c+DB_MARKETBID_PRICE], buffer[c+DB_MARKETBID_BIDID] );
  }
  httpString( cnt, "<br>" );
  if( buffer )
   free( buffer );
 }

 httpPrintf( cnt, "<a href=\"%s&view=1\">View your bids</a><br>", URLAppend( cnt, "market" ) );
 httpPrintf( cnt, "<a href=\"%s&full=1\">View market statistics</a><br><br>", URLAppend( cnt, "market" ) );

 if( !( fullstring ) )
 {
  if( cmdExecGetMarket( rmarket ) < 0 )
  {
   httpString( cnt, "Error while retriving market" );
   iohtmlBodyEnd( cnt );
   return;
  }
  httpString( cnt, "<table width=\"400\"><tr><td width=\"150\">&nbsp;</td><td width=\"60\" align=\"center\">Buy</td><td width=\"60\" align=\"center\">Needed</td><td width=\"10\">&nbsp;</td><td width=\"60\" align=\"center\">Sell</td><td width=\"60\" align=\"center\">Stock</td></tr>" );
  for( a = 0, b = 1 ; a < 3*4 ; a += 4, b++ )
  {
   httpPrintf( cnt, "<tr><td>%s</td><td align=\"center\">", cmdRessourceName[b] );
   if( rmarket[a+0] >= 0 )
    httpPrintf( cnt, "%d</td><td align=\"center\">%d", rmarket[a+1], rmarket[a+0] );
   else
    httpString( cnt, "-</td><td align=\"center\">-" );
   httpString( cnt, "</td><td>...</td><td align=\"center\">" );
   if( rmarket[a+2] >= 0 )
    httpPrintf( cnt, "%d</td><td align=\"center\">%d</td>", rmarket[a+3], rmarket[a+2] );
   else
    httpString( cnt, "-</td><td align=\"center\">-</td>" );
   httpString( cnt, "</tr>" );
  }
  httpPrintf( cnt, "</table>" );
 }
 else
 {
  if( dbMarketFull( fullmarket ) < 0 )
  {
   httpString( cnt, "Error while retriving market" );
   iohtmlBodyEnd( cnt );
   return;
  }
  for( b = c = 0 ; b < 3*2 ; b += 2 )
  {
   rmarket[b+0] = -1;
   rmarket[b+1] = -1;
   for( a = DB_MARKET_RANGE-1 ; a >= 1 ; a-- )
   {
    if( fullmarket[c+a] )
    {
     rmarket[b+0] = a;
     break;
    }
   }
   c += DB_MARKET_RANGE;
   for( a = 1 ; a < DB_MARKET_RANGE ; a++ )
   {
    if( fullmarket[c+a] )
    {
     rmarket[b+1] = a;
     break;
    }
   }
   c += DB_MARKET_RANGE;
  }
  for( c = d = 0 ; c < 3 ; c++, d += 2 )
  {
   httpPrintf( cnt, "<b>Buying %s</b><br>", cmdRessourceName[c+1] );
   if( rmarket[d+0] == -1 )
    httpString( cnt, "No bids<br><br>" );
   else
   {
    httpString( cnt, "<table cellspacing=\"6\" cellpading=\"6\"><tr align=\"center\"><td align=\"right\">Price</td>" );
    b = rmarket[d+0] - 10;
    if( b <= 0 )
     b = 1;
    for( a = b ; a <= rmarket[d+0] ; a++ )
     httpPrintf( cnt, "<td>%d</td>", a );
    httpString( cnt, "</tr><tr align=\"center\"><td align=\"right\">Needed</td>" );
    for( a = b ; a <= rmarket[d+0] ; a++ )
     httpPrintf( cnt, "<td>%d</td>", fullmarket[(d+0)*DB_MARKET_RANGE+a] );
    httpString( cnt, "</tr></table><br>" );
   }
   httpPrintf( cnt, "<b>Selling %s</b><br>", cmdRessourceName[c+1] );
   if( rmarket[d+1] == -1 )
    httpString( cnt, "No bids<br><br>" );
   else
   {
    httpString( cnt, "<table cellspacing=\"6\" cellpading=\"6\"><tr align=\"center\"><td align=\"right\">Price</td>" );
    b = rmarket[d+1] + 10;
    if( b >= DB_MARKET_RANGE )
     b = DB_MARKET_RANGE-1;
    for( a = rmarket[d+1] ; a <= b ; a++ )
     httpPrintf( cnt, "<td>%d</td>", a );
    httpString( cnt, "</tr><tr align=\"center\"><td align=\"right\">Stock</td>" );
    for( a = rmarket[d+1] ; a <= b ; a++ )
     httpPrintf( cnt, "<td>%d</td>", fullmarket[(d+1)*DB_MARKET_RANGE+a] );
    httpString( cnt, "</tr></table><br>" );
   }
  }
 }

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><table cellspacing=\"4\" border=\"0\"><tr><td><input type=\"radio\" value=\"0\" name=\"action\">Buy<br><input type=\"radio\" value=\"1\" name=\"action\">Sell</td><td><select name=\"res\">", URLAppend( cnt, "market" ) );
 for( a = 0 ; a < 3 ; a++ )
  httpPrintf( cnt, "<option value=\"%d\">%s", a, cmdRessourceName[a+1] );
 httpString( cnt, "</select></td></tr><tr><td align=\"right\">Price :</td><td><input type=\"text\" name=\"price\" size=\"10\"></td></tr><tr><td>Quantity :</td><td><input type=\"text\" name=\"quantity\" size=\"10\"></td></tr></table><input type=\"submit\" value=\"Execute\"></form>" );
 httpString( cnt, "<br><i>Note that resources placed on the market still decay, including energy.</i>" );

 iohtmlBodyEnd( cnt );
 return;
}



void iohtmlFunc_planets( ReplyDataPtr cnt )
{
 int a, b, c, d, id, sort;
 char szColor[10];
 float fa;
 int *buffer;
 dbUserMainDef maind, main2d;
 dbMainPlanetDef planetd;
 int totals[7];
 float totalob;
 char *sortstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Planets" );

 sortstring = iohtmlVarsFind( cnt, "sort" );

 if( !( sortstring ) || ( sscanf( sortstring, "%d", &sort ) <= 0 ) )
  sort = 0;

if( ( b = dbUserPlanetListIndicesSorted( id, &buffer, sort ) ) <= 0 ) {
	httpString( cnt, "Error while retriving planets list" );
	iohtmlBodyEnd( cnt );
	return;
}



 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "massbuild" ) );

if (sort == 0)
 httpPrintf( cnt, "<table width=\"100%%\"><tr><td width=\"15%%\"><a href=\"/%s&sort=10\">Planet</a></td>", URLAppend( cnt, "planets" ) );
else
 httpPrintf( cnt, "<table width=\"100%%\"><tr><td width=\"15%%\"><a href=\"/%s&sort=0\">Planet</a></td>", URLAppend( cnt, "planets" ) );

if ( ( sort == 1 ) || ( sort == 6 ) ) {
 httpPrintf( cnt, "<td width=\"10%%\"><a href=\"/%s&sort=11\">Size</a> - ", URLAppend( cnt, "planets" ) );
 httpPrintf( cnt, "<a href=\"/%s&sort=16\">Bonus</a></td>", URLAppend( cnt, "planets" ) );
} else {
 httpPrintf( cnt, "<td width=\"10%%\"><a href=\"/%s&sort=1\">Size</a> - ", URLAppend( cnt, "planets" ) );
 httpPrintf( cnt, "<a href=\"/%s&sort=6\">Bonus</a></td>", URLAppend( cnt, "planets" ) );
}
if ( ( sort == 2 || sort == 3 ) ) {
 httpPrintf( cnt, "<td width=\"20%%\"><a href=\"/%s&sort=12\">Buildings</a> - ", URLAppend( cnt, "planets" ) );
 httpPrintf( cnt, "<a href=\"/%s&sort=13\">Overbuilding</a></td>", URLAppend( cnt, "planets" ) );
} else {
 httpPrintf( cnt, "<td width=\"20%%\"><a href=\"/%s&sort=2\">Buildings</a> - ", URLAppend( cnt, "planets" ) );
 httpPrintf( cnt, "<a href=\"/%s&sort=3\">Overbuilding</a></td>", URLAppend( cnt, "planets" ) );
}
if (sort == 4)
 httpPrintf( cnt, "<td width=\"25%%\"><a href=\"/%s&sort=14\">Population</a></td>", URLAppend( cnt, "planets" ) );
else
 httpPrintf( cnt, "<td width=\"25%%\"><a href=\"/%s&sort=4\">Population</a></td>", URLAppend( cnt, "planets" ) );

if (sort == 5)
 httpPrintf( cnt, "<td width=\"28%\">Build - <a href=\"/%s&sort=15\">Protection</a></td>", URLAppend( cnt, "planets" ) );
else
 httpPrintf( cnt, "<td width=\"28%\">Build - <a href=\"/%s&sort=5\">Protection</a></td>", URLAppend( cnt, "planets" ) );

 httpString( cnt, "<td width=\"2%\"><a href=\"#\" onclick=\"javascript:togglemb(0);return false;\"><font size=\"1\">Toggle</font></a></td></tr>" );



 memset( totals, 0, 7*sizeof(int) );
 totalob = 0.0;

 for( a = 0 ; a < b ; a++ )
 {
 	dbMapRetrievePlanet( buffer[a], &planetd );
  for( c = d = 0 ; c < CMD_BLDG_NUMUSED ; c++ )
   d += planetd.building[c];
  if(planetd.flags & CMD_PLANET_FLAGS_BEACON)
  	httpPrintf( cnt, "<tr><td><a href=\"%s&id=%d\">%d,%d:%d</a>&nbsp;<img src=\"files?type=image&name=beacon.gif\">", URLAppend( cnt, "planets" ) , buffer[a], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );
  else
  	httpPrintf( cnt, "<tr><td><a href=\"%s&id=%d\">%d,%d:%d</a>", URLAppend( cnt, "planets" ) , buffer[a], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );

if( planetd.surrender != -1 ) {
    if( dbUserMainRetrieve( planetd.surrender, &main2d ) < 0 )
	return;
	httpPrintf( cnt, "&nbsp;offered to <a href=\"%s&id=%d\">%s</a>", URLAppend( cnt, "player" ) , planetd.surrender, main2d.faction );
}


httpPrintf( cnt, "</td><td>%d</td><td>%d", planetd.size, d );

  if( planetd.construction )
   httpPrintf( cnt, " ( %d )", planetd.construction );
  for( c = CMD_BLDG_OVERBUILD ; c < CMD_BLDG_NUMUSED ; c++ )
   d -= planetd.building[c];
  fa = cmdGetBuildOvercost( planetd.size, d+planetd.construction );
  if( fa > 1.0 )
   httpPrintf( cnt, " <font color=\"#FF2020\">+%.2f%%</font>", 100.0 * (fa-1.0) );
  totalob += fa;
  totals[0] += planetd.size;
  totals[1] += d;
  totals[2] += planetd.construction;
  sprintf(szColor, "C0D0D8");
  if((planetd.population*10/planetd.maxpopulation)>=8)
  {
  	sprintf(szColor, "6699FF");
  }
  if((planetd.population/planetd.maxpopulation)==1)
  {
  	sprintf(szColor, "FFFF66");
  }
  httpPrintf( cnt, "</td><td><font color=\"#%s\">%.0f0 / %d0</font></td><td><a href=\"%s&id=%d\">Build</a>", szColor, planetd.population, planetd.maxpopulation, URLAppend( cnt, "build" ), buffer[a] );
  if( planetd.flags & CMD_PLANET_FLAGS_PORTAL )
  {
   httpString( cnt, " Portal" );
   totals[5]++;
  }
  if( planetd.protection >= 70 )
   httpString( cnt, " <font color=\"#00FF00" );
  else if( planetd.protection >= 40 )
   httpString( cnt, " <font color=\"#FFFF00" );
  else
   httpString( cnt, " <font color=\"#FF0000" );
  httpPrintf( cnt, "\">- %d%%</font>", planetd.protection );
  for( c = d = 0 ; c < CMD_UNIT_NUMUSED ; c++ )
   d += planetd.unit[c];
  if( d )
  {
   httpPrintf( cnt, " <i>Units (%d)</i>", d );
   totals[6] += d;
  }

  d = (int)artefactPrecense( &planetd );
  if( d >= 0 )
   httpPrintf( cnt, " <img src=\"files?type=image&name=artifacts/%s\" alt=\"%s\" title=\"%s\">", artefactImage[d], artefactName[d], artefactName[d] );
  else if(planetd.special[1])
  	httpPrintf( cnt, " <img src=\"files?type=image&name=pr%d.gif\" alt=\"%s\" title=\"%s\">+%d%%", planetd.special[0], cmdBonusName[planetd.special[0]], cmdBonusName[planetd.special[0]], planetd.special[1] );

  httpPrintf( cnt, "</td><td align=\"center\"><input type=\"checkbox\" name=\"m%d\"></td></tr>", buffer[a] );
  totals[3] += planetd.population;
  totals[4] += planetd.maxpopulation;
 }

 httpPrintf( cnt, "<tr><td>%d Planets</td><td>%d</td><td>%d", b, totals[0], totals[1] );
 if( totals[2] )
  httpPrintf( cnt, " ( %d )", totals[2] );
 totalob /= (float)b;
 if( totalob > 1.001 )
  httpPrintf( cnt, " <font color=\"#FF2020\">+%.2f%%</font>", 100.0 * (totalob-1.0) );

//Total
	sprintf(szColor, "FFFFFF");
 if((totals[3]*10/totals[4])>=8)
 {
 	sprintf(szColor, "FFFF00");
 }
 if((totals[3]/totals[4])==1)
 {
 	sprintf(szColor, "FF2020");
 }
 httpPrintf( cnt, "</td><td><font color=\"#%s\">%d0 / %d0</font></td><td> %d Portals", szColor, totals[3], totals[4], totals[5] );
 if( totals[6] )
  httpPrintf( cnt, " <i>Units (%d)</i>", totals[6] );
 httpString( cnt, "</td><td></td></tr>" );
 httpString( cnt, "</table><br><div align=\"right\"><input type=\"submit\" value=\"Build on selected planets\"></div></form>" );

 free( buffer );
 iohtmlBodyEnd( cnt );

 return;
}


//FIXME : Merge into _planet
void iohtmlFunc_pltake( ReplyDataPtr cnt )
{
 int id, plnid;
 dbMainPlanetDef planetd;
 dbUserMainDef maind;
 char *planetstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 planetstring = iohtmlVarsFind( cnt, "id" );

 if( !( planetstring ) || ( sscanf( planetstring, "%d", &plnid ) <= 0 ) || ( dbMapRetrievePlanet( plnid, &planetd ) < 0 ) )
 {
  httpString( cnt, "This planet doesn't seem to exist!</body></html>" );
  return;
 }

 httpPrintf( cnt, "<b>Planet %d,%d:%d</b><br><br>", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );

 if( planetd.owner == id )
  httpString( cnt, "You already own this planet!<br>" );
 else if( cmdExecTakePlanet( id, plnid ) >= 0 )
  httpString( cnt, "You took control of the planet!<br>" );
 else
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
  else
   httpString( cnt, "<i>You couldn't take control of the planet, the owner must have cancelled the offer.</i><br><br>" );
 }

 httpString( cnt, "</center></body></html>" );

 return;
}




void iohtmlFunc_empire( ReplyDataPtr cnt )
{
 int a, b, c, d, nAlly, first, id, curtime, curfam;
 dbUserMainDef maind;
 dbMainEmpireDef empired;
 char *empirestring;
 char fname[REDIRECT_MAX];
 dbUserPtr user;
 dbUserMainDef mainp[32];
 int stats[64];

maind.empire = -1;
if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	iohtmlBase( cnt, 1 );
	if( !( iohtmlHeader( cnt, id, &maind ) ) )
		return;
} else {
	iohtmlBase( cnt, 8 );
	iohtmlFunc_frontmenu( cnt, FMENU_NONE );
}

 empirestring = iohtmlVarsFind( cnt, "id" );

 if( !( empirestring ) || ( sscanf( empirestring, "%d", &curfam ) <= 0 ) )
  curfam = maind.empire;

 if( dbMapRetrieveEmpire( curfam, &empired ) < 0 )
 {
  httpString( cnt, "This empire does not seem to exist!</body></html>" );
  return;
 }

 if( empired.name[0] )
  iohtmlBodyInit( cnt, "%s #%d ( <a href=\"%s&id=%d\">%d,%d</a> )", empired.name, curfam, URLAppend( cnt, "system" ), empired.homeid, empired.homepos & 0xFFFF, empired.homepos >> 16 );
 else
  iohtmlBodyInit( cnt, "Empire #%d ( <a href=\"%s&id=%d\">%d,%d</a> )", curfam, URLAppend( cnt, "system" ), empired.homeid, empired.homepos & 0xFFFF, empired.homepos >> 16 );

if ( curfam == maind.empire ) {
	if( ( empired.leader == -1 ) ) {
		httpString( cnt, "<div class=\"genred\">Your Empire has no leader!!</div>" );
	}
	httpPrintf( cnt, "<a href=\"%s&type=eleader\">Change your vote</a>", URLAppend( cnt, "vote" ) );
	if( id == empired.leader ) {
		httpPrintf( cnt, "&nbsp;-&nbsp;<a href=\"%s\" target=\"main\">Leadership Options</a>", URLAppend( cnt, "famleader" ) );
	}
	httpString( cnt, "<br>" );
}


if( empired.picture > 0 ) {
	sprintf( fname, "/files?type=eimage&name=empire%d/pic%d", curfam, empired.picture );
	httpPrintf( cnt, "<br><img src=\"%s\"><br>", &fname[1] );
}

 if( !( empired.numplayers ) )
 {
  iohtmlBodyEnd( cnt );
  return;
 }

 for( a = 0 ; a < empired.numplayers ; a++ )
 {
  if( dbUserMainRetrieve( empired.player[a], &mainp[a] ) < 0 )
  {
   httpString( cnt, "Error while retriving user's main data" );
   continue;
  }
 }
 first = 0;
 stats[0] = -1;
 stats[1] = -1;
 for( b = 1, c = 2 ; b < empired.numplayers ; b++, c += 2 )
 {
  a = first >> 1;
  if( ( mainp[b].planets > mainp[a].planets ) || ( ( mainp[b].planets == mainp[a].planets ) && ( mainp[b].networth > mainp[a].networth ) ) )
  {
   stats[first+0] = c;
   stats[c+1] = first;
   stats[c+0] = -1;
   first = c;
   continue;
  }
  d = first;
  a = stats[d+1];
  for( ; ; )
  {
   if( a == -1 )
   {
    stats[d+1] = c;
    stats[c+0] = d;
    stats[c+1] = -1;
    break;
   }
   if( ( mainp[b].planets > mainp[a>>1].planets ) || ( ( mainp[b].planets == mainp[a>>1].planets ) && ( mainp[b].networth > mainp[a>>1].networth ) ) )
   {
    stats[c+0] = d;
    stats[c+1] = a;
    stats[d+1] = c;
    stats[a+0] = c;
    break;
   }
   d = a;
   a = stats[a+1];
  }
 }

 httpString( cnt, "<table cellspacing=\"8\"><tr><td>&nbsp;</td><td><b>Empire factions</b></td><td>Race</td><td>Planets</td><td>Networth</td><td></td></tr>" );
 curtime = time( 0 );
 for( a = first ; a != -1 ; )
 {
  b = a >> 1;
  c = empired.player[b];
  user = dbUserLinkID( c );
  bitflag_add( &user->flags, CMD_USER_FLAGS_ACTIVATED );
  dbUserSave( c, user);
  httpString( cnt, "<tr>" );
  if( !( user ) )
   httpString( cnt, "<td>&nbsp;</td>" );
  else if( bitflag( user->flags, CMD_USER_FLAGS_LEADER ) )
   httpString( cnt, "<td><i>Leader</i></td>" );
  else if( bitflag( user->flags, CMD_USER_FLAGS_VICELEADER ) )
   httpString( cnt, "<td><i>Vice-leader</i></td>" );
  else if( bitflag( user->flags, CMD_USER_FLAGS_COMMINISTER ) )
   httpString( cnt, "<td><i>Minister of Communications</i></td>" );
  else if( bitflag( user->flags, CMD_USER_FLAGS_DEVMINISTER ) )
   httpString( cnt, "<td><i>Minister of Development</i></td>" );
  else if( bitflag( user->flags, CMD_USER_FLAGS_WARMINISTER ) )
   httpString( cnt, "<td><i>Minister of War</i></td>" );
  else if( bitflag( user->flags, CMD_USER_FLAGS_INDEPENDENT ) )
   httpString( cnt, "<td><i>Independent</i></td>" );
  else
   httpString( cnt, "<td>&nbsp;</td>" );

  httpPrintf( cnt, "<td><a href=\"%s&id=%d\">", URLAppend( cnt, "player" ), c );
  if( empired.leader == c )
   httpPrintf( cnt, "<font color=\"#FFC040\"><b>%s</b></font>", mainp[b].faction );
  else
   httpString( cnt, mainp[b].faction );
  httpPrintf( cnt, "</a></td><td><a href=\"%s\">%s</a></td><td>%d</td><td>%lld</td><td>", URLAppend( cnt, "races" ), cmdRaceName[mainp[b].raceid], mainp[b].planets, (long long)mainp[b].networth );
  nAlly = cmdExecFindRelation(maind.empire, curfam, 0, 0);

  if( ( id >= 0 ) && ( user ) && ( ( curfam == maind.empire ) || (nAlly == CMD_RELATION_ALLY) || ( ( (cnt->session)->dbuser ) && ( (cnt->session)->dbuser->level >= LEVEL_MODERATOR ) ) ) )
  {

   b = curtime - user->lasttime;
   if( b < 5*60 )
    httpString( cnt, "[online]" );
   else if( bitflag( (cnt->session)->dbuser->flags, ( CMD_USER_FLAGS_LEADER | CMD_USER_FLAGS_VICELEADER | CMD_USER_FLAGS_COMMINISTER ) ) || ( (cnt->session)->dbuser->level >= LEVEL_MODERATOR ) )
   {
    httpString( cnt, "<i>Last : " );
    if( b >= 24*60*60 )
    {
     httpPrintf( cnt, "%dd ", b/(24*60*60) );
     b %= 24*60*60;
    }
    if( b >= 60*60 )
    {
     httpPrintf( cnt, "%dh ", b/(60*60) );
     b %= 60*60;
    }
    if( b >= 60 )
     httpPrintf( cnt, "%dm ", b/60 );
   }
  }
  httpString( cnt, "</td></tr>" );
  a = stats[a+1];

}
 httpString( cnt, "</table><br>" );
if( ( id >= 0 ) && ( user ) && ( ( curfam == maind.empire ) || ( ( (cnt->session)->dbuser ) && ( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) ) ) ) {
	if( bitflag( ((cnt->session)->dbuser)->flags, ( CMD_USER_FLAGS_LEADER | CMD_USER_FLAGS_VICELEADER | CMD_USER_FLAGS_DEVMINISTER ) ) || ( ((cnt->session)->dbuser)->level >= LEVEL_MODERATOR ) ) {
		httpString( cnt, "Empire Fund: " );
		if( empired.taxation ) {
			httpPrintf( cnt, "<i>Taxation set at %.02f%%</i>", ( empired.taxation * 100.0 ) );
		} else {
			httpString( cnt, "<i>No tax set</i>" );
		}
		httpString( cnt, "<table>" );
		httpString( cnt, "<tr>" );
		httpPrintf( cnt, "<td align=\"center\">%s</td><td align=\"center\">%lld</td>", cmdRessourceName[CMD_RESSOURCE_ENERGY], (long long)empired.fund[CMD_RESSOURCE_ENERGY] );
		httpPrintf( cnt, "<td align=\"center\">%s</td><td align=\"center\">%lld</td>", cmdRessourceName[CMD_RESSOURCE_CRYSTAL], (long long)empired.fund[CMD_RESSOURCE_CRYSTAL] );
		httpString( cnt, "</tr><tr>" );
		httpPrintf( cnt, "<td align=\"center\">%s</td><td align=\"center\">%lld</td>", cmdRessourceName[CMD_RESSOURCE_MINERAL], (long long)empired.fund[CMD_RESSOURCE_MINERAL] );
		httpPrintf( cnt, "<td align=\"center\">%s</td><td align=\"center\">%lld</td>", cmdRessourceName[CMD_RESSOURCE_ECTROLIUM], (long long)empired.fund[CMD_RESSOURCE_ECTROLIUM] );
		httpString( cnt, "</tr>" );
		httpString( cnt, "</table>" );
	}
}

 if( empired.artefacts )
 {
 	if( ( id >= 0 ) && ( ( curfam == maind.empire ) || ( ( (cnt->session)->dbuser ) && ( (cnt->session)->dbuser->level >= LEVEL_MODERATOR ) ) ) )
   c = ARTEFACT_NUMUSED;
  else
  {
   c = 0;
   for( a = 0, b = 1 ; a < ARTEFACT_NUMUSED ; a++, b <<= 1 )
   {
    if( empired.artefacts & b )
     c++;
   }
  }
  if( ( 3*c >= ARTEFACT_NUMUSED ) || ( (3*dbArtefactMax)/2 >= ARTEFACT_NUMUSED ) )
  {
   httpString( cnt, "<table border=\"0\"><tr><td>" );
   for( a = 0, b = 1 ; a < ARTEFACT_NUMUSED ; a++, b <<= 1 )
   {
   	if( empired.artefacts & b )
   	{
   		httpPrintf( cnt, " <img src=\"files?type=image&name=artifacts/%s\" alt=\"%s\" title=\"%s\"> %s<br>", artefactImage[a], artefactName[a], artefactName[a], artefactDescription[a] );
   	}
   }
   httpString( cnt, "</td></tr></table>" );
  }
 }
 if( curfam == maind.empire )
  httpString( cnt, "<br>Empire members are marked online if a page was requested in the last 5 minutes." );


 iohtmlBodyEnd( cnt );
 return;
}



void iohtmlFunc_famaid( ReplyDataPtr cnt )
{
 int a, b, id, res[4];
 dbUserMainDef maind;
 dbMainEmpireDef empired;
 char *playerstring, *resstring[4];
 char *reportstring;
 dbUserPtr user;

 if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 playerstring = iohtmlVarsFind( cnt, "id" );
 resstring[0] = iohtmlVarsFind( cnt, "r0" );
 resstring[1] = iohtmlVarsFind( cnt, "r1" );
 resstring[2] = iohtmlVarsFind( cnt, "r2" );
 resstring[3] = iohtmlVarsFind( cnt, "r3" );

if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Send Aid" );

 reportstring = 0;
 if( playerstring )
 {
  if( sscanf( playerstring, "%d", &b ) != 1 )
   goto iohttpFunc_famaidL0;

  memset( res, 0, 4*sizeof(int) );
  for( a = 0 ; a < 4 ; a++ )
  {
   if( resstring[a] )
    sscanf( resstring[a], "%d", &res[a] );
  }

  if( cmdExecSendAid( id, b, maind.empire, res ) < 0 )
  {
   if( cmdErrorString )
    reportstring = cmdErrorString;
   else
    reportstring = "Error while sending aid";
  }
  else
   reportstring = "Aid sent";
 }
 iohttpFunc_famaidL0:



 if( reportstring )
  httpPrintf( cnt, "<i>%s</i><br><br>", reportstring );

 if( dbMapRetrieveEmpire( maind.empire, &empired ) < 0 )
 {
  httpString( cnt, "Error while retrieving empire data" );
  iohtmlBodyEnd( cnt );
  return;
 }
 if( empired.numplayers == 1 )
 {
  httpString( cnt, "There is no one to send aid to in your empire yet!" );
  iohtmlBodyEnd( cnt );
  return;
 }

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">Send to : <select name=\"id\">", URLAppend( cnt, "famaid" ) );
 for( a = 0 ; a < empired.numplayers ; a++ )
 {
  if( empired.player[a] == id )
   continue;
  if( !( user = dbUserLinkID( empired.player[a] ) ) )
   continue;
  httpPrintf( cnt, "<option value=\"%d\">%s", empired.player[a], user->faction );
 }

 httpString( cnt, "</select><br><br><table width=\"100%\" cellspacing=\"4\">" );
 for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
  httpPrintf( cnt, "<tr><td width=\"50%%\" align=\"right\">%lld %s</td><td width=\"50%%\"><input type=\"text\" name=\"r%d\" size=\"10\"></td></tr>", (long long)maind.ressource[a], cmdRessourceName[a], a );
 httpString( cnt, "</table><br>" );
 httpString( cnt, "<input type=\"submit\" value=\"Send\"></form>" );

 iohtmlBodyEnd( cnt );
 return;
}



void iohtmlFunc_famgetaid( ReplyDataPtr cnt )
{
 int a, b, id, res[4];
 dbUserMainDef maind, main2d;
 dbMainEmpireDef empired;
 char *accessstring, *playerstring, *resstring[4];
 char *reportstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 accessstring = iohtmlVarsFind( cnt, "access" );
 playerstring = iohtmlVarsFind( cnt, "id" );
 resstring[0] = iohtmlVarsFind( cnt, "r0" );
 resstring[1] = iohtmlVarsFind( cnt, "r1" );
 resstring[2] = iohtmlVarsFind( cnt, "r2" );
 resstring[3] = iohtmlVarsFind( cnt, "r3" );

 reportstring = 0;
 if( accessstring )
 {
  if( sscanf( accessstring, "%d", &b ) != 1 )
   goto iohttpFunc_famaidL0;
  cmdExecAidAccess( id, b );
  reportstring = "Faction resources access rights have been changed";
 }
 if( playerstring )
 {
  if( sscanf( playerstring, "%d", &b ) != 1 )
   goto iohttpFunc_famaidL0;
  if( dbUserMainRetrieve( id, &maind ) < 0 )
  {
   reportstring = "Error retrieving user's infos";
   goto iohttpFunc_famaidL0;
  }
  memset( res, 0, 4*sizeof(int) );
  for( a = 0 ; a < 4 ; a++ )
  {
   if( resstring[a] )
    sscanf( resstring[a], "%d", &res[a] );
  }
  if( cmdExecGetAid( b, id, maind.empire, res ) < 0 )
  {
   if( cmdErrorString )
    reportstring = cmdErrorString;
   else
    reportstring = "Error while receiving aid";
  }
  else
   reportstring = "Aid received";
 }
 iohttpFunc_famaidL0:

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Receive Aid" );

 if( reportstring )
  httpPrintf( cnt, "<i>%s</i><br><br>", reportstring );

 if( dbMapRetrieveEmpire( maind.empire, &empired ) < 0 )
 {
  httpString( cnt, "Error while retrieving empire data" );
  iohtmlBodyEnd( cnt );
  return;
 }

 for( a = 0 ; a < empired.numplayers ; a++ )
 {
  if( empired.player[a] == id )
   continue;
  if( dbUserMainRetrieve( empired.player[a], &main2d ) < 0 )
   continue;
  httpPrintf( cnt, "<b>%s</b> - ", main2d.faction );
  httpPrintf( cnt, "Energy: %lld&nbsp;&nbsp;Mineral: %lld&nbsp;&nbsp;Crystal: %lld&nbsp;&nbsp;Ectrolium: %lld", (long long)main2d.ressource[0], (long long)main2d.ressource[1], (long long)main2d.ressource[2], (long long)main2d.ressource[3] );
  httpPrintf( cnt, "<br><br>" );
 }

 if( empired.numplayers == 1 )
 {
  httpString( cnt, "There is no one to get aid from in your empire yet!" );
  iohtmlBodyEnd( cnt );
  return;
 }

 httpString( cnt, "<table border=\"0\"><tr><td width=\"50%\" align=\"center\">" );

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">Get aid from : <select name=\"id\">", URLAppend( cnt, "famgetaid" ) );
 for( a = 0 ; a < empired.numplayers ; a++ )
 {
  if( empired.player[a] == id )
   continue;
  if( dbUserMainRetrieve( empired.player[a], &main2d ) < 0 )
   continue;
  httpPrintf( cnt, "<option value=\"%d\">%s", empired.player[a], main2d.faction );
 }
 httpString( cnt, "</select><br><br><table width=\"100%\" cellspacing=\"4\">" );
 for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
  httpPrintf( cnt, "<tr><td width=\"50%%\" align=\"right\">%s</td><td width=\"50%%\"><input type=\"text\" name=\"r%d\" size=\"10\"></td></tr>", cmdRessourceName[a], a );
 httpString( cnt, "</table><br>" );
 httpString( cnt, "<input type=\"submit\" value=\"Send\"></form>" );

 httpString( cnt, "</td><td width=\"50%\" align=\"center\">" );

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><br>Allow access to your faction reserves to :<br><select name=\"access\">", URLAppend( cnt, "famgetaid" ) );
 httpPrintf( cnt, "<option value=\"0\"" );
 if( maind.aidaccess == 0 )
  httpPrintf( cnt, " selected" );
 httpPrintf( cnt, ">No one" );
 httpPrintf( cnt, "<option value=\"1\"" );
 if( maind.aidaccess == 1 )
  httpPrintf( cnt, " selected" );
 httpPrintf( cnt, ">Only the leader" );
 httpPrintf( cnt, "<option value=\"2\"" );
 if( maind.aidaccess == 2 )
  httpPrintf( cnt, " selected" );
 httpPrintf( cnt, ">The leader and vice-leaders" );
 httpPrintf( cnt, "<option value=\"3\"" );
 if( maind.aidaccess == 3 )
  httpPrintf( cnt, " selected" );
 httpPrintf( cnt, ">All factions" );
 httpString( cnt, "</select><br><br>" );
 httpString( cnt, "<input type=\"submit\" value=\"Change access\"></form>" );

 httpString( cnt, "</td></tr></table>" );


/*
 httpString( cnt, "<form action=\"famgetaid\" method=\"POST\">Transfer a planet to : <select name=\"id\">" );
 for( a = 0 ; a < empired.numplayers ; a++ )
 {
  if( empired.player[a] == id )
   continue;
  if( dbUserMainRetrieve( empired.player[a], &main2d ) < 0 )
   continue;
  httpPrintf( cnt, "<option value=\"%d\">%s", empired.player[a], main2d.faction );
 }
 httpString( cnt, "</select><br><br><table width=\"100%\" cellspacing=\"4\">" );
 for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
  httpPrintf( cnt, "<tr><td width=\"50%%\" align=\"right\">%s</td><td width=\"50%%\"><input type=\"text\" name=\"r%d\" size=\"10\"></td></tr>", cmdRessourceName[a], a );
 httpString( cnt, "</table><br>" );
 httpString( cnt, "<input type=\"submit\" value=\"Send\"></form>" );
*/


/*
int battleReadinessLoss( dbUserMainPtr maind, dbUserMainPtr main2d )
*/


 iohtmlBodyEnd( cnt );
 return;
}



void iohtmlFunc_vote( ReplyDataPtr cnt ) {
	int a, b, id, fampos, vote;
	dbUserMainDef maind, main2d;
	dbMainEmpireDef empired;
	dbUserPtr user;
	char *votestring, *typestring;
	bool evote = false;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
	return;

iohtmlBase( cnt, 1 );

if( !( iohtmlHeader( cnt, id, &maind ) ) )
	return;

votestring = iohtmlVarsFind( cnt, "id" );
typestring = iohtmlVarsFind( cnt, "type" );

if( typestring ) {
	evote = strcmp(typestring,"eleader") ? false : true;
}


iohtmlBodyInit( cnt, "Vote %s", evote ? " for Empire Leader" : ( "Unavalible" ) );

if( evote ) {
	if( dbMapRetrieveEmpire( maind.empire, &empired ) < 0 ) {
		httpString( cnt, "Error while retrieving empire data" );
		goto VOTEEND;
	}
	for( a = 0 ; ; a++ ) {
		if( a == empired.numplayers ) {
			httpString( cnt, "This is a strange error" );
			goto VOTEEND;
		}
		if( id == empired.player[a] ) {
			fampos = a;
			break;
		}
	}
	if( ( votestring ) && ( sscanf( votestring, "%d", &vote ) == 1 ) ) {
		if( ( user = dbUserLinkID( vote ) ) && ( bitflag( user->flags, CMD_USER_FLAGS_FROZEN ) ) ) {
			httpPrintf( cnt, "<i>Unable to vote for %s, as they have been Frozen by Administration!</i><br><br>", user->faction );
		} else if( cmdExecChangeVote( id, vote ) < 0 ) {
			httpString( cnt, "<i>Failed to change vote...?!</i><br><br>" );
		} else {
			httpString( cnt, "<i>Vote changed</i><br><br>" );
			dbMapRetrieveEmpire( maind.empire, &empired );
		}
	}
	if( empired.leader == -1 ) {
		httpString( cnt, "<b>Your empire didn't elect a leader yet</b>" );
	} else if( empired.leader == id ) {
		httpString( cnt, "<b>You are the leader</b>" );
	} else {
		if( dbUserMainRetrieve( empired.leader, &main2d ) < 0 ) {
			httpString( cnt, "Error while retriving leader's main data" );
		} else {
			httpPrintf( cnt, "<b>The empire leader is %s</b>", main2d.faction );
		}
	}
	httpPrintf( cnt, "<br><form action=\"%s\" method=\"POST\"><table cellspacing=\"8\"><tr><td><b>Empire members</b></td><td>Networth</td><td>Planets</td><td>Your vote</td></tr>", URLAppend( cnt, "vote" ) );
	for( a = b = 0 ; a < empired.numplayers ; a++ ) {
		if( dbUserMainRetrieve( empired.player[a], &main2d ) < 0 ) {
			httpString( cnt, "Error while retriving user's main data" );
			return;
		}
		httpPrintf( cnt, "<tr><td><a href=\"%s&id=%d\">", URLAppend( cnt, "player" ), empired.player[a] );
		if( empired.leader == empired.player[a] ) {
			httpPrintf( cnt, "<font color=\"#FFC040\"><b>%s</b></font>", main2d.faction );
		} else {
			httpString( cnt, main2d.faction );
		}
		httpPrintf( cnt, "</a></td><td>%lld</td><td>%d</td><td align=\"center\"><input type=\"radio\" value=\"%d\" name=\"id\"", (long long)main2d.networth, main2d.planets, empired.player[a] );
		if( ( user = dbUserLinkID( empired.player[a] ) ) && ( bitflag( user->flags, CMD_USER_FLAGS_FROZEN ) ) ) {
			httpString( cnt, " disabled" );
		} else if( empired.vote[fampos] == a ) {
			httpString( cnt, " checked" );
			b = 1;
		}
		httpString( cnt, "></td></tr>" );
	}
	httpString( cnt, "<tr><td>Vote blank</td><td>&nbsp;</td><td>&nbsp;</td><td align=\"center\"><input type=\"radio\" value=\"-1\" name=\"id\"" );
	if( !( b ) )
		httpString( cnt, " checked" );
	httpString( cnt, "></td></tr>" );
	httpString( cnt, "</table><br><input type=\"hidden\" name=\"type\" value=\"eleader\"><input type=\"submit\" value=\"Change vote\"></form>" );
} else {
	httpString( cnt, "Not yet avalible. =(" );
}


VOTEEND:
iohtmlBodyEnd( cnt );


return;
}

void iohtmlFunc_famnews( ReplyDataPtr cnt )
{
 int a, id, num, curfam;
 dbUserMainDef maind;
 dbMainEmpireDef empired;
 ConfigArrayPtr settings;
 char *empirestring;
 char COREDIR[PATH_MAX];
 int64_t *newsp, *newsd;
	FILE *fFile;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 empirestring = iohtmlVarsFind( cnt, "id" );

 curfam = maind.empire;
 if( empirestring )
 {
  sscanf( empirestring, "%d", &curfam );
  a = cmdExecFindRelation( maind.empire, curfam, 0, 0 );
  if( ( a != CMD_RELATION_ALLY ) && ( (cnt->session)->dbuser->level < LEVEL_MODERATOR ) )
   curfam = maind.empire;
  if((cnt->session)->dbuser->level >= LEVEL_MODERATOR)
 	{
 		settings = GetSetting( "Directory" );
		sprintf( COREDIR, "%s/logs/modlog.txt", settings->string_value );
 		fFile = fopen( COREDIR, "a+t" );
 		if( fFile )
 		{
 			fprintf( fFile, "%s >view news of empire %d\n",maind.faction, curfam);
 			fclose(fFile);
 		}
 	}
 }

 if( ( dbMapRetrieveEmpire( curfam, &empired ) ) < 0 )
 {
  httpString( cnt, "This empire does not seem to exist!</body></html>" );
  return;
 }

 iohtmlBodyInit( cnt, "Empire #%d news", curfam );

 num = dbFamNewsList( curfam, &newsp, ticks.number );
 newsd = newsp;
 iohtmlFamNews( cnt, num, newsd, &empired );
 if( newsp )
  free( newsp );

 iohtmlBodyEnd( cnt );
 return;
}

void iohtmlFunc_famrels( ReplyDataPtr cnt )
{
 int a, b, c, id, curfam, nEmp;
 dbUserMainDef maind;
 dbMainEmpireDef empired;
 char *empirestring;
 int *rel;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 empirestring = iohtmlVarsFind( cnt, "id" );

 curfam = maind.empire;
 if(empirestring)
 	nEmp = atoi(empirestring);
 else
 	nEmp = -1;
 if(( ( (cnt->session)->dbuser->level >= LEVEL_MODERATOR ) && ( empirestring ) )|| ( cmdExecFindRelation(maind.empire, nEmp, &c, 0) == CMD_RELATION_ALLY))
  sscanf( empirestring, "%d", &curfam );

	if( ( dbMapRetrieveEmpire( curfam, &empired ) ) < 0 )
 {
  httpString( cnt, "This empire does not seem to exist!</body></html>" );
  return;
 }

 iohtmlBodyInit( cnt, "Empire #%d relations", curfam );

 if( strlen( empired.message[1] ) )
 {
  httpString( cnt, "<b>Message from your leader</b><br>" );
  httpString( cnt, empired.message[1] );
  httpString( cnt, "<br><br>" );
 }

 if( ( b = dbEmpireRelsList( curfam, &rel ) ) >= 0 )
 {
  httpString( cnt, "<b>Empire relations</b><br><br>" );
  if( b == 0 )
   httpString( cnt, "No relations" );
  b <<= 2;
  for( a = 0 ; a < b ; a += 4 )
  {
   if( rel[a+3] & 1 )
    continue;
   if( rel[a+1] == CMD_RELATION_ALLY )
    httpPrintf( cnt, "You are offering an <font color=\"#80FF80\">alliance</font> to <a href=\"%s&id=%d\">empire #%d</a><br><br>", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
   else if( rel[a+1] == CMD_RELATION_WAR )
    httpPrintf( cnt, "You declared <font color=\"#FF0000\">war</font> to <a href=\"%s&id=%d\">empire #%d</a><br><br>", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
  }
  for( a = 0 ; a < b ; a += 4 )
  {
   if( !( rel[a+3] & 1 ) )
    continue;
   if( rel[a+1] == CMD_RELATION_ALLY )
    httpPrintf( cnt, "<a href=\"%s&id=%d\">Empire #%d</a> offered a <font color=\"#80FF80\">alliance</font><br><br>", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
   else if( rel[a+1] == CMD_RELATION_WAR )
    httpPrintf( cnt, "<a href=\"%s&id=%d\">Empire #%d</a> declared <font color=\"#FF0000\">war</font>!<br><br>", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
  }
  httpString( cnt, "<br>" );
  for( a = 0 ; a < b ; a += 4 )
  {
   if( ( rel[a+3] & 1 ) || ( rel[a+1] != CMD_RELATION_ALLY ) )
    continue;
   for( c = 0 ; c < b ; c += 4 )
   {
    if( !( rel[c+3] & 1 ) || ( rel[c+1] != CMD_RELATION_ALLY ) || ( rel[a+2] != rel[c+2] ) )
     continue;
    httpPrintf( cnt, "Your empire is <font color=\"#00FF00\">allied</font> to <a href=\"%s&id=%d\">empire #%d</a> - ", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
    httpPrintf( cnt, "<a href=\"/%s&forum=%d\">Empire Forum</a> - ", URLAppend( cnt, "forum" ), 100+rel[a+2] );
    httpPrintf( cnt, "<a href=\"/%s&id=%d\">Empire news</a> - ", URLAppend( cnt, "famnews" ), rel[a+2] );
    httpPrintf( cnt, "<a href=\"/%s&id=%d\">Empire relations</a><br>", URLAppend( cnt, "famrels" ), rel[a+2]);
   }
  }
  free( rel );
 }

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_famleader( ReplyDataPtr cnt )
{
 int a, b, c, id, curfam, sid, status, relfam, reltype;
 float tax;
 dbUserMainDef maind, main2d;
 dbMainEmpireDef empired;
 char *empirestring, *fnamestring, *sidstring, *statusstring, *fampassstring, *relfamstring, *reltypestring, *hqmesstring, *relsmesstring, *filename, *taxstring;
 char fname[256];
 int *rel;
 char message[USER_DESC_SIZE], message2[USER_DESC_SIZE];

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 empirestring = fnamestring = sidstring = statusstring = fampassstring = reltypestring = relfamstring = hqmesstring = relsmesstring = filename = taxstring = 0;

  empirestring = iohtmlVarsFind( cnt, "id" );
  fnamestring = iohtmlVarsFind( cnt, "fname" );
  sidstring = iohtmlVarsFind( cnt, "sid" );
  statusstring = iohtmlVarsFind( cnt, "status" );
  fampassstring = iohtmlVarsFind( cnt, "fampass" );
  taxstring = iohtmlVarsFind( cnt, "taxlevel" );
  relfamstring = iohtmlVarsFind( cnt, "relfam" );
  reltypestring = iohtmlVarsFind( cnt, "reltype" );
  hqmesstring = iohtmlVarsFind( cnt, "hqmes" );
  relsmesstring = iohtmlVarsFind( cnt, "relsmes" );

 curfam = maind.empire;
 if( ( (cnt->session)->dbuser->level >= LEVEL_MODERATOR ) && ( empirestring ) )
  sscanf( empirestring, "%d", &curfam );

 if( dbMapRetrieveEmpire( curfam, &empired ) < 0 )
 {
  httpString( cnt, "This empire does not seem to exist!</body></html>" );
  return;
 }
 if( ( empired.leader != id ) && ( (cnt->session)->dbuser->level < LEVEL_MODERATOR ) )
 {
  httpString( cnt, "You are not the leader of this empire!</body></html>" );
  return;
 }

 iohtmlBodyInit( cnt, "Leader options" );

if( taxstring ){
	if( ( sscanf( taxstring, "%f", &tax ) != 1 ) ) {
		httpString( cnt, "<i>Invalid input, numbers only!!</i><br><br>" );
	} else if( tax > 25.0 ) {
		httpString( cnt, "<i>Tax level too high!!</i><br><br>" );
	} else {
		empired.taxation = fmax( 0.0, ( tax / 100.0 ) );
		if( dbMapSetEmpire( curfam, &empired ) < 0 ) {
			httpString( cnt, "<i>Error setting new tax...</i><br><br>" );
		} else {
			if( empired.taxation ) {
				httpPrintf( cnt, "<i>Taxation set to %.02f%%</i><br><br>", ( empired.taxation * 100.0 ) );
			} else {
				httpString( cnt, "<i>Empire taxation removed!</i><br><br>" );
			}
		}
	}
}

 if( fnamestring )
 {
  a = iohttpForumFilter( fname, fnamestring, USER_NAME_MAX, 0 );
  if( cmdExecChangFamName( curfam, fname ) < 0 )
  {
   if( cmdErrorString )
    httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
   else
    httpString( cnt, "<i>Error while changing empire name!</i><br><br>" );
  }
  else
  {
   httpString( cnt, "<i>Empire name changed</i><br><br>" );
  }
 }

 if( fampassstring )
 {
  iohttpForumFilter( fname, fampassstring, USER_PASS_MAX, 0 );


  if(cmdExecSetFamPass( curfam, fname) != 1)
			httpString( cnt, "<i>Empire password did not changed</i><br><br>" );
		else
			httpString( cnt, "<i>Empire password changed</i><br><br>" );
 }
 if( ( sidstring ) && ( statusstring ) && ( sscanf( sidstring, "%d", &sid ) == 1 ) && ( sscanf( statusstring, "%d", &status ) == 1 ) )
 {
  if( cmdExecFamMemberFlags( sid, curfam, status ) < 0 )
  {
   httpString( cnt, "Error while changing player status</body></html>" );
   return;
  }
  httpString( cnt, "<i>Empire member status updated</i><br><br>" );
 }

 if( reltypestring )
 {
 	if( sscanf( reltypestring, "%d", &reltype ) != 1 )
   goto iohttpFunc_famleaderL0;
  if( !( relfamstring ) )
  {
   if( cmdExecDelRelation( curfam, reltype ) < 0 )
   {
    if( cmdErrorString )
     httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
    else
     httpString( cnt, "<i>Error while cancelling relation</i><br><br>" );
   }
   else
    httpString( cnt, "<i>Relation cancelled</i><br><br>" );
   goto iohttpFunc_famleaderL0;
  }
  if( sscanf( relfamstring, "%d", &relfam ) != 1 )
   goto iohttpFunc_famleaderL0;

  if( cmdExecAddRelation( curfam, reltype, relfam ) < 0 )
  {
   if( cmdErrorString )
    httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
   else
    httpString( cnt, "<i>Error while adding relation</i><br><br>" );
  }
  else
   httpPrintf( cnt, "<i>Changed relation with empire #%d</i><br><br>", relfam );
 }

if( hqmesstring ) {
	iohttpForumFilter( message, hqmesstring, USER_DESC_SIZE, 0 );
	iohttpForumFilter2( message2, message, USER_DESC_SIZE );
	strcpy(empired.message[0],message2);
	if( dbMapSetEmpire( curfam, &empired ) < 0 ) {
		httpString( cnt, "<i>Error changing Leader message...</i><br><br>" );
	} else {
		httpString( cnt, "<i>Leader message changed</i><br><br>" );
	}
}

if( relsmesstring ) {
	iohttpForumFilter( message, relsmesstring, USER_DESC_SIZE, 0 );
	iohttpForumFilter2( message2, message, USER_DESC_SIZE );
	strcpy(empired.message[1],message2);
	if( dbMapSetEmpire( curfam, &empired ) < 0 ) {
		httpString( cnt, "<i>Error changing Leader message...</i><br><br>" );
	} else {
		httpString( cnt, "<i>Leader message changed</i><br><br>" );
	}
}

 iohttpFunc_famleaderL0:
 dbMapRetrieveEmpire( curfam, &empired );

httpString( cnt, "<div id=\"progblock\" style=\"display:none\">" );
httpString( cnt, "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>" );
httpString( cnt, "<h3 id=\"status\"></h3>" );
httpString( cnt, "<p id=\"loaded_n_total\"></p>" );
httpString( cnt, "</div>" );
 httpString( cnt, "<table>" );

 httpPrintf( cnt, "<tr><td><form action=\"%s\" method=\"POST\">Empire name</td></tr>", URLAppend( cnt, "famleader" ) );
 httpPrintf( cnt, "<tr><td><input type=\"hidden\" name=\"id\" value=\"%d\"><input type=\"text\" name=\"fname\" size=\"64\" value=\"%s\"></td></tr>", curfam, empired.name );
 httpString( cnt, "<tr><td><input type=\"submit\" value=\"Change\"></form><br><br><br></td></tr>" );

 httpPrintf( cnt, "<tr><td><form action=\"%s\" method=\"POST\">Taxation</td></tr>", URLAppend( cnt, "famleader" ) );
 httpPrintf( cnt, "<tr><td><input type=\"text\" name=\"taxlevel\" size=\"8\" value=\"%.2f\"></td></tr>", ( empired.taxation * 100.0 ) );
 httpString( cnt, "<tr><td><input type=\"submit\" value=\"Change\"></form><br><br><br></td></tr>" );



 httpPrintf( cnt, "<tr><td><form enctype=\"multipart/form-data\" action=\"%s\" method=\"POST\">Empire picture</td></tr>", URLAppend( cnt, "famleader" ) );
 httpString( cnt, "<tr><td><i>Note : Empire pictures should not exceed 256k</i></td></tr>" );
 httpString( cnt, "<tr><td><input type=\"hidden\" name=\"upload_type\" value=\"empire\"></td></tr>" );
 httpPrintf( cnt, "<tr><td><input type=\"hidden\" id=\"destination\" value=\"%s\"></td></tr>", URLAppend( cnt, "files" ) );
 httpString( cnt, "<tr><td><input type=\"file\" id=\"fileupload\"></td></tr>" );
 httpString( cnt, "<tr><td><input type=\"button\" value=\"Upload\" onclick=\"uploadFile()\"></form><br><br><br></td></tr>" );




 if( empired.numplayers > 1 ) {
 httpString( cnt, "<tr><td>Set an empire member status</td></tr>" );
 httpString( cnt, "<tr><td><i>Vice-leaders can edit and delete posts in the forum.<br>Factions marked independent aren't allowed to read the empire forum, and can lose their home planet.</i></td></tr>" );
 httpPrintf( cnt, "<tr><td><form action=\"%s\" method=\"POST\"><select name=\"sid\">", URLAppend( cnt, "famleader" ) );
 for( a = 0 ; a < empired.numplayers ; a++ )
 {
  if( empired.player[a] == id )
   continue;
  if( dbUserMainRetrieve( empired.player[a], &main2d ) < 0 )
   continue;
  httpPrintf( cnt, "<option value=\"%d\">%s", empired.player[a], main2d.faction );
 }
 httpString( cnt, "</select>" );
 httpString( cnt, "<select name=\"status\">" );
 httpString( cnt, "<option value=\"0\">No tag</option>" );
 httpPrintf( cnt, "<option value=\"%d\">Vice-leader</option>", CMD_USER_FLAGS_VICELEADER );
 httpPrintf( cnt, "<option value=\"%d\">Minister of Communication</option>", CMD_USER_FLAGS_COMMINISTER );
 httpPrintf( cnt, "<option value=\"%d\">Minister of Development</option>", CMD_USER_FLAGS_DEVMINISTER );
 httpPrintf( cnt, "<option value=\"%d\">Minister of War</option>", CMD_USER_FLAGS_WARMINISTER );
 httpPrintf( cnt, "<option value=\"%d\">Independent</option>", CMD_USER_FLAGS_INDEPENDENT );
 httpString( cnt, "</select></td></tr>" );
 httpString( cnt, "<tr><td><input type=\"submit\" value=\"Change\"></form><br><br><br></td></tr>" );
 }
 cmdExecGetFamPass( curfam, fname );
 httpPrintf( cnt, "<tr><td><form action=\"%s\" method=\"POST\">Empire password</td></tr>", URLAppend( cnt, "famleader" ) );
 httpString( cnt, "<tr><td><i>The empire password is stored in a non-reversable format.<br>Leave the field blank to let anyone join, if you forget it... just change it! =)</i></td></tr>" );
 httpPrintf( cnt, "<tr><td><input type=\"hidden\" name=\"id\" value=\"%d\"><input type=\"text\" name=\"fampass\" size=\"64\" value=\"%s\"></td></tr>", curfam, fname );
 httpString( cnt, "<tr><td><input type=\"submit\" value=\"Change\"></form><br><br><br></td></tr>" );

 httpString( cnt, "<tr><td><table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\">" );
 httpPrintf( cnt, "<tr><td width=\"40%\"><form action=\"%s\" method=\"POST\">Offer an alliance to an empire</td><td width=\"60%\" rowspan=\"4\" valign=\"top\">", URLAppend( cnt, "famleader" ) );
 if( ( b = dbEmpireRelsList( curfam, &rel ) ) >= 0 )
 {
  httpString( cnt, "<b>Empire relations</b><br><br>" );
  if( b == 0 )
   httpString( cnt, "No relations" );
  b <<= 2;
  for( a = 0 ; a < b ; a += 4 )
  {
   if( rel[a+3] & 1 )
    continue;
   if( rel[a+1] == CMD_RELATION_ALLY ) {
    httpPrintf( cnt, "You are offering an alliance to <a href=\"%s&id=%d\">empire #%d</a> - ", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
    httpPrintf( cnt, "<a href=\"%s&id=%d&reltype=%d\">Cancel</a><br>", URLAppend( cnt, "famleader" ), curfam, a >> 2 );
   } else if( rel[a+1] == CMD_RELATION_WAR ) {
    httpPrintf( cnt, "You declared war on <a href=\"%s&id=%d\">empire #%d</a> - ", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
    httpPrintf( cnt, "<a href=\"%s&id=%d&reltype=%d\">Cancel</a><br>", URLAppend( cnt, "famleader" ), curfam, a >> 2 );
    }
  }
  for( a = 0 ; a < b ; a += 4 )
  {
   if( !( rel[a+3] & 1 ) )
    continue;
   if( rel[a+1] == CMD_RELATION_ALLY )
    httpPrintf( cnt, "<a href=\"%s&id=%d\">Empire #%d</a> offered a alliance<br>", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
   else if( rel[a+1] == CMD_RELATION_WAR )
    httpPrintf( cnt, "<a href=\"%s&id=%d\">Empire #%d</a> declared war!<br>", URLAppend( cnt, "empire" ), rel[a+2], rel[a+2] );
  }
  httpString( cnt, "<br>" );
  for( a = 0 ; a < b ; a += 4 )
  {
   if( ( rel[a+3] & 1 ) || ( rel[a+1] != CMD_RELATION_ALLY ) )
    continue;
   for( c = 0 ; c < b ; c += 4 )
   {
    if( !( rel[c+3] & 1 ) || ( rel[c+1] != CMD_RELATION_ALLY ) || ( rel[a+2] != rel[c+2] ) )
     continue;
    httpPrintf( cnt, "<i>Allied to empire #%d</i><br>", rel[a+2] );
   }
  }
  free( rel );
 }
 httpString( cnt, "</td></tr>" );
 httpString( cnt, "<tr><td><input type=\"text\" name=\"relfam\" size=\"8\"> <input type=\"hidden\" name=\"reltype\" value=\"0\"> <input type=\"submit\" value=\"Send\"></form><br><br><br></td></tr>" );
 httpPrintf( cnt, "<tr><td><form action=\"%s\" method=\"POST\">Declare war to an empire</td></tr>", URLAppend( cnt, "famleader" ) );
 httpString( cnt, "<tr><td><input type=\"text\" name=\"relfam\" size=\"8\"> <input type=\"hidden\" name=\"reltype\" value=\"1\"> <input type=\"submit\" value=\"Send\"></form><br><br><br></td></tr>" );
 httpString( cnt, "</table></td></tr>" );

 iohttpForumFilter3( message2, empired.message[0], USER_DESC_SIZE );
 httpPrintf( cnt, "<tr><td><form action=\"%s\" method=\"POST\">Leader message</td></tr>", URLAppend( cnt, "famleader" ) );
 httpPrintf( cnt, "<tr><td><input type=\"hidden\" name=\"id\" value=\"%d\"><textarea name=\"hqmes\" wrap=\"soft\" rows=\"4\" cols=\"64\">%s</textarea></td></tr>", curfam, message2 );
 httpString( cnt, "<tr><td><input type=\"submit\" value=\"Change\"></form><br><br><br></td></tr>" );

 iohttpForumFilter3( message2, empired.message[1], USER_DESC_SIZE );
 httpPrintf( cnt, "<tr><td><form action=\"%s\" method=\"POST\">Relations message</td></tr>", URLAppend( cnt, "famleader" ) );
 httpPrintf( cnt, "<tr><td><input type=\"hidden\" name=\"id\" value=\"%d\"><textarea name=\"relsmes\" wrap=\"soft\" rows=\"4\" cols=\"64\">%s</textarea></td></tr>", curfam, message2 );
 httpString( cnt, "<tr><td><input type=\"submit\" value=\"Change\"></form><br><br><br></td></tr>" );

 httpString( cnt, "</table>" );


 iohtmlBodyEnd( cnt );

 return;
}


#define IOHTTP_MAPPICK_DIVIDE (3)

#define IOHTTP_MAPADV_ENTRIES (8)

void iohttpMapSystemsBuffer( int *mapp, int *buffer, int andl, int numpl, int type )
{
 int a, b, x, y;
 if( type )
 {
  for( a = 0 ; a < numpl ; a++ )
  {
   x = ( buffer[a] >> 8 ) & 0xFFF;
   y = buffer[a] >> 20;
   b = y * dbMapBInfoStatic[MAP_SIZEX] + x;
   mapp[b] &= 0xFF00FFFF;
   mapp[b] |= andl;
  }
 }
 else
 {
  for( a = 0 ; a < numpl ; a++ )
  {
   x = ( buffer[a] >> 8 ) & 0xFFF;
   y = buffer[a] >> 20;
   mapp[ y * dbMapBInfoStatic[MAP_SIZEX] + x ] |= andl;
  }
 }
 free( buffer );
 return;
}


void iohtmlFunc_map( ReplyDataPtr cnt )
{
 int a, b, c, d, numpl, id, x, y, i, explcol, newwins, cid;
 int *buffer;
 dbUserMainDef maind;
 dbMainSystemDef systemd;
 dbMainEmpireDef empired;
 int *mapp;
 int zoompos[2];
 char advname[16];
 char *varstring;
 int advopt[IOHTTP_MAPADV_ENTRIES];
 int advcol[IOHTTP_MAPADV_ENTRIES];
 char *advdet[IOHTTP_MAPADV_ENTRIES];
 char advdetnum[IOHTTP_MAPADV_ENTRIES][16];
 char *configstring;
 int basex, basey, endx, endy, zoomsize;
 int config[8];

 if( ( id = iohtmlIdentify( cnt, 1|2|8 ) ) < 0 )
  return;
 if( dbUserMainRetrieve( id, &maind ) < 0 )
  maind.empire = -1;

 if( !( mapp = malloc( dbMapBInfoStatic[MAP_SIZEX]*dbMapBInfoStatic[MAP_SIZEY]*sizeof(int) ) ) )
 {
  iohtmlBase( cnt, 1|2 );
  httpString( cnt, "Error, could not allocate memory</body></html>" );
  return;
 }
 memset( mapp, 0, dbMapBInfoStatic[MAP_SIZEX]*dbMapBInfoStatic[MAP_SIZEY]*sizeof(int) );

 if( !( iohtmlVarsMapcoords( cnt, zoompos ) ) )
 {
  zoompos[0] = 0;
  zoompos[1] = 0;
  zoomsize = -1;
 }
 else
 {
  zoompos[0] /= IOHTTP_MAPPICK_DIVIDE;
  zoompos[1] /= IOHTTP_MAPPICK_DIVIDE;
  zoomsize = maind.config_mapsize & 0xFFFF;
  if( (unsigned int)zoomsize > 60 )
   zoomsize = 15;
 }

 configstring = iohtmlVarsFind( cnt, "setdefault" );
 for( a = 0 ; a < IOHTTP_MAPADV_ENTRIES ; a++ )
 {
  config[a] = 0;
  sprintf( advname, "e%d", a );
  varstring = iohtmlVarsFind( cnt, advname );
  if( !( varstring ) || ( sscanf( varstring, "%d", &advopt[a] ) != 1 ) )
   advopt[a] = -1;
  advname[0] = 'c';
  varstring = iohtmlVarsFind( cnt, advname );
  if( !( varstring ) || ( sscanf( varstring, "%d", &advcol[a] ) != 1 ) || ( (unsigned int)advcol[a] >= 6 ) )
   advopt[a] = -1;
  else
   config[a] = ( advopt[a] ) + ( advcol[a] << 4 ) + 0x10000000;
  advname[0] = 'u';
  advdet[a] = iohtmlVarsFind( cnt, advname );
 }
 newwins = 0;
 if( ( advopt[0] == -1 ) && ( maind.config_mapsize & 0x10000 ) )
  newwins = 1;
 else if( iohtmlVarsFind( cnt, "newwin" ) )
  newwins = 1;

 if( newwins )
  iohtmlBase( cnt, 1|2|4 );
 else
  iohtmlBase( cnt, 1|2 );

 if( advopt[0] == -1 )
 {
  for( a = 0 ; a < IOHTTP_MAPADV_ENTRIES ; a++ )
  {
   advopt[a] = maind.config_map[a] & 0xF;
   advcol[a] = ( maind.config_map[a] >> 4 ) & 0xF;
   b = maind.config_map[a] >> 8;
   if( b >= 0x10000 )
    continue;
   sprintf( advdetnum[a], "%d", b );
   advdet[a] = advdetnum[a];
  }
 }

 explcol = 0;
 for( a = 0 ; a < IOHTTP_MAPADV_ENTRIES ; a++ )
 {
  if( (unsigned int)advcol[a] < 3 )
   advcol[a] = ( ( 1 << advcol[a] ) << 16 ) | ( 1 << 20 );
  else
   advcol[a] = ( 1 << (advcol[a]-3) ) << 16;
  if( advopt[a] == 6 )
   explcol = advcol[a] & 0xF0000;
 }
 for( a = 0 ; a < dbMapBInfoStatic[MAP_SYSTEMS] ; a++ )
 {
  dbMapRetrieveSystem( a, &systemd );
  x = systemd.position & 0xFFFF;
  y = systemd.position >> 16;
  i = y * dbMapBInfoStatic[MAP_SIZEX] + x;
  mapp[i] = a+1;
  if( systemd.empire != -1 )
   mapp[i] |= 0x1000000;
  else if( systemd.unexplored )
   mapp[i] |= explcol;
 }

 for( a = 0, d = explcol ; a < IOHTTP_MAPADV_ENTRIES ; a++ )
 {
  if( (unsigned int)advopt[a] >= 6 )
   continue;
  d |= advopt[a];

  if( advopt[a] == 1 )
  {
   if( ( numpl = dbUserPlanetListCoords( id, &buffer ) ) < 0 )
    continue;
   iohttpMapSystemsBuffer( mapp, buffer, advcol[a] & 0xF0000, numpl, advcol[a] >> 20 );
  }
  else if( advopt[a] == 2 )
  {
   if( ( numpl = dbUserPortalsListCoords( id, &buffer ) ) < 0 )
    continue;
   iohttpMapSystemsBuffer( mapp, buffer, advcol[a] & 0xF0000, numpl, advcol[a] >> 20 );
  }
  else if( advopt[a] == 3 )
  {
   if( dbUserMainRetrieve( id, &maind ) < 0 )
    continue;
   cid = maind.empire;
   goto iohttpFunc_mapL0;
  }
  else if( advopt[a] == 4 )
  {
   if( !( advdet[a] ) || !( advdet[a][0] ) )
   {
    httpPrintf( cnt, "#%d You must specify the faction name or user ID.<br><br>", a+1 );
    continue;
   }
   if( ( advdet[a][0] >= '0' ) && ( advdet[a][0] <= '9' ) )
    sscanf( advdet[a], "%d", &cid );
   else
   {
    for( c = 0 ; advdet[a][c] ; c++ )
    {
     if( advdet[a][c] == '+' )
      advdet[a][c] = ' ';
     else if( ( advdet[a][c] == 10 ) || ( advdet[a][c] == 13 ) )
      advdet[a][c] = 0;
    }
    cid = dbUserSearchFaction( advdet[a] );
    if( cid < 0 )
    {
     httpPrintf( cnt, "#%d The faction %s does not seem to exist, the syntax must be exact or use the user ID.<br><br>", a+1, advdet[a] );
     continue;
    }
   }
   if( ( numpl = dbUserPlanetListCoords( cid, &buffer ) ) < 0 )
   {
    httpPrintf( cnt, "#%d There is no user with an ID of %d.<br><br>", a+1, cid );
    continue;
   }
   iohttpMapSystemsBuffer( mapp, buffer, advcol[a] & 0xF0000, numpl, advcol[a] >> 20 );
   config[a] &= 0xFF;
   config[a] |= cid << 8;
  }
  else if( advopt[a] == 5 )
  {
   if( !( advdet[a] ) || !( advdet[a][0] ) || ( sscanf( advdet[a], "%d", &cid ) <= 0 ) )
   {
    httpPrintf( cnt, "#%d You must specify the empire number.<br><br>", a+1 );
    continue;
   }
   iohttpFunc_mapL0:
   if( dbMapRetrieveEmpire( cid, &empired ) < 0 )
   {
    httpPrintf( cnt, "#%d This empire doesn't seem to exist.<br><br>", a+1 );
    continue;
   }
   config[a] &= 0xFF;
   config[a] |= cid << 8;
   for( c = 0 ; c < empired.numplayers ; c++ )
   {
    if( ( numpl = dbUserPlanetListCoords( empired.player[c], &buffer ) ) < 0 )
    {
     httpPrintf( cnt, "#%d There is no user with an ID of %d.<br><br>", a+1, cid );
     continue;
    }
    iohttpMapSystemsBuffer( mapp, buffer, advcol[a] & 0xF0000, numpl, advcol[a] >> 20 );
   }
  }
 }
 if( !( d ) )
 {
  if( ( numpl = dbUserPlanetListCoords( id, &buffer ) ) >= 0 )
   iohttpMapSystemsBuffer( mapp, buffer, 1 << 16, numpl, 0 );
 }

 if( ( configstring ) && ( dbUserMainRetrieve( id, &maind ) >= 0 ) )
 {
  maind.config_mapsize &= 0xFFFF;
  if( newwins )
   maind.config_mapsize |= 0x10000;
  memcpy( maind.config_map, config, 8*sizeof(int) );
  dbUserMainSet( id, &maind );
 }

 if( zoomsize <= 0 )
 {
  basex = basey = 0;
  endx = dbMapBInfoStatic[MAP_SIZEX];
  endy = dbMapBInfoStatic[MAP_SIZEY];
 }
 else
 {
  if( zoompos[0] < zoomsize )
  {
   basex = 0;
   endx = ( zoomsize << 1 );
  }
  else if( zoompos[0] > dbMapBInfoStatic[MAP_SIZEX]-zoomsize )
  {
   basex = dbMapBInfoStatic[MAP_SIZEX] - ( zoomsize << 1 );
   endx = dbMapBInfoStatic[MAP_SIZEX];
  }
  else
  {
   basex = zoompos[0] - zoomsize;
   endx = zoompos[0] + zoomsize;
  }
  if( zoompos[1] < zoomsize )
  {
   basey = 0;
   endy = ( zoomsize << 1 );
  }
  else if( zoompos[1] > dbMapBInfoStatic[MAP_SIZEY]-zoomsize )
  {
   basey = dbMapBInfoStatic[MAP_SIZEY] - ( zoomsize << 1 );
   endy = dbMapBInfoStatic[MAP_SIZEY];
  }
  else
  {
   basey = zoompos[1] - zoomsize;
   endy = zoompos[1] + zoomsize;
  }
 }
 httpPrintf( cnt, "<table cellspacing=\"0\" cellpading=\"0\" width=\"%d\"><colgroup><col span=\"%d\" width=\"24\">", 24*zoomsize, zoomsize );
 httpString( cnt, "<tr><td>&nbsp;</td>" );
 for( x = basex ; x < endx ; x++ )
  httpPrintf( cnt, "<td align=\"center\">%d</td>", x );
 httpString( cnt, "<td>&nbsp;</td></tr>" );
 i = basex + ( basey * dbMapBInfoStatic[MAP_SIZEX] );
 srand( 0 );
 for( y = basey ; y < endy ; y++ )
 {
  httpPrintf( cnt, "<tr><td>%d</td>", y );
  for( x = basex ; x < endx ; x++, i++ )
  {
   if( !( mapp[i] ) )
    httpString( cnt, "<td> " );
   else
   {
    a = '0' + ( ( mapp[i] >> 16 ) & 0xF );
    b = '5';
    if( !( mapp[i] >> 24 ) )
     b = '0' + ( rand() % 5 );
    httpPrintf( cnt, "<td><a href=\"%s&id=%d\"", URLAppend( cnt, "system" ), ( mapp[i] & 0xFFFF ) - 1 );
    httpPrintf( cnt, " target=\"main\"><img border=\"0\" src=\"files?type=image&name=m%c%c.gif\" title=\"%d,%d\"></a>", a, b, x, y );
   }
  }
  if( zoomsize >= 0 )
   i += dbMapBInfoStatic[MAP_SIZEX] - ( zoomsize << 1 );
  httpPrintf( cnt, "<td>%d</tr>", y );
 }
 RANDOMIZE_SEED;
 httpString( cnt, "<tr><td>&nbsp;</td>" );
 for( x = basex ; x < endx ; x++ )
  httpPrintf( cnt, "<td align=\"center\">%d</td>", x );
 httpString( cnt, "<td>&nbsp;</td></tr>" );
 httpString( cnt, "</table>" );

 free( mapp );
 httpString( cnt, "</center></body></html>" );
 return;
}

#define MAPPICKSIZES (4) // Disable 45 and 60, due to bad rendering
void iohtmlFunc_mappick( ReplyDataPtr cnt )
{
 int a, b, i, id;
 int px, py;
 dbUserMainDef maind;
 dbMainSystemDef systemd;
 char *sizestring;
 static int sizes[MAPPICKSIZES] = { 15, 20, 25, 30/*, 45, 60*/ };

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Galaxy map" );

 sizestring = iohtmlVarsFind( cnt, "size" );

 if( ( sizestring ) && ( sscanf( sizestring, "%d", &a ) == 1 ) )
 {
  if( cmdExecSetSectsize( id, a ) >= 0 )
  {
   httpPrintf( cnt, "<i>Default map sector size changed for %d</i><br><br>", a );
   maind.config_mapsize = a | ( maind.config_mapsize & 0xFFFF0000 );
  }
  else
   httpString( cnt, "<i>Error while changing default sector size</i><br><br>" );
 }

 a = ( maind.home >> 8 ) & 0xFFF;
 b = maind.home >> 20;
 httpPrintf( cnt, "Click on the part of the map you want to render with your default <a href=\"%s\">map generation</a> settings.<br>Your home system is located at the coordinates ", URLAppend( cnt, "mapadv" ) );
 httpPrintf( cnt, "<a href=\"%s&sectorzoom=%d,%d\">%d,%d</a> ( click to zoom there ).<br><br>", URLAppend( cnt, "map" ), a * IOHTTP_MAPPICK_DIVIDE, b * IOHTTP_MAPPICK_DIVIDE, a, b );

 a = dbMapBInfoStatic[MAP_SIZEX] * IOHTTP_MAPPICK_DIVIDE;
 httpPrintf( cnt, "<table border=\"0\" width=\"%d\" cellpadding=\"0\" cellspacing=\"0\">", a + 80 );
 a /= 3;
 httpPrintf( cnt, "<tr><td width=\"40\">&nbsp;</td><td width=\"%d\" align=\"left\"><b>0</b></td><td width=\"%d\" align=\"center\"><b>%d</b></td><td width=\"%d\" align=\"right\"><b>%d</b></td><td width=\"40\">&nbsp;</td></tr>", a, a, dbMapBInfoStatic[MAP_SIZEX] >> 1, a, dbMapBInfoStatic[MAP_SIZEX] );

 httpPrintf( cnt, "<tr><td height=\"%d\" align=\"right\" valign=\"top\"><b>0</b></td>", a );
 httpPrintf( cnt, "<td colspan=\"3\" rowspan=\"3\"><img src=\"files?type=image&name=galaxies/galaxyr%d.png\" style=\"width:100%%; height:100%%;\" alt=\"Planets\" usemap=\"#systemmap\">", ticks.round/*, dbMapBInfoStatic[MAP_SIZEX] *IOHTTP_MAPPICK_DIVIDE, dbMapBInfoStatic[MAP_SIZEY] *IOHTTP_MAPPICK_DIVIDE*/ );

httpString( cnt, "<map name=\"systemmap\">" );
for( i = 0; i < dbMapBInfoStatic[MAP_SYSTEMS]; i++ ) {
	dbMapRetrieveSystem( i, &systemd );
	px = ( ( systemd.position & 0xFFFF )* IOHTTP_MAPPICK_DIVIDE );
	py = ( (systemd.position >> 16)* IOHTTP_MAPPICK_DIVIDE );
	httpPrintf( cnt, "<area shape=\"circle\" coords=\"%d,%d,8\" alt=\"system%d\" href=\"%s&sectorzoom=%d,%d\">", px, py, i, URLAppend( cnt, "map" ), px, py );

}
httpString( cnt, "</map></td>" );
 
 
 
 
 
 httpPrintf( cnt, "<td height=\"%d\" align=\"left\" valign=\"top\"><b>0</b></td></tr>", a );
 httpPrintf( cnt, "<tr><td height=\"%d\" align=\"right\" valign=\"center\"><b>%d</b></td><td height=\"%d\" align=\"left\" valign=\"center\"><b>%d</b></td></tr>", a, dbMapBInfoStatic[MAP_SIZEX] >> 1, a, dbMapBInfoStatic[MAP_SIZEX] >> 1 );
 httpPrintf( cnt, "<tr><td height=\"%d\" align=\"right\" valign=\"bottom\"><b>%d</b></td><td height=\"%d\" align=\"left\" valign=\"bottom\"><b>%d</b></td></tr>", a, dbMapBInfoStatic[MAP_SIZEX], a, dbMapBInfoStatic[MAP_SIZEX] );

 httpPrintf( cnt, "<tr><td>&nbsp;</td><td align=\"left\"><b>0</b></td><td align=\"center\"><b>%d</b></td><td align=\"right\"><b>%d</b></td><td>&nbsp;</td></tr>", dbMapBInfoStatic[MAP_SIZEX] >> 1, dbMapBInfoStatic[MAP_SIZEX] );
 httpString( cnt, "</table>" );

 httpPrintf( cnt, "<br><form action=\"%s\" method=\"GET\"><select name=\"size\">", URLAppend( cnt, "mappick" ) );
 for( a = 0 ; a < MAPPICKSIZES ; a++ )
 {
  httpPrintf( cnt, "<option value=\"%d\"", sizes[a] );
  if( ( maind.config_mapsize & 0xFFFF ) == sizes[a] )
   httpString( cnt, " selected" );
  httpPrintf( cnt, ">Galaxy sectors of %d by %d", sizes[a], sizes[a] );
 }
 httpString( cnt, "</select>&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"submit\" value=\"Change\"></form>" );

 iohtmlBodyEnd( cnt );
 return;
}

void iohtmlFunc_mapadv( ReplyDataPtr cnt )
{
 int a, b, id;
 dbUserMainDef maind;
 static char *advopt[7] = { ">Unused", ">Your planets", ">Your portals", ">Your empire", ">Planets of faction :", ">Planets of empire :", ">Unexplored planets" };
 int advoptbase[IOHTTP_MAPADV_ENTRIES] = { 1, 2, 0, 0, 0, 0, 0, 0 };
 static char *advcol[6] = { ">Green", ">Blue", ">Red", ">Green merged", ">Blue merged", ">Red merged" };
 int advcolbase[IOHTTP_MAPADV_ENTRIES] = { 1, 0, 0, 0, 0, 0, 0, 0 };
 int advnumbase[IOHTTP_MAPADV_ENTRIES] = { -1, -1, -1, -1, -1, -1, -1, -1 };

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Map generation" );

 for( a = 0 ; a < IOHTTP_MAPADV_ENTRIES ; a++ )
 {
  advoptbase[a] = maind.config_map[a] & 0xF;
  advcolbase[a] = ( maind.config_map[a] >> 4 ) & 0xF;
  if( ( advoptbase[a] == 4 ) || ( advoptbase[a] == 5 ) )
   advnumbase[a] = maind.config_map[a] >> 8;
 }

 httpPrintf( cnt, "<form action=\"%s\" method=\"GET\"><table cellspacing=\"6\"><tr><td>#</td><td>Planets list</td><td>Details</td><td>Colour</td></tr>", URLAppend( cnt, "map" ) );
 for( a = 0 ; a < IOHTTP_MAPADV_ENTRIES ; a++ )
 {
  httpPrintf( cnt, "<tr><td>%d</td><td><select name=\"e%d\">", a+1, a );
  for( b = 0 ; b < 7 ; b++ )
  {
   httpPrintf( cnt, "<option value=\"%d\"", b );
   if( advoptbase[a] == b )
    httpString( cnt, " selected" );
   httpString( cnt, advopt[b] );
  }
  httpString( cnt, "</select></td>" );
  httpPrintf( cnt, "<td><input type=\"text\" name=\"u%d\" size=\"16\"", a );
  if( (unsigned int)advnumbase[a] < 0x10000 )
   httpPrintf( cnt, " value=\"%d\"", advnumbase[a] );
  httpString( cnt, "></td>" );
  httpPrintf( cnt, "<td><select name=\"c%d\">", a );
  for( b = 0 ; b < 6 ; b++ )
  {
   httpPrintf( cnt, "<option value=\"%d\"", b );
   if( advcolbase[a] == b )
    httpString( cnt, " selected" );
   httpString( cnt, advcol[b] );
  }
  httpString( cnt, "</select></td></tr>" );
 }
 httpString( cnt, "<tr><td colspan=\"4\" align=\"center\"><table cellspacing=\"0\" cellpadding=\"0\"><tr><td><input type=\"checkbox\" name=\"newwin\"" );
 if( maind.config_mapsize & 0x10000 )
  httpString( cnt, " checked" );
 httpString( cnt, "> Open system views in new windows<br><input type=\"checkbox\" name=\"setdefault\"> Save these settings as the default</td></tr></table></td></tr>" );
 httpString( cnt, "<tr><td colspan=\"4\" align=\"center\"><input type=\"submit\" value=\"Generate map\"></td></tr>" );
 httpString( cnt, "</table></form>" );

 httpString( cnt, "<i>Note : The details fields are used to specify exact faction names or ID, or empire numbers.</i><br><br><br>" );

 httpString( cnt, "<b>Reset map defaults to</b><br><table><tr><td>" );
 httpPrintf( cnt, "<a href=\"%s&e0=1&c0=0&newwin=1&setdefault=1\">Your planets in green</a><br>", URLAppend( cnt, "map" ) );
 httpPrintf( cnt, "<a href=\"%s&e0=1&c0=1&e1=2&c1=0&newwin=1&setdefault=1\">Your planets in blue and portals in green</a><br>", URLAppend( cnt, "map" ) );
 httpPrintf( cnt, "<a href=\"%s&e0=3&c0=2&e1=1&c1=1&e2=2&c2=0&newwin=1&setdefault=1\">Your empire in red, your planets in blue and portals in green</a><br>", URLAppend( cnt, "map" ) );
 httpString( cnt, "</td></tr></table>" );

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_system( ReplyDataPtr cnt )
{
 int a, id, sysid, plnid;
 dbUserMainDef maind, main2d;
 dbMainSystemDef systemd;
 dbMainPlanetDef planetd;
 char *systemstring;

 int b, c, d, ln, lns[32], pics[64];
 float fa;

 iohtmlBase( cnt, 1 );

 if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 systemstring = iohtmlVarsFind( cnt, "id" );

 if( !( systemstring ) || ( sscanf( systemstring, "%d", &sysid ) <= 0 ) || ( dbMapRetrieveSystem( sysid, &systemd ) < 0 ) )
 {
  httpString( cnt, "This system doesn't seem to exist!</body></html>" );
  return;
 }

 srand( sysid );
 for( a = 0 ; a < systemd.numplanets ; a++ )
  pics[a] = rand() & 0xF;

 httpPrintf( cnt, "<b>System %d,%d</b><br>", systemd.position & 0xFFFF, systemd.position >> 16 );

 if( systemd.empire != -1 )
  httpPrintf( cnt, "<b>Home of <a href=\"%s&?id=%d\">empire #%d</a></b><br><br>", URLAppend( cnt, "empire" ), systemd.empire, systemd.empire );


 plnid = systemd.indexplanet;
if( ( systemd.unexplored > 1 ) && ( systemd.empire == -1 ) )
 httpPrintf( cnt, "<br>There are %d unexplored planets in this system<br><a href=\"%s&system=%d\">Mass explore system</a>", systemd.unexplored, URLAppend( cnt, "explore" ), sysid );
 httpString( cnt, "<table width=\"100%\" cellspacing=\"6\" cellpadding=\"6\">" );
 for( a = b = ln = 0 ; a < systemd.numplanets ; a++, plnid++ )
 {
  if( !( b ) )
  {
   httpString( cnt, "<tr><td><table width=\"100%\"><tr>" );
   ln = ( rand() % 3 ) + 2;
   for( c = d = 0 ; c < ln ; c++ )
   {
    lns[c] = 2 + ( rand() & 15 );
    d += lns[c];
   }
   fa = 100.0 / (float)d;
   for( c = 0 ; c < ln ; c++, d += lns[c] )
    lns[c] = (int)( (float)lns[c] * fa );
  }

  httpPrintf( cnt, "<td align=\"center\" width=\"%d%%\">", lns[b] );
  dbMapRetrievePlanet( plnid, &planetd );
  httpPrintf( cnt, "<a href=\"%s&id=%d\"><img src=\"files?type=image&name=p%02d.gif\" border=\"0\" alt=\"Planet #%d\"></a><br>", URLAppend( cnt, "planet" ), plnid, pics[a], plnid );

  if( planetd.owner == -1 )
  {
   httpPrintf( cnt, "<font color=\"#8090A0\">Planet %d unexplored</font>", a );
   goto iohttpFunc_systemL0;
  }

  if( dbUserMainRetrieve( planetd.owner, &main2d ) < 0 )
   goto iohttpFunc_systemL0;

  httpPrintf( cnt, "Planet %d owned by <a href=\"%s&id=%d\">%s</a> of ", a, URLAppend( cnt, "player" ), planetd.owner, main2d.faction );
  httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a><br>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire );

  iohttpFunc_systemL0:
   httpString( cnt, "</td>" );
  b = ( b + 1 ) % ln;
  if( !( b ) )
   httpString( cnt, "</tr></td></table></tr>" );
 }

 if( b )
  httpString( cnt, "</tr></td></table></tr>" );
 httpString( cnt, "</table><br><br>" );

 httpString( cnt, "</center></body></html>" );

 return;
}


void iohtmlFunc_player( ReplyDataPtr cnt )
{
 int a, b, id, playerid;
 dbUserMainDef maind, main2d;
 char *playerstring;
 dbUserInfoDef infod;
 dbUserRecordPtr recordd;

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	iohtmlBase( cnt, 1 );
	if( !( iohtmlHeader( cnt, id, &maind ) ) )
		return;
} else {
	iohtmlBase( cnt, 8 );
	iohtmlFunc_frontmenu( cnt, FMENU_NONE );
}

 playerstring = iohtmlVarsFind( cnt, "id" );

 if( !( playerstring ) || ( sscanf( playerstring, "%d", &playerid ) <= 0 ) || ( dbUserMainRetrieve( playerid, &main2d ) < 0 ) || ( dbUserInfoRetrieve( playerid, &infod ) < 0 ) )
 {
  httpString( cnt, "Are you sure this user exists?</body></html>" );
  return;
 }
 iohtmlBodyInit( cnt, main2d.faction );
 httpPrintf( cnt, "<table border=\"0\"><tr><td>" );
 httpPrintf( cnt, "<a href=\"%s&id=%d\">Empire : #%d</a>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire );
 httpPrintf( cnt, "<br>Networth : %lld<br>Planets : %d<br>Race : %s<br>Forum tag : <b>%s</b><br>Population : %lld0<br>Home planet : %d,%d:%d<br><br>Faction ID : %d<br>", (long long)main2d.networth, main2d.planets, cmdRaceName[main2d.raceid], infod.forumtag, (long long)main2d.ressource[CMD_RESSOURCE_POPULATION], ( main2d.home >> 8 ) & 0xFFF, main2d.home >> 20, main2d.home & 0xFF, playerid );
 httpPrintf( cnt, "<a href=\"%s&to=%d\">Send a message</a><br>", URLAppend( cnt, "mail" ), playerid );
 httpPrintf( cnt, "<a href=\"%s&e0=4&u0=%d&c0=5\">Display planets on map</a><br>", URLAppend( cnt, "map" ), playerid );
 httpPrintf( cnt, "<a href=\"%s&e0=1&u0=&c0=3&e1=4&u1=%d&c1=5\">Display planets on map with yours</a><br>", URLAppend( cnt, "map" ), playerid );
 httpPrintf( cnt, "<a href=\"%s&id=%d\">See planets list</a><br>", URLAppend( cnt, "playerlist" ), playerid );
 httpString( cnt, "<br></td></tr></table>" );

 if( strlen(infod.desc) )
 {
  httpString( cnt, "<i>Faction description</i><br><table border=\"0\"><tr><td>" );
  httpString( cnt, infod.desc );
  httpString( cnt, "</td></tr></table>" );
 }

 if( ( b = dbUserRecordList( playerid, &recordd ) ) >= 0 )
 {
  httpString( cnt, "<table border=\"0\"><tr><td><br><i>Player records</i><br>" );
  httpPrintf( cnt, "Rounds played : %d", b );
  for( a = b-1 ; a >= 0 ; a-- )
  {
   httpPrintf( cnt, "<br><br>Round : %d", recordd[a].roundid );
   httpPrintf( cnt, "<br>Faction name : %s", recordd[a].faction );
   httpPrintf( cnt, "<br>Title : %s", recordd[a].forumtag );
   httpPrintf( cnt, "<br>Planets : %d", recordd[a].planets );
   httpPrintf( cnt, "<br>Networth : %d", recordd[a].networth );
   httpPrintf( cnt, "<br>Faction rank : %d", recordd[a].rank );
   httpPrintf( cnt, "<br>Empire : #%d", recordd[a].empire );
   httpPrintf( cnt, "<br>Empire name : %s", recordd[a].famname );
   httpPrintf( cnt, "<br>Empire rank : %d", recordd[a].famrank );
   httpPrintf( cnt, "<br>Empire planets : %d", recordd[a].famplanets );
   httpPrintf( cnt, "<br>Empire networth : %d", recordd[a].famnetworth );
   httpPrintf( cnt, "<br>Empire artefacts : %d", recordd[a].artefacts );
  }
  httpString( cnt, "</td></tr></table>" );
  free( recordd );
 }

 iohtmlBodyEnd( cnt );
 return;
}

void iohtmlFunc_playerlist( ReplyDataPtr cnt )
{
 int a, num, id, playerid;
 int *buffer;
 dbUserMainDef maind, main2d;
 char *playerstring;

 iohtmlBase( cnt, 1 );

 if( ( id = iohtmlIdentify( cnt, 1|2 ) ) >= 0 )
 {
  if( !( iohtmlHeader( cnt, id, &maind ) ) )
   return;
 }

 playerstring = iohtmlVarsFind( cnt, "id" );

 if( !( playerstring ) || ( sscanf( playerstring, "%d", &playerid ) <= 0 ) || ( dbUserMainRetrieve( playerid, &main2d ) < 0 ) )
 {
  httpPrintf( cnt, "Are you sure this user %d exists?</body></html>", playerid );
  return;
 }
 iohtmlBodyInit( cnt, main2d.faction );

 httpString( cnt, "Planets list<br>" );

 if( ( num = dbUserPlanetListCoords( playerid, &buffer ) ) < 0 )
 {
  httpString( cnt, "Are you sure this user exists?</body></html>" );
  iohtmlBodyEnd( cnt );
  return;
 }
 if( num )
 {
  num--;
  for( a = 0 ; ; a++ )
  {
   httpPrintf( cnt, "%d,%d:%d", ( buffer[a] >> 8 ) & 0xFF, buffer[a] >> 20, buffer[a] & 0xFF );
   if( a >= num )
    break;
   if( ( a & 7 ) == 7 )
    httpString( cnt, "<br>" );
   else
    httpString( cnt, " &nbsp; " );
  }
 }
 free( buffer );

 iohtmlBodyEnd( cnt );
 return;
}



void iohtmlFunc_planet( ReplyDataPtr cnt )
{
 int a, b, cmd[3], cmd2[2], id, plnid, plgive;
 char szString[450];
 char szTemp[100];
 dbUserMainDef maind, main2d;
 dbMainEmpireDef empired;
 dbMainPlanetDef planetd;
 dbUserPtr user2;
 char *planetstring;
 char *unstationstring;
 char *plgivestring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 planetstring = iohtmlVarsFind( cnt, "id" );
 unstationstring = iohtmlVarsFind( cnt, "unstation" );
 plgivestring = iohtmlVarsFind( cnt, "plgive" );

 if( !( planetstring ) || ( sscanf( planetstring, "%d", &plnid ) <= 0 ) || ( dbMapRetrievePlanet( plnid, &planetd ) < 0 ) )
 {
  httpString( cnt, "This planet doesn't seem to exist!</body></html>" );
  return;
 }
 plgive = -2;
 if( plgivestring )
  sscanf( plgivestring, "%d", &plgive );

 b = planetd.position & 0xFF;
 httpPrintf( cnt, "<b>Planet %d,%d:%d</b><br><br>", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, b );

 if( plgive != -2 )
 {
  httpPrintf( cnt, "<i>Planet offer setting changed</i><br><br>" );
  cmdExecOfferPlanet( id, plgive, plnid );
  planetd.surrender = plgive;
 }

 srand( planetd.system );
 for( a = 0 ; a < b ; a++ )
  rand();
 httpPrintf( cnt, "<img src=\"files?type=image&name=p%02d.gif\" border=\"0\" alt=\"Planet %d\"><br><br>", rand() & 0xF, plnid );

 if( unstationstring )
 {
  cmd[0] = CMD_UNSTATION_PLANET;
  cmd[1] = id;
  cmd[2] = plnid;
  if( cmdExecute( cnt, cmd, 0, 0 ) < 0 )
   httpString( cnt, "Error encountered while unstationing forces<br><br>" );
  else
   httpString( cnt, "<i>Fleet unstationed</i><br><br>" );
  memset( planetd.unit, 0, CMD_UNIT_NUMUSED*sizeof(int) );
 }

 if( planetd.owner == -1 )
 {
  if( planetd.flags & CMD_PLANET_FLAGS_HOME )
   httpPrintf( cnt, "No one owns this planet, but it is part of a home system and unavailable for exploration" );
	else {
	httpPrintf( cnt, "No one owns this planet, it is free to explore.<br><br><a href=\"%s&id=%d\">Explore this planet</a><br><br>", URLAppend( cnt, "explore" ), plnid );
	httpPrintf( cnt, "<a href=\"%s&id=%d\">Special operation</a>", URLAppend( cnt, "spec" ), plnid );
	}
 }
 else if( planetd.owner == id )
 {
  httpString( cnt, "This planet is yours.<br>" );
  httpPrintf( cnt, "Population : %.0f0<br>", planetd.population );
 b = (int)artefactPrecense( &planetd );
  if( b >= 0 )
   httpPrintf( cnt, "<br><img src=\"files?type=image&name=artifacts/%s\" alt=\"%s\" title=\"%s\"> %s<br>", artefactImage[b], artefactName[b], artefactName[b], artefactDescription[b] );
  else if(planetd.special[1])
   httpPrintf( cnt, "<br><img src=\"files?type=image&name=pr%d.gif\" alt=\"%s\" title=\"%s\"> %s production : <font color=\"#20FF20\">+%d%%</font><br>", planetd.special[0], cmdBonusName[planetd.special[0]], cmdBonusName[planetd.special[0]], cmdBonusName[planetd.special[0]], planetd.special[1] );



  httpPrintf( cnt, "<SCRIPT type=\"text/JavaScript\">\n ");
  sprintf(szString, " function Areyousure(plnid)\n{if(confirm(\"Are you sure you want to raze eveything on this planet??\"))open(\"raze?id=\"+plnid+\"");
  for( b = 0 ; b < CMD_BLDG_NUMUSED ; b++ )
	 {
	  sprintf(szTemp, "&bldg%d=%d", b, planetd.building[b]);
	  strcat(szString, szTemp);
	 }
  strcat(szString, "\",\"_self\");\n}");
  httpString( cnt, szString);

  httpPrintf( cnt,"</SCRIPT>\n");

  httpPrintf( cnt, "<br><a href=\"%s&id=%d\">Build on this planet</a><br><br>", URLAppend( cnt, "build" ), plnid );
  httpPrintf( cnt, "<a href=\"%s&id=%d\">Station on this planet</a><br><br>", URLAppend( cnt, "station" ), plnid );
  httpPrintf( cnt, "<a href=\"%s&id=%d\">Raze buildings</a><br><br>", URLAppend( cnt, "raze" ), plnid );
  httpPrintf( cnt, "<a href=# onClick=\"Areyousure(%d)\">Raze all buildings</a><br>", plnid );


  if( ( dbMapRetrieveEmpire( maind.empire, &empired ) >= 0 ) && ( empired.numplayers >= 2 ) && !( planetd.flags & CMD_PLANET_FLAGS_HOME ) )
  {
   httpPrintf( cnt, "<br><form action=\"%s\" method=\"POST\">Offer this planet to :", URLAppend( cnt, "planet" ) );
   httpPrintf( cnt, "<input type=\"hidden\" value=\"%d\" name=\"id\">", plnid );
   httpString( cnt, "<select name=\"plgive\">" );
   httpPrintf( cnt, "<option value=\"%d\">%s", -1, "No one" );
   for( a = b = 0 ; a < empired.numplayers ; a++ )
   {
    if( !( user2 = dbUserLinkID( empired.player[a] ) ) )
     continue;
    if( user2->id == id )
     continue;
    if( user2->id != planetd.surrender )
    {
     httpPrintf( cnt, "<option value=\"%d\">%s", user2->id, user2->faction );
     continue;
    }
    httpPrintf( cnt, "<option value=\"%d\" selected>%s", user2->id, user2->faction );
    b = 1;
   }
   httpString( cnt, "</select><br>" );
   httpString( cnt, "<input type=\"submit\" value=\"Confirm\"></form>" );
  }

  httpString( cnt, "<table width=\"90%\"><tr><td width=\"45%\" align=\"center\" valign=\"top\"><b>Buildings</b><br><br>" );
  for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
  {
   if( planetd.building[a] )
    httpPrintf( cnt, "%d %s<br>", planetd.building[a], cmdBuildingName[a] );
  }
  if( planetd.flags & CMD_PLANET_FLAGS_PORTAL )
   httpString( cnt, "Portal<br>" );
  if( planetd.construction )
   httpPrintf( cnt, "<br>%d additional buildings are under construction", planetd.construction );
  httpString( cnt, "</td><td width=\"45%\" align=\"center\" valign=\"top\"><b>Units</b><br><br>" );
  for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
  {
   if( planetd.unit[a] )
   {
    httpPrintf( cnt, "%d %s<br>", planetd.unit[a], cmdUnitName[a] );
    b = 1;
   }
  }
  if( b )
   httpPrintf( cnt, "<a href=\"%s&id=%d&unstation=1\">Unstation fleet</a>", URLAppend( cnt, "planet" ), plnid );
  httpString( cnt, "</td></tr></table>" );
 }
 else
 {
  cmd2[0] = CMD_RETRIEVE_USERMAIN;
  cmd2[1] = planetd.owner;
  if( cmdExecute( cnt, cmd2, &main2d, 0 ) < 0 )
   httpString( cnt, "Error while retriving user's main data" );
  else {
   httpPrintf( cnt, "This planet is owned by : <a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), planetd.owner, main2d.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a>, networth %lld.<br><br>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire, (long long)main2d.networth );
   }
   httpPrintf( cnt, "<a href=\"%s&id=%d\">Attack this planet</a><br><br>", URLAppend( cnt, "attack" ), plnid );
   httpPrintf( cnt, "<a href=\"%s&id=%d\">Special operation</a>", URLAppend( cnt, "spec" ), plnid );
 }
 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View system</a>", URLAppend( cnt, "system" ), planetd.system );

if( planetd.surrender == id )
	httpPrintf( cnt, "<br><br>%s offered you this planet <a href=\"%s&id=%d\">accept offer?</a>", main2d.faction, URLAppend( cnt, "pltake" ), plnid );

 httpString( cnt, "</center></body></html>" );

 return;
}


void iohtmlFunc_build( ReplyDataPtr cnt )
{
 int a, b, c, id, cmd[6], plnid, totalb;
 int64_t resbuild[CMD_RESSOURCE_NUMUSED+2];
 float fa;
 dbUserMainDef maind;
 dbMainPlanetDef planetd;
 char *buildbldg[CMD_BLDG_NUMUSED];
 char *buildplnid;
 char buildname[16];
 char buildstring[CMD_BLDG_NUMUSED+1][128];
 char *portalstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );


 for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
 {
  sprintf( buildname, "bldg%d", a );
  buildbldg[a] = iohtmlVarsFind( cnt, buildname );
 }
 buildplnid = iohtmlVarsFind( cnt, "id" );
 portalstring = iohtmlVarsFind( cnt, "portal" );

 //cmd[0] = CMD_RETRIEVE_PLANET;
 if( !( buildplnid ) || ( sscanf( buildplnid, "%d", &cmd[1] ) <= 0 ) || ( dbMapRetrievePlanet( cmd[1], &planetd ) < 0 ) )
 {
  httpString( cnt, "This planet doesn't seem to exist!</body></html>" );
  return;
 }
 if( planetd.owner != id )
 {
  httpString( cnt, "You can't build on a planet that you do not own!</body></html>" );
  return;
 }
 plnid = cmd[1];

 cmd[0] = CMD_ADD_BUILD;
 cmd[1] = id;
 cmd[4] = plnid;
 cmd[5] = 0xFFFFFF;
 for( a = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ )
  buildstring[a][0] = 0;
 for( a = c = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
 {
  if( !( buildbldg[a] ) )
   continue;
  if( sscanf( buildbldg[a], "%d", &b ) <= 0 )
   continue;
  if( b <= 0 )
   continue;
  cmd[2] = a;
  cmd[3] = b;
  b = cmdExecAddBuild( cmd[1], cmd[2], cmd[3], cmd[4], cmd[5] );
/*
  b = cmdExecute( cnt, cmd, 0, 0 );
*/
  if( b >= 0 )
  {
   sprintf( buildstring[a], "<i>You started building %d %s</i><br><br>", cmd[3], cmdBuildingName[a] );
   c++;
  }
  else if( cmdErrorString )
   sprintf( buildstring[a], "<i>%s</i><br><br>", cmdErrorString );
  else
   sprintf( buildstring[a], "<i>Error while adding build item</i><br>" );
 }
 if( portalstring )
 {
  cmd[0] = CMD_ADD_BUILD;
  cmd[1] = id;
  cmd[2] = CMD_BLDG_NUMUSED;
  cmd[3] = 1;
  cmd[4] = plnid;
  b = cmdExecute( cnt, cmd, 0, 0 );
  if( b >= 0 )
  {
   sprintf( buildstring[CMD_BLDG_NUMUSED], "<i>You started building a portal</i><br><br>" );
   c++;
  }
  else if( cmdErrorString )
   sprintf( buildstring[CMD_BLDG_NUMUSED], "<i>%s</i><br><br>", cmdErrorString );
  else
   sprintf( buildstring[CMD_BLDG_NUMUSED], "<i>Error while adding portal build item</i><br><br>" );
 }

 if( c )
 {
  dbMapRetrievePlanet( plnid, &planetd );
 }

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Build on planet %d,%d:%d", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );
 httpPrintf( cnt, "<a href=\"%s\">Building Information</a><br><br>", URLAppend( cnt, "stats" ) );
 for( a = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ )
 {
  if( buildstring[a][0] )
   httpString( cnt, buildstring[a] );
 }

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><input type=\"hidden\" value=\"%d\" name=\"id\">", URLAppend( cnt, "build" ), plnid );

 totalb = planetd.construction;
 for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
  totalb += planetd.building[a];
 if( totalb )
 {
  httpPrintf( cnt, "There are %d buildings on this planet", totalb );
  if( planetd.construction )
   httpPrintf( cnt, ", %d of them are still under construction", planetd.construction );
  httpString( cnt, ". " );
 }
 httpPrintf( cnt, "This planet can support %d buildings, additional ones will be very expensive.<br>", planetd.size );

 for( a = CMD_BLDG_OVERBUILD ; a < CMD_BLDG_NUMUSED ; a++ )
  totalb -= planetd.building[a];
 if( ( fa = cmdGetBuildOvercost( planetd.size, totalb ) ) != 1.0 )
 {
  httpPrintf( cnt, "The overbuilding penalty for this planet is <font color=\"#FF2020\">+%.2f%%</font><br>", (fa-1.0)*100.0 );
 }

 httpString( cnt, "<br><table cellspacing=\"6\"><tr><td><b>Building</b></td><td><b>Cost</b></td><td><b>Owned</b></td><td><b>Build</b></td></tr>" );
 for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
 {
  cmdGetBuildCosts( &maind, a, resbuild );
  if( !( cmdBuildingFlags[a] ) )
   cmdGetBuildOverbuild( planetd.size, totalb, resbuild );

  httpPrintf( cnt, "<tr><td valign=\"top\"><font color=\"#FFFFFF\">%s</font><br></td><td valign=\"top\" nowrap>", cmdBuildingName[a] );
  if( resbuild[0] < 0 )
  {
   httpPrintf( cnt, "<font color=\"#FF0000\">Unavailable<br>Tech too low</font></td><td valign=\"top\" align=\"center\">%lld</td><td>&nbsp;</td></tr>", (long long)maind.totalbuilding[a] );
   continue;
  }
  for( b = 0 ; b < CMD_RESSOURCE_NUMUSED+1 ; b++ )
  {
   if( resbuild[b] )
    httpPrintf( cnt, "%lld %s<br>", (long long)resbuild[b], cmdRessourceName[b] );
  }
  if( resbuild[b] )
   httpPrintf( cnt, "<font color=\"#FF2020\">Tech : +%lld%% costs</font><br>", (long long)resbuild[b] );
  httpPrintf( cnt, "</td><td valign=\"top\" align=\"center\">%lld</td><td valign=\"top\"><input type=\"text\" name=\"bldg%d\" size=\"10\"></td></tr>", (long long)maind.totalbuilding[a], a );
 }

 if( planetd.flags & CMD_PLANET_FLAGS_PORTAL )
  httpString( cnt, "<tr><td valign=\"top\" colspan=\"2\"><font color=\"#FFFFFF\">Portal</font><br>Already present" );
 else if( planetd.flags & CMD_PLANET_FLAGS_PORTAL_BUILD )
  httpString( cnt, "<tr><td valign=\"top\" colspan=\"2\"><font color=\"#FFFFFF\">Portal</font><br>Under construction" );
 else
 {
  cmdGetBuildCosts( &maind, CMD_BLDG_NUMUSED, resbuild );
  httpPrintf( cnt, "<tr><td valign=\"top\"><font color=\"#FFFFFF\">Portal</font></td><td valign=\"top\" nowrap>" );
  if( resbuild[0] < 0 )
   httpPrintf( cnt, "<font color=\"#FF0000\">Unavailable<br>Tech too low</font></td><td valign=\"top\" align=\"center\">%lld</td><td>&nbsp;</td></tr>", (long long)maind.totalbuilding[CMD_BLDG_NUMUSED] );
  else
  {
   for( b = 0 ; b < CMD_RESSOURCE_NUMUSED+1 ; b++ )
   {
    if( resbuild[b] )
     httpPrintf( cnt, "%lld %s<br>", (long long)resbuild[b], cmdRessourceName[b] );
   }
   if( resbuild[b] )
    httpPrintf( cnt, "<font color=\"#FF2020\">Tech : +%lld%% costs</font><br>", (long long)resbuild[b] );
  }
 }
 httpPrintf( cnt, "</td><td valign=\"top\" align=\"center\">%lld</td><td valign=\"top\">", (long long)maind.totalbuilding[CMD_BLDG_NUMUSED] );
 if( !( planetd.flags & ( CMD_PLANET_FLAGS_PORTAL | CMD_PLANET_FLAGS_PORTAL_BUILD ) ) )
  httpString( cnt, "<input type=\"checkbox\" name=\"portal\"> Build a portal" );
 else
  httpString( cnt, "&nbsp;" );
 httpString( cnt, "</td></tr>" );

 httpString( cnt, "</table><input type=\"submit\" value=\"Build\"></form>" );
 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View planet</a>", URLAppend( cnt, "planet" ), plnid );
 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View system</a><br><br>", URLAppend( cnt, "system" ), planetd.system );

 iohtmlBodyEnd( cnt );
 return;
}

void iohtmlFunc_cancelbuild( ReplyDataPtr cnt )
{
	int id, i, j, nNbr=0, nTotalBuild, nTotal[4];
	int *nBuildp=0, *nTemp=0;
	dbUserMainDef maind;
	dbUserBuildPtr buildp;
	dbMainPlanetDef planetd;
	char buildid[10];
	char *cBuild;

	iohtmlBase( cnt, 1 );
 if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
	memset(&nTotal, 0, 4*sizeof(int));
	nTotalBuild = dbUserBuildList(id, &buildp);

 nBuildp = NULL;
 for( i=0; i<nTotalBuild; i++)
 {
  sprintf( buildid, "b%d", i );
  cBuild = iohtmlVarsFind( cnt, buildid );
  if(cBuild)
  {
  	if(cBuild[1] == 'n')
  	{
	  	if(nNbr)
	  	{
	  		nTemp = malloc(nNbr*sizeof(int));
	  		memcpy(nTemp, nBuildp, nNbr*sizeof(int));
	  		free(nBuildp);
	  	}
	  	nBuildp = malloc((nNbr+1) * sizeof(int));
	  	if(nNbr)
	  	{
	  		memcpy(nBuildp, nTemp, (nNbr)*sizeof(int));
	  	}
	  	nBuildp[nNbr] = i;
	  	nNbr++;
	  }
  }
 }

	dbUserMainRetrieve(id, &maind);

	for(i=nNbr-1;i>=0;i--)
 {
 	for(j=0;j<4;j++)
 	{
 		nTotal[j] += (buildp[nBuildp[i]]).cost[j] / 2;
 	}
 	dbUserBuildRemove(id, nBuildp[i]);
 	if( !( buildp[nBuildp[i]].type >> 16 ) )
   {
    dbMapRetrievePlanet( buildp[nBuildp[i]].plnid, &planetd );
    if( buildp[nBuildp[i]].type == CMD_BLDG_NUMUSED )
    {
     // portal
     planetd.flags = 0;
     dbUserPlanetSetFlags( id, buildp[nBuildp[i]].plnid, planetd.flags );
     planetd.construction--;
    }
    else
    {
     planetd.construction -= buildp[nBuildp[i]].quantity;
    }
    dbMapSetPlanet( buildp[nBuildp[i]].plnid, &planetd );
   }
 }

 for(i=0;i<4;i++)
 	maind.ressource[i] += (nTotal[i]);

 dbUserMainSet(id, &maind);


 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Cancelling %d buildings or units", nNbr);
 if(nNbr)
 {
	 	httpPrintf(cnt, "%d building or unit under construction have been remove<br>", nNbr);
	 	httpPrintf(cnt, "<table><tr><td>You were refunded with:</td><td></td><td></td></tr>");
	 	for(i=0;i<4;i++)
	 	{
	 		if(nTotal[i])
	 			httpPrintf(cnt, "<tr><td></td><td>%d</td><td>%s</td></tr>", nTotal[i], cmdRessourceName[i]);
	 	}
	 	httpString(cnt, "</table>");
	}
 	httpPrintf(cnt, "<a href=\"%s\">Back</a>", URLAppend( cnt, "council" ) );
 free( buildp );
 if(nBuildp)
free(nBuildp);
 iohtmlBodyEnd( cnt );
}

#define IOHTTP_MASSBUILD_STRING 16384

void iohtmlFunc_massbuild( ReplyDataPtr cnt )
{
 int a, b, c, d, e, id, cmd[7], numpl, totalpl, totalportal, buildoff, oblimit, oblimitlow;
 int64_t resbuild[CMD_RESSOURCE_NUMUSED+2];
 float fa, cost;
 int *buffer;
 dbUserMainDef maind;
 dbMainPlanetDef planetd;
 char *buildbldg[CMD_BLDG_NUMUSED];
 char buildname[16];
 char buildstring[IOHTTP_MASSBUILD_STRING];
 char *portalstring;
 char *oblimitstring;
 char *oblimitstringlow;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );


 cmd[0] = CMD_RETRIEVE_USERPLISTINDICES;
 cmd[1] = id;
 if( ( numpl = cmdExecute( cnt, cmd, &buffer, 0 ) ) < 0 )
 {
  httpString( cnt, "Error while retriving planets list" );
  return;
 }

 buildoff = 0;
 buildstring[0] = 0;
 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "massbuild" ) );

 cost = 0.0;
 for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
 {
  sprintf( buildname, "bldg%d", a );
  buildbldg[a] = iohtmlVarsFind( cnt, buildname );
 }
 portalstring = iohtmlVarsFind( cnt, "portal" );

 oblimit = 0xFFFFFF;
 oblimitlow = 0;
 if( iohtmlVarsFind( cnt, "obcheck" ) )
 {
  oblimitstring = iohtmlVarsFind( cnt, "oblimit" );
  oblimitstringlow = iohtmlVarsFind( cnt, "oblimitlow" );
  if( oblimitstring )
   sscanf( oblimitstring, "%d", &oblimit );
  if( oblimitstringlow )
   sscanf( oblimitstringlow, "%d", &oblimitlow );
 }
 cmd[5] = oblimit;
 cmd[6] = oblimitlow;

 for( a = e = totalpl = totalportal = 0 ; a < numpl ; a++ )
 {
  sprintf( buildname, "m%d", buffer[a] );
  if( !( iohtmlVarsFind( cnt, buildname ) ) )
   continue;

  if( (   dbMapRetrievePlanet( buffer[a], &planetd ) < 0 ) )
   continue;
  totalpl++;

  httpPrintf( cnt, "<input type=\"hidden\" value=\"on\" name=\"m%d\">", buffer[a] );
  cmd[0] = CMD_ADD_BUILD;
  cmd[1] = id;
  cmd[4] = buffer[a];
  for( c = d = 0 ; c < CMD_BLDG_NUMUSED ; c++ )
  {
   if( !( buildbldg[c] ) )
    continue;
   if( sscanf( buildbldg[c], "%d", &b ) <= 0 )
    continue;
   if( b <= 0 )
    continue;
   cmd[2] = c;
   cmd[3] = b;
/*
cmdExecAddBuild !!!
*/
   b = cmdExecute( cnt, cmd, 0, 0 );
   e = 1;
   if( buildoff >= IOHTTP_MASSBUILD_STRING - 256 )
    continue;
   if( b == 0 )
    buildoff += sprintf( &buildstring[buildoff], "<i>No additionnal %s were built", cmdBuildingName[c] );
   else if( b > 0 )
   {
    buildoff += sprintf( &buildstring[buildoff], "<i>You started building %d %s", b, cmdBuildingName[c] );
    d += b;
   }
   else if( cmdErrorString )
    buildoff += sprintf( &buildstring[buildoff], "<i>%s", cmdErrorString );
   else
    buildoff += sprintf( &buildstring[buildoff], "<i>Error while adding build item" );
   buildoff += sprintf( &buildstring[buildoff], " on <a href=\"%s&id=%d\">%d,%d:%d</a></i><br>", URLAppend( cnt, "planet" ), buffer[a], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );
  }
  b = planetd.construction + d;
  for( c = 0 ; c < CMD_BLDG_OVERBUILD ; c++ )
   b += planetd.building[c];
  cost += cmdGetBuildOvercost( planetd.size, b );
  if( ( portalstring ) && !( planetd.flags & ( CMD_PLANET_FLAGS_PORTAL | CMD_PLANET_FLAGS_PORTAL_BUILD ) ) )
  {
   cmd[0] = CMD_ADD_BUILD;
   cmd[1] = id;
   cmd[2] = CMD_BLDG_NUMUSED;
   cmd[3] = 1;
   cmd[4] = buffer[a];
   b = cmdExecute( cnt, cmd, 0, 0 );
   e = 1;
   if( buildoff >= IOHTTP_MASSBUILD_STRING - 256 )
    continue;
   if( b >= 0 )
   {
    buildoff += sprintf( &buildstring[buildoff], "<i>You started building a portal" );
    planetd.flags |= CMD_PLANET_FLAGS_PORTAL_BUILD;
    d = -1;
   }
   else if( cmdErrorString )
    buildoff += sprintf( &buildstring[buildoff], "<i>%s", cmdErrorString );
   else
    buildoff += sprintf( &buildstring[buildoff], "<i>Error while adding portal build item" );
   buildoff += sprintf( &buildstring[buildoff], " on <a href=\"%s&id=%d\">%d,%d:%d</a></i><br>", URLAppend( cnt, "planet" ), buffer[a], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );
  }
  if( planetd.flags & CMD_PLANET_FLAGS_PORTAL )
   totalportal++;
  else if( ( planetd.flags & CMD_PLANET_FLAGS_PORTAL_BUILD ) || ( d == -1 ) )
   totalportal += 1 << 16;
 }

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
 {
  free( buffer );
  return;
 }

 iohtmlBodyInit( cnt, "Build on %d planets", totalpl );
 httpString( cnt, "<a href=\"stats\">Building Information</a><br><br>" );
 httpString( cnt, buildstring );

 httpString( cnt, "<table><tr><td>" );
 fa = cost / (float)totalpl;
 if( fa > 1.001 )
  httpPrintf( cnt, "The average overbuilding penalty is <font color=\"#FF2020\">+%.2f%%</font><br>", (fa-1.0)*100.0 );
 httpString( cnt, "<input type=\"checkbox\" name=\"obcheck\"> build between overbuilding penalty from <input type=\"text\" name=\"oblimitlow\" size=\"4\"> to <input type=\"text\" name=\"oblimit\" size=\"4\"> %" );
 httpString( cnt, "</td></tr></table>" );

 httpString( cnt, "<br><table cellspacing=\"6\"><tr><td><b>Building</b></td><td><b>Cost</b></td><td><b>Owned</b></td><td><b>Build</b></td></tr>" );
 for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
 {
  cmdGetBuildCosts( &maind, a, resbuild );
  httpPrintf( cnt, "<tr><td valign=\"top\"><font color=\"#FFFFFF\">%s</font><br></td><td valign=\"top\" nowrap>", cmdBuildingName[a] );
  if( resbuild[0] < 0 )
  {
   httpPrintf( cnt, "<font color=\"#FF0000\">Unavailable<br>Tech too low</font></td><td valign=\"top\" align=\"center\">%lld</td><td>&nbsp;</td></tr>", (long long)maind.totalbuilding[a] );
   continue;
  }
  for( b = 0 ; b < CMD_RESSOURCE_NUMUSED ; b++ )
  {
   if( resbuild[b] )
    httpPrintf( cnt, "%lld %s<br>", (long long)( resbuild[b] * cost ), cmdRessourceName[b] );
  }
  httpPrintf( cnt, "%lld %s<br>", (long long)resbuild[b], cmdRessourceName[b] );
  b++;
  if( resbuild[b] )
   httpPrintf( cnt, "<font color=\"#FF2020\">Tech : +%lld%% costs</font><br>", (long long)resbuild[b] );
  httpPrintf( cnt, "</td><td valign=\"top\" align=\"center\">%lld</td><td valign=\"top\"><input type=\"text\" name=\"bldg%d\" size=\"10\"></td></tr>", (long long)maind.totalbuilding[a], a );
 }

 a = ( totalportal & 0xFFFF ) + ( totalportal >> 16 );
 b = 0;
 if( totalpl == totalportal )
  httpString( cnt, "<tr><td valign=\"top\" colspan=\"2\"><font color=\"#FFFFFF\">Portal</font><br>All present" );
 else if( totalpl == a )
  httpString( cnt, "<tr><td valign=\"top\" colspan=\"2\"><font color=\"#FFFFFF\">Portal</font><br>Under construction" );
 else
 {
  b = totalpl - a;
  cmdGetBuildCosts( &maind, CMD_BLDG_NUMUSED, resbuild );
  httpPrintf( cnt, "<tr><td valign=\"top\"><font color=\"#FFFFFF\">Portal ( %d )</font></td><td valign=\"top\" nowrap>", b );
  if( resbuild[0] < 0 )
   httpPrintf( cnt, "<font color=\"#FF0000\">Unavailable<br>Tech too low</font></td><td valign=\"top\" align=\"center\">%lld</td><td>&nbsp;</td></tr>", (long long)maind.totalbuilding[CMD_BLDG_NUMUSED] );
  else
  {
   for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ )
   {
    if( resbuild[a] )
     httpPrintf( cnt, "%lld %s<br>", (long long)b*resbuild[a], cmdRessourceName[a] );
   }
   httpPrintf( cnt, "%lld %s<br>", (long long)resbuild[a], cmdRessourceName[a] );
   a++;
   if( resbuild[a] )
    httpPrintf( cnt, "<font color=\"#FF2020\">Tech : +%lld%% costs</font><br>", (long long)resbuild[a] );
  }
 }
 httpPrintf( cnt, "</td><td valign=\"top\" align=\"center\">%lld</td><td valign=\"top\">", (long long)maind.totalbuilding[CMD_BLDG_NUMUSED] );
 if( b )
  httpString( cnt, "<input type=\"checkbox\" name=\"portal\"> Build portals" );
 else
  httpString( cnt, "&nbsp;" );
 httpString( cnt, "</td></tr>" );

 httpString( cnt, "</table><input type=\"submit\" value=\"Build\"></form>" );

 free( buffer );
 iohtmlBodyEnd( cnt );
 return;
}

/*
//Use to chat on irc
void iohtmlFunc_chat( ReplyDataPtr cnt )
{
	dbUserMainDef maind;
	int id;
	char *szFaction;
	char szTemp[32];

	iohtmlBase( cnt, 1 );
	if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

	szFaction = iohtmlVarsFind( cnt, "faction" );

	if(!szFaction)
	{
		sprintf(szTemp,"User%d", id);
		szFaction = szTemp;
	}

	iohtmlBodyInit( cnt, "Chat %s", szFaction);
	httpPrintf( cnt, "<b>Have fun chatting %s</b><br>", szFaction);
	httpString( cnt, "<applet code=IRCApplet.class archive=\"irc.jar,pixx.jar\" width=800 height=600>" );
	httpString( cnt,"<param name=\"CABINETS\" value=\"irc.cab,securedirc.cab,pixx.cab\">");
	httpPrintf( cnt, "<param name=\"nick\" value=\"%s\"><param name=\"alternatenick\" value=\"user%d\"><param name=\"name\" value=\"EctroverseChat\"><param name=\"host\" value=\"%s\"><param name=\"gui\" value=\"pixx\"><param name=\"command1\" value=\"join #ectroverse\"></applet>", szFaction, id, IRC_SERVER);
	iohtmlBodyEnd( cnt );

	return;
}
*/
void iohtmlFunc_raze( ReplyDataPtr cnt )
{
 int a, b, c, id, cmd[5], plnid, totalb;
 float fa;
 dbUserMainDef maind;
 dbMainPlanetDef planetd;
 char *razebldg[CMD_BLDG_NUMUSED];
 char *razeplnid;
 char razename[16];
 char *portalstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
 {
  sprintf( razename, "bldg%d", a );
  razebldg[a] = iohtmlVarsFind( cnt, razename );
 }
 razeplnid = iohtmlVarsFind( cnt, "id" );
 portalstring = iohtmlVarsFind( cnt, "portal" );

 //cmd[0] = CMD_RETRIEVE_PLANET;
 if( !( razeplnid ) || ( sscanf( razeplnid, "%d", &cmd[1] ) <= 0 ) || (   dbMapRetrievePlanet( cmd[1], &planetd ) < 0 ) )
 {
  httpString( cnt, "This planet doesn't seem to exist!</body></html>" );
  return;
 }
 if( planetd.owner != id )
 {
  httpString( cnt, "You can't raze on a planet that you do not own!</body></html>" );
  return;
 }
 plnid = cmd[1];

 iohtmlBodyInit( cnt, "Raze buildings on planet %d,%d:%d", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );

 cmd[0] = CMD_RAZE_BUILDINGS;
 cmd[1] = id;
 cmd[4] = plnid;
 for( a = c = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
 {
  if( !( razebldg[a] ) )
   continue;
  if( sscanf( razebldg[a], "%d", &b ) <= 0 )
   continue;
  if( b <= 0 )
   continue;
  cmd[2] = a;
  cmd[3] = b;
  b = cmdExecute( cnt, cmd, 0, 0 );
  if( b >= 0 )
  {
   httpPrintf( cnt, "<i>You razed %d %s</i><br><br>", cmd[3], cmdBuildingName[a] );
   c++;
  }
  else if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error while razing building</i><br>" );
 }
 if( portalstring )
 {
  cmd[0] = CMD_RAZE_PORTAL;
  cmd[1] = id;
  cmd[2] = plnid;
  b = cmdExecute( cnt, cmd, 0, 0 );
  if( b >= 0 )
  {
   httpPrintf( cnt, "<i>You razed the %s</i><br><br>", cmdBuildingName[CMD_BLDG_NUMUSED] );
   c++;
  }
  else if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error while razing portal</i><br><br>" );
 }
 if( c )
 {
  dbMapRetrievePlanet( plnid, &planetd );
  dbUserMainRetrieve( id, &maind );
 }
 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><input type=\"hidden\" value=\"%d\" name=\"id\">", URLAppend( cnt, "raze" ), plnid );
 totalb = planetd.construction;
 for( a = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
  totalb += planetd.building[a];
 if( totalb )
 {
  httpPrintf( cnt, "There are %d buildings on this planet", totalb );
  if( planetd.construction )
   httpPrintf( cnt, ", %d of them are still under construction", planetd.construction );
  httpString( cnt, ". " );
 }
 httpPrintf( cnt, "This planet can support %d buildings, additional ones will be very expensive.<br>", planetd.size );

 for( a = CMD_BLDG_OVERBUILD ; a < CMD_BLDG_NUMUSED ; a++ )
  totalb -= planetd.building[a];
 if( ( fa = cmdGetBuildOvercost( planetd.size, totalb ) ) != 1.0 )
  httpPrintf( cnt, "The overbuilding penalty for this planet is %.2f%%<br>", (fa-1.0)*100.0 );
 httpString( cnt, "<br><br><table width=\"320\" border=\"2\">" );
 for( a = b = 0 ; a < CMD_BLDG_NUMUSED ; a++ )
 {
  if( !( planetd.building[a] ) )
   continue;
  httpPrintf( cnt, "<tr><td align=\"center\"><b>%s</b><br><br>", cmdBuildingName[a] );
 httpPrintf( cnt, "<input id=\"disbandrange%d\" type=\"text\" name=\"bldg%d\" size=\"10\" value=\"0\" onchange=\"changeslider(this.value,\'disbandslider%d\')\"><br>", a, a, a );
httpPrintf( cnt, "<input id=\"disbandslider%d\" type=\"range\" min=\"0\" max=\"%d\" value=\"0\" onchange=\"changeslider(this.value,\'disbandrange%d\')\" />", a, planetd.building[a], a );
httpPrintf( cnt, "<br>There are %d on this planet<br>( %lld total )</td></tr>", planetd.building[a], (long long)maind.totalbuilding[a] );

  b = 1;
 }
 if( planetd.flags & CMD_PLANET_FLAGS_PORTAL )
 {
  httpPrintf( cnt, "<tr><td align=\"center\"><b>%s</b><br><br>", cmdBuildingName[CMD_BLDG_NUMUSED] );
  httpPrintf( cnt, "<input type=\"checkbox\" name=\"portal\"> Raze the portal<br><br>" );
  b = 1;
 }
 if( b )
  httpString( cnt, "</table><br><input type=\"submit\" value=\"Raze\"></form>" );
 else
  httpString( cnt, "</table><br>There are no buildings on this planet</form>" );
 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View planet</a>", URLAppend( cnt, "planet" ), plnid );
 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View system</a><br><br>", URLAppend( cnt, "system" ), planetd.system );

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_fleets( ReplyDataPtr cnt )
{
 int a, b, id, num, x, y;
 dbUserFleetPtr fleetd;
 dbMainPlanetDef planetd;
 dbUserMainDef maind, main2d;
 char varname[8];
 char *varstring;
 int szTotal[CMD_UNIT_NUMUSED];

memset(szTotal, 0, CMD_UNIT_NUMUSED*sizeof(int));
	iohtmlBase( cnt, 1 );
 if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 varstring = iohtmlVarsFind( cnt, "attack" );
 if( varstring )
 {
  if( ( sscanf( varstring, "%d", &a ) == 1 ) && ( (unsigned int)a < 3 ) )
   maind.config_fleet = a;
  varname[0] = 'f';
  varname[2] = 0;
  for( b = 0 ; b < 4 ; b++ )
  {
   varname[1] = '0' + b;
   varstring = iohtmlVarsFind( cnt, varname );
   if( ( varstring ) && ( sscanf( varstring, "%d", &a ) == 1 ) && ( (unsigned int)a < 10000 ) )
    maind.config_flee[b] = a;
  }
  dbUserMainSet( id, &maind );
 }
 iohtmlBodyInit( cnt, "Fleets" );

 if( varstring )
  httpString( cnt, "<i>Fleets orders changed</i><br><br>" );

 if( ( num = dbUserFleetList( id, &fleetd ) ) <= 0 )
 {
  httpString( cnt, "Error while retriving user fleets list" );
  iohtmlBodyEnd( cnt );
  return;
 }

 httpString( cnt, "<table width=\"100%\"><tr><td width=\"50%\" align=\"center\" valign=\"top\">" );
 httpString( cnt, "<b>Main fleet</b><br>" );
 httpString( cnt, "<table>" );

for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
	if( fleetd[0].unit[a] ) {
		httpPrintf( cnt, "<tr><td>%s</td><td>&nbsp;</td><td align=\"right\" id=\"funt%d\">%d</td></tr>", cmdUnitName[a], a, fleetd[0].unit[a] );
		b = 1;
	}
}
 httpString( cnt, "</table><br>" );
 httpPrintf( cnt, "<a href=\"%s&id=0\">Disband units</a>", URLAppend( cnt, "fleetdisband" ) );

 httpString( cnt, "</td><td width=\"50%\" align=\"center\" valign=\"top\">" );
 httpString( cnt, "<b>Fleets orders</b><br>" );

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><table cellspacing=\"8\">", URLAppend( cnt, "fleets" ) );
 httpString( cnt, "<tr><td align=\"center\">Order after an attack</td></tr><tr><td align=\"center\"><select name=\"attack\"><option value=\"0\"" );
 if( maind.config_fleet == 0 )
  httpString( cnt, " selected" );
 httpString( cnt, ">Station fleet on planet<option value=\"1\"" );
 if( maind.config_fleet == 1 )
  httpString( cnt, " selected" );
 httpString( cnt, ">Wait in system<option value=\"2\"" );
 if( maind.config_fleet == 2 )
  httpString( cnt, " selected" );
 httpString( cnt, ">Recall to main fleet</select></td></tr>" );

 httpString( cnt, "<tr><td align=\"center\">Approximations of enemy forces to flee battles<tr><td align=\"center\">" );

 httpString( cnt, "<table>" );
 httpPrintf( cnt, "<tr><td align=\"right\" nowrap>Long-range attack</td><td><input type=\"text\" name=\"f0\" size=\"4\" value=\"%d\"> %%</td></tr>", maind.config_flee[0] );
 httpPrintf( cnt, "<tr><td align=\"right\" nowrap>Air vs air</td><td><input type=\"text\" name=\"f1\" size=\"4\" value=\"%d\"> %%</td></tr>", maind.config_flee[1] );
 httpPrintf( cnt, "<tr><td align=\"right\" nowrap>Ground vs air</td><td><input type=\"text\" name=\"f2\" size=\"4\" value=\"%d\"> %%</td></tr>", maind.config_flee[2] );
 httpPrintf( cnt, "<tr><td align=\"right\" nowrap>Ground vs ground</td><td><input type=\"text\" name=\"f3\" size=\"4\" value=\"%d\"> %%</td></tr>", maind.config_flee[3] );
 httpString( cnt, "</table>" );

 httpString( cnt, "<tr><td align=\"center\"><input type=\"submit\" value=\"Change\"></td></tr></table></form>" );

 httpString( cnt, "</td></tr></table>" );

 httpString( cnt, "<br><b>Travelling fleets</b><br><br>" );
 if( num == 1 )
  httpString( cnt, "None<br>" );
 else
 {
  httpString( cnt, "<table width=\"90%%\" cellspacing=\"5\"><tr><td width=\"18%\">Units</td><td width=\"34%\">Order</td><td width=\"18%\">Status</td><td width=\"10%\">Position</td><td width=\"10%\">Changes</td></tr>" );
  for( a = 1 ; a < num ; a++ )
  {
   httpString( cnt, "<tr><td>" );
   for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ )
   {
    if( fleetd[a].unit[b] )
    {
     httpPrintf( cnt, "%d %s<br>", fleetd[a].unit[b], cmdUnitName[b] );
     szTotal[b] += fleetd[a].unit[b];
    }
   }
   httpString( cnt, "</td><td>" );
   if( fleetd[a].order == CMD_FLEET_ORDER_MOVE )
   {
    if( fleetd[a].sysid != -1 )
     httpPrintf( cnt, "%s <a href=\"%s&id=%d\">%d,%d</a></td>", cmdFleetOrderName[fleetd[a].order], URLAppend( cnt, "system" ), fleetd[a].sysid, ( fleetd[a].destination >> 8 ) & 0xFFF, fleetd[a].destination >> 20 );
    else
     httpPrintf( cnt, "%s %d,%d</td>", cmdFleetOrderName[fleetd[a].order], ( fleetd[a].destination >> 8 ) & 0xFFF, fleetd[a].destination >> 20 );
   }
   else if(fleetd[a].order == CMD_FLEET_ORDER_MERGE )
   {
   	httpPrintf(cnt, "%s at %d,%d</td>", cmdFleetOrderName[fleetd[a].order], ( fleetd[a].destination >> 8 ) & 0xFFF, fleetd[a].destination >> 20);
   }
   else if( fleetd[a].order == CMD_FLEET_ORDER_CANCELED )
    httpPrintf( cnt, "%s</td>", cmdFleetOrderName[fleetd[a].order] );
   else if( fleetd[a].order == CMD_FLEET_ORDER_RECALL )
    httpPrintf( cnt, "%s at <a href=\"%s&id=%d\">%d,%d:%d</a></td>", cmdFleetOrderName[fleetd[a].order], URLAppend( cnt, "planet" ), fleetd[a].destid, ( fleetd[a].destination >> 8 ) & 0xFFF, fleetd[a].destination >> 20, fleetd[a].destination & 0xFF );
   else
   {
    if( dbMapRetrievePlanet( fleetd[a].destid, &planetd ) < 0 ) {
     	httpPrintf( cnt, "%s %d,%d</td>", cmdFleetOrderName[fleetd[a].order], ( fleetd[a].destination >> 8 ) & 0xFFF, fleetd[a].destination >> 20 );
    } else if( ( planetd.owner != -1 ) && ( dbUserMainRetrieve( planetd.owner, &main2d ) >= 0 ) ) {
     	httpPrintf( cnt, "%s <a href=\"%s&id=%d\">%d,%d:%d</a> owned by ", cmdFleetOrderName[fleetd[a].order], URLAppend( cnt, "planet" ), fleetd[a].destid, ( fleetd[a].destination >> 8 ) & 0xFFF, fleetd[a].destination >> 20, fleetd[a].destination & 0xFF );
     	httpPrintf( cnt, "<a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), planetd.owner, main2d.faction );
     	httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a></td>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire );
     } else {
     		httpPrintf( cnt, "%s <a href=\"%s&id=%d\">%d,%d:%d</a>, uninhabited</td>", cmdFleetOrderName[fleetd[a].order], URLAppend( cnt, "planet" ), fleetd[a].destid, ( fleetd[a].destination >> 8 ) & 0xFFF, fleetd[a].destination >> 20, fleetd[a].destination & 0xFF );
	}
   }
   if( fleetd[a].time != -1 )
    httpPrintf( cnt, "<td>Moving, %d weeks</td>", fleetd[a].time );
   else if(( fleetd[a].order == CMD_FLEET_ORDER_MOVE ) || (fleetd[a].order == CMD_FLEET_ORDER_MERGE))
    httpPrintf( cnt, "<td>Waiting</td>" );
   else if( fleetd[a].order == CMD_FLEET_ORDER_CANCELED )
    httpPrintf( cnt, "<td>Order canceled</td>" );
   else
    httpPrintf( cnt, "<td><a href=\"fleetattack?id=%d\">Ready to attack</a></td>", a );
   cmdFleetGetPosition( &fleetd[a], &x, &y );
   httpPrintf( cnt, "<td>%d,%d</td>", x, y );
	if((fleetd[a].unit[2]>0)||(fleetd[a].unit[3]>0)||(fleetd[a].unit[4]>0)||(fleetd[a].unit[8]>0)) {
   		httpPrintf( cnt, "<td><a href=\"%s&id=%d\">Details</a></td><td>", URLAppend( cnt, "fleetchange" ), a );
   		httpPrintf( cnt, "<a href=\"%s&fleetid=%d\">Split</a></a></td><td>", URLAppend( cnt, "fleetssplit" ), a );
   		httpPrintf( cnt, "<a href=\"%s&fltid=%d\">Merge</a></td></tr>", URLAppend( cnt, "fleetsmerge" ), a );
	} else {
		httpPrintf( cnt, "<td><a href=\"%s&id=%d\">Details</a></td></tr>", URLAppend( cnt, "fleetchange" ), a);
  	}
  }

  httpString( cnt, "</table>" );
 }
  if( num > 1 )
 httpString( cnt, "<br><b>Total traveling fleets</b><br><br>" );
  for(b=0;b < CMD_UNIT_NUMUSED ; b++ )
  {
  	if( szTotal[b] )
     httpPrintf( cnt, "%d %s<br>", szTotal[b], cmdUnitName[b] );
  }

 httpPrintf( cnt, "<br><br><b>Dispatch a new fleet</b><br><br><form action=\"%s\" method=\"POST\">", URLAppend( cnt, "fleetsend" ) );
 httpPrintf( cnt, "<select name=\"order\"><option value=\"%d\">%s<option value=\"%d\">%s<option value=\"%d\">%s</select>", CMD_FLEET_ORDER_ATTACK, cmdFleetOrderName[CMD_FLEET_ORDER_ATTACK], CMD_FLEET_ORDER_STATION, cmdFleetOrderName[CMD_FLEET_ORDER_STATION], CMD_FLEET_ORDER_MOVE, cmdFleetOrderName[CMD_FLEET_ORDER_MOVE] );
 httpString( cnt, "<table cellspacing=\"8\">" );
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  if( fleetd[0].unit[a] )
   httpPrintf( cnt, "<tr><td align=\"right\" id=\"dunt%d\">%d</td><td align=\"right\">%s</td><td><input type=\"text\" name=\"u%d\" size=\"12\"></td></tr>", a, fleetd[0].unit[a], cmdUnitName[a], a );
 }
 httpString( cnt, "</table>X:<input type=\"text\" name=\"X\" size=\"4\"> Y:<input type=\"text\" name=\"Y\" size=\"4\"> Planet:<input type=\"text\" name=\"Z\" size=\"4\"><br><br><input type=\"submit\" value=\"Send\"></form>" );

 free( fleetd );
 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_fleetsend( ReplyDataPtr cnt )
{
 int a, id, order, x, y, z;
 dbUserMainDef maind;
 int sendunit[CMD_UNIT_NUMUSED];
 char sendname[CMD_UNIT_NUMUSED];
 char *orderstring, *xstring, *ystring, *zstring, *sptr;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Sending fleet" );

 memset( sendunit, 0, CMD_UNIT_NUMUSED*sizeof(int) );
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  sprintf( sendname, "u%d", a );
  sptr = iohtmlVarsFind( cnt, sendname );
  if( !( sptr ) || ( sscanf( sptr, "%d", &sendunit[a] ) != 1 ) )
   sendunit[a] = 0;
 }
 orderstring = iohtmlVarsFind( cnt, "order" );
 xstring = iohtmlVarsFind( cnt, "X" );
 ystring = iohtmlVarsFind( cnt, "Y" );
 zstring = iohtmlVarsFind( cnt, "Z" );

 if( !( orderstring ) || ( sscanf( orderstring, "%d", &order ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( xstring ) || ( sscanf( xstring, "%d", &x ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( ystring ) || ( sscanf( ystring, "%d", &y ) != 1 ) )
 {
  iohttpFunc_fleetsendL0:
  httpString( cnt, "<i>Invalid order or coordinates</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }
 z = 0;
 if( zstring )
  sscanf( zstring, "%d", &z );

 if( ( a = cmdExecSendFleet( id, x, y, z, order, sendunit ) ) < 0 )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error encountered when sending fleet</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 httpString( cnt, "<i>Fleet sent!</i><br><br>" );
 if( a )
  httpPrintf( cnt, "The fleet will reach its destination in %d weeks<br>", a );
 else
  httpString( cnt, "The fleet reached its destination<br>" );

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_fleetssplit( ReplyDataPtr cnt )
{
	int id, i, nId, nError;
	dbUserMainDef maind;
	dbUserFleetDef fleetd, fleet2d;
	int64_t disunit[CMD_UNIT_NUMUSED];
	char disname[10];
	char *varstring;

	iohtmlBase( cnt, 1 );
 if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

	iohtmlBodyInit( cnt, "Splitting fleet" );

 varstring = iohtmlVarsFind( cnt, "fleetid" );
 if( varstring )
 {
 	//Getting info on the fleet to split
 	nId = atoi(varstring);
 	for(i=0;i<CMD_UNIT_NUMUSED;i++)
 	{
 		sprintf(disname, "u%d", i);
 		varstring = iohtmlVarsFind( cnt, disname );
 		if(varstring)
 			disunit[i] = strtoll(varstring, NULL, 10);
 		else
 			disunit[i] = 0;
 		if(disunit[i] < 0)
 			disunit[i] = 0;
 	}
 	//We have everything now we check if we can split that
 	if(dbUserFleetRetrieve( id, nId, &fleetd ) > 0)
 	{
 		nError = 0;
 		for(i=0;i<CMD_UNIT_NUMUSED;i++)
 		{
 			if(fleetd.unit[i] < disunit[i])
 				nError = 1;
 		}
 		//Not enough transport remain
 		if ((fleetd.unit[2] - disunit[2] )*100< ((fleetd.unit[5] - disunit[5]) + (fleetd.unit[6] - disunit[6]) + ((fleetd.unit[7] - disunit[7])*4)) )
 		{
 			nError = 2;
 		}
 		else if(disunit[2]*100 < (disunit[5] + disunit[6] + disunit[7]*4))
 		{
 			nError = 3;
 		}
 		else if((fleetd.unit[4]-disunit[4])*100 < (fleetd.unit[0]-disunit[0] + fleetd.unit[1]-disunit[1] + fleetd.unit[2]-disunit[2]))
 		{
 			nError = 4;
 		}
 		else if((disunit[4]*100) < (disunit[0] + disunit[1] + disunit[2]))
 		{
 			nError = 5;
 		}
 		else if((disunit[0] == 0)&&(disunit[1] == 0)&&(disunit[2] == 0)&&(disunit[3] == 0)&&(disunit[4] == 0)&&(disunit[5] == 0)&&(disunit[6] == 0)&&(disunit[7] == 0)&&(disunit[8] == 0))
 			nError = 6;  //In fact its not an error its just split fleet but no fleet is selected
 		else if((fleetd.unit[0]-disunit[0] == 0)&&(fleetd.unit[1]-disunit[1] == 0)&&(fleetd.unit[2]-disunit[2] == 0)&&(fleetd.unit[3]-disunit[3] == 0)&&(fleetd.unit[4]-disunit[4] == 0)&&(fleetd.unit[5]-disunit[5] == 0)&&(fleetd.unit[6]-disunit[6] == 0)&&(fleetd.unit[7]-disunit[7] == 0)&&(fleetd.unit[8]-disunit[8] == 0))
 			nError = 6;  //In fact its not an error its just split fleet but no fleet is selected

 		if(nError == 1)
 			httpString( cnt, "<i>Error while splitting fleet</i><br>You can split more than what you have!" );
 		else if(nError == 2)
 			httpString( cnt, "<i>Error while splitting fleet</i><br>You must leave enough transports in your Main Fleet<br>Each transport can hold 100 soldiers or droids, or 25 goliaths." );
 		else if(nError == 3)
 			httpString( cnt, "<i>Error while splitting fleet</i><br>You don't have enough transports in your split fleet<br>Each transport can hold 100 soldiers or droids, or 25 goliaths." );
 		else if(nError == 4)
 			httpString( cnt, "<i>Error while splitting fleet</i><br>You don't leave enough carriers in your Main Fleet<br>Each carrier can hold 100 fighters, bombers or transports." );
 		else if(nError == 5)
 			httpString( cnt, "<i>Error while splitting fleet</i><br>You don't have enough carriers in your split fleet<br>Each carrier can hold 100 fighters, bombers or transports." );
 		else if(nError == 0) //time to split some fleet
 		{
 			//make the same fleet (same order and dest) but not the same unit amount
 			memcpy(&fleet2d, &fleetd, sizeof(dbUserFleetDef));
 			for(i=0;i<CMD_UNIT_NUMUSED;i++)
 			{
 				fleetd.unit[i] -= disunit[i];
 				fleet2d.unit[i] = disunit[i];
 			}
 			dbUserFleetAdd(id, &fleet2d);
 			dbUserFleetSet(id, nId, &fleetd );
 			httpString(cnt, "<i>Splitting successful</i>");
 		}
 		//Menu go over here
 		//Some JavaScript to do cute things
 		httpString(cnt, "<SCRIPT language=\"JavaScript\"><!-- // Hiding for no JavaScript\n function calcul()\n{\n");
 		for(i=0;i<CMD_UNIT_NUMUSED;i++)
 		{
 			if(fleetd.unit[i])
 				httpPrintf(cnt, "if(isNaN(document.forms[\"fleet\"].u%d.value)){document.forms[\"fleet\"].u%d.value = 0}", i, i);
 			if(fleetd.unit[i])
 				httpPrintf(cnt, "document.forms[\"fleet\"].unit%d.value = document.forms[\"fleet\"].uh%d.value-document.forms[\"fleet\"].u%d.value\n", i, i, i);
 		}
 		httpString(cnt, "}");

 		httpString(cnt, "\nfunction check()\n{\nif(document.forms[\"fleet\"].cpct.checked==true){document.forms[\"fleet\"].pct.readOnly=false\n}\nelse{document.forms[\"fleet\"].pct.readOnly=true}\ndocument.forms[\"fleet\"].pct.value=\"\" \n}");

 		httpString(cnt, "\nfunction pctchange()\n{\nif(document.forms[\"fleet\"].pct.value > 100){document.forms[\"fleet\"].pct.value = 100}\n");
 		for(i=0;i<CMD_UNIT_NUMUSED;i++)
 		{
 			if(fleetd.unit[i])
 				httpPrintf(cnt, "document.forms[\"fleet\"].u%d.value = Math.round((document.forms[\"fleet\"].uh%d.value*document.forms[\"fleet\"].pct.value)/100)\n", i, i);
 		}
 		httpString(cnt, "calcul()\n}\n");
 		httpString(cnt, "\n//--> // done hiding</SCRIPT>");
 		httpPrintf(cnt, "<form action=\"%s\" method=\"POST\" name=\"fleet\">", URLAppend( cnt, "fleetssplit" ) );
 		httpPrintf(cnt, "<input type=\"hidden\" name=\"fleetid\" value=\"%d\"><br><table>", nId);
 		httpString(cnt, "<tr align=\"center\"><b><td></td><td>Main Fleet</td><td></td><td>Split fleet</td></></tr>");
 		for(i=0;i<CMD_UNIT_NUMUSED;i++)
 		{
	 		if(fleetd.unit[i])
	 		{
	 			httpPrintf(cnt, "<tr><td>%s </td><td><input type=\"hidden\" name=\"uh%d\" value=\"%d\"><input type=\"text\" name=\"unit%d\" value=\"%d\" READONLY</td>", cmdUnitName[i],i, fleetd.unit[i], i, fleetd.unit[i]);
	 			httpPrintf(cnt, "<td>%s </td><td><input type=\"text\" name=\"u%d\" onKeyUp=\"calcul();\"></td></tr>", cmdUnitName[i], i);
	 		}
	 	}
 		httpString(cnt, "</table><br><input type=\"submit\" value=\"Split\"> Use % of fleet<input type=\"CHECKBOX\" name=\"cpct\" onClick=\"check()\"> <input type=\"text\" name=\"pct\" READONLY onKeyUp=\"pctchange()\">");
 		httpString(cnt, "</form>");
 	}
 	else
 		httpString( cnt, "<i>Error while splitting fleet</i><br>Fleet not found" );
 }
 iohtmlBodyEnd( cnt );
}

void iohtmlFunc_fleetsmerge( ReplyDataPtr cnt)
{
	int i, j, nError, id ,nId, nMax, nX0, nY0, nX1, nY1, fltid;
	int nNbrFleet = 0;
	char mergename[8];
	char *var;
	dbUserMainDef maind;
	int nfltid[2];
	dbUserFleetPtr pFleet;

nError = nX0 = 0;

iohtmlBase(cnt, 1);

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
	return;
if( !( iohtmlHeader( cnt, id, &maind ) ) )
	return;

iohtmlBodyInit(cnt, "Merging fleets");
nMax = dbUserFleetList(id, &pFleet);

 var = iohtmlVarsFind( cnt, "fltid" );
 if(var)
 	fltid = atoi(var);
 else
 	fltid = -1;
 nError = 0;
 //Read all var here
 for(i=0;i<nMax;i++)
 {
 	sprintf(mergename, "f%d", i);
 	var = iohtmlVarsFind( cnt, mergename );
 	if(var)
 	{
 		nId = atoi(var);
 		if((nId<1)||(nId > (nMax -1)))
 		{
 			nError = 2;
 			continue;
 		}
 		if((pFleet[nId].unit[2]>0)||(pFleet[nId].unit[3]>0)||(pFleet[nId].unit[4]>0)||(pFleet[nId].unit[8]>0))
 		{
 			nfltid[nNbrFleet] = nId;
 			nNbrFleet++;
 		}
 		else
 			nError = 1;
 	}
 	if(nNbrFleet == 2)
 		break;
 }

 if(nfltid[0] == nfltid[1])
 	nNbrFleet = 1;
 if((nNbrFleet == 1)&&(nError == 0))
 	httpString(cnt, "<i>Error while merging</i><br>Only one fleet is selected<br>");
 if(nNbrFleet == 2)
 {
 	cmdFleetGetPosition( &pFleet[nfltid[0]], &nX0, &nY0 );
 	cmdFleetGetPosition( &pFleet[nfltid[1]], &nX1, &nY1 );
 	nX0 = (nX1 + nX0)/2;
 	nY0 = (nY1 + nY0)/2;
 	//mean it was merging with another fleet
 	for(i=0;i<nNbrFleet;i++)
 	{
	 	if(pFleet[nfltid[i]].order == CMD_FLEET_ORDER_MERGE)
	 	{
	 		//We find the other one and cancel his order to merge else he will wait way too long :D
	 		for(j=1;j<nMax;j++)
	 		{
	 			if(j == nfltid[i])
	 				continue;
	 			if((pFleet[j].order == CMD_FLEET_ORDER_MERGE)&&(pFleet[j].destid==pFleet[nfltid[i]].destid))
	 			{
	 				pFleet[j].order = CMD_FLEET_ORDER_CANCELED;
	 				pFleet[j].time = -1;
	 				dbUserFleetSet(id, j, &pFleet[j]);
	 			}
	 		}
	 	}
	 }
 	nX1 = cmdExecChangeFleet(id, nX0, nY0, 0, CMD_FLEET_ORDER_MERGE, nfltid[0]);
 	nX0 = cmdExecChangeFleet(id, nX0, nY0, 0, CMD_FLEET_ORDER_MERGE, nfltid[1]);
 	httpString(cnt, "<i>Merge sucessful</i><br>");
 	if(nX1 > nX0)
 		nX0 = nX1;
 	httpPrintf(cnt, "They will merge in %d weeks<br>", nX0);
 } else if(nError == 2) {
 	httpString(cnt, "<i>Error while merging</i><br>Your fleet must exist before getting merge!!!<br>");
 } else if(nError == 1) {
 	httpString(cnt, "<i>Error while merging</i><br>Your fleet must have at least one transport, carrier, cruiser or phantoms to be merge<br>");
 }
 if( (fltid >= 1) && (nX0 == 0) && (nMax > 2) ){
 	httpPrintf(cnt, "<br><table><tr>Fleet %d</tr>", fltid);
 	for(i=0;i<CMD_UNIT_NUMUSED;i++)
 	{
 		if(pFleet[fltid].unit[i])
 			httpPrintf(cnt, "<tr><td>%s</td><td>%d</td></tr>", cmdUnitName[i], pFleet[fltid].unit[i]);
 	}
 	httpString(cnt, "</table><br><br>");

 	httpPrintf(cnt, "<form action=\"%s\" method=\"POST\"><table>", URLAppend( cnt, "fleetsmerge" ) );
 	httpPrintf(cnt, "<input name=\"fltid\" type=\"hidden\" value=\"%d\"><input name=\"f0\" type=\"hidden\" value=\"%d\"><tr>", fltid, fltid);

 	for(i=1;i<nMax;i++)
 	{
 		if(i == fltid)		//Skip it if the same
 			continue;
 		if((pFleet[i].unit[2]==0)&&(pFleet[i].unit[3]==0)&&(pFleet[i].unit[4]==0)&&(pFleet[i].unit[8]==0))
 			continue;
 		httpPrintf(cnt, "<td><input type=\"radio\" name=\"f1\" value=\"%d\" CHECKED >Fleet %d</input><table>", i, i);
 		for(j=0;j<CMD_UNIT_NUMUSED;j++)
 		{
 			if(pFleet[i].unit[j] != 0)
 				httpPrintf(cnt, "<tr><td>%s</td><td>%d</td></tr>", cmdUnitName[j], pFleet[i].unit[j]);
 		}

 		httpString(cnt, "</table></td>");
 	}
 	httpPrintf(cnt, "</tr><tr>");
 	httpString(cnt, "</table>");
 	httpPrintf(cnt, "<input type=\"submit\" value=\"Merge\">");
 	httpString(cnt, "</form>");
 } else if (fltid < 1) {
 	httpString(cnt, "<i>Error while merging</i><br><br>You haven't selected any fleet!<br>");
} else if (nMax < 3) {
	httpString(cnt, "<i>Not enough fleets to merge!</i><br><br>You only have the one fleet, you can't exactly merge!<br>Perhaps you want to recall it instead?<br>");
}

 httpPrintf(cnt, "<br><a href=\"%s\">Return to fleets</a><br>", URLAppend( cnt, "fleets" ) );
 free(pFleet);
 iohtmlBodyEnd( cnt );
}

void iohtmlFunc_fleetdisband( ReplyDataPtr cnt )
{
 int a, b, c, id, cmd[6], fltid;
 dbUserFleetDef fleetd;
 dbUserMainDef maind;
 int64_t disunit[CMD_UNIT_NUMUSED];
 char disname[8];
 char *sptr, *fleetstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Disband units" );

 memset( disunit, 0, CMD_UNIT_NUMUSED*sizeof(int64_t) );
 for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
 {
  sprintf( disname, "dis%d", a );
  sptr = iohtmlVarsFind( cnt, disname );
  if( !( sptr ) || ( sscanf( sptr, "%" SCNd64, &disunit[a] ) != 1 ) )
   disunit[a] = 0;
 }
 fleetstring = iohtmlVarsFind( cnt, "id" );

 cmd[0] = CMD_RETRIEVE_FLEET;
 cmd[1] = id;
 if( !( fleetstring ) || ( sscanf( fleetstring, "%d", &cmd[2] ) <= 0 ) || ( cmdExecute( cnt, cmd, &fleetd, 0 ) < 0 ) )
 {
  httpString( cnt, "This fleet doesn't exist!</body></html>" );
  return;
 }
 fltid = cmd[2];

 cmd[0] = CMD_DISBAND_UNITS;
 cmd[1] = id;
 cmd[4] = fltid;
 for( a = c = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
 {
  if( disunit[a] <= 0 )
   continue;
  cmd[2] = a;
  cmd[3] = disunit[a];
  b = cmdExecute( cnt, cmd, 0, 0 );
  if( b >= 0 )
  {
   httpPrintf( cnt, "<i>You disbanded %d %s</i><br><br>", cmd[3], cmdUnitName[a] );
   c++;
  }
  else if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error while disbanding units</i><br>" );
 }
 if( c )
 {
  cmd[0] = CMD_RETRIEVE_FLEET;
  cmd[1] = id;
  cmd[2] = fltid;
  cmdExecute( cnt, cmd, &fleetd, 0 );
  cmd[0] = CMD_RETRIEVE_USERMAIN;
  cmd[1] = id;
  cmdExecute( cnt, cmd, &maind, 0 );
 }

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><input type=\"hidden\" value=\"%d\" name=\"id\">", URLAppend( cnt, "fleetdisband" ), fltid );
 httpString( cnt, "<table width=\"320\" border=\"2\">" );
 for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
 {
  if( !( fleetd.unit[a] ) )
   continue;
  httpPrintf( cnt, "<tr><td align=\"center\"><b>%s</b><br><br>", cmdUnitName[a] );
 httpPrintf( cnt, "<input id=\"disbandrange%d\" type=\"text\" name=\"dis%d\" size=\"10\" value=\"0\" onchange=\"changeslider(this.value,\'disbandslider%d\')\"><br>", a, a, a );
httpPrintf( cnt, "<input id=\"disbandslider%d\" type=\"range\" min=\"0\" max=\"%d\" value=\"0\" onchange=\"changeslider(this.value,\'disbandrange%d\')\" />", a, fleetd.unit[a], a );
httpPrintf( cnt, "<br>There are %d in this fleet<br>( %lld total )</td></tr>", fleetd.unit[a], (long long)maind.totalunit[a] );
 }
 httpString( cnt, "</table><br><input type=\"submit\" value=\"Disband\"></form>" );

 iohtmlBodyEnd( cnt );
 return;
}



void iohtmlFunc_fleetchange( ReplyDataPtr cnt )
{
 int a, id, i, fltid, order, x, y, z;
 dbUserFleetDef fleetd;
 dbUserFleetPtr fleetp;
 dbUserMainDef maind;
 char *fleetstring, *orderstring, *xstring, *ystring, *zstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Viewing fleet" );

 fleetstring = iohtmlVarsFind( cnt, "id" );
 orderstring = iohtmlVarsFind( cnt, "order" );
 xstring = iohtmlVarsFind( cnt, "X" );
 ystring = iohtmlVarsFind( cnt, "Y" );
 zstring = iohtmlVarsFind( cnt, "Z" );

 if( !( fleetstring ) || ( sscanf( fleetstring, "%d", &fltid ) <= 0 ) || ( dbUserFleetRetrieve( id, fltid, &fleetd ) < 0 ) )
 {
  httpString( cnt, "This fleet doesn't exist!</body></html>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 order = -1;
 if( ( orderstring ) && ( sscanf( orderstring, "%d", &order ) == 1 ) )
 {
  x = y = z = 0;
  if( xstring )
   sscanf( xstring, "%d", &x );
  if( ystring )
   sscanf( ystring, "%d", &y );
  if( zstring )
   sscanf( zstring, "%d", &z );
  if (dbUserFleetRetrieve(id, fltid, &fleetd) > 0)
  {
  	if(fleetd.order == CMD_FLEET_ORDER_MERGE)
  	{
  		//if you cancel this one you also need to cancel the one that will merge with it
  		a = dbUserFleetList(id, &fleetp);
  		for(i=1;i<a;i++)
  		{
  			if((fleetp[i].order==CMD_FLEET_ORDER_MERGE)&&(fleetp[i].destid == fleetd.destid)&&(i != fltid))
  			{
  				fleetp[i].time = -1;
  				fleetp[i].order = CMD_FLEET_ORDER_CANCELED;
  				dbUserFleetSet(id, i, &fleetp[i]);
  			}
  		}
  		free(fleetp);
  	}
  }
  if( cmdExecChangeFleet( id, x, y, z, order, fltid ) < 0 )
  {
   if( cmdErrorString )
    httpPrintf( cnt, "<i>%s</i>", cmdErrorString );
   else
    httpString( cnt, "<i>Error encountered when changing fleet orders</i>" );
   iohtmlBodyEnd( cnt );
   return;
  }
  else
  {
   httpString( cnt, "<i>Order changed</i><br><br>" );
   dbUserFleetRetrieve( id, fltid, &fleetd );
  }
 }

 httpString( cnt, "<b>Fleet units</b><br><br>" );
 for( a = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
 {
  if( fleetd.unit[a] )
   httpPrintf( cnt, "%d %s<br>", fleetd.unit[a], cmdUnitName[a] );
 }

 cmdFleetGetPosition( &fleetd, &x, &y );
 httpPrintf( cnt, "<br><b>Current position</b> : %d,%d", x, y );

 httpString( cnt, "<br><b>Order</b> : " );
 if( fleetd.order == CMD_FLEET_ORDER_MOVE )
 {
  if( fleetd.sysid != -1 )
   httpPrintf( cnt, "%s <a href=\"%s&id=%d\">%d,%d</a>", cmdFleetOrderName[fleetd.order], URLAppend( cnt, "system" ), fleetd.sysid, ( fleetd.destination >> 8 ) & 0xFFF, fleetd.destination >> 20 );
  else
   httpPrintf( cnt, "%s %d,%d", cmdFleetOrderName[fleetd.order], ( fleetd.destination >> 8 ) & 0xFFF, fleetd.destination >> 20 );
 }
 else if( fleetd.order == CMD_FLEET_ORDER_CANCELED )
  httpPrintf( cnt, "%s", cmdFleetOrderName[fleetd.order] );
 else
  httpPrintf( cnt, "%s <a href=\"%s&id=%d\">%d,%d:%d</a>", cmdFleetOrderName[fleetd.order], URLAppend( cnt, "planet" ), fleetd.destid, ( fleetd.destination >> 8 ) & 0xFFF, fleetd.destination >> 20, fleetd.destination & 0xFF );

 httpString( cnt, "<br><b>Status</b> : " );
 if( fleetd.time != -1 )
  httpPrintf( cnt, "Moving, %d weeks", fleetd.time );
 else if( fleetd.order == CMD_FLEET_ORDER_MOVE )
  httpPrintf( cnt, "Waiting" );
 else if( fleetd.order == CMD_FLEET_ORDER_CANCELED )
  httpPrintf( cnt, "Order canceled" );
 else
  httpPrintf( cnt, "Ready to attack" );

 httpPrintf( cnt, "<br><br><b>Change order</b><br><form action=\"%s\" method=\"POST\"><input type=\"hidden\" name=\"id\" value=\"%d\">", URLAppend( cnt, "fleetchange" ), fltid );
 if( fleetd.unit[CMD_UNIT_EXPLORATION] )
  httpPrintf( cnt, "<select name=\"order\"><option value=\"%d\">%s<option value=\"%d\">%s<option value=\"%d\">%s</select>", CMD_FLEET_ORDER_EXPLORE, cmdFleetOrderName[CMD_FLEET_ORDER_EXPLORE], CMD_FLEET_ORDER_MOVE, cmdFleetOrderName[CMD_FLEET_ORDER_MOVE], CMD_FLEET_ORDER_RECALL, cmdFleetOrderName[CMD_FLEET_ORDER_RECALL] );
 else if( fleetd.unit[CMD_UNIT_AGENT] || fleetd.unit[CMD_UNIT_GHOST] )
  httpPrintf( cnt, "<select name=\"order\"><option value=\"%d\">%s</select>", CMD_FLEET_ORDER_RECALL, cmdFleetOrderName[CMD_FLEET_ORDER_RECALL] );
 else
  httpPrintf( cnt, "<select name=\"order\"><option value=\"%d\">%s<option value=\"%d\">%s<option value=\"%d\">%s<option value=\"%d\">%s</select>", CMD_FLEET_ORDER_ATTACK, cmdFleetOrderName[CMD_FLEET_ORDER_ATTACK], CMD_FLEET_ORDER_STATION, cmdFleetOrderName[CMD_FLEET_ORDER_STATION], CMD_FLEET_ORDER_MOVE, cmdFleetOrderName[CMD_FLEET_ORDER_MOVE], CMD_FLEET_ORDER_RECALL, cmdFleetOrderName[CMD_FLEET_ORDER_RECALL] );
 httpPrintf( cnt, "<br><br>X:<input type=\"text\" name=\"X\" size=\"4\" value=\"%d\"> Y:<input type=\"text\" name=\"Y\" size=\"4\" value=\"%d\"> Planet:<input type=\"text\" name=\"Z\" size=\"4\" value=\"%d\"><br><br><input type=\"submit\" value=\"Change order\"></form>", ( fleetd.destination >> 8 ) & 0xFFF, fleetd.destination >> 20, fleetd.destination & 0xFF );
	if((fleetd.unit[2]>0)||(fleetd.unit[3]>0)||(fleetd.unit[4]>0)||(fleetd.unit[8]>0)) {
		httpPrintf(cnt, "<br><a href=\"%s&fleetid=%d\">Split</a><br>" , URLAppend( cnt, "fleetssplit" ), fltid );
		httpPrintf(cnt, "<a href=\"%s&fltid=%d\">Merge</a>", URLAppend( cnt, "fleetsmerge" ), fltid );
	}

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlAttackReport( ReplyDataPtr cnt, int *results, int sats )
{
 int a;
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  if( results[4+0*CMD_UNIT_FLEET+a] )
   goto iohtmlAttackReportL0;
 }
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  if( results[4+1*CMD_UNIT_FLEET+a] )
   goto iohtmlAttackReportL0;
 }
 httpString( cnt, "<br>" );
 return;
 iohtmlAttackReportL0:
 httpString( cnt, "<table width=\"60%\"><tr><td width=\"30%\" align=\"center\" valign=\"top\"><i>Attacker losses</i><br>" );
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  if( results[4+0*CMD_UNIT_FLEET+a] )
   httpPrintf( cnt, "%d %s<br>", results[4+0*CMD_UNIT_FLEET+a], cmdUnitName[a] );
 }
 httpString( cnt, "</td><td width=\"30%\" align=\"center\" valign=\"top\"><i>Defender losses</i><br>" );
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  if( results[4+1*CMD_UNIT_FLEET+a] )
   httpPrintf( cnt, "%d %s<br>", results[4+1*CMD_UNIT_FLEET+a], cmdUnitName[a] );
 }
 if( sats )
  httpPrintf( cnt, "%d %s<br>", sats, cmdBuildingName[CMD_BUILDING_SATS] );
 httpString( cnt, "</tr></table>" );
 return;
}


void iohtmlFunc_fleetattack( ReplyDataPtr cnt )
{
 int id, cmd[3];
 int results[4+8*CMD_UNIT_FLEET+2];
 dbUserMainDef maind, main2d;
 char *fleetstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Battle report" );

 fleetstring = iohtmlVarsFind( cnt, "id" );

 cmd[0] = CMD_ATTACK_PLANET;
 cmd[1] = id;
 if( !( fleetstring ) || ( sscanf( fleetstring, "%d", &cmd[2] ) <= 0 ) || ( battle( cmd[1], cmd[2], results ) < 0 ) )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i>", cmdErrorString );
  else
   httpString( cnt, "<i>Error encountered when attacking</i>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 if( cmdErrorString )
  httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );

 results[2] += ( rand() % 17 ) - 8;
 if( results[2] < 0 )
  results[2] = 0;
 if( results[2] > 100 )
  results[2] = 100;
 dbUserMainRetrieve( results[0], &main2d );
 httpPrintf( cnt, "Attacking <a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), results[0], main2d.faction );
 httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a><br>Estimated defender portal coverage : %d%%<br>", URLAppend( cnt, "empire" ), results[1], results[1], results[2] );

 httpPrintf( cnt, "<br><b>Phase 1 - Defending cruisers engage approaching cruisers and carriers, carrying all other vessels, at long range.</b><br>" );
 if( ( results[3] & 0x10 ) )
  httpString( cnt, "Overwhelmed by defending enemy forces, your fleet hastidly retreated to minimize losses<br>" );
 else if( results[3] & 0x100 )
  httpString( cnt, "Your fleet didn't encounter much resistance, it seems enemy forces preferred not to directly engage us<br>" );
 iohtmlAttackReport( cnt, &results[0*CMD_UNIT_FLEET], results[4+8*CMD_UNIT_FLEET+0] );
 if( ( results[3] & 0x10 ) )
  goto iohttpFunc_fleetattackL0;

 httpPrintf( cnt, "<br><b>Phase 2 - Defending fighters and cruisers intercepts enemy vessels, attempting to destroy transports before they land.</b><br>" );
 if( ( results[3] & 0x20 ) )
  httpString( cnt, "Overwhelmed by defending enemy forces, your fleet hastidly retreated to minimize losses<br>" );
 else if( results[3] & 0x200 )
  httpString( cnt, "Your fleet didn't encounter much resistance, it seems enemy forces preferred not to directly engage us<br>" );
 iohtmlAttackReport( cnt, &results[2*CMD_UNIT_FLEET], results[4+8*CMD_UNIT_FLEET+1] );
 if( ( results[3] & 0x20 ) )
  goto iohttpFunc_fleetattackL0;

 httpPrintf( cnt, "<br><b>Phase 3 - Ground units guarding the planet with anti-air capabilities fire at the landing transports, protected by bombers and cruisers.</b><br>" );
 if( ( results[3] & 0x40 ) )
  httpString( cnt, "Overwhelmed by defending enemy forces, your fleet hastidly retreated to minimize losses<br>" );
 else if( results[3] & 0x400 )
  httpString( cnt, "Your fleet didn't encounter much resistance, it seems enemy forces preferred not to directly engage us<br>" );
 iohtmlAttackReport( cnt, &results[4*CMD_UNIT_FLEET], 0 );
 if( ( results[3] & 0x40 ) )
  goto iohttpFunc_fleetattackL0;

 httpPrintf( cnt, "<br><b>Phase 4 - Attacking ground forces, with support from bombers and cruisers, attempting to take control of the planet.</b><br>" );
 if( ( results[3] & 0x80 ) )
  httpString( cnt, "Overwhelmed by defending enemy forces, your fleet hastidly retreated to minimize losses<br>" );
 else if( results[3] & 0x800 )
  httpString( cnt, "Your fleet didn't encounter much resistance, it seems enemy forces preferred not to directly engage us<br>" );
 iohtmlAttackReport( cnt, &results[6*CMD_UNIT_FLEET], 0 );
 if( ( results[3] & 0x80 ) )
  goto iohttpFunc_fleetattackL0;


 iohttpFunc_fleetattackL0:

 if( ( results[3] & 1 ) )
  httpPrintf( cnt, "<br><b>You won, the planet is yours!</b><br>" );
 else
  httpPrintf( cnt, "<br><b>Defender wins!</b><br>" );

 iohtmlBodyEnd( cnt );
 return;
}





void iohtmlFunc_explore( ReplyDataPtr cnt ) {
	int a, id, num, plnid, system, explore;
	char *planetstring, *explorestring, *systemstring;
	dbMainPlanetDef planetd;
	dbMainSystemDef systemd;
	dbUserFleetPtr fleetd;
	dbUserMainDef maind;


if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
	return;
iohtmlBase( cnt, 1 );

if( !( iohtmlHeader( cnt, id, &maind ) ) )
	return;

iohtmlBodyInit( cnt, "Exploration" );

planetstring = iohtmlVarsFind( cnt, "id" );
explorestring = iohtmlVarsFind( cnt, "dispatch" );
systemstring = iohtmlVarsFind( cnt, "system" );

if( ( num = dbUserFleetList( id, &fleetd ) ) <= 0 ) {
	httpString( cnt, "Error while retriving user fleets list" );
	goto iohttpFunc_exploreL1;
}

if( systemstring ) {
	if( ( sscanf( systemstring, "%d", &system ) <= 0 ) || dbMapRetrieveSystem( system, &systemd ) < 0 ) {
		httpString( cnt, "Error retriving system!<br>" );
		goto iohttpFunc_exploreL1;
	} else if ( systemd.empire != -1 ) {
		httpString( cnt, "Unable to explore an Empire System!<br>" );
		goto iohttpFunc_exploreL1;
	} else {
		goto SYSTEMEXPO;
	}

}
if( !( planetstring ) || ( sscanf( planetstring, "%d", &plnid ) <= 0 ) || ( dbMapRetrievePlanet( plnid, &planetd ) < 0 ) ) {
	httpString( cnt, "This planet doesn't seem to exist!<br>" );
	goto iohttpFunc_exploreL1;
}
if( planetd.flags & CMD_PLANET_FLAGS_HOME ) {
	httpString( cnt, "You can't explore a home planet!<br>" );
	goto iohttpFunc_exploreL1;
}
SYSTEMEXPO:
if( systemstring ) {
	if( explorestring ) {
		for(a = 0, num = systemd.indexplanet; a < systemd.numplanets; a++, num++) {
			if( ( dbMapRetrievePlanet( num, &planetd ) < 0 ) ) {
					httpString( cnt, "Planet retrvial error!<br>" );
					goto iohttpFunc_exploreL1;
			}
			if ( planetd.owner == -1 ) {
				if( ( cmdExecExplore( id, num, &explore ) ) < 0 ) {
					if( cmdErrorString ) {
						httpPrintf( cnt, "%s<br>", cmdErrorString );
					} else {
						httpString( cnt, "Error encountered while retrieving exploration information<br>" );
						goto iohttpFunc_exploreL0;
					}
				} else {
					httpPrintf( cnt, "We have sent our %s! If everything goes well, this planet will be ours in %d weeks<br>", cmdUnitName[CMD_UNIT_EXPLORATION], explore );
				}
			}
		}
	goto iohttpFunc_exploreL0;
	} else {
		for(a = 0, num = systemd.indexplanet; a < systemd.numplanets; a++, num++) {
			if( ( dbMapRetrievePlanet( num, &planetd ) < 0 ) ) {
					httpString( cnt, "Planet retrvial error!<br>" );
					goto iohttpFunc_exploreL1;
			}
			if ( planetd.owner == -1 ) {
				break;
			}
		}
		if( ( cmdExecExploreInfo( id, num, &explore ) ) < 0 ) {
			if( cmdErrorString )
				httpPrintf( cnt, "%s", cmdErrorString );
			else
				httpString( cnt, "Error encountered while retrieving exploration information<br>" );
				goto iohttpFunc_exploreL0;
		}
		httpPrintf( cnt, "%d planets are avalible for exploration in this system, you have %d %s", systemd.unexplored, fleetd[0].unit[CMD_UNIT_EXPLORATION], cmdUnitName[CMD_UNIT_EXPLORATION] );
		httpString( cnt, "<br><br>" );
		httpPrintf( cnt, "It will take %d weeks for an %s to reach this system.<br><br>", explore, cmdUnitName[CMD_UNIT_EXPLORATION] );
		httpPrintf( cnt, "<a href=\"%s&system=%d&dispatch=1\">Dispatch %s</a>", URLAppend( cnt, "explore" ), system, cmdUnitName[CMD_UNIT_EXPLORATION] );
	}
} else if( explorestring ) {
	if( ( cmdExecExplore( id, plnid, &explore ) ) < 0 ) {
		if( cmdErrorString ) {
			httpPrintf( cnt, "%s", cmdErrorString );
		} else {
			httpString( cnt, "Error encountered while retrieving exploration information<br>" );
			goto iohttpFunc_exploreL0;
		}
	}
	httpPrintf( cnt, "We have sent our %s! If everything goes well, this planet will be ours in %d weeks<br><br>", cmdUnitName[CMD_UNIT_EXPLORATION], explore );
} else {
	if( ( cmdExecExploreInfo( id, plnid, &explore ) ) < 0 ) {
		if( cmdErrorString )
			httpPrintf( cnt, "%s", cmdErrorString );
		else
			httpString( cnt, "Error encountered while retrieving exploration information<br>" );
			goto iohttpFunc_exploreL0;
	}
	if( fleetd[0].unit[CMD_UNIT_EXPLORATION] ) {
		httpPrintf( cnt, "It would take %d weeks for an %s to reach this planet.<br><br>", explore, cmdUnitName[CMD_UNIT_EXPLORATION] );
		httpPrintf( cnt, "<b><a href=\"%s&id=%d&dispatch=1\">Explore this planet</a></b><br>", URLAppend( cnt, "explore" ), plnid );
	} else {
		httpPrintf( cnt, "You don't have any %s to send!", cmdUnitName[CMD_UNIT_EXPLORATION] );
	}
}

iohttpFunc_exploreL0:
if ( !( systemstring ) )
	httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View planet</a>", URLAppend( cnt, "planet" ), plnid );

httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View system</a><br>", URLAppend( cnt, "system" ), systemstring ? system : planetd.system );

iohttpFunc_exploreL1:
iohtmlBodyEnd( cnt );

return;
}


void iohtmlFunc_attack( ReplyDataPtr cnt )
{
 int a, id, plnid, fr;
 float fa;
 dbMainPlanetDef planetd;
 dbUserMainDef maind;
 dbUserFleetDef fleetd;
 char *planetstring;
 dbUserPtr user;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 planetstring = iohtmlVarsFind( cnt, "id" );

 if( dbUserFleetRetrieve( id, 0, &fleetd ) < 0 )
 {
  httpString( cnt, "Error encountered while getting main fleet stats</body></html>" );
  iohtmlBodyEnd( cnt );
  return;
 }
 if( !( planetstring ) || ( sscanf( planetstring, "%d", &plnid ) != 1 ) || ( dbMapRetrievePlanet( plnid, &planetd ) < 0 ) )
 {
  httpString( cnt, "This planet doesn't seem to exist!</body></html>" );
  iohtmlBodyEnd( cnt );
  return;
 }
 iohtmlBodyInit( cnt, "Attack the planet %d,%d:%d", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );

 if( !( user = dbUserLinkID( planetd.owner ) ) )
 {
  httpString( cnt, "You can't attack an uninhabited planet!</body></html>" );
  iohtmlBodyEnd( cnt );
  return;
 }
 else if( ( planetd.flags & CMD_PLANET_FLAGS_HOME ) && !( bitflag( user->flags, CMD_USER_FLAGS_INDEPENDENT ) ) )
 {
  httpString( cnt, "You can't attack a home planet!</body></html>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 if( ( a = cmdExecSendFleetInfos( id, plnid, &fr ) ) < 0 )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error while retrieving send fleet information</i><br>" );
 }
 else
 {
  if( !( a ) )
   httpString( cnt, "A fleet could reach this planet instantaneously." );
  else
   httpPrintf( cnt, "A fleet would require %d weeks to reach this planet.", a );
  fa = specopDarkWebCalc( planetd.owner );

  if(cmdRace[maind.raceid].special & CMD_RACE_SPECIAL_IDW)
  	fa = 1;

  httpPrintf( cnt, " The loss of fleet readiness for an attack is estimated to %d%%.<br>", ( (int)( fa * (float)fr ) ) >> 16 );
  if( fa > 1.05 )
   httpPrintf( cnt, "<br><i>The area seems filled with a thick <b>Dark Mist</b>, making it more difficult to attack.<br></i>" );
 }

 httpString( cnt, "<script>function sendmax() {" );
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  if( fleetd.unit[a] )
   httpPrintf( cnt, "document.attack.u%d.value=%d; ", a, fleetd.unit[a] );
 }
 httpString( cnt, "}</script>" );
 httpPrintf( cnt, "<form name=\"attack\" action=\"%s\" method=\"POST\">", URLAppend( cnt, "fleetsend" ) );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"order\" value=\"%d\">", CMD_FLEET_ORDER_ATTACK );
 httpString( cnt, "<table cellspacing=\"8\">" );
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  if( fleetd.unit[a] )
   httpPrintf( cnt, "<tr><td align=\"right\">%d %s</td><td><input type=\"text\" name=\"u%d\" size=\"12\"></td></tr>", fleetd.unit[a], cmdUnitName[a], a );
 }
 httpPrintf( cnt, "</table><input type=\"hidden\" name=\"X\" value=\"%d\"><input type=\"hidden\" name=\"Y\" value=\"%d\"><input type=\"hidden\" name=\"Z\" value=\"%d\"><input type=\"button\" value=\"Send maximum\" onclick=\"sendmax()\"> <input type=\"submit\" value=\"Send\"></form>", ( planetd.position >> 8 ) & 0xFFF, ( planetd.position >> 20 ), planetd.position & 0xFF );

 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View planet</a>", URLAppend( cnt, "planet" ), plnid );
 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View system</a>", URLAppend( cnt, "system" ), planetd.system );

 iohtmlBodyEnd( cnt );
 return;
}

void iohtmlFunc_station( ReplyDataPtr cnt )
{
 int a, id, cmd[3], plnid;
 dbMainPlanetDef planetd;
 dbUserMainDef maind;
 dbUserFleetDef fleetd;
 char *planetstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 planetstring = iohtmlVarsFind( cnt, "id" );

 cmd[0] = CMD_RETRIEVE_FLEET;
 cmd[1] = id;
 cmd[2] = 0;
 if( cmdExecute( cnt, cmd, &fleetd, 0 ) < 0 )
 {
  httpString( cnt, "Error encountered while getting main fleet stats</body></html>" );
  return;
 }
 //cmd[0] = CMD_RETRIEVE_PLANET;
 if( !( planetstring ) || ( sscanf( planetstring, "%d", &cmd[1] ) <= 0 ) || ( dbMapRetrievePlanet( cmd[1], &planetd ) < 0 ) )
 {
  httpString( cnt, "This planet doesn't seem to exist!</body></html>" );
  return;
 }
 plnid = cmd[1];

 iohtmlBodyInit( cnt, "Station forces on the planet %d,%d:%d", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "fleetsend" ) );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"order\" value=\"%d\">", CMD_FLEET_ORDER_STATION );
 httpString( cnt, "<table cellspacing=\"8\">" );
 for( a = 0 ; a < CMD_UNIT_FLEET ; a++ )
 {
  if( fleetd.unit[a] )
   httpPrintf( cnt, "<tr><td align=\"right\">%d %s</td><td><input type=\"text\" name=\"u%d\" size=\"12\"></td></tr>", fleetd.unit[a], cmdUnitName[a], a );
 }
 httpPrintf( cnt, "</table><input type=\"hidden\" name=\"X\" value=\"%d\"><input type=\"hidden\" name=\"Y\" value=\"%d\"><input type=\"hidden\" name=\"Z\" value=\"%d\"><input type=\"submit\" value=\"Send\"></form>", ( planetd.position >> 8 ) & 0xFFF, ( planetd.position >> 20 ), planetd.position & 0xFF );
 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View planet</a>", URLAppend( cnt, "planet" ), plnid );
 httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View system</a>", URLAppend( cnt, "system" ), planetd.system );

 iohtmlBodyEnd( cnt );
 return;
}





void iohtmlFunc_spec( ReplyDataPtr cnt )
{
 int a, b, c, id, plnid, optype, opid;
 dbMainPlanetDef planetd;
 dbUserMainDef maind, main2d;
 dbUserFleetDef fleetd;
 char *planetstring;
 dbUserSpecOpPtr specopd;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 planetstring = iohtmlVarsFind( cnt, "id" );

 if( dbUserFleetRetrieve( id, 0, &fleetd ) < 0 )
 {
  httpString( cnt, "Error encountered while getting main fleet agent stats</body></html>" );
  return;
 }

 memset( &main2d.faction, 0, sizeof(main2d.faction) );
 plnid = -1;
 if ( ( planetstring ) && ( sscanf( planetstring, "%d", &plnid ) == 1 ) )
 {
  if( dbMapRetrievePlanet( plnid, &planetd ) < 0 )
  {
   httpString( cnt, "This planet doesn't seem to exist!</body></html>" );
   return;
  }
  iohtmlBodyInit( cnt, "Special Operations on planet %d,%d:%d", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );
  dbUserMainRetrieve( planetd.owner, &main2d );
 }
 else
  iohtmlBodyInit( cnt, "Special Operations" );


 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><table border=\"0\" width=\"70%\" cellspacing=\"4\" cellpadding=\"4\"><tr><td width=\"35%\" valign=\"top\" nowrap>", URLAppend( cnt, "operation" ) );
 httpPrintf( cnt, "<b>Operations</b><br>" );
 for( a = 0 ; a < CMD_AGENTOP_NUMUSED ; a++ )
 {
 	//ARTI CODE give agent op all races
 	if(!((maind.artefacts & ARTEFACT_1_BIT)&&(a == CMD_OPER_OBSERVEPLANET )))
 	{
	  if( !( specopAgentsAllowed( a, maind.raceid ) ) )
	  	continue;
	}
  b = cmdGetOpPenalty( maind.totalresearch[CMD_RESEARCH_OPERATIONS], cmdAgentopTech[a] );
  if( b == -1 )
   httpPrintf( cnt, "<input type=\"radio\" name=\"op\" value=\"%d\" disabled> %s<br>&nbsp;&nbsp;&nbsp;<font color=\"#FF0000\">Unavailable</font><br>", a, cmdAgentopName[a] );
  else
  {
   httpPrintf( cnt, "<input type=\"radio\" name=\"op\" value=\"%d\"> %s<br>", a, cmdAgentopName[a] );
   if( b )
    httpPrintf( cnt, "&nbsp;&nbsp;&nbsp;<font color=\"#FF0000\">Low research penalty : %d%%</font><br>", b );
  }
 }
 httpString( cnt, "</td><td width=\"35%\" align=\"center\" valign=\"top\" nowrap>" );
 httpPrintf( cnt, "%d %s<br>", fleetd.unit[CMD_UNIT_AGENT], cmdUnitName[CMD_UNIT_AGENT] );
 httpPrintf( cnt, "%d%% Agents readiness<br>", maind.readiness[CMD_READY_AGENT] >> 16 );
 httpPrintf( cnt, "Send: <input type=\"text\" size=\"10\" name=\"sendagents\" value=\"%d\"><br><br>", fleetd.unit[CMD_UNIT_AGENT] );
 if( plnid >= 0 )
  httpPrintf( cnt, "Target planet<br> X:<input type=\"text\" size=\"4\" name=\"planetx\" value=\"%d\"> Y:<input type=\"text\" size=\"4\" name=\"planety\" value=\"%d\"> Planet:<input type=\"text\" size=\"4\" name=\"planetnum\" value=\"%d\">", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );
 else
  httpPrintf( cnt, "Target planet<br> X:<input type=\"text\" size=\"4\" name=\"planetx\"> Y:<input type=\"text\" size=\"4\" name=\"planety\"> Planet:<input type=\"text\" size=\"4\" name=\"planetnum\">" );
 httpString( cnt, "<br><br><input type=\"submit\" value=\"Prepare operation\">" );
 httpString( cnt, "</td></tr></table></form>" );


 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><table border=\"0\" width=\"70%\" cellspacing=\"4\" cellpadding=\"4\"><tr><td width=\"35%\" valign=\"top\" nowrap>", URLAppend( cnt, "spell" ) );
 httpPrintf( cnt, "<b>Spells</b><br>" );
 for( a = 0 ; a < CMD_PSYCHICOP_NUMUSED ; a++ )
 {
  if( !( specopPsychicsAllowed( a, maind.raceid ) ) )
   continue;
  b = cmdGetOpPenalty( maind.totalresearch[CMD_RESEARCH_WELFARE], cmdPsychicopTech[a] );
  if( b == -1 )
   httpPrintf( cnt, "<input type=\"radio\" name=\"op\" value=\"%d\" disabled> %s<br>&nbsp;&nbsp;&nbsp;<font color=\"#FF0000\">Unavailable</font><br>", a, cmdPsychicopName[a] );
  else
  {
   httpPrintf( cnt, "<input type=\"radio\" name=\"op\" value=\"%d\"> %s<br>", a, cmdPsychicopName[a] );
   if( b )
    httpPrintf( cnt, "&nbsp;&nbsp;&nbsp;<font color=\"#FF0000\">Low research penalty : %d%%</font><br>", b );
  }
 }
 httpString( cnt, "</td><td width=\"35%\" align=\"center\" valign=\"top\" nowrap>" );
 httpPrintf( cnt, "%d %s<br>", fleetd.unit[CMD_UNIT_WIZARD], cmdUnitName[CMD_UNIT_WIZARD] );
 httpPrintf( cnt, "%d%% Psychics readiness<br>", maind.readiness[CMD_READY_PSYCH] >> 16 );
 httpPrintf( cnt, "Send: <input type=\"text\" size=\"10\" name=\"sendpsychics\" value=\"%d\"><br><br>", fleetd.unit[CMD_UNIT_WIZARD] );
 httpPrintf( cnt, "Target faction<br><input type=\"text\" size=\"20\" name=\"target\" value=\"%s\">", ( planetd.owner != -1 ) ? ( main2d.faction ) : ( "Faction name or ID" ) );
 httpString( cnt, "<br><br><input type=\"submit\" value=\"Prepare spell\">" );
 httpString( cnt, "</td></tr></table></form>" );


 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><table border=\"0\" width=\"70%\" cellspacing=\"4\" cellpadding=\"4\"><tr><td width=\"35%\" valign=\"top\" nowrap>", URLAppend( cnt, "incant" ) );
 httpPrintf( cnt, "<b>Incantations</b><br>" );
 for( a = 0 ; a < CMD_GHOSTOP_NUMUSED ; a++ )
 {
  if( !( specopGhostsAllowed( a, maind.raceid ) ) )
   continue;
  b = cmdGetOpPenalty( maind.totalresearch[CMD_RESEARCH_WELFARE], cmdGhostopTech[a] );
  if( b == -1 )
   httpPrintf( cnt, "<input type=\"radio\" name=\"op\" value=\"%d\" disabled> %s<br>&nbsp;&nbsp;&nbsp;<font color=\"#FF0000\">Unavailable</font><br>", a, cmdGhostopName[a] );
  else
  {
   httpPrintf( cnt, "<input type=\"radio\" name=\"op\" value=\"%d\"> %s<br>", a, cmdGhostopName[a] );
   if( b )
    httpPrintf( cnt, "&nbsp;&nbsp;&nbsp;<font color=\"#FF0000\">Low research penalty : %d%%</font><br>", b );
  }
 }

 httpString( cnt, "</td><td width=\"35%\" align=\"center\" valign=\"top\" nowrap>" );
 httpPrintf( cnt, "%d %s<br>", fleetd.unit[CMD_UNIT_GHOST], cmdUnitName[CMD_UNIT_GHOST] );
 httpPrintf( cnt, "%d%% Psychics readiness<br>", maind.readiness[CMD_READY_PSYCH] >> 16 );
 httpPrintf( cnt, "Send: <input type=\"text\" size=\"10\" name=\"sendghosts\" value=\"%d\"><br><br>", fleetd.unit[CMD_UNIT_GHOST] );
 if( plnid >= 0 )
  httpPrintf( cnt, "Target system or planet<br> X:<input type=\"text\" size=\"4\" name=\"sysx\" value=\"%d\"> Y:<input type=\"text\" size=\"4\" name=\"sysy\" value=\"%d\"> Planet:<input type=\"text\" size=\"4\" name=\"planet\" value=\"%d\">", ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF );
 else
  httpPrintf( cnt, "Target system or planet<br> X:<input type=\"text\" size=\"4\" name=\"sysx\"> Y:<input type=\"text\" size=\"4\" name=\"sysy\"> Planet:<input type=\"text\" size=\"4\" name=\"planet\">" );
 httpString( cnt, "<br><br><input type=\"submit\" value=\"Prepare incantation\">" );
 httpString( cnt, "</td></tr></table></form>" );


 if( ( b = dbUserSpecOpList( id, &specopd ) ) >= 0 )
 {
  httpString( cnt, "<table width=\"80%\"><tr><td width=\"40%\" align=\"center\" valign=\"top\"><b>Your operations</b><br><br>" );
  for( a = c = 0 ; a < b ; a++ )
  {
   if( specopd[a].type & 0x10000 )
    continue;
   c++;
   optype = ( specopd[a].type & 0xF000 ) >> 12;
   opid = specopd[a].type & 0xFFF;
   if( optype == 0 )
   {
    httpPrintf( cnt, "%s for %d weeks at <a href=\"%s&id=%d\">%d,%d:%d</a>", cmdAgentopName[opid], specopd[a].time, URLAppend( cnt, "planet" ), specopd[a].plnid, ( specopd[a].plnpos >> 8 ) & 0xFFF, specopd[a].plnpos >> 20, specopd[a].plnpos & 0xFF );
    dbMapRetrievePlanet( specopd[a].plnid, &planetd );
    if( planetd.owner == -1 )
     httpString( cnt, ", unhabited" );
    else
    {
     if( dbUserMainRetrieve( specopd[a].factionid, &main2d ) )
      httpPrintf( cnt, " on <a href=\"%s&id=%d\">%s</a>", URLAppend( cnt, "player" ), specopd[a].factionid, main2d.faction );
    }
    if( opid == CMD_OPER_HIGHINFIL )
     httpPrintf( cnt, " - <a href=\"%s&id=%d\">See information</a>", URLAppend( cnt, "specinfos" ), a );
    httpString( cnt, "<br>" );
   }
   else if( optype == 1 )
   {
    if( !( dbUserMainRetrieve( specopd[a].factionid, &main2d ) ) )
     continue;
    httpPrintf( cnt, "%s for %d weeks on ", cmdPsychicopName[opid], specopd[a].time );
    if( id != specopd[a].factionid )
     httpPrintf( cnt, "<a href=\"%s&id=%d\">%s</a>", URLAppend( cnt, "player" ), specopd[a].factionid, main2d.faction );
    else
     httpString( cnt, "yourself" );
    if( opid == CMD_SPELL_DARKWEB )
     httpPrintf( cnt, " - +%d%% FR protection", specopd[a].vars[0] );
    else if( opid == CMD_SPELL_BLACKMIST )
     httpPrintf( cnt, " - -%d%% solar energy", specopd[a].vars[0] );
    else if( opid == CMD_SPELL_WARILLUSIONS )
     httpPrintf( cnt, " - +%d%% units protection", specopd[a].vars[0] );

    httpString( cnt, "<br>" );
   }
   else if( optype == 2 )
   {
    if( !( dbUserMainRetrieve( specopd[a].factionid, &main2d ) ) )
     continue;
    httpPrintf( cnt, "%s for %d weeks on ", cmdGhostopName[opid], specopd[a].time );
    if( !( cmdGhostopFlags[opid] & 4 ) )
    {
     httpPrintf( cnt, "<a href=\"%s&id=%d\">%d,%d:%d</a>", URLAppend( cnt, "planet" ), specopd[a].plnid, ( specopd[a].plnpos >> 8 ) & 0xFFF, specopd[a].plnpos >> 20, specopd[a].plnpos & 0xFF );
     dbMapRetrievePlanet( specopd[a].plnid, &planetd );
     if( planetd.owner == -1 )
      httpString( cnt, ", unhabited" );
     else
     {
      if( ( dbUserMainRetrieve( specopd[a].factionid, &main2d ) ) )
       httpPrintf( cnt, " of <a href=\"%s&id=%d\">%s</a>", URLAppend( cnt, "player" ), specopd[a].factionid, main2d.faction );
     }
    }
    else
    {
     if( specopd[a].plnid != -1 )
      httpPrintf( cnt, "<a href=\"%s&id=%d\">%d,%d</a>", URLAppend( cnt, "system" ), specopd[a].plnid, ( specopd[a].plnpos >> 8 ) & 0xFFF, specopd[a].plnpos >> 20 );
     else
      httpPrintf( cnt, "%d,%d", ( specopd[a].plnpos >> 8 ) & 0xFFF, specopd[a].plnpos >> 20 );
    }
    if( opid == CMD_INCANT_SURVEY )
     httpPrintf( cnt, " - <a href=\"%s&id=%d\">See information</a>", URLAppend( cnt, "specinfos" ), a );
    else if( opid == CMD_INCANT_SHIELDING )
     httpPrintf( cnt, " - Shielding : %d", specopd[a].vars[0] );
    httpString( cnt, "<br>" );
   }
  }
  if( !( c ) )
   httpString( cnt, "None" );
  httpString( cnt, "</td><td width=\"40%\" align=\"center\" valign=\"top\"><b>Known operations affecting your faction</b><br><br>" );
  for( a = c = 0 ; a < b ; a++ )
  {
   if( !( specopd[a].type & 0x10000 ) )
    continue;
   c++;
   optype = ( specopd[a].type & 0xF000 ) >> 12;
   opid = specopd[a].type & 0xFFF;
   if( optype == 0 )
    httpPrintf( cnt, "%s for %d weeks from", cmdAgentopName[opid], specopd[a].time );
   else if( optype == 1 )
    httpPrintf( cnt, "%s for %d weeks from", cmdPsychicopName[opid], specopd[a].time );
   else
    httpPrintf( cnt, "%s for %d weeks from", cmdGhostopName[opid], specopd[a].time );
   if( ( specopd[a].factionid == -1 ) || !( dbUserMainRetrieve( specopd[a].factionid, &main2d ) ) )
    httpString( cnt, " an unknown faction<br>" );
   else
    httpPrintf( cnt, " <a href=\"%s&id=%d\">%s</a>", URLAppend( cnt, "player" ), specopd[a].factionid, main2d.faction );

   if( optype == 2 )
   {
    if( !( cmdGhostopFlags[opid] & 4 ) && ( dbMapRetrievePlanet( specopd[a].plnid, &planetd ) >= 0 ) )
     httpPrintf( cnt, " on planet <a href=\"%s&id=%d\">%d,%d:%d</a>", URLAppend( cnt, "planet" ), specopd[a].plnid, ( specopd[a].plnpos >> 8 ) & 0xFF, specopd[a].plnpos >> 20, specopd[a].plnpos & 0xFF );
    if( opid == CMD_INCANT_SHIELDING )
     httpPrintf( cnt, " - Shielding : %d", specopd[a].vars[0] );
   }

   httpString( cnt, "<br>" );
  }
  if( !( c ) )
   httpString( cnt, "None" );
  httpString( cnt, "</td></tr></table>" );
  free( specopd );
 }

 if( plnid != -1 )
 {
  httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View planet</a>", URLAppend( cnt, "planet" ), plnid );
  httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View system</a>", URLAppend( cnt, "system" ), planetd.system );
 }

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_specinfos( ReplyDataPtr cnt )
{
 int a, b, c, d, x, y, id, optype, opid, specopid, num;
 int totals[7];
 int *buffer;
 float fa, totalob;
 char *specopstring;
 dbUserPtr user;
 dbUserMainDef maind;
 dbUserSpecOpDef specopd;
 dbMainPlanetDef planetd;
 dbUserMainDef main2d;
 dbUserFleetPtr fleetd;
 dbMainSystemDef systemd;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 specopstring = iohtmlVarsFind( cnt, "id" );

 iohtmlBodyInit( cnt, "Operation information" );
 if( !( specopstring ) || ( sscanf( specopstring, "%d", &specopid ) != 1 ) || ( dbUserSpecOpRetrieve( id, specopid, &specopd ) < 0 ) || ( specopd.type & 0x10000 ) )
 {
  httpString( cnt, "<i>Invalid operation</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 optype = ( specopd.type & 0xF000 ) >> 12;
 opid = specopd.type & 0xFFF;
 if( optype == 0 )
 {
  if( opid != CMD_OPER_HIGHINFIL )
   goto iohttpFunc_specinfosL0;
  httpPrintf( cnt, "<b>%s</b> for %d weeks at <a href=\"%s&id=%d\">%d,%d:%d</a>", cmdAgentopName[opid], specopd.time, URLAppend( cnt, "planet" ), specopd.plnid, ( specopd.plnpos >> 8 ) & 0xFFF, specopd.plnpos >> 20, specopd.plnpos & 0xFF );
  dbMapRetrievePlanet( specopd.plnid, &planetd );
  if( planetd.owner == -1 )
   httpString( cnt, ", unhabited<br><br>" );
  else
  {
   if( ( user = dbUserLinkID( planetd.owner ) ) )
    httpPrintf( cnt, " on <a href=\"%s&id=%d\">%s</a><br><br>", URLAppend( cnt, "player" ), planetd.owner, user->faction );
  }

  if( opid == CMD_OPER_HIGHINFIL )
  {
   if( dbUserMainRetrieve( specopd.factionid, &main2d ) < 0 )
   {
    httpString( cnt, "Error while retriving main user data" );
    return;
   }
   httpPrintf( cnt, "<br><table><tr><td valign=\"top\"><i>Faction</i><br>" );
   httpPrintf( cnt, "Planets : <b>%d</b><br>", main2d.planets );
   httpPrintf( cnt, "Networth : <b>%lld</b><br>", (long long)main2d.networth );
   httpPrintf( cnt, "Empire : <b>#%d</b><br>", main2d.empire );
   httpPrintf( cnt, "Fleet readiness : <b>%d %%</b><br>", main2d.readiness[CMD_READY_FLEET] >> 16 );
   httpPrintf( cnt, "Psychics readiness : <b>%d %%</b><br>", main2d.readiness[CMD_READY_PSYCH] >> 16 );
   httpPrintf( cnt, "Agents readiness : <b>%d %%</b><br>", main2d.readiness[CMD_READY_AGENT] >> 16 );
   httpPrintf( cnt, "</td><td valign=\"top\"><i>Resources</i><br>" );
   for( a = 0; a < CMD_RESSOURCE_NUMUSED ; a++ )
    httpPrintf( cnt, "%s : <b>%lld</b><br>", cmdRessourceName[a], (long long)main2d.ressource[a] );
   httpPrintf( cnt, "</td><td valign=\"top\"><i>Research fields</i><br>" );
   for( a = 0; a < CMD_RESEARCH_NUMUSED ; a++ )
    httpPrintf( cnt, "%s : <b>%lld %%</b><br>", cmdResearchName[a], (long long)main2d.totalresearch[a] );
   httpPrintf( cnt, "</td></tr></table><br><br>" );
   if( specopd.vars[0] & 1 )
   {
    if( ( b = dbUserPlanetListIndices( specopd.factionid, &buffer ) ) < 0 )
    {
     httpString( cnt, "Error while retriving planets list" );
     return;
    }
    httpString( cnt, "<table width=\"100%%\"><tr><td width=\"15%%\"><b>Planet</b></td><td width=\"10%%\">Size</td><td width=\"20%%\">Buildings - Penalty</td><td width=\"25%%\">Population</td><td width=\"28%\">Protection</td></tr>" );
    memset( totals, 0, 7*sizeof(int) );
    totalob = 0.0;
    for( a = 0 ; a < b ; a++ )
    {
     dbMapRetrievePlanet( buffer[a], &planetd );
     for( c = d = 0 ; c < CMD_BLDG_NUMUSED ; c++ )
      d += planetd.building[c];
     httpPrintf( cnt, "<tr><td><a href=\"%s&id=%d\">%d,%d:%d</a></td><td>%d</td><td>%d", URLAppend( cnt, "planet" ), buffer[a], ( planetd.position >> 8 ) & 0xFFF, planetd.position >> 20, planetd.position & 0xFF, planetd.size, d );
     if( planetd.construction )
      httpPrintf( cnt, " ( %d )", planetd.construction );
     for( c = CMD_BLDG_OVERBUILD ; c < CMD_BLDG_NUMUSED ; c++ )
      d -= planetd.building[c];
     fa = cmdGetBuildOvercost( planetd.size, d+planetd.construction );
     if( fa > 1.0 )
      httpPrintf( cnt, " <font color=\"#FF2020\">+%.2f%%</font>", 100.0 * (fa-1.0) );
     totalob += fa;
     totals[0] += planetd.size;
     totals[1] += d;
     totals[2] += planetd.construction;
     httpPrintf( cnt, "</td><td>%d0 / %d0</td><td>", planetd.population, planetd.maxpopulation );
     if( planetd.flags & CMD_PLANET_FLAGS_PORTAL )
     {
      httpString( cnt, " Portal -" );
      totals[5]++;
     }
     if( planetd.protection >= 70 )
      httpString( cnt, " <font color=\"#00FF00" );
     else if( planetd.protection >= 40 )
      httpString( cnt, " <font color=\"#FFFF00" );
     else
      httpString( cnt, " <font color=\"#FF0000" );
     httpPrintf( cnt, "\">%d%%</font>", planetd.protection );
     for( c = d = 0 ; c < CMD_UNIT_NUMUSED ; c++ )
      d += planetd.unit[c];
     if( d )
     {
      httpPrintf( cnt, " <i>Units (%d)</i>", d );
      totals[6] += d;
     }

     d = (int)artefactPrecense( &planetd );
     if( d >= 0 )
      httpPrintf( cnt, "  <img src=\"files?type=image&name=artifacts/%s\" alt=\"%s\" title=\"%s\">", artefactImage[d], artefactName[d], artefactName[d] );

     if(planetd.special[1])
     	httpPrintf( cnt, " <img src=\"files?type=image&name=pr%d.gif\">+%d%%", planetd.special[0], planetd.special[1] );

     httpPrintf( cnt, "</td></tr>" );
     totals[3] += planetd.population;
     totals[4] += planetd.maxpopulation;
    }
    httpPrintf( cnt, "<tr><td>%d Planets</td><td>%d</td><td>%d", b, totals[0], totals[1] );
    if( totals[2] )
     httpPrintf( cnt, " ( %d )", totals[2] );
    totalob /= (float)b;
    if( totalob > 1.001 )
     httpPrintf( cnt, " <font color=\"#FF2020\">+%.2f%%</font>", 100.0 * (totalob-1.0) );
    httpPrintf( cnt, "</td><td>%d0 / %d0</td><td> %d Portals", totals[3], totals[4], totals[5] );
    if( totals[6] )
     httpPrintf( cnt, " <i>Units (%d)</i>", totals[6] );
    httpString( cnt, "</td><td>&nbsp;</td></tr></table><br><br>" );
    free( buffer );
   }
   else
    httpString( cnt, "Gathering information on the faction planets network failed!<br><br>" );
   if( specopd.vars[0] & 2 )
   {
    if( ( num = dbUserFleetList( specopd.factionid, &fleetd ) ) <= 0 )
    {
     httpString( cnt, "Error while retriving user fleets list</body></html>" );
     iohtmlBodyEnd( cnt );
     return;
    }
    httpString( cnt, "<b>Main fleet</b><br><br>" );
    for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ )
    {
     if( fleetd[0].unit[a] )
     {
      httpPrintf( cnt, "%d %s<br>", fleetd[0].unit[a], cmdUnitName[a] );
      b = 1;
     }
    }
    httpString( cnt, "<br><b>Travelling fleets</b><br><br>" );
    if( num == 1 )
     httpString( cnt, "None<br>" );
    else
    {
     httpString( cnt, "<table width=\"90%%\" cellspacing=\"5\"><tr><td width=\"50%\">Units</td><td width=\"50%\">Position</td></tr>" );
     for( a = 1 ; a < num ; a++ )
     {
      httpString( cnt, "<tr><td>" );
      for( b = 0 ; b < CMD_UNIT_NUMUSED ; b++ )
      {
       if( fleetd[a].unit[b] )
        httpPrintf( cnt, "%d %s<br>", fleetd[a].unit[b], cmdUnitName[b] );
      }
      cmdFleetGetPosition( &fleetd[a], &x, &y );
      httpPrintf( cnt, "<td>%d,%d</td></tr>", x, y );
     }
     httpString( cnt, "</table>" );
    }
   }
   else
    httpString( cnt, "Gathering information on the faction fleets failed!<br><br>" );
  }
  if( specopd.plnid != -1 )
   httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View planet</a>", URLAppend( cnt, "planet" ), specopd.plnid );
 }
 else if( optype == 1 )
 {
  goto iohttpFunc_specinfosL0;
  if( !( user = dbUserLinkID( specopd.factionid ) ) )
   return;
  httpPrintf( cnt, "<b>%s</b> for %d weeks on <a href=\"%s&id=%d\">%s</a><br>", cmdPsychicopName[opid], specopd.time, URLAppend( cnt, "player" ), specopd.factionid, user->faction );
  if( specopd.plnid != -1 )
   httpPrintf( cnt, "<br><br><a href=\"%s&id=%d\">View planet</a>", URLAppend( cnt, "planet" ), specopd.plnid );
 }
 else if( optype == 2 )
 {
  httpPrintf( cnt, "<b>%s</b> for %d weeks", cmdGhostopName[opid], specopd.time );
  if( specopd.plnid != -1 )
  {
   if( !( cmdGhostopFlags[opid] & 4 ) )
    httpPrintf( cnt, " on <a href=\"%s&id=%d\">%d,%d:%d</a>", URLAppend( cnt, "planet" ), specopd.plnid, ( specopd.plnpos >> 8 ) & 0xFF, specopd.plnpos >> 20, specopd.plnpos & 0xFF );
   else
    httpPrintf( cnt, " on <a href=\"%s&id=%d\">%d,%d</a>", URLAppend( cnt, "system" ), specopd.plnid, ( specopd.plnpos >> 8 ) & 0xFF, specopd.plnpos >> 20 );
  }
  httpString( cnt, "<br><br>" );

  if( opid == CMD_INCANT_SURVEY )
  {
   if( dbMapRetrieveSystem( specopd.plnid, &systemd ) < 0 )
    return;
   b = systemd.numplanets;
   if( b > 32 )
    b = 32;
   httpString( cnt, "<table border=\"0\"><tr><td>" );
   for( a = 0 ; a < b ; a++ )
   {
    if( dbMapRetrievePlanet( systemd.indexplanet + a, &planetd ) < 0 )
     continue;
    httpPrintf( cnt, "<b>Planet <a href=\"%s&id=%d\">%d,%d:%d</a></b>", URLAppend( cnt, "planet" ), systemd.indexplanet + a, ( planetd.position >> 8 ) & 0xFF, planetd.position >> 20, planetd.position & 0xFF );
    if( planetd.owner == -1 ) {
     httpString( cnt, ", uninhabited<br>" );
    } else if( dbUserMainRetrieve( planetd.owner, &main2d ) >= 0 ) {
     httpPrintf( cnt, " owned by <a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), planetd.owner, main2d.faction );
     httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a><br>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire );
    }
    if( !( specopd.vars[0] & ( 1 << a ) ) )
    {
     httpString( cnt, "No information gathered<br><br>" );
     continue;
    }
    httpPrintf( cnt, "Size : %d<br>", planetd.size );
    d = (int)artefactPrecense( &planetd );
    if( d >= 0 )
     httpPrintf( cnt, "Artefact :  <img src=\"files?type=image&name=artifacts/%s\" alt=\"%s\" title=\"%s\"><br>", artefactImage[d], artefactName[d], artefactName[d]  );
    if( planetd.special[1] )
    {
     httpPrintf( cnt, "%s : <img src=\"files?type=image&name=pr%d.gif\">+%d%%<br>", cmdBonusName[planetd.special[0]], planetd.special[0], planetd.special[1] );
    }
    httpPrintf( cnt, "Portal : %s<br><br>", ( planetd.flags & CMD_PLANET_FLAGS_PORTAL ) ? "yes" : "no" );
   }
   httpString( cnt, "</td></tr></table>" );
  }

 }

 iohtmlBodyEnd( cnt );
 return;

 iohttpFunc_specinfosL0:
 httpString( cnt, "This operation doesn't have any additionnal information.<br>" );
 iohtmlBodyEnd( cnt );
 return;
}




void iohtmlFunc_operation( ReplyDataPtr cnt )
{
 int a, id, plnid, x, y, z, sysid;
 int agents, specop, arl;
 char *agentstring, *orderstring, *xstring, *ystring, *zstring;
 dbMainPlanetDef planetd;
 dbUserMainDef maind, main2d;
 dbMainSystemDef systemd;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 orderstring = iohtmlVarsFind( cnt, "op" );
 agentstring = iohtmlVarsFind( cnt, "sendagents" );
 xstring = iohtmlVarsFind( cnt, "planetx" );
 ystring = iohtmlVarsFind( cnt, "planety" );
 zstring = iohtmlVarsFind( cnt, "planetnum" );

 if( !( orderstring ) || ( sscanf( orderstring, "%d", &specop ) != 1 ) )
  goto iohttpFunc_operationL0;
 if( (unsigned int)specop >= CMD_AGENTOP_NUMUSED )
  goto iohttpFunc_operationL0;
 if( !( xstring ) || ( sscanf( xstring, "%d", &x ) != 1 ) )
  goto iohttpFunc_operationL0;
 if( !( ystring ) || ( sscanf( ystring, "%d", &y ) != 1 ) )
  goto iohttpFunc_operationL0;
 if( !( zstring ) || ( sscanf( zstring, "%d", &z ) != 1 ) )
  goto iohttpFunc_operationL0;
 if( !( agentstring ) || ( sscanf( agentstring, "%d", &agents ) != 1 ) )
 {
  iohttpFunc_operationL0:
  iohtmlBodyInit( cnt, "Agents operation", x, y, z );
  httpString( cnt, "<i>Invalid order or coordinates</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 iohtmlBodyInit( cnt, "Agents operation on planet %d,%d:%d", x, y, z );

 if( ( sysid = dbMapFindSystem( x, y ) ) < 0 )
 {
  httpString( cnt, "<i>It seems there is no system at the coordinates entered.</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 if( dbMapRetrieveSystem( sysid, &systemd ) < 0 )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error encountered when getting System Data</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }
 if( (unsigned int)z >= systemd.numplanets )
  goto iohttpFunc_operationL0;
 plnid = systemd.indexplanet + z;

 if( dbMapRetrievePlanet( plnid, &planetd ) < 0 )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i>", cmdErrorString );
  else
   httpString( cnt, "<i>Error encountered when getting planet data</i>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 // only with observe planets you can cast ops on an uninhabited planet.
 if( ( planetd.owner == -1 ) && ( specop != CMD_OPER_OBSERVEPLANET ) )
 {
  httpPrintf ( cnt, "<i>You can't perform %s on an uninhabited planet.</i>", cmdAgentopName[specop] );
  iohtmlBodyEnd( cnt );
  return;
 }

 if( planetd.owner != -1 )
 {
   if( dbUserMainRetrieve( planetd.owner, &main2d ) < 0 ) {
   	httpString( cnt, "<i>Error encountered when getting Faction Data</i><br>" );
   	iohtmlBodyEnd( cnt );
   	return;
   }
   httpPrintf( cnt, "Preparing to perform <b>%s</b> on planet ", cmdAgentopName[specop] );
   httpPrintf( cnt, "<a href=\"%s&id=%d\">%d,%d:%d</a>, owned by ", URLAppend( cnt, "planet" ), plnid, x, y, z );
   httpPrintf( cnt, "<a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), planetd.owner, main2d.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a>.<br><br>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire );
 }
 else
  httpPrintf( cnt, "Preparing to perform <b>%s</b> on the unhabited planet <a href=\"%s&id=%d\">%d,%d:%d</a>.<br><br>", cmdAgentopName[specop], URLAppend( cnt, "planet" ), plnid, x, y, z );

// we need a table here.
 httpString( cnt, "<i>" );
 if( specop == CMD_OPER_SPYTARGET )
  httpString( cnt, "Spy target will reveal information regarding an faction resources and readiness." );
 else if( specop == CMD_OPER_OBSERVEPLANET )
  httpString( cnt, "Your agents will observe the planet and provide you with all information regarding it, habited or not." );
 else if( specop == CMD_OPER_NETWORKVIRUS )
  httpString( cnt, "Your agents will introduce viruses in the target faction computers network, destryoing a part of their research files and temporarily increasing buildings upkeep." );
 else if( specop == CMD_OPER_INFILTRATION )
  httpString( cnt, "Infiltrating the target network will provide you with information regarding its resources, research and buildings." );
 else if( specop == CMD_OPER_BIOINFECTION )
  httpString( cnt, "Your agents will attempt to spread a dangerous infection which will kill most of the population and spread in nearby planets." );
 else if( specop == CMD_OPER_ENERGYTRANSFER )
  httpString( cnt, "Your Agents will infiltrate the enemy energy storage facilities and transfer energy to your own energy storage." );
 else if( specop == CMD_OPER_MILITARYSAB )
  httpString( cnt, "Through an enemy portal, your agents will attempt to reach the enemy fleet and destroy military units." );
 else if( specop == CMD_OPER_NUKEPLANET )
  httpString( cnt, "Your agents will place powerful nuclear devices on the surface of the planet, destroying all buildings and units, leaving in uninhabited." );
 else if( specop == CMD_OPER_HIGHINFIL )
  httpString( cnt, "Performing this operation will provide you with detailled information about an faction for several weeks." );
 else if( specop == CMD_OPER_PLANETBEACON )
  httpString( cnt, "Planetary definition." );
 httpString( cnt, "</i><br><br>" );

 if( ( a = cmdExecSendFleetInfos( id, plnid, 0 ) ) < 0 )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error while retrieving send fleet information</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 if( !( a ) )
  httpPrintf( cnt, "Your %d %s could reach this planet instantaneously", agents, cmdUnitName[CMD_UNIT_AGENT] );
 else
  httpPrintf( cnt, "Your %d %s would reach this planet in %d weeks", agents, cmdUnitName[CMD_UNIT_AGENT], a );

 arl = specopAgentsReadiness( specop, &maind, ( planetd.owner != -1 ) ? ( &main2d ) : ( 0 ) );
 if( arl == -1 )
 {
  httpPrintf( cnt, ". Unfortunately, we don't have enough %s to perform %s.", cmdResearchName[CMD_RESEARCH_OPERATIONS], cmdAgentopName[specop] );
  iohtmlBodyEnd( cnt );
  return;
 }
 httpPrintf( cnt, " , performing the operation would use %d%% Agent Readiness.<br><br>", arl >> 16 );

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "operationsend" ) );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"X\" value=\"%d\"><input type=\"hidden\" name=\"Y\" value=\"%d\">", x, y );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"Z\" value=\"%d\"><input type=\"hidden\" name=\"agents\" value=\"%d\">", z, agents );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"order\" value=\"%d\">", ( specop + CMD_FLEET_ORDER_FIRSTOP ) );
 httpString( cnt, "<input type=\"submit\" value=\"Send agents\" size=\"10\"></form>" );

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_spell( ReplyDataPtr cnt )
{
 int a, id, targetid;
 int psychics, specop, wrl;
 char *psychicstring, *orderstring, *targetstring;
 dbUserMainDef maind, main2d;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 orderstring = iohtmlVarsFind( cnt, "op" );
 psychicstring = iohtmlVarsFind( cnt, "sendpsychics" );
 targetstring = iohtmlVarsFind( cnt, "target" );

 if( !( orderstring ) || ( sscanf( orderstring, "%d", &specop ) != 1 ) )
  goto iohttpFunc_spellL0;
 if( (unsigned int)specop >= CMD_PSYCHICOP_NUMUSED )
  goto iohttpFunc_spellL0;
 if( !( psychicstring ) || ( sscanf( psychicstring, "%d", &psychics ) != 1 ) )
 {
  iohttpFunc_spellL0:
  iohtmlBodyInit( cnt, "Psychics operation" );
  httpString( cnt, "<i>Invalid order or coordinates</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 if( cmdPsychicopFlags[specop] & 2 )
 {
  targetid = id;
  memcpy( &main2d, &maind, sizeof(dbUserMainDef) );
 }
 else
 {
  targetid = -1;
  if( !( targetstring ) )
   goto iohttpFunc_spellL0;
  if( ( targetstring[0] >= '0' ) && ( targetstring[0] <= '9' ) )
   sscanf( targetstring, "%d", &targetid );
  else
  {
   for( a = 0 ; targetstring[a] ; a++ )
   {
    if( targetstring[a] == '+' )
     targetstring[a] = ' ';
    else if( ( targetstring[a] == 10 ) || ( targetstring[a] == 13 ) )
     targetstring[a] = 0;
   }
   targetid = dbUserSearchFaction( targetstring );
  }
  if( ( targetid < 0 ) || ( dbUserMainRetrieve( targetid, &main2d ) < 0 ) )
  {
   iohtmlBodyInit( cnt, "Psychics operation" );
   httpPrintf( cnt, "<i>The faction %s does not seem to exist, the syntax must be exact or use the user ID.</i>", targetstring );
   iohtmlBodyEnd( cnt );
   return;
  }
 }

 iohtmlBodyInit( cnt, "Psychics operation on %s", main2d.faction );

 httpPrintf( cnt, "Casting %s on <a href=\"%s&id=%d\">%s</a> of ", cmdPsychicopName[specop], URLAppend( cnt, "player" ), targetid, main2d.faction );
 httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a>.<br><br>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire );

// we need a table here too.
 httpString( cnt, "<i>" );
 if( specop == CMD_SPELL_IRRAECTRO )
  httpString( cnt, "Your psychics will attempt to irradiate the target's ectrolium reserves, making it unusable." );
 else if( specop == CMD_SPELL_DARKWEB )
  httpString( cnt, "Creating a thick dark web spreading in space around your planets will make them much harder to locate and attack." );
 else if( specop == CMD_SPELL_INCANDESCENCE )
  httpString( cnt, "Your psychics will convert crystal into vast amounts of energy." );
 else if( specop == CMD_SPELL_BLACKMIST )
  httpString( cnt, "Creating a dense black mist around the target's planets will greatly reduce the effectiveness of solar collectors." );
 else if( specop == CMD_SPELL_WARILLUSIONS )
  httpString( cnt, "These illusions will help keeping enemy fire away from friendly units." );
 else if( specop == CMD_SPELL_ASSAULT )
  httpString( cnt, "Your psychics will engage the targeted faction's psychics, to cause large casualities." );
 else if( specop == CMD_SPELL_PHANTOMS )
  httpString( cnt, "Your psychics will create etheral creatures to join your fleets in battle." );
 httpString( cnt, "</i><br><br>" );

 wrl = specopPsychicsReadiness( specop, &maind, ( id != targetid ) ? ( &main2d ) : ( 0 ) );
 if( wrl == -1 )
 {
  httpPrintf( cnt, "Unfortunately, we don't have enough %s to perform %s.", cmdResearchName[CMD_RESEARCH_WELFARE], cmdPsychicopName[specop] );
  iohtmlBodyEnd( cnt );
  return;
 }
 httpPrintf( cnt, "Performing the operation would use %d%% Psychic Readiness.<br><br>", wrl >> 16 );

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "spellsend" ) );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"target\" value=\"%d\"><input type=\"hidden\" name=\"psychics\" value=\"%d\">", targetid, psychics );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"order\" value=\"%d\">", specop );
 httpString( cnt, "<input type=\"submit\" value=\"Cast spell\" size=\"10\"></form>" );

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_incant( ReplyDataPtr cnt )
{
 int a, id, plnid, x, y, z, sysid;
 int ghosts, specop, grl;
 char *ghoststring, *orderstring, *xstring, *ystring, *zstring;
 dbMainPlanetDef planetd;
 dbUserMainDef maind, main2d;
 dbMainSystemDef systemd;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;

 orderstring = iohtmlVarsFind( cnt, "op" );
 ghoststring = iohtmlVarsFind( cnt, "sendghosts" );
 xstring = iohtmlVarsFind( cnt, "sysx" );
 ystring = iohtmlVarsFind( cnt, "sysy" );
 zstring = iohtmlVarsFind( cnt, "planet" );

 if( !( orderstring ) || ( sscanf( orderstring, "%d", &specop ) != 1 ) )
  goto iohttpFunc_operationL0;
 if( (unsigned int)specop >= CMD_GHOSTOP_NUMUSED )
  goto iohttpFunc_operationL0;
 if( !( xstring ) || ( sscanf( xstring, "%d", &x ) != 1 ) )
  goto iohttpFunc_operationL0;
 if( !( ystring ) || ( sscanf( ystring, "%d", &y ) != 1 ) )
  goto iohttpFunc_operationL0;
 if( !( zstring ) || ( sscanf( zstring, "%d", &z ) != 1 ) )
  z = -1;
 if( !( ghoststring ) || ( sscanf( ghoststring, "%d", &ghosts ) != 1 ) )
 {
  iohttpFunc_operationL0:
  iohtmlBodyInit( cnt, "Ghost ships incantation" );
  httpString( cnt, "<i>Invalid order or coordinates</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }
 if( cmdGhostopFlags[specop] & 4 )
  z = -1;

 if( z != -1 )
  iohtmlBodyInit( cnt, "Ghost ships operation on planet %d,%d:%d", x, y, z );
 else
  iohtmlBodyInit( cnt, "Ghost ships operation on system %d,%d", x, y, z );

 planetd.owner = -1;
 if( !( cmdGhostopFlags[specop] & 4 ) )
 {
  if( ( sysid = dbMapFindSystem( x, y ) ) < 0 )
  {
   httpString( cnt, "<i>It seems there is no system at the coordinates entered.</i><br>" );
   iohtmlBodyEnd( cnt );
   return;
  }
  if( dbMapRetrieveSystem( sysid, &systemd ) < 0 )
  {
   if( cmdErrorString )
    httpPrintf( cnt, "<i>%s</i><br>", cmdErrorString );
   else
    httpString( cnt, "<i>Error encountered when getting System Data</i><br>" );
   iohtmlBodyEnd( cnt );
   return;
  }

  if( z == -1 )
   goto iohttpFunc_operationL0;
  if( (unsigned int)z >= systemd.numplanets )
   goto iohttpFunc_operationL0;
  plnid = systemd.indexplanet + z;

  if( dbMapRetrievePlanet( plnid, &planetd ) < 0 )
  {
   if( cmdErrorString )
    httpPrintf( cnt, "<i>%s</i>", cmdErrorString );
   else
    httpString( cnt, "<i>Error encountered when getting planet data</i>" );
   iohtmlBodyEnd( cnt );
   return;
  }
  if( planetd.owner == -1 )
  {
   httpPrintf ( cnt, "<i>You can't perform %s on an uninhabited planet.</i>", cmdGhostopName[specop] );
   iohtmlBodyEnd( cnt );
   return;
  }
  if( dbUserMainRetrieve( planetd.owner, &main2d ) < 0 )
   httpString( cnt, "<i>Error encountered when getting faction Data</i><br>" );
   httpPrintf( cnt, "Preparing to perform <b>%s</b> on planet ", cmdGhostopName[specop] );
   httpPrintf( cnt, "<a href=\"%s&id=%d\">%d,%d:%d</a>, owned by ", URLAppend( cnt, "planet" ), plnid, x, y, z );
   httpPrintf( cnt, "<a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), planetd.owner, main2d.faction );
   httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a>.<br><br>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire );
 }
 else
  httpPrintf( cnt, "Preparing to perform <b>%s</b> at the coordinates %d,%d</a>.<br><br>", cmdGhostopName[specop], x, y );


// we need a table here.
 httpString( cnt, "<i>" );
 if( specop == CMD_INCANT_SENSE )
  httpString( cnt, "Your ghost ships will attempt to locate a nearby artefact from the system." );
 else if( specop == CMD_INCANT_SURVEY )
  httpString( cnt, "Your forces will attempt to determine the size, resource bonus and the precense of portals for each planet of the system." );
 else if( specop == CMD_INCANT_SHIELDING )
  httpString( cnt, "Your ghost ships will generate shielding protecting defensive units for the planet specified." );
 else if( specop == CMD_INCANT_FORCEFIELD )
  httpString( cnt, "A Portal force field makes portal travel very difficult for the targeted planet, reducing the owner capabilities to protect it." );
 else if( specop == CMD_INCANT_VORTEX )
  httpString( cnt, "A vortex portal allows your forces to travel through it to attack, defend nearby planets or return to the main fleet." );
 else if( specop == CMD_INCANT_MINDCONTROL )
  httpString( cnt, "The ghost ships will attempt to seize control of the targeted planet, through the conversion of leaders and automatic systems. Most of the population is usually killed in the process." );
 else if( specop == CMD_INCANT_ENERGYSURGE )
  httpString( cnt, "Spreads a destructive wave in a faction network, feeding on its energy reserves, destroying reserves, research and buildings producing energy." );


 httpString( cnt, "</i><br><br>" );

 if( cmdFindDistPortal( id, x, y, &a, 0 ) <= -2 )
 {
  httpString( cnt, "<i>You don't have any portal to send the ghost ships from!</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }
 a = (int)( (float)a / cmdRace[maind.raceid].speed ) >> 8;
	if( !( a ) )
  httpPrintf( cnt, "Your %d %s could reach this planet instantaneously", ghosts, cmdUnitName[CMD_UNIT_GHOST] );
 else
  httpPrintf( cnt, "Your %d %s would reach this planet in %d weeks", ghosts, cmdUnitName[CMD_UNIT_GHOST], a );


 grl = specopGhostsReadiness( specop, &maind, ( planetd.owner != -1 ) ? ( &main2d ) : ( 0 ) );
 if( grl == -1 )
 {
  httpPrintf( cnt, ". Unfortunately, we don't have enough %s to perform %s.", cmdResearchName[CMD_RESEARCH_WELFARE], cmdGhostopName[specop] );
  iohtmlBodyEnd( cnt );
  return;
 }
 httpPrintf( cnt, " , performing the operation would use %d%% Psychic Readiness.<br><br>", grl >> 16 );


 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\">", URLAppend( cnt, "incantsend" ) );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"X\" value=\"%d\"><input type=\"hidden\" name=\"Y\" value=\"%d\">", x, y );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"Z\" value=\"%d\"><input type=\"hidden\" name=\"ghosts\" value=\"%d\">", z, ghosts );
 httpPrintf( cnt, "<input type=\"hidden\" name=\"order\" value=\"%d\">", ( specop + CMD_FLEET_ORDER_FIRSTINCANT ) );
 httpString( cnt, "<input type=\"submit\" value=\"Send ghost ships\" size=\"10\"></form>" );

 iohtmlBodyEnd( cnt );
 return;
}



void iohtmlFunc_operationsend( ReplyDataPtr cnt )
{
 int a, id, order, x, y, z, agents;
 dbUserMainDef maind;
 char *orderstring, *xstring, *ystring, *zstring, *agentstring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Sending agents" );

 orderstring = iohtmlVarsFind( cnt, "order" );
 xstring = iohtmlVarsFind( cnt, "X" );
 ystring = iohtmlVarsFind( cnt, "Y" );
 zstring = iohtmlVarsFind( cnt, "Z" );
 agentstring = iohtmlVarsFind( cnt, "agents" );

 if( !( orderstring ) || ( sscanf( orderstring, "%d", &order ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( xstring ) || ( sscanf( xstring, "%d", &x ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( ystring ) || ( sscanf( ystring, "%d", &y ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( zstring ) || ( sscanf( zstring, "%d", &z ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( agentstring ) || ( sscanf( agentstring, "%d", &agents ) != 1 ) )
 {
  iohttpFunc_fleetsendL0:
  httpString( cnt, "<i>Invalid order or coordinates</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 if( ( a = cmdExecSendAgents( id, x, y, z, order, agents ) ) < 0 )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error encountered when sending agents</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 httpString( cnt, "<i>Fleet sent!</i><br><br>" );
 if( a )
  httpPrintf( cnt, "The agents will reach their destination in %d weeks<br>", a );
 else
 {
  httpString( cnt, "The agents reached their destination<br>" );
  if( !( cmdFleetActionNewd[1] & CMD_NEWS_FLAGS_NEW ) )
   iohtmlNewsString( cnt, cmdFleetActionNewd );
 }

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_spellsend( ReplyDataPtr cnt )
{
 int id, order, psychics, targetid;
 dbUserMainDef maind;
 char *orderstring, *psychicstring, *targetstring;
 int64_t newd[DB_USER_NEWS_BASE];

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Casting spell" );

 orderstring = iohtmlVarsFind( cnt, "order" );
 psychicstring = iohtmlVarsFind( cnt, "psychics" );
 targetstring = iohtmlVarsFind( cnt, "target" );

 if( !( orderstring ) || ( sscanf( orderstring, "%d", &order ) != 1 ) )
  goto iohttpFunc_spellsendL0;
 if( !( targetstring ) || ( sscanf( targetstring, "%d", &targetid ) != 1 ) )
  goto iohttpFunc_spellsendL0;
 if( !( psychicstring ) || ( sscanf( psychicstring, "%d", &psychics ) != 1 ) )
 {
  iohttpFunc_spellsendL0:
  httpString( cnt, "<i>Invalid order or coordinates</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 newd[0] = ticks.number;
 newd[1] = 0;
 specopPsychicsPerformOp( id, targetid, order, psychics, newd );

 iohtmlNewsString( cnt, newd );

 httpString( cnt, "<br>" );

 iohtmlBodyEnd( cnt );
 return;
}


void iohtmlFunc_incantsend( ReplyDataPtr cnt )
{
 int a, id, order, x, y, z, ghosts;
 dbUserMainDef maind;
 char *orderstring, *xstring, *ystring, *zstring, *ghoststring;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
 iohtmlBase( cnt, 1 );

 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Sending ghost ships" );

 orderstring = iohtmlVarsFind( cnt, "order" );
 xstring = iohtmlVarsFind( cnt, "X" );
 ystring = iohtmlVarsFind( cnt, "Y" );
 zstring = iohtmlVarsFind( cnt, "Z" );
 ghoststring = iohtmlVarsFind( cnt, "ghosts" );

 if( !( orderstring ) || ( sscanf( orderstring, "%d", &order ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( xstring ) || ( sscanf( xstring, "%d", &x ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( ystring ) || ( sscanf( ystring, "%d", &y ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( zstring ) || ( sscanf( zstring, "%d", &z ) != 1 ) )
  goto iohttpFunc_fleetsendL0;
 if( !( ghoststring ) || ( sscanf( ghoststring, "%d", &ghosts ) != 1 ) )
 {
  iohttpFunc_fleetsendL0:
  httpString( cnt, "<i>Invalid order or coordinates</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 if( ( a = cmdExecSendGhosts( id, x, y, z, order, ghosts ) ) < 0 )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error encountered when sending ghosts</i><br>" );
  iohtmlBodyEnd( cnt );
  return;
 }

 httpString( cnt, "<i>Fleet sent!</i><br><br>" );
 if( a )
  httpPrintf( cnt, "The ghosts will reach their destination in %d weeks<br>", a );
 else
 {
  httpString( cnt, "The ghosts reached their destination<br>" );
  if( !( cmdFleetActionNewd[1] & CMD_NEWS_FLAGS_NEW ) )
   iohtmlNewsString( cnt, cmdFleetActionNewd );
 }

 iohtmlBodyEnd( cnt );
 return;
}







void iohtmlFunc_research( ReplyDataPtr cnt )
{
 int a, b, id, cmd[3];
 char fundstring[128] = {0};
 char *fund;
 char *rschptr[CMD_RESEARCH_NUMUSED];
 int rschvalue[CMD_RESEARCH_NUMUSED];
 char rschname[16];
 dbUserMainDef maind;

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
  return;
if( dbUserMainRetrieve( id, &maind ) < 0 )
	return;

 for( a = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ )
 {
  sprintf( rschname, "rsch%d", a );
  rschptr[a] = iohtmlVarsFind( cnt, rschname );
 }
 fund = iohtmlVarsFind( cnt, "fund" );
 
if( ( fund ) && ( sscanf( fund, "%d", &a ) == 1 ) ) {
	if( a > maind.ressource[CMD_RESSOURCE_ENERGY] ) {
		sprintf( fundstring, "<i>%s</i><br><br>", "You don't have so much energy" );
	} else {
		maind.fundresearch += a;
		maind.ressource[CMD_RESSOURCE_ENERGY] -= a;
		if( dbUserMainSet( id, &maind ) < 0 ) {
			sprintf( fundstring, "%s", "<i>Error while funding research</i><br><br>" );
		} else {
			sprintf( fundstring, "<i>Research funding increased by %d</i><br><br>", a );
		}
	}
}


 iohtmlBase( cnt, 1 );
 if( !( iohtmlHeader( cnt, id, &maind ) ) )
  return;
 iohtmlBodyInit( cnt, "Research" );
 

 for( a = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ )
 {
  if( !( rschptr[a] ) || ( sscanf( rschptr[a], "%d", &rschvalue[a] ) < 1 ) )
   goto iohttpFunc_researchL0;
 }
 cmd[0] = CMD_SET_RESEARCH;
 cmd[1] = id;
 if( ( b = cmdExecute( cnt, cmd, rschvalue, 0 ) ) < 0 )
 {
  if( cmdErrorString )
   httpPrintf( cnt, "<i>%s</i><br><br>", cmdErrorString );
  else
   httpString( cnt, "<i>Error while setting research</i><br><br>" );
 }
 else
 {
  httpString( cnt, "<i>Research priorities changed</i><br><br>" );
  memcpy( maind.allocresearch, rschvalue, CMD_RESEARCH_NUMUSED*sizeof(int) );
 }
 iohttpFunc_researchL0:
 if( fundstring[0] )
  httpString( cnt, fundstring );

 httpPrintf( cnt, "Increase fundings<br><form action=\"%s\" method=\"POST\"><input type=\"text\" name=\"fund\" size=\"12\">&nbsp;<input type=\"submit\" value=\"Fund\"></form><br>Current research fundings : %lld<br><br>", URLAppend( cnt, "research" ), (long long)maind.fundresearch );

 httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><table width=\"90%%\" cellspacing=\"8\">", URLAppend( cnt, "research" ) );
 for( a = 0 ; a < CMD_RESEARCH_NUMUSED ; a++ )
  httpPrintf( cnt, "<tr><td nowrap><b>%s</b></td><td nowrap>%lld Points</td><td nowrap>%lld%%</td><td>%s</td><td><input type=\"text\" name=\"rsch%d\" value=\"%d\" size=\"6\">%%</td></tr>", cmdResearchName[a], (long long)maind.research[a], (long long)maind.totalresearch[a], cmdResearchDesc[a], a, maind.allocresearch[a] );
 httpString( cnt, "<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><input type=\"submit\" value=\"Change\"></td></tr></table></form>" );

 iohtmlBodyEnd( cnt );
 return;
}



#define IOHTTP_MAIL_MAXMESSAGES (256)

void iohtmlFunc_mail( ReplyDataPtr cnt ) {
	int a, b, skip, id, type, msent;
	dbUserMainDef maind, main2d;
	dbMailPtr mails;
	dbMailDef maild;
	char *tostring, *mailstring, *typestring, *deletestring, *deleteallstring, *skipstring;
	char timebuf[512];
	int64_t newd[DB_USER_NEWS_BASE];

if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
	return;

iohtmlBase( cnt, 1 );


tostring = iohtmlVarsFind( cnt, "to" );
mailstring = iohtmlVarsFind( cnt, "mail" );
typestring = iohtmlVarsFind( cnt, "type" );
deletestring = iohtmlVarsFind( cnt, "delete" );
deleteallstring = iohtmlVarsFind( cnt, "deleteall" );
skipstring = iohtmlVarsFind( cnt, "skip" );

if( !( iohtmlHeader( cnt, id, &maind ) ) )
	return;

iohtmlBodyInit( cnt, "Mail" );

httpPrintf( cnt, "<a href=\"%s&type=0\">Received messages</a> - ", URLAppend( cnt, "mail" ) );
httpPrintf( cnt, "<a href=\"%s&type=1\">Sent messages</a> - ", URLAppend( cnt, "mail" ) );
httpPrintf( cnt, "<a href=\"%s\">Write a message</a>", URLAppend( cnt, "mail" ) );

if( typestring ) {
	type = 0;
	sscanf( typestring, "%d", &type );
	httpPrintf( cnt, " - <a href=\"%s&type=%d&deleteall=1\">Delete all</a>", URLAppend( cnt, "mail" ), type );
}

httpString( cnt, "<br><br>" );

skip = 0;
if( ( skipstring ) )
	sscanf( skipstring, "%d", &skip );

msent = 0;
maild.text = 0;

if( ( tostring ) && ( mailstring ) ) {
	if( ( maild.text = malloc( 2*MAIL_MAX ) ) ) {
		a = 0;
		if(id != -1 ) {
			if((cnt->session)->dbuser) {
				if(((cnt->session)->dbuser)->level >= LEVEL_MODERATOR )
					a = 1;
			}
		}
		iohttpForumFilter( &maild.text[MAIL_MAX], mailstring, MAIL_MAX, a );
		(maild.mail).length = iohttpForumFilter2( maild.text, &maild.text[MAIL_MAX], MAIL_MAX );

		a = -1;
		if( ( tostring[0] >= '0' ) && ( tostring[0] <= '9' ) ) {
			sscanf( tostring, "%d", &a );
		} else {
			for( a = 0 ; tostring[a] ; a++ ) {
				if( tostring[a] == '+' )
					tostring[a] = ' ';
				else if( ( tostring[a] == 10 ) || ( tostring[a] == 13 ) )
					tostring[a] = 0;
			}
			a = dbUserSearchFaction( tostring );
		}
		if( a < 0 ) {
			httpPrintf( cnt, "<i>The faction %s does not seem to exist, the syntax must be exact or use the user ID.</i><br><br>", tostring );
		} else {
			if( dbUserMainRetrieve( a, &main2d ) < 0 ) {
				httpString( cnt, "<i>Are you sure this user exist?</i><br><br>" );
			} else {
				(maild.mail).authorid = id;
				sprintf( (maild.mail).authorname, "%s", ((cnt->session)->dbuser)->faction );
				(maild.mail).authorempire = maind.empire;
				(maild.mail).time = time( 0 );
				(maild.mail).tick = ticks.number;
				(maild.mail).flags = 0;
				if( dbMailAdd( a, MAIL_IN, &maild ) < 0 ) {
					httpString( cnt, "<i>Error while sending message</i><br><br>" );
				} else {
					httpPrintf( cnt, "<i>Message sent to <a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), a, main2d.faction );
					httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a></i><br><br>", URLAppend( cnt, "empire" ), main2d.empire, main2d.empire );
				}
				newd[0] = ticks.number;
				newd[1] = CMD_NEWS_FLAGS_NEW;
				newd[2] = CMD_NEWS_MAIL;
				newd[3] = 0;
				newd[4] = id;
				newd[5] = maind.empire;
				int __damn_overflows = USER_NAME_MAX / 2 + 3 * 2;
				memcpy( &newd[6], maind.faction, __damn_overflows );
				cmdUserNewsAdd( a, newd, CMD_NEWS_FLAGS_MAIL );
				(maild.mail).authorid = a;
				sprintf( (maild.mail).authorname, "%s", main2d.faction );
				(maild.mail).authorempire = main2d.empire;
				dbMailAdd( id, MAIL_OUT, &maild );
				msent = 1;
			}
		}
	}
}

if( maild.text )
	free( maild.text );

if( typestring ) {
 	if( deleteallstring ) {
		dbMailEmpty( id, ( type ? MAIL_OUT : MAIL_IN ) );
		deletestring = 0;
	}
	if( type == MAIL_IN ) {
		if( ( deletestring ) && ( sscanf( deletestring, "%d", &a ) == 1 ) ) {
			if( dbMailRemove( id, MAIL_IN, a ) >= 0 )
				httpString( cnt, "<i>Message deleted</i><br><br>" );
			else
				httpString( cnt, "<i>Error while deleting message</i><br><br>" );
		}
		b = dbMailList( id, MAIL_IN, 0, IOHTTP_MAIL_MAXMESSAGES, &mails, &a );
		//httpPrintf( cnt, "<i>%d / %d Messages Displayed</i><br><br>", b, a );
		if( a > IOHTTP_MAIL_MAXMESSAGES )
			httpPrintf( cnt, "<i>Your mailbox seems crowded! You have %d messages waiting that won't be displayed until you delete some old messages.</i><br><br>", a - b );
		if( b < 0 ) {
			httpString( cnt, "Error retrieving mailbox" );
			iohtmlBodyEnd( cnt );
			return;
		}
		if( b == 0 )
			httpString( cnt, "<br><b>No messages</b><br>" );
		for( b-- ; b >= 0 ; b-- ) {
			strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &(mails[b].mail).time ) );
			httpString( cnt, "<table width=\"80%%\"><tr><td><b>From</b> : " );
			httpPrintf( cnt, "<a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), (mails[b].mail).authorid, (mails[b].mail).authorname );
			httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a></td><td align=\"right\">", URLAppend( cnt, "empire" ), (mails[b].mail).authorempire, (mails[b].mail).authorempire );
			httpPrintf( cnt, "<a href=\"%s&type=0&delete=%d\">Delete</a></td></tr><tr><td><b>Received</b> : Week %d, Year %d - %s</td><td align=\"right\">", URLAppend( cnt, "mail" ), b, (mails[b].mail).tick % 52, (mails[b].mail).tick / 52, timebuf );
			httpPrintf( cnt, "<a href=\"%s&to=%d\">Reply</a></td></tr><tr><td colspan=\"2\">", URLAppend( cnt, "mail" ), (mails[b].mail).authorid );
			httpString( cnt, mails[b].text );
			httpString( cnt, "</td></tr></table><br><br>" );
			if( mails[b].text )
				free( mails[b].text );
		}
		if( mails )
			free( mails );
	} else if( type == MAIL_OUT ) {
		if( ( deletestring ) && ( sscanf( deletestring, "%d", &a ) == 1 ) ) {
			if( dbMailRemove( id, MAIL_OUT, a ) >= 0 )
				httpString( cnt, "<i>Message deleted</i><br><br>" );
			else
				httpString( cnt, "<i>Error while deleting message</i><br><br>" );
		}
		b = dbMailList( id, MAIL_OUT, 0, IOHTTP_MAIL_MAXMESSAGES, &mails, &a );
		if( a > IOHTTP_MAIL_MAXMESSAGES )
			httpPrintf( cnt, "<i>Your mailbox seems crowded! There are %d sent messages that won't be displayed until you delete some old messages.</i><br><br>", a - b );
		if( b < 0 ) {
			httpString( cnt, "Error retrieving mailbox" );
			iohtmlBodyEnd( cnt );
			return;
		}
		if( b == 0 )
			httpString( cnt, "<br><b>No messages</b><br>" );
		for( b-- ; b >= 0 ; b-- ) {
			strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &(mails[b].mail).time ) );
			httpString( cnt, "<table width=\"80%%\"><tr><td><b>From</b> : " );
			httpPrintf( cnt, "<a href=\"%s&id=%d\">%s</a> of ", URLAppend( cnt, "player" ), (mails[b].mail).authorid, (mails[b].mail).authorname );
			httpPrintf( cnt, "<a href=\"%s&id=%d\">empire #%d</a></td><td align=\"right\">", URLAppend( cnt, "empire" ), (mails[b].mail).authorempire, (mails[b].mail).authorempire );
			httpPrintf( cnt, "<a href=\"%s&type=0&delete=%d\">Delete</a></td></tr><tr><td><b>Received</b> : Week %d, Year %d - %s</td><td align=\"right\">", URLAppend( cnt, "mail" ), b, (mails[b].mail).tick % 52, (mails[b].mail).tick / 52, timebuf );
			httpPrintf( cnt, "<a href=\"%s&to=%d\">Reply</a></td></tr><tr><td colspan=\"2\">", URLAppend( cnt, "mail" ), (mails[b].mail).authorid );
			httpString( cnt, mails[b].text );
			httpString( cnt, "</td></tr></table><br><br>" );
			if( mails[b].text )
				free( mails[b].text );
		}
		if( mails )
			free( mails );
	}
} else if( msent == 1 ) { 
	if( ( mailstring ) ) {
		if( ( maild.text = malloc( 2*MAIL_MAX ) ) ) {
			a = 0;
			if((cnt->session)->dbuser) {
				if(id != -1 )
					a = 1;
			}
			iohttpForumFilter( &maild.text[MAIL_MAX], mailstring, MAIL_MAX, a );
			(maild.mail).length = iohttpForumFilter3( maild.text, &maild.text[MAIL_MAX], MAIL_MAX );
			httpString( cnt, maild.text );
			free( maild.text );
			httpString( cnt, "<br>" );
		}
	} else {
		httpString( cnt, "Strange, you sent a blank message?" );
	}
} else {
	httpString( cnt, "To specify the player to send the message to, enter either the exact faction name or the user ID.<br><br>" );
	httpPrintf( cnt, "<form action=\"%s\" method=\"POST\"><table width=\"50%%\" cellspacing=\"3\"><tr><td width=\"20%%\">Recipient</td><td>", URLAppend( cnt, "mail" ) );
	if( tostring ) {
		for( a = 0 ; ( a < USER_NAME_MAX-1 ) && ( tostring[a] ) ; a++ ) {
			if( tostring[a] == '+' )
				tostring[a] = ' ';
			else if( ( tostring[a] == 10 ) || ( tostring[a] == 13 ) )
				tostring[a] = 0;
		}
		tostring[a] = 0;
		httpPrintf( cnt, "<input type=\"text\" name=\"to\" size=\"32\" value=\"%s\">", tostring );
	} else {
		httpString( cnt, "<input type=\"text\" name=\"to\" size=\"32\">" );
	}
	httpString( cnt, "</td></tr><tr><td>Message</td><td><textarea name=\"mail\" wrap=\"soft\" rows=\"10\" cols=\"60\">" );
	if( ( mailstring ) ) {
		if( ( maild.text = malloc( 2*MAIL_MAX ) ) ) {
			a = 0;
			if((cnt->session)->dbuser) {
				if(id != -1 )
					a = 1;
			}
			iohttpForumFilter( &maild.text[MAIL_MAX], mailstring, MAIL_MAX, a );
			(maild.mail).length = iohttpForumFilter3( maild.text, &maild.text[MAIL_MAX], MAIL_MAX );
			httpString( cnt, maild.text );
			free( maild.text );
		}
	}
	httpString( cnt, "</textarea></td></tr><tr><td></td><td><input type=\"submit\" value=\"Send\"></td></tr></table></form>" );
}

iohtmlBodyEnd( cnt );


return;
}




//New rankings output,  fmt 0 = html, fmt 1 = plaintext, typ 0 = Faction ranks, typ 1 = Empire ranks, rnd (unset) = current round, rnd (set) = display for round #rnd (if it exists)
void iohtmlFunc_rankings( ReplyDataPtr cnt ) {
	int id, round, type, format;
	FILE *file;
	struct stat stdata;
	char *data, *roundstring, *typestring, *formatstring;
	char COREDIR[PATH_MAX];
	dbUserMainDef maind;
	ConfigArrayPtr settings;

type = format = 0;
round = ticks.round;


formatstring = iohtmlVarsFind( cnt, "fmt" );
roundstring = iohtmlVarsFind( cnt, "rnd" );
typestring = iohtmlVarsFind( cnt, "typ" );



if( roundstring ) {
	round = atoi(roundstring);
}

if( typestring ) {
	type = atoi(typestring);
}

if( formatstring ) {
	if( 0 == strcmp("plain", formatstring) ) {
		format = true;
		goto PLAINTEXT;
	}
}

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	iohtmlBase( cnt, 1 );
	if( !( iohtmlHeader( cnt, id, &maind ) ) )
		return;
} else {
	iohtmlBase( cnt, 8 );
	iohtmlFunc_frontmenu( cnt, FMENU_NONE );
}


if( (round != ticks.round) ) {
	iohtmlBodyInit( cnt, "%s rankings for Round #%d", ( type ? "Empire" : "Faction" ), round );
} else {
	iohtmlBodyInit( cnt, "%s rankings", ( type ? "Empire" : "Faction" ) );
}

PLAINTEXT:
settings = GetSetting( "Directory" );
sprintf( COREDIR, "%s/rankings/round%d%sranks%s.txt", settings->string_value, round, ( type ? "fam" : "" ), ( format ? "plain" : "" )  );

if( stat( COREDIR, &stdata ) != -1 ) {
	if( ( data = malloc( stdata.st_size + 1 ) ) ) {
		data[stdata.st_size] = 0;
		if( ( file = fopen( COREDIR, "rb" ) ) ) {
			if( ( fread( data, 1, stdata.st_size, file ) < 1 ) && ( stdata.st_size ) ) {
				error( "Failure reading round rankings infomation: %s", COREDIR );
			} else {
				/*
				 * So this might not be the best way of doing things
				 * But it saves hard calling the ranks each time.
				 * This will append all links in the rankings
				 */
				char temp[REDIRECT_MAX];
				snprintf( temp, REDIRECT_MAX, "%s&", URLAppend( cnt, "empire") );
				char *block = str_replace(data, "empire?", temp );
				if( block ) {
					snprintf( temp, REDIRECT_MAX, "%s&", URLAppend( cnt, "player") );
					char *block2 = str_replace( block, "player?", temp );
					if( block2 ) {
						httpString( cnt, block2 );
						free( block2 );
					} else {
						free( block );
						goto SKIP;
					}
				free( block );
				} else {
					SKIP:
					httpString( cnt, data );
				}
					
			}
			fclose( file );
		}
		free( data );
	}
}
if( format ) {
	goto PLAINEXIT;
}

iohtmlBodyEnd( cnt );

PLAINEXIT:
return;
}




void iohtmlFunc_search( ReplyDataPtr cnt )
{
 int id, a, x, y, z, status, sysid;
 dbUserMainDef maind;
 char *search, *str0, *str1, *str2, *error;
 dbMainSystemDef systemd;
 char content[256], buf0[256], buf1[256];
 dbUserPtr user;

 search = iohtmlVarsFind( cnt, "search" );
 
 status = 0;
 str0 = 0;
 error = 0;
 if( !( search ) || !( search[0] ) )
 {
  iohttpFunc_searchL0:
  iohtmlBase( cnt, 1 );
  if( ( id = iohtmlIdentify( cnt, 1|2 ) ) < 0 )
   return;
  if( !( iohtmlHeader( cnt, id, &maind ) ) )
   return;
  iohtmlBodyInit( cnt, "Search" );

  if( status & 1 )
  {
   if( error )
    httpString( cnt, error );
   else
    httpString( cnt, "Unrecognized query format<br><br>" );
  }
  else if( status & 4 )
  {
   httpPrintf( cnt, "Player search with <i>%s</i> :<br><table><tr><td>", content );
   iohttpCaseLow( buf0, content );
   a = 0;
   for( user = dbUserList ; user ; user = user->next )
   {
    if( !( bitflag( user->flags, CMD_USER_FLAGS_ACTIVATED ) ) )
     continue;
    if( a == 256 )
    {
     httpString( cnt, "<br><i>Too many results!</i>" );
     break;
    }
    iohttpCaseLow( buf1, user->faction );
    if( !( ioCompareFindWords( buf1, buf0 ) ) )
     continue;
    httpPrintf( cnt, "<a href=\"%s&id=%d\">%s</a><br>", URLAppend( cnt, "player" ), user->id, user->faction );
    a++;
   }
   if( a == 0 )
    httpString( cnt, "No results<br>" );
   httpString( cnt, "</td></tr></table><br>" );
  }

  httpPrintf( cnt, "<form action=\"%s\" method=\"POST\" target=\"main\"><input type=\"text\" name=\"search\" size=\"32\" value=\"", URLAppend( cnt, "search" ) );
  if( str0 )
   httpString( cnt, str0 );
  else
   httpString( cnt, "Search string" );
  httpString( cnt, "\"><input type=\"submit\" size=\"2\" value=\"Search\"></form><br><br>" );

  httpString( cnt, "<table><tr><td>" );
  httpPrintf( cnt, "<b>To find an empire</b><br>Specify the empire number in the search field, as <b>%d</b> or <b>#%d</b>.<br><br>", rand() % dbMapBInfoStatic[MAP_EMPIRES], rand() % dbMapBInfoStatic[MAP_EMPIRES] );
  httpPrintf( cnt, "<b>To find a system</b><br>Enter the system coordinates in the format <b>x,y</b>, as <b>%d,%d</b>.<br><br>", rand() % dbMapBInfoStatic[MAP_SIZEX], rand() % dbMapBInfoStatic[MAP_SIZEY] );
  httpPrintf( cnt, "<b>To find a planet</b><br>Enter the planet coordinates in the format <b>x,y:z</b>, as <b>%d,%d:%d</b>.<br><br>", rand() % dbMapBInfoStatic[MAP_SIZEX], rand() % dbMapBInfoStatic[MAP_SIZEY], rand() & 15 );
  httpPrintf( cnt, "<b>To see a map sector</b><br>To zoom on a specific area on the map, enter coordinates in the format <b>!x,y</b>, as <b>!%d,%d</b>.<br><br>", rand() % dbMapBInfoStatic[MAP_SIZEX], rand() % dbMapBInfoStatic[MAP_SIZEY] );
  httpString( cnt, "<b>To search an faction name</b><br>To search an faction from its name, enter the partial or full name, it is not case sensitive.<br><br>" );
  httpPrintf( cnt, "<b>To find an faction</b><br>Enter the faction ID number in the format <b>@ID</b>, as <b>@%d</b>.<br><br>", rand() & 0xFF );
  httpPrintf( cnt, "<b>To attack a planet</b><br>Enter the planet coordinates in the format <b>&x,y:z</b>, as <b>&%d,%d:%d</b>.<br><br>", rand() % dbMapBInfoStatic[MAP_SIZEX], rand() % dbMapBInfoStatic[MAP_SIZEY], rand() & 15 );
  httpPrintf( cnt, "<b>To perform a special operation on a planet</b><br>Enter the planet coordinates in the format <b>*x,y:z</b>, as <b>*%d,%d:%d</b>.<br><br>", rand() % dbMapBInfoStatic[MAP_SIZEX], rand() % dbMapBInfoStatic[MAP_SIZEY], rand() & 15 );
  httpString( cnt, "</td></tr></table>" );

  iohtmlBodyEnd( cnt );
 }
 else
 {
status = 1;
  iohttpForumFilter( content, search, 256, 0 );
  str0 = content;

  // attack
  if( str0[0] == '&' )
  {
   status |= 8;
   str0++;
  }
  // spec
  if( str0[0] == '*' )
  {
   status |= 16;
   str0++;
  }

  // system or planet
  if( ( str1 = ioCompareFindWords( str0, "," ) ) )
  {
   if( ( str2 = ioCompareFindWords( str0, "!" ) ) )
   {
    str0 = str2;
    status |= 2;
   }
   if( sscanf( str0, "%d", &x ) != 1 )
    goto iohttpFunc_searchL0;
   if( sscanf( str1, "%d", &y ) != 1 )
    goto iohttpFunc_searchL0;
   if( status & 2 )
   {
    sprintf( content, "%d,%d", x*IOHTTP_MAPPICK_DIVIDE, y*IOHTTP_MAPPICK_DIVIDE );
    MHD_set_connection_value(cnt->connection,  MHD_GET_ARGUMENT_KIND, "sectorzoom", content );
    iohtmlFunc_map( cnt );
    goto iohttpFunc_searchL1;
   }
   if( ( sysid = dbMapFindSystem( x, y ) ) < 0 )
   {
    error = "There is no system at the coordinates specified<br><br>";
    goto iohttpFunc_searchL0;
   }
   if( dbMapRetrieveSystem( sysid, &systemd ) < 0 ) {
    goto iohttpFunc_searchL0;
    }
   if( ( str2 = ioCompareFindWords( str1, ":" ) ) )
   {
    if( sscanf( str2, "%d", &z ) != 1 )
     goto iohttpFunc_searchL0;
    if( (unsigned int)z >= systemd.numplanets )
    {
     error = "There is no planet at the coordinates specified<br><br>";
     goto iohttpFunc_searchL0;
    }
    sprintf( content, "%d", systemd.indexplanet + z );
    MHD_set_connection_value(cnt->connection,  MHD_GET_ARGUMENT_KIND, "id", content );
    if( status & 8 )
     iohtmlFunc_attack( cnt );
    else if( status & 16 )
     iohtmlFunc_spec( cnt );
    else
     iohtmlFunc_planet( cnt );
    goto iohttpFunc_searchL1;
   }
   sprintf( content, "%d", sysid );
   MHD_set_connection_value(cnt->connection,  MHD_GET_ARGUMENT_KIND, "id", content );
   iohtmlFunc_system( cnt );
   goto iohttpFunc_searchL1;
  }

  // empire
  if( ( str0[0] >= '0' ) && ( str0[0] <= '9' ) )
  {
   iohttpFunc_searchL2:
   if( sscanf( str0, "%d", &a ) != 1 )
    goto iohttpFunc_searchL0;
   sprintf( content, "%d", a );
   MHD_set_connection_value(cnt->connection,  MHD_GET_ARGUMENT_KIND, "id", content);
   iohtmlFunc_empire( cnt );
   goto iohttpFunc_searchL1;
  }
  if( str0[0] == '#' )
  {
   str0++;
   goto iohttpFunc_searchL2;
  }

  // player
  if( str0[0] == '@' )
  {
   if( sscanf( &str0[1], "%d", &a ) != 1 )
    goto iohttpFunc_searchL0;
   sprintf( content, "%d", a );
   MHD_set_connection_value(cnt->connection,  MHD_GET_ARGUMENT_KIND, "id", content);
   iohtmlFunc_player( cnt );
   goto iohttpFunc_searchL1;
  }

  if( ( ( str0[0] >= 'a' ) && ( str0[0] <= 'z' ) ) || ( ( str0[0] >= 'A' ) && ( str0[0] <= 'Z' ) ) )
   status = 4;

  goto iohttpFunc_searchL0;
 }
 
 iohttpFunc_searchL1:

 return;
}


