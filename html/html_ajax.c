//Ok, so here goes nothing...
void iohtmlFunc_ajax( ReplyDataPtr cnt ) {
	#if IRCBOT_SUPPORT
	ConfigArrayPtr settings[2];
	#endif
	int a, c, id, numbuild;
	int64_t b;
	int64_t bsums[CMD_BLDG_NUMUSED+1];
	int64_t usums[CMD_UNIT_NUMUSED];
	const char *typestring, *idstring, *refer;
	char CHECKER[256];
	char timebuf[512];
	dbMainEmpireDef empired;
	dbBuildPtr build;
	dbUserMainDef maind;
	proginfoDef pinfod;
	urlinfoPtr urlp;
	cpuInfo cpuinfo;
	time_t tint;
	struct sysinfo sysinfod;

refer = idstring = typestring = NULL;
cpuGetInfo( &cpuinfo );
getsys_infos( &pinfod );
if( sysinfo(&sysinfod) != 0 ) {
	critical( "Failure getting system infomation... Critical failure." );
	sysconfig.shutdown = true; return;
}

idstring = iohtmlVarsFind( cnt, "id" );
typestring = iohtmlVarsFind( cnt, "typ" );

refer = iohtmlHeaderFind(cnt, "Referer");

if( ( id = iohtmlIdentify( cnt, 2 ) ) >= 0 ) {
	if( dbUserMainRetrieve( id, &maind ) < 0 )
		goto BAILAJAX;
	else if( dbEmpireGetInfo( maind.empire, &empired ) < 0 )
		goto BAILAJAX;

}

if( ( typestring ) && ( refer ) ) {
	urlp = parse_url(refer);
	refer = urlp->path;
	httpString( cnt, "<?xml version=\"1.0\"?>" );
	httpPrintf( cnt, "<xml>" );
	//Begin XML generation, we only make one request now... so we have to structure carefully!
	if( !strcmp(typestring,"ticker") ) {
		//Send basic tick info, and check if user is loged in.
		httpPrintf( cnt, "<pass>%d</pass>", ( id != -1 ) ? ( ( bitflag( ((cnt->session)->dbuser)->flags, CMD_USER_FLAGS_ACTIVATED ) ) ? true : false ) : false );
		if( refer )
			httpPrintf( cnt, "<page>%s</page>", refer );
		httpPrintf( cnt, "<u_online>%d</u_online><u_activated>%d</u_activated>", dbRegisteredInfo[DB_TOTALS_USERS_ONLINE], dbRegisteredInfo[DB_TOTALS_USERS_ACTIVATED] );
		httpPrintf( cnt, "<time><next>%d</next><week>%d</week><year>%d</year></time>", (int)fmax( 0.0, ( ticks.next - time(0) ) ), ticks.number % 52, ticks.number / 52 );
		if( !strcmp(refer,"status") ) {
			snprintf( CHECKER, sizeof(CHECKER), "%lu bytes ( %5.1f mb )", pinfod.stvsize, pinfod.stvsize  / megabyte );
			httpString( cnt, "<general>" );
			httpPrintf( cnt, "<servpriority>%ld</servpriority>", pinfod.stpriority );
			httpPrintf( cnt, "<servthreads>%ld</servthreads>", pinfod.threads );
			httpString( cnt, "</general>" );
			httpString( cnt, "<memory>" );
			httpPrintf( cnt, "<memused>%s</memused>", CHECKER );
			httpPrintf( cnt, "<memavbytes>%ld bytes</memavbytes>", sysinfod.freeram );
			httpPrintf( cnt, "<memavmeg>( %4.1f mb )</memavmeg>", (sysinfod.freeram  / megabyte ) );


			httpPrintf( cnt, "<totalswapbytes>%ld bytes</totalswapbytes>", sysinfod.totalswap );
			httpPrintf( cnt, "<totalswapmeg>( %5.1f mb )</totalswapmeg>", (sysinfod.totalswap / megabyte ) );

			httpPrintf( cnt, "<freeswapbytes>%ld bytes</freeswapbytes>", sysinfod.freeswap );
			httpPrintf( cnt, "<freeswapmeg>( %5.1f mb )</freeswapmeg>", (sysinfod.freeswap  / megabyte ) );

			httpPrintf( cnt, "<bufferbytes>%ld bytes</bufferbytes>", sysinfod.bufferram );
			httpPrintf( cnt, "<bufermeg>( %5.1f mb )</bufermeg>", (sysinfod.bufferram  / megabyte ) );

			httpPrintf( cnt, "<sharedbytes>%ld bytes</sharedbytes>", sysinfod.sharedram );
			httpPrintf( cnt, "<sharedmeg>( %5.1f mb )</sharedmeg>", (sysinfod.sharedram  / megabyte ) );

			time( &tint );
			strftime(timebuf,512,"%a, %d %b %G %T %Z", localtime( &tint ) );
			httpPrintf( cnt, "<timeserver>%s</timeserver>", timebuf );
			strftime(timebuf,512,"%a, %d %b %G %T %Z", gmtime( &tint ) );
			httpPrintf( cnt, "<timegmt>%s</timegmt>", timebuf );
			
			httpPrintf( cnt, "<strss>%lu pages</strss>", pinfod.strss );
			httpPrintf( cnt, "</memory>" );
			httpString( cnt, "<cpu>" );
			httpPrintf( cnt, "<cpuprocs>%d</cpuprocs>", sysinfod.procs );
			httpPrintf( cnt, "<cpuloads>%f (1 min) - %f (5 mins) - %f (15 mins)</cpuloads>",pinfod.loadavg[0],pinfod.loadavg[1],pinfod.loadavg[2]);
			httpPrintf( cnt, "<cputotal>%.3f %%</cputotal>", pinfod.userload + pinfod.kernelload );
			httpPrintf( cnt, "<cpukernel>%.3f %%</cpukernel>", pinfod.kernelload );
			httpPrintf( cnt, "<cpuuser>%.3f %%</cpuuser>", pinfod.userload );
			httpString( cnt, "</cpu>" );
			
			#if IRCBOT_SUPPORT
			if( sysconfig.irc_enabled ) {
				if( irc_is_connected(sysconfig.irc_session) ) {
					settings[0] = GetSetting( "IRC Host" );
					settings[1] = GetSetting( "IRC Channel" );
					snprintf( CHECKER, sizeof(CHECKER), "Enabled (Host:%s, Channel:%s)", settings[0]->string_value, settings[1]->string_value );
				} else {
					snprintf( CHECKER, sizeof(CHECKER), "%s", "Enabled but not connected" );
				}
			} else {
				snprintf( CHECKER, sizeof(CHECKER), "%s", "Disabled");
			}
			httpPrintf( cnt, "<botstatus>%s</botstatus>", CHECKER );
			#endif

		//End Status block -- Start user block
		}
		if( id < 0 )
			goto ENDXML;
		a = dbUserNewsGetFlags( id );
		//OK, so they are loged in... time to send some info. Lets start with the header. =)
		httpString( cnt, "<header>" );
		httpPrintf( cnt, "<networth>%lld</networth>", (long long)maind.networth );
		httpString( cnt, "<notification>" );
		httpPrintf( cnt, "<mail>%d</mail>",  ( a & CMD_NEWS_FLAGS_MAIL ) ? true : false );
		httpPrintf( cnt, "<build>%d</build>",  ( a & CMD_NEWS_FLAGS_BUILD ) ? true : false );
		httpPrintf( cnt, "<aid>%d</aid>", ( a & CMD_NEWS_FLAGS_AID ) ? true : false );
		httpPrintf( cnt, "<fleet>%d</fleet>", ( a & CMD_NEWS_FLAGS_ATTACK ) ? 1 : ( ( a & CMD_NEWS_FLAGS_FLEET ) ? 2 : false ) );
		httpString( cnt, "</notification>" );
		httpString( cnt, "<ressources>" );
		for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ ) {
			snprintf( CHECKER, sizeof(CHECKER),"%s",cmdRessourceName[a]);
			for(b = 0; CHECKER[b]; b++){
				CHECKER[b] = tolower(CHECKER[b]);
			}
			httpPrintf( cnt, "<%s>%lld</%s>", CHECKER, (long long)maind.ressource[a], CHECKER );
		}
		httpPrintf( cnt, "<population>%lld</population>", (long long)maind.ressource[CMD_RESSOURCE_POPULATION] );
		httpString( cnt, "</ressources>" );
		httpString( cnt, "</header>" );
		httpPrintf( cnt, "<%s>", refer );
		//End Header block -- Start HQ block
		if( !strcmp(refer,"hq") ) {
			httpString( cnt, "<readiness>" );
			for( a = 0 ; a < CMD_READY_NUMUSED ; a++ ) {
				httpPrintf( cnt, "<%sready>%d</%sready>", cmdReadyName[a], maind.readiness[a] >> 16, cmdReadyName[a] );
			}
			httpString( cnt, "</readiness>" );
		httpPrintf( cnt, "<planets>%d</planets>", maind.planets );
		//End HQ block -- Start Council block
		} else if( !strcmp(refer,"council") ) {
			httpString( cnt, "<incomes>" );
			httpPrintf( cnt, "<taxlevel>%.02f</taxlevel>", ( empired.taxation * 100 ) );
			for( a = 0 ; a < INFOS_TOTAL_NUMUSED ; a++ ) {
				httpPrintf( cnt, "<%s>%lld</%s>", cmdMainInfoNames[a], (long long)maind.infos[a], cmdMainInfoNames[a] );
			}
			httpString( cnt, "</incomes>" );
			httpString( cnt, "<bldings>" );
			for( a = b = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ ) {
				httpPrintf( cnt, "<bld%d>%lld</bld%d>", a, (long long)maind.totalbuilding[a], a );
				b += maind.totalbuilding[a];
			}
			httpPrintf( cnt, "<bldnum>%lld</bldnum>", (long long)b );
			httpString( cnt, "</bldings>" );
			httpString( cnt, "<allunits>" );
			for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
				httpPrintf( cnt, "<unt%d>%lld</unt%d>", a, (long long)maind.totalunit[a], a );
				b += maind.totalunit[a];
			}
			httpPrintf( cnt, "<untnum>%lld</untnum>", (long long)b );
			httpString( cnt, "</allunits>" );
			
			memset( bsums, 0, (CMD_BLDG_NUMUSED+1)*sizeof(int64_t) );
			memset( usums, 0, CMD_UNIT_NUMUSED*sizeof(int64_t) );
			httpPrintf( cnt, "<council_html_buildings><![CDATA[" );
			if( ( numbuild = dbBuildList( true, id, &build ) ) < 0 ) {
				httpString( cnt, "Error while retriving user build list" );
				httpPrintf( cnt, "]]></council_html_buildings>" );
				return;
			}
			httpPrintf( cnt, "<table><form name=\"cancelbuild\" action=\"%s\">", URLAppend( cnt, "cancelbuild" ) );
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
					httpPrintf( cnt, "%lld %s<br>", (long long)bsums[a], cmdBuildingName[a] );
					b += bsums[a];
				}
				httpPrintf( cnt, "<i>Total of %lld buildings under construction</i><br>", (long long)b );
			}
			httpPrintf( cnt, "]]></council_html_buildings>" );
			httpPrintf( cnt, "<council_html_units><![CDATA[" );
			httpPrintf( cnt, "<table><form name=\"cancelunit\" action=\"%s\">", URLAppend( cnt, "cancelbuild" ) );
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
					httpPrintf( cnt, "%lld %s<br>", (long long)usums[a], cmdUnitName[a] );
					b += usums[a];
  				}
				httpPrintf( cnt, "<i>Total of %lld units under construction</i><br>", (long long)b );
			}
			httpPrintf( cnt, "]]></council_html_units>" );

		//End of Council block
		}
		httpPrintf( cnt, "</%s>", refer );
	}
	ENDXML:
	httpString( cnt, "</xml>" );
	urlinfo_free( urlp );
} else {
	httpString( cnt, "Bad Request\n\n" );
}
return;

BAILAJAX:
if( id != -1 )
httpPrintf( cnt, "Error getting info! %s\n\n", cmdErrorString ? cmdErrorString : "" );

return;
}

void iohtmlFunc_javaforajax( ReplyDataPtr cnt ) {
	int a, b;
	char *refer;
	char CHECKER[256];
	struct sysinfo sysajaxd;
	proginfoDef pinfod;
	timeDef javatime;
	urlinfoPtr urlp;
	cpuInfo cpuinfo;

cpuGetInfo( &cpuinfo );

refer = iohtmlHeaderFind(cnt, "Referer");

if( refer ) {
	urlp = parse_url(refer);
	refer = urlp->path;
	converttime_todef( &javatime, ( ticks.next - time(0) ) );
	httpString( cnt, "//This file is not really here... it's auto-generated by the game code.\n" );
	httpString( cnt, "//I got really sick of having to add all these functions in... so BLA!.\n" );
	httpString( cnt, "//Time to use this to more of its potential!!.\n" );
	httpString( cnt, "var page;\n" );
	httpString( cnt, "var login = false;\n" );
	httpPrintf( cnt, "var sec = %02ld;\n", javatime.seconds );
	httpPrintf( cnt, "var min = %02ld;\n", javatime.minutes );
	if( !strcmp(refer,"status") ) {
		getsys_infos( &pinfod );
		if( sysinfo(&sysajaxd) != 0 ) {
			critical( "Failure getting system infomation... Critical failure." );
			sysconfig.shutdown = true; return;
		}
		converttime_todef( &javatime, pinfod.runtime );
		httpPrintf( cnt, "var gsec = %02ld;\n", javatime.seconds );
		httpPrintf( cnt, "var gmin = %02ld;\n", javatime.minutes );
		httpPrintf( cnt, "var ghour = %02ld;\n", javatime.hours );
		httpPrintf( cnt, "var gday = %02ld;\n", javatime.days );
		converttime_todef( &javatime, sysajaxd.uptime );
		httpPrintf( cnt, "var hsec = %02ld;\n", javatime.seconds );
		httpPrintf( cnt, "var hmin = %02ld;\n", javatime.minutes );
		httpPrintf( cnt, "var hhour = %02ld;\n", javatime.hours );
		httpPrintf( cnt, "var hday = %02ld;\n", javatime.days );
	}
	//OK, Time for some AJAX! =/ ...
	//It's been a while, forgive me if its messy lol! =P
	httpString( cnt, "function getInfo(str) {\n" );
	httpString( cnt, "\tvar xmlhttp;\n" );
	httpString( cnt, "if (str==\"\") {\n" );
	httpString( cnt, "\treturn;\n" );
	httpString( cnt, "}\n" );
	httpString( cnt, "if (window.XMLHttpRequest) { \n" );
	httpString( cnt, "\txmlhttp=new XMLHttpRequest(); \n" );
	httpString( cnt, "} else { \n" );
	httpString( cnt, "\txmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\"); \n" );
	httpString( cnt, "}\n" );
	httpString( cnt, "\n" );
	httpString( cnt, "xmlhttp.onreadystatechange=function() {\n" );
	httpString( cnt, "\tif (xmlhttp.readyState==4 && xmlhttp.status==200) {\n" );
	httpString( cnt, "\t\t//Fetch uinfos tick data, and check if user is loged in... this controls weither we will call more data.\n" );
	httpString( cnt, "\t\tif( getnodevar(xmlhttp.responseXML,\"pass\") > 0 ) { login = true; } else { login = false; }\n" );
	httpString( cnt, "\n" );
	httpString( cnt, "\t\tsec = ( getnodevar(xmlhttp.responseXML,\"next\") % 60 );\n" );
	httpString( cnt, "\t\tmin = Math.floor( ( getnodevar(xmlhttp.responseXML,\"next\") / 60 ) );\n" );
	httpString( cnt, "\t\tpage = getnodevar(xmlhttp.responseXML,\"page\");\n" );
	httpString( cnt, "\n" );
	httpString( cnt, "\t\tvar week = getnodevar(xmlhttp.responseXML,\"week\");\n" );
	httpString( cnt, "\t\tvar year = getnodevar(xmlhttp.responseXML,\"year\");\n" );
	httpString( cnt, "\t\tvar u_online = getnodevar(xmlhttp.responseXML,\"u_online\");\n" );
	httpString( cnt, "\t\tvar u_activated = getnodevar(xmlhttp.responseXML,\"u_activated\");\n" );
	httpString( cnt, "\n" );
	if( ( strlen( refer ) == 0 ) || ( strcmp(refer,"faq") == 0 ) || ( strcmp(refer,"halloffame") == 0 ) || ( strcmp(refer,"gettingstarted") == 0 ) || ( strcmp(refer,"register") == 0 ) || ( strcmp(refer,"login") == 0 ) || ( strcmp(refer,"logout") == 0 ) || ( strcmp(refer,"status") == 0 ) ) {
		httpString( cnt, "\t\t\tupdatehtml(\"u_online\",u_online);\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"u_activated\",u_activated);\n" );
	}
	
	if( !strcmp(refer,"hq") ) {
		httpString( cnt, "\t\t\tupdatehtml(\"hqweeks\",week);\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"hqyears\",year);\n" );
	} else if( !strcmp(refer,"status") ) {
		httpString( cnt, "\t\tvar hostprocs = getnodevar(xmlhttp.responseXML,\"cpuprocs\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"hostprocs\",hostprocs);\n" );
		httpString( cnt, "\t\tvar servpriority = getnodevar(xmlhttp.responseXML,\"servpriority\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"servpriority\",servpriority);\n" );
		httpString( cnt, "\t\tvar servthreads = getnodevar(xmlhttp.responseXML,\"servthreads\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"servthreads\",servthreads);\n" );

		httpString( cnt, "\t\tvar memused = getnodevar(xmlhttp.responseXML,\"memused\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"memused\",memused);\n" );
		httpString( cnt, "\t\tvar strss = getnodevar(xmlhttp.responseXML,\"strss\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"strss\",strss);\n" );
		httpString( cnt, "\t\tvar cputotal = getnodevar(xmlhttp.responseXML,\"cputotal\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"cputotal\",cputotal);\n" );
		httpString( cnt, "\t\tvar cpukernel = getnodevar(xmlhttp.responseXML,\"cpukernel\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"cpukernel\",cpukernel);\n" );
		httpString( cnt, "\t\tvar cpuuser = getnodevar(xmlhttp.responseXML,\"cpuuser\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"cpuuser\",cpuuser);\n" );
		httpString( cnt, "\t\tvar cpuloads = getnodevar(xmlhttp.responseXML,\"cpuloads\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"cpuloads\",cpuloads);\n" );
		httpString( cnt, "\t\tvar memavbytes = getnodevar(xmlhttp.responseXML,\"memavbytes\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"memavbytes\",memavbytes);\n" );
		httpString( cnt, "\t\tvar memavmeg = getnodevar(xmlhttp.responseXML,\"memavmeg\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"memavmeg\",memavmeg);\n" );
		httpString( cnt, "\t\tvar totalswapbytes = getnodevar(xmlhttp.responseXML,\"totalswapbytes\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"totalswapbytes\",totalswapbytes);\n" );
		httpString( cnt, "\t\tvar totalswapmeg = getnodevar(xmlhttp.responseXML,\"totalswapmeg\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"totalswapmeg\",totalswapmeg);\n" );
		httpString( cnt, "\t\tvar freeswapbytes = getnodevar(xmlhttp.responseXML,\"freeswapbytes\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"freeswapbytes\",freeswapbytes);\n" );
		httpString( cnt, "\t\tvar freeswapmeg = getnodevar(xmlhttp.responseXML,\"freeswapmeg\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"freeswapmeg\",freeswapmeg);\n" );
		httpString( cnt, "\t\tvar bufferbytes = getnodevar(xmlhttp.responseXML,\"bufferbytes\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"bufferbytes\",bufferbytes);\n" );
		httpString( cnt, "\t\tvar bufermeg = getnodevar(xmlhttp.responseXML,\"bufermeg\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"bufermeg\",bufermeg);\n" );
		httpString( cnt, "\t\tvar sharedbytes = getnodevar(xmlhttp.responseXML,\"sharedbytes\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"sharedbytes\",sharedbytes);\n" );
		httpString( cnt, "\t\tvar sharedmeg = getnodevar(xmlhttp.responseXML,\"sharedmeg\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"sharedmeg\",sharedmeg);\n" );

		httpString( cnt, "\t\tvar timeserver = getnodevar(xmlhttp.responseXML,\"timeserver\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"timeserver\",timeserver);\n" );
		httpString( cnt, "\t\tvar timegmt = getnodevar(xmlhttp.responseXML,\"timegmt\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"timegmt\",timegmt);\n" );

		#if IRCBOT_SUPPORT
		httpString( cnt, "\t\tvar botstatus = getnodevar(xmlhttp.responseXML,\"botstatus\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"botstatus\",botstatus);\n" );
		#endif

		httpString( cnt, "\t\t\tupdatehtml(\"sstatweeks\",week);\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"sstatyears\",year);\n" );
	}
	httpString( cnt, "\t\t//Okie, Update uinfos user infos...\n" );
	httpString( cnt, "\t\tif( login ) {\n" );
	httpString( cnt, "\t\t\t//Here we set images for mail, build etc...\n" );
	httpString( cnt, "\t\t\tif( getnodevar(xmlhttp.responseXML,\"mail\") == 1 ) { changeimage(\"headermail\", \"files?type=image&name=i09a.jpg\"); }\n" );
	httpString( cnt, "\t\t\tif( getnodevar(xmlhttp.responseXML,\"build\") == 1 ) { changeimage(\"headerbuild\", \"files?type=image&name=i10a.jpg\"); }\n" );
	httpString( cnt, "\t\t\tif( getnodevar(xmlhttp.responseXML,\"aid\") == 1 ) { changeimage(\"headeraid\", \"files?type=image&name=i11a.jpg\"); }\n" );
	httpString( cnt, "\t\t\tif( getnodevar(xmlhttp.responseXML,\"fleet\") == 1 ) { changeimage(\"headerfleet\", \"files?type=image&name=i12a.jpg\"); }\n" );
	httpString( cnt, "\t\t\telse if( getnodevar(xmlhttp.responseXML,\"fleet\") == 2 ) { changeimage(\"headerfleet\", \"files?type=image&name=i12b.jpg\"); }\n" );
	httpString( cnt, "\n" );
	httpString( cnt, "\t\t\tvar networth = getnodevar(xmlhttp.responseXML,\"networth\");\n" );
	httpString( cnt, "\t\t\tvar population = getnodevar(xmlhttp.responseXML,\"population\");\n" );
	for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ ) {
		snprintf( CHECKER, sizeof(CHECKER),"%s",cmdRessourceName[a]);
		for(b = 0; CHECKER[b]; b++){
			CHECKER[b] = tolower(CHECKER[b]);
		}
		httpPrintf( cnt, "\t\t\tvar %s = getnodevar(xmlhttp.responseXML,\"%s\");\n", CHECKER, CHECKER );
		httpPrintf( cnt, "\t\t\tupdatehtml(\"header%s\",%s);\n", CHECKER, CHECKER );
	}
	httpString( cnt, "\t\t\tupdatehtml(\"headernetworth\",networth);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"headerpopulation\",population);\n" );
	httpString( cnt, "\n" );
	if( !strcmp(refer,"hq") ) {
		for( a = 0 ; a < CMD_READY_NUMUSED ; a++ ) {
			httpPrintf( cnt, "\t\t\tvar %sready = getnodevar(xmlhttp.responseXML,\"%sready\");\n", cmdReadyName[a], cmdReadyName[a] );
			httpPrintf( cnt, "\t\t\tupdatehtml(\"hq%sready\",%sready);\n", cmdReadyName[a], cmdReadyName[a] );
		}
		httpString( cnt, "\t\t\tvar planets = getnodevar(xmlhttp.responseXML,\"planets\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"hqpopulation\",population);\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"hqnetworth\",networth);\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"hqplanets\",planets);\n" );
	} else if( !strcmp(refer,"council") ) {
		for( a = 0 ; a < INFOS_TOTAL_NUMUSED ; a++ ) {
			httpPrintf( cnt, "\t\t\tvar %s = getnodevar(xmlhttp.responseXML,\"%s\");\n", cmdMainInfoNames[a], cmdMainInfoNames[a] );
		}
	httpString( cnt, "\t\t\tvar empiretax = getnodevar(xmlhttp.responseXML,\"taxlevel\");\n" );
	//Ack, these are going to be tricky to convert.
	httpString( cnt, "\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"energyincome\",( energyincome >= 0 ? \"+\" : \"<span class=\\\"genred\\\">\" ) + energyincome + ( energyincome >= 0 ? \"\" : \"</span>\" ));\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"mineralincome\",( mineralincome >= 0 ? \"+\" : \"\" ) + mineralincome);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"crystalincome\",( crystalincome >= 0 ? \"+\" : \"<span class=\\\"genred\\\">\" ) + crystalincome + ( crystalincome >= 0 ? \"\" : \"</span>\" ));\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"ectroliumincome\",( ectroliumincome >= 0 ? \"+\" : \"\" ) + ectroliumincome);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"energyproduction\",\"+\" + energyproduction);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"energydecay\",\"-\" + energydecay);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"buildingupkeep\",\"-\" + buildingupkeep);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"populationreduction\",\"+\" + populationreduction);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"portalsupkeep\",\"-\" + portalsupkeep);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"unitupkeep\",\"-\" + unitupkeep);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"mineralproduction\",\"+\" + mineralproduction);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"crystalproduction\",\"+\" + crystalproduction);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"crystaldecay\",\"-\" + crystaldecay);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"ectroliumproduction\",\"+\" + ectroliumproduction);\n" );
	httpString( cnt, "\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"counciltax\",empiretax);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"energytax\",\"-\" + energytax);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"mineraltax\",\"-\" + mineraltax);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"crystaltax\",\"-\" + crystaltax);\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"ectroliumtax\",\"-\" + ectroliumtax);\n" );
	httpString( cnt, "\n" );
	httpString( cnt, "\t\t\tvar buildhtml = getnodevar(xmlhttp.responseXML,\"council_html_buildings\");\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"council_build_que\",buildhtml);\n" );
	httpString( cnt, "\t\t\tvar unitshtml = getnodevar(xmlhttp.responseXML,\"council_html_units\");\n" );
	httpString( cnt, "\t\t\tupdatehtml(\"council_unit_que\",unitshtml);\n" );	
	httpString( cnt, "\n" );
		for( a = b = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ ) {
			httpPrintf( cnt, "\t\t\tvar bld%d = getnodevar(xmlhttp.responseXML,\"bld%d\");\n", a, a );
			httpPrintf( cnt, "\t\t\tupdatehtml(\"bld%d\",bld%d);\n", a, a );
		}
		httpString( cnt, "\t\t\tvar bldnum = getnodevar(xmlhttp.responseXML,\"bldnum\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"bldnum\",bldnum);\n" );
		httpString( cnt, "\n" );
		for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
			httpPrintf( cnt, "\t\t\tvar unt%d = getnodevar(xmlhttp.responseXML,\"unt%d\");\n", a, a );
			httpPrintf( cnt, "\t\t\tupdatehtml(\"unt%d\",unt%d);\n", a, a );
		}
		httpString( cnt, "\t\t\tvar untnum = getnodevar(xmlhttp.responseXML,\"untnum\");\n" );
		httpString( cnt, "\t\t\tupdatehtml(\"untnum\",untnum);\n" );
		httpString( cnt, "\n" );		
	}
	httpString( cnt, "\t\t}\n" );
	httpString( cnt, "\t\tSD=window.setTimeout(\"countDown();\", 1000);\n" );
	httpString( cnt, "\t} else {\n" );
	httpString( cnt, "\twindow.clearTimeout(SD);\n" );
	httpString( cnt, "\t}\n" );
	httpString( cnt, "}\n" );
	httpPrintf( cnt, "xmlhttp.open(\"GET\",\"%s&typ=\"+str,true);\n", URLAppend( cnt, "ajax" ) );
	httpString( cnt, "xmlhttp.send();\n" );
	httpString( cnt, "}\n" );
	httpString( cnt, "\n" );
	httpString( cnt, "\n" );

	urlinfo_free( urlp );
} else {
	//iohttpFunc_front( cnt, "Bad page request." );
}

return;
}



