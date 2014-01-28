#ifndef GAMEPLAYINCLUDES
#define GAMEPLAYINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

void iohttpMapSystemsBuffer( int *mapp, int *buffer, int andl, int numpl, int type );

void iohtmlAttackReport( ReplyDataPtr cnt, int64_t *results, int sats );

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

void iohtmlFunc_ministers( ReplyDataPtr cnt );

void iohtmlFunc_map( ReplyDataPtr cnt );

void iohtmlFunc_mappick( ReplyDataPtr cnt );

void iohtmlFunc_mapadv( ReplyDataPtr cnt );

void iohtmlFunc_system( ReplyDataPtr cnt );

void iohtmlFunc_player( ReplyDataPtr cnt );

void iohtmlFunc_playerlist( ReplyDataPtr cnt );

void iohtmlFunc_planet( ReplyDataPtr cnt );

void iohtmlFunc_build( ReplyDataPtr cnt );

void iohtmlFunc_cancelbuild( ReplyDataPtr cnt );

void iohtmlFunc_massbuild( ReplyDataPtr cnt );

void iohtmlFunc_raze( ReplyDataPtr cnt );

void iohtmlFunc_fleets( ReplyDataPtr cnt );

void iohtmlFunc_fleetsend( ReplyDataPtr cnt );

void iohtmlFunc_fleetssplit( ReplyDataPtr cnt );

void iohtmlFunc_fleetsmerge( ReplyDataPtr cnt);

void iohtmlFunc_fleetdisband( ReplyDataPtr cnt );

void iohtmlFunc_fleetchange( ReplyDataPtr cnt );

void iohtmlFunc_fleetattack( ReplyDataPtr cnt );

void iohtmlFunc_explore( ReplyDataPtr cnt );

void iohtmlFunc_attack( ReplyDataPtr cnt );

void iohtmlFunc_station( ReplyDataPtr cnt );

void iohtmlFunc_spec( ReplyDataPtr cnt );

void iohtmlFunc_specinfos( ReplyDataPtr cnt );

void iohtmlFunc_operation( ReplyDataPtr cnt );

void iohtmlFunc_spell( ReplyDataPtr cnt );

void iohtmlFunc_incant( ReplyDataPtr cnt );

void iohtmlFunc_operationsend( ReplyDataPtr cnt );

void iohtmlFunc_spellsend( ReplyDataPtr cnt );

void iohtmlFunc_incantsend( ReplyDataPtr cnt );

void iohtmlFunc_research( ReplyDataPtr cnt );

void iohtmlFunc_mail( ReplyDataPtr cnt );

void iohtmlFunc_search( ReplyDataPtr cnt );

void iohtmlFunc_rankings( ReplyDataPtr cnt );

void iohtmlNewsString( ReplyDataPtr cnt, int64_t *newsd );

void iohtmlFamNewsEntry( ReplyDataPtr cnt, int picture, int64_t *newsd );

void iohtmlFamNews( ReplyDataPtr cnt, int num, int64_t *newsd, dbMainEmpirePtr empired );

#endif
