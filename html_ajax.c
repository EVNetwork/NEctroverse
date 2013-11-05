#ifndef GLOBALINCLUDED
#include "global.h"
#endif


//Ok, so here goes nothing...
void iohttpFunc_ajax( svConnectionPtr cnt ) {
	int id, a, b;
	int bsums[CMD_BLDG_NUMUSED+1];
	int usums[CMD_UNIT_NUMUSED];
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
	if( !( refer ) )
		refer = "ajax";

	svSendString( cnt, "Content-Type: text/xml\n\n" );
	svSendString( cnt, "<?xml version=\"1.0\"?>" );
	svSendPrintf( cnt, "<xml>" );
	//Begin XML generation, we only make one request now... so we have to structure carefully!
	if( !strcmp(typestring,"ticker") ) {
		//Send basic tick info, and check if user is loged in.
		svSendPrintf( cnt, "<pass>%d</pass>", ( id != -1 ) ? ( ( (cnt->dbuser)->flags & cmdUserFlags[CMD_FLAGS_ACTIVATED] ) ? true : false ) : false );
		svSendPrintf( cnt, "<page>%s</page>", refer  );
		svSendPrintf( cnt, "<time><next>%d</next><week>%d</week><year>%d</year></time>", (int)fmax( 0.0, ( ticks.next - time(0) ) ), ticks.number % 52, ticks.number / 52 );
		if( id < 0 )
			goto ENDXML;
		a = dbUserNewsGetFlags( id );
		//OK, so they are loged in... time to send some info. Lets start with the header. =)
		svSendString( cnt, "<header>" );
		svSendPrintf( cnt, "<networth>%lld</networth>", (long long)maind.networth );
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
			svSendPrintf( cnt, "<%s>%lld</%s>", CHECKER, (long long)maind.ressource[a], CHECKER );
		}
		svSendPrintf( cnt, "<population>%lld</population>", (long long)maind.ressource[CMD_RESSOURCE_POPULATION] );
		svSendString( cnt, "</ressources>" );
		svSendString( cnt, "</header>" );
		svSendPrintf( cnt, "<%s>", refer );
		//End Header -- Start HQ block
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
		//End HQ block -- Start Council block
		} else if( !strcmp(refer,"council") ) {
			svSendString( cnt, "<incomes>" );
			svSendPrintf( cnt, "<taxlevel>%.02f</taxlevel>", ( empired.taxation * 100 ) );
			for( a = 0 ; a < INFOS_TOTAL_NUMUSED ; a++ ) {
				svSendPrintf( cnt, "<%s>%lld</%s>", cmdMainInfoNames[a], (long long)maind.infos[a], cmdMainInfoNames[a] );
			}
			svSendString( cnt, "</incomes>" );
			svSendString( cnt, "<bldings>" );
			for( a = b = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ ) {
				svSendPrintf( cnt, "<bld%d>%lld</bld%d>", a, (long long)maind.totalbuilding[a], a );
				b += (int)maind.totalbuilding[a];
			}
			svSendPrintf( cnt, "<bldnum>%d</bldnum>", b );
			svSendString( cnt, "</bldings>" );
			svSendString( cnt, "<allunits>" );
			for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
				svSendPrintf( cnt, "<unt%d>%lld</unt%d>", a, (long long)maind.totalunit[a], a );
				b += (int)maind.totalunit[a];
			}
			svSendPrintf( cnt, "<untnum>%d</untnum>", b );
			svSendString( cnt, "</allunits>" );
		memset( bsums, 0, (CMD_BLDG_NUMUSED+1)*sizeof(int) );
		memset( usums, 0, CMD_UNIT_NUMUSED*sizeof(int) );

		//End Council block -- Start new block
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



void iohttpFunc_javaforajax( svConnectionPtr cnt ) {
	int a, b;
	char *refer;
	char CHECKER[256];
	iohttpDataPtr iohttp = cnt->iodata;

if( iohttp->referer ) {
	refer = strtok( (strrchr( iohttp->referer, '/' ) + 1), "?");
} else {
	refer = "ajax";
}

if( !( refer ) )
	refer = "ajax";


svSendString( cnt, "Content-Type: text/javascript\n\n" );
svSendString( cnt, "//This file is not really here... it's auto-generated by the game code.\n" );
svSendString( cnt, "//I got really sick of having to add all these functions in... so BLA!.\n" );
svSendString( cnt, "//Time to use this to more of its potential!!.\n" );
svSendString( cnt, "var page;\n" );
svSendString( cnt, "var login = false;\n" );
svSendPrintf( cnt, "var sec = %d;\n", (int)( ticks.next - time(0) ) % 60 );
svSendPrintf( cnt, "var min = %d;\n\n", (int)( ticks.next - time(0) ) / 60 );

//OK, Time for some AJAX! =/ ... 
//It's been a while, forgive me if its messy lol! =P
svSendString( cnt, "function getInfo(str) {\n" );
svSendString( cnt, "\tvar xmlhttp;\n" );
svSendString( cnt, "if (str==\"\") {\n" );
svSendString( cnt, "\treturn;\n" );
svSendString( cnt, "}\n" );
svSendString( cnt, "if (window.XMLHttpRequest) { \n" );
svSendString( cnt, "\txmlhttp=new XMLHttpRequest(); \n" );
svSendString( cnt, "} else { \n" );
svSendString( cnt, "\txmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\"); \n" );
svSendString( cnt, "}\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "xmlhttp.onreadystatechange=function() {\n" );
svSendString( cnt, "\tif (xmlhttp.readyState==4 && xmlhttp.status==200) {\n" );
svSendString( cnt, "\t\t//Fetch uinfos tick data, and check if user is loged in... this controls weither we will call more data.\n" );
svSendString( cnt, "\t\tif( getnodevar(xmlhttp.responseXML,\"pass\") > 0 ) { login = true; } else { login = false; }\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "\t\tsec = getnodevar(xmlhttp.responseXML,\"next\");\n" );
svSendString( cnt, "\t\tpage = getnodevar(xmlhttp.responseXML,\"page\");\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "\t\tvar week = getnodevar(xmlhttp.responseXML,\"week\");\n" );
svSendString( cnt, "\t\tvar year = getnodevar(xmlhttp.responseXML,\"year\");\n" );
svSendString( cnt, "\n" );
if( !strcmp(refer,"hq") ) {
svSendString( cnt, "\t\t\tupdatehtml(\"hqweeks\",week);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"hqyears\",year);\n" );
} else if( !strcmp(refer,"status") ) {
svSendString( cnt, "\t\t\tupdatehtml(\"sstatweeks\",week);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"sstatyears\",year);\n" );
}
svSendString( cnt, "\t\t//Okie, Update uinfos user infos...\n" );
svSendString( cnt, "\t\tif( login ) {\n" );
svSendString( cnt, "\t\t\t//Here we set images for mail, build etc...\n" );
svSendString( cnt, "\t\t\tif( getnodevar(xmlhttp.responseXML,\"mail\") == 1 ) { changeimage(\"headermail\", \"images/i09a.jpg\"); }\n" );
svSendString( cnt, "\t\t\tif( getnodevar(xmlhttp.responseXML,\"build\") == 1 ) { changeimage(\"headerbuild\", \"images/i10a.jpg\"); }\n" );
svSendString( cnt, "\t\t\tif( getnodevar(xmlhttp.responseXML,\"aid\") == 1 ) { changeimage(\"headeraid\", \"images/i11a.jpg\"); }\n" );
svSendString( cnt, "\t\t\tif( getnodevar(xmlhttp.responseXML,\"fleet\") == 1 ) { changeimage(\"headerfleet\", \"images/i12a.jpg\"); }\n" );
svSendString( cnt, "\t\t\telse if( getnodevar(xmlhttp.responseXML,\"fleet\") == 2 ) { changeimage(\"headerfleet\", \"images/i12b.jpg\"); }\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "\t\t\tvar networth = getnodevar(xmlhttp.responseXML,\"networth\");\n" );
svSendString( cnt, "\t\t\tvar population = getnodevar(xmlhttp.responseXML,\"population\");\n" );

for( a = 0 ; a < CMD_RESSOURCE_NUMUSED ; a++ ) {
	sprintf(CHECKER,"%s",cmdRessourceName[a]);
	for(b = 0; CHECKER[b]; b++){
		CHECKER[b] = tolower(CHECKER[b]);
	}
	svSendPrintf( cnt, "\t\t\tvar %s = getnodevar(xmlhttp.responseXML,\"%s\");\n", CHECKER, CHECKER );
	svSendPrintf( cnt, "\t\t\tupdatehtml(\"header%s\",%s);\n", CHECKER, CHECKER );
}

svSendString( cnt, "\t\t\tupdatehtml(\"headernetworth\",networth);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"headerpopulation\",population);\n" );
svSendString( cnt, "\n" );
if( !strcmp(refer,"hq") ) {
	for( a = 0 ; a < CMD_READY_NUMUSED ; a++ ) {
		sprintf(CHECKER,"%s",cmdReadyName[a]);
		for(b = 0; CHECKER[b]; b++){
			CHECKER[b] = tolower(CHECKER[b]);
		}
		svSendPrintf( cnt, "\t\t\tvar %sready = getnodevar(xmlhttp.responseXML,\"%sready\");\n", CHECKER, CHECKER );
		svSendPrintf( cnt, "\t\t\tupdatehtml(\"hq%s\",%sready);\n", CHECKER, CHECKER );
	}
	svSendString( cnt, "\t\t\tvar planets = getnodevar(xmlhttp.responseXML,\"planets\");\n" );
	svSendString( cnt, "\t\t\tupdatehtml(\"hqpopulation\",population);\n" );
	svSendString( cnt, "\t\t\tupdatehtml(\"hqnetworth\",networth);\n" );
	svSendString( cnt, "\t\t\tupdatehtml(\"hqplanets\",planets);\n" );
} else if( !strcmp(refer,"council") ) {
	for( a = 0 ; a < INFOS_TOTAL_NUMUSED ; a++ ) {
		svSendPrintf( cnt, "\t\t\tvar %s = getnodevar(xmlhttp.responseXML,\"%s\");\n", cmdMainInfoNames[a], cmdMainInfoNames[a] );
	}
	svSendString( cnt, "\t\t\tvar empiretax = getnodevar(xmlhttp.responseXML,\"taxlevel\");\n" );
//Ack, these are going to be tricky to convert.
svSendString( cnt, "\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"energyincome\",( energyincome >= 0 ? \"+\" : \"<span class=\\\"genred\\\">\" ) + energyincome + ( energyincome >= 0 ? \"\" : \"</span>\" ));\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"mineralincome\",( mineralincome >= 0 ? \"+\" : \"\" ) + mineralincome);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"crystalincome\",( crystalincome >= 0 ? \"+\" : \"<span class=\\\"genred\\\">\" ) + crystalincome + ( crystalincome >= 0 ? \"\" : \"</span>\" ));\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"ectroliumincome\",( ectroliumincome >= 0 ? \"+\" : \"\" ) + ectroliumincome);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"energyproduction\",\"+\" + energyproduction);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"energydecay\",\"-\" + energydecay);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"buildingupkeep\",\"-\" + buildingupkeep);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"populationreduction\",\"+\" + populationreduction);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"portalsupkeep\",\"-\" + portalsupkeep);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"unitupkeep\",\"-\" + unitupkeep);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"mineralproduction\",\"+\" + mineralproduction);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"crystalproduction\",\"+\" + crystalproduction);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"crystaldecay\",\"-\" + crystaldecay);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"ectroliumproduction\",\"+\" + ectroliumproduction);\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"counciltax\",empiretax);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"energytax\",\"-\" + energytax);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"mineraltax\",\"-\" + mineraltax);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"crystaltax\",\"-\" + crystaltax);\n" );
svSendString( cnt, "\t\t\tupdatehtml(\"ectroliumtax\",\"-\" + ectroliumtax);\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "\n" );
	for( a = b = 0 ; a < CMD_BLDG_NUMUSED+1 ; a++ ) {
		svSendPrintf( cnt, "\t\t\tvar bld%d = getnodevar(xmlhttp.responseXML,\"bld%d\");\n", a, a );
		svSendPrintf( cnt, "\t\t\tupdatehtml(\"bld%d\",bld%d);\n", a, a );
	}
	svSendString( cnt, "\t\t\tvar bldnum = getnodevar(xmlhttp.responseXML,\"bldnum\");\n" );
	svSendString( cnt, "\t\t\tupdatehtml(\"bldnum\",bldnum);\n" );
	svSendString( cnt, "\n" );
	for( a = b = 0 ; a < CMD_UNIT_NUMUSED ; a++ ) {
		svSendPrintf( cnt, "\t\t\tvar unt%d = getnodevar(xmlhttp.responseXML,\"unt%d\");\n", a, a );
		svSendPrintf( cnt, "\t\t\tupdatehtml(\"unt%d\",unt%d);\n", a, a );
	}
	svSendString( cnt, "\t\t\tvar untnum = getnodevar(xmlhttp.responseXML,\"untnum\");\n" );
	svSendString( cnt, "\t\t\tupdatehtml(\"untnum\",untnum);\n" );
	svSendString( cnt, "\n" );
}

svSendString( cnt, "\t\t}\n" );
svSendString( cnt, "\t\tSD=window.setTimeout(\"countDown();\", 1000);\n" );
svSendString( cnt, "\t} else {\n" );
svSendString( cnt, "\twindow.clearTimeout(SD);\n" );
svSendString( cnt, "\t}\n" );
svSendString( cnt, "}\n" );
svSendString( cnt, "xmlhttp.open(\"GET\",\"ajax?typ=\"+str,true);\n" );
svSendString( cnt, "xmlhttp.send();\n" );
svSendString( cnt, "}\n" );
svSendString( cnt, "\n" );
svSendString( cnt, "\n" );


return;
}



