#ifndef GAMEPLAYINCLUDES
#define GAMEPLAYINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

void iohttpMapSystemsBuffer( int *mapp, int *buffer, int andl, int numpl, int type );


void iohttpFunc_build( svConnectionPtr cnt );

void iohttpFunc_cancelbuild(svConnectionPtr cnt);

void iohttpFunc_massbuild( svConnectionPtr cnt );

void iohttpFunc_raze( svConnectionPtr cnt );

void iohttpFunc_fleets( svConnectionPtr cnt );

void iohttpFunc_fleetssplit( svConnectionPtr cnt );

void iohttpFunc_fleetsmerge( svConnectionPtr cnt);

void iohttpFunc_fleetdisband( svConnectionPtr cnt );

void iohttpFunc_fleetsend( svConnectionPtr cnt );

void iohttpFunc_fleetchange( svConnectionPtr cnt );

void iohttpAttackReport( svConnectionPtr cnt, int *results, int sats );

void iohttpFunc_fleetattack( svConnectionPtr cnt );

void iohttpFunc_explore( svConnectionPtr cnt );

void iohttpFunc_attack( svConnectionPtr cnt );

void iohttpFunc_station( svConnectionPtr cnt );

void iohttpFunc_spec( svConnectionPtr cnt );

void iohttpFunc_specinfos( svConnectionPtr cnt );

void iohttpFunc_operation( svConnectionPtr cnt );

void iohttpFunc_spell( svConnectionPtr cnt );

void iohttpFunc_incant( svConnectionPtr cnt );

void iohttpFunc_operationsend( svConnectionPtr cnt );

void iohttpFunc_spellsend( svConnectionPtr cnt );

void iohttpFunc_incantsend( svConnectionPtr cnt );

void iohttpFunc_research( svConnectionPtr cnt );

void iohttpFunc_mail( svConnectionPtr cnt );


//New Section
void iohtmlFunc_main( ReplyDataPtr cnt );

void iohtmlFunc_menu( ReplyDataPtr cnt );

void iohtmlFunc_hq( ReplyDataPtr cnt );

void iohtmlFunc_news( ReplyDataPtr cnt );

void iohtmlFunc_council( ReplyDataPtr cnt );

void iohtmlFunc_units( ReplyDataPtr cnt );

void iohtmlFunc_market( ReplyDataPtr cnt );

void iohtmlFunc_planets( ReplyDataPtr cnt );

void iohtmlFunc_pltake( ReplyDataPtr cnt );

void iohtmlFunc_empire( ReplyDataPtr cnt );

void iohtmlFunc_famaid( ReplyDataPtr cnt );

void iohtmlFunc_famgetaid( ReplyDataPtr cnt );

void iohtmlFunc_vote( ReplyDataPtr cnt );

void iohtmlFunc_famnews( ReplyDataPtr cnt );

void iohtmlFunc_famrels( ReplyDataPtr cnt );

void iohtmlFunc_famleader( ReplyDataPtr cnt );

void iohtmlFunc_map( ReplyDataPtr cnt );

void iohtmlFunc_mappick( ReplyDataPtr cnt );

void iohtmlFunc_mapadv( ReplyDataPtr cnt );

void iohtmlFunc_system( ReplyDataPtr cnt );

void iohtmlFunc_player( ReplyDataPtr cnt );

void iohtmlFunc_playerlist( ReplyDataPtr cnt );

void iohtmlFunc_planet( ReplyDataPtr cnt );


//void iohtmlFunc_search( ReplyDataPtr cnt );

void iohtmlFunc_rankings( ReplyDataPtr cnt );

void iohtmlNewsString( ReplyDataPtr cnt, int64_t *newsd );

void iohtmlFamNewsEntry( ReplyDataPtr cnt, int picture, int64_t *newsd );

void iohtmlFamNews( ReplyDataPtr cnt, int num, int64_t *newsd, dbMainEmpirePtr empired );

#endif
