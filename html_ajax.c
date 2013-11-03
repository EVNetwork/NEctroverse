#ifndef GLOBALINCLUDED
#include "global.h"
#endif


//Ok, so here goes nothing...
void iohttpFunc_ajax( svConnectionPtr cnt ) {
	int id, a, b;
	char *typestring, *idstring, *refer;
	char CHECKER[256];
	dbUserMainDef maind;
	dbMainEmpireDef empired;
	iohttpDataPtr iohttp = cnt->iodata;

iohttpVarsInit( cnt );
idstring = iohttpVarsFind( "id" );
typestring = iohttpVarsFind( "typ" );
iohttpVarsCut();


if( ( id = iohttpIdentify( cnt, 2 ) ) >= 0 ) {
	if( dbUserMainRetrieve( id, &maind ) < 0 )
		goto BAILAJAX;
	else if( dbMapRetrieveEmpire( maind.empire, &empired ) < 0 )
		goto BAILAJAX;

}

if( ( typestring ) && ( iohttp->referer ) ) {
	refer = strtok( (strrchr( iohttp->referer, '/' ) + 1), "?");
	if( strlen(refer) < 1 )
		refer = "ajax";
	svSendString( cnt, "Content-Type: text/xml\n\n" );
	svSendString( cnt, "<?xml version=\"1.0\"?>" );
	svSendPrintf( cnt, "<xml>" );
	if( !strcmp(typestring,"ticker") ) {
		svSendPrintf( cnt, "<pass>%d</pass>", ( id != -1 ) ? ( ( (cnt->dbuser)->flags & cmdUserFlags[CMD_FLAGS_ACTIVATED] ) ? true : false ) : false );
		svSendPrintf( cnt, "<page>%s</page>", refer  );
		svSendPrintf( cnt, "<time><next>%d</next><week>%d</week><year>%d</year></time>", (int)fmax( 0.0, ( ticks.next - time(0) ) ), ticks.number % 52, ticks.number / 52 );
		if( id < 0 )
			goto ENDXML;
		a = dbUserNewsGetFlags( id );
		svSendString( cnt, "<header>" );
		svSendPrintf( cnt, "<networth>%lld</networth>", maind.networth );
		svSendString( cnt, "<notification>" );
		svSendPrintf( cnt, "<mail>%d</mail>",  ( a & CMD_NEWS_FLAGS_MAIL ) ? true : false );
		svSendPrintf( cnt, "<build>%d</build>",  ( a & CMD_NEWS_FLAGS_BUILD ) ? true : false );
		svSendPrintf( cnt, "<aid>%d</aid>", ( a & CMD_NEWS_FLAGS_AID ) ? true : false ); 
		svSendPrintf( cnt, "<fleet>%d</fleet>", ( a & CMD_NEWS_FLAGS_ATTACK ) ? 1 : ( ( a & CMD_NEWS_FLAGS_FLEET ) ? 2 : false ) );
		svSendString( cnt, "</notification>" );
		svSendString( cnt, "<ressources>" );
		for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ ) {
			sprintf(CHECKER,"%s",cmdRessourceName[a]);
			for(b = 0; CHECKER[b]; b++){
				CHECKER[b] = tolower(CHECKER[b]);
			}
			svSendPrintf( cnt, "<%s>%lld</%s>", CHECKER, maind.ressource[a], CHECKER );
		}
		svSendPrintf( cnt, "<population>%lld</population>", maind.ressource[CMD_RESSOURCE_POPULATION] );
		svSendString( cnt, "</ressources>" );
		svSendString( cnt, "</header>" );
		svSendPrintf( cnt, "<%s>", refer );
		if( !strcmp(refer,"hq") ) {
			svSendString( cnt, "<readiness>" );
			for( a = 0 ; a < CMD_READY_NUMUSED ; a++ ) {
				sprintf(CHECKER,"%s",cmdReadyName[a]);
				for(b = 0; CHECKER[b]; b++){
					CHECKER[b] = tolower(CHECKER[b]);
				}
				svSendPrintf( cnt, "<%sready>%d</%sready>", CHECKER, maind.readiness[a] >> 16, CHECKER );
			}
			svSendString( cnt, "</readiness>" );
		svSendPrintf( cnt, "<planets>%d</planets>", maind.planets );
		} else if( !strcmp(refer,"council") ) {
			svSendString( cnt, "<incomes>" );
			for( a = 0 ; a < INFOS_TOTAL_NUMUSED ; a++ ) {
				svSendPrintf( cnt, "<%s>%lld</%s>", cmdMainInfoNames[a], maind.infos[a], cmdMainInfoNames[a] );
			}
			svSendString( cnt, "</incomes>" );
			svSendString( cnt, "<bldings>" );
			for( a = b = 0 ; a < CMD_BLDG_NUMUSED ; a++ ) {
				svSendPrintf( cnt, "<bld%d>%lld</bld%d>", a, maind.totalbuilding[a], a );
				b += (int)maind.totalbuilding[a];
			}
			svSendPrintf( cnt, "<bldnum>%d</bldnum>", b );
			svSendPrintf( cnt, "<taxlevel>%.02f</taxlevel>", ( empired.taxation * 100 ) );
			svSendString( cnt, "</bldings>" );
		}
		svSendPrintf( cnt, "</%s>", refer );
	}
	ENDXML:
	svSendString( cnt, "</xml>" );
} else {
	iohttpFunc_front( cnt, "Bad page request." );
}

return;

BAILAJAX:
if( id != -1 )
svSendPrintf( cnt, "Error getting info! %s\n\n", cmdErrorString ? cmdErrorString : "" );

return;
}
