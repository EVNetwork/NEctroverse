#ifndef GAMEPLAYINCLUDES
#define GAMEPLAYINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

void iohttpMapSystemsBuffer( int *mapp, int *buffer, int andl, int numpl, int type );

void iohttpFunc_main( svConnectionPtr cnt ); //done

void iohttpFunc_menu( svConnectionPtr cnt ); //done

void iohttpNewsString( svConnectionPtr cnt, int64_t *newsd ); //done

void iohttpFamNewsEntry( svConnectionPtr cnt, int picture, int64_t *newsd ); //done

void iohttpFamNews( svConnectionPtr cnt, int num, int64_t *newsd, dbMainEmpirePtr empired ); //done

void iohttpFunc_hq( svConnectionPtr cnt ); //done

void iohttpFunc_news( svConnectionPtr cnt );

void iohttpFunc_council( svConnectionPtr cnt );

void iohttpFunc_units( svConnectionPtr cnt );

void iohttpFunc_market( svConnectionPtr cnt );

void iohttpFunc_planets( svConnectionPtr cnt );

void iohttpFunc_empire( svConnectionPtr cnt );

void iohttpFunc_famaid( svConnectionPtr cnt );

void iohttpFunc_famgetaid( svConnectionPtr cnt );

void iohttpFunc_vote( svConnectionPtr cnt );

void iohttpFunc_famnews( svConnectionPtr cnt );

void iohttpFunc_famrels( svConnectionPtr cnt );

void iohttpFunc_famleader( svConnectionPtr cnt );

void iohttpFunc_map( svConnectionPtr cnt );

void iohttpFunc_mappick( svConnectionPtr cnt );

void iohttpFunc_mapadv( svConnectionPtr cnt );

void iohttpFunc_system( svConnectionPtr cnt );

void iohttpFunc_player( svConnectionPtr cnt );

void iohttpFunc_playerlist( svConnectionPtr cnt );

void iohttpFunc_planet( svConnectionPtr cnt );

void iohttpFunc_pltake( svConnectionPtr cnt );

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

void iohttpFunc_rankings( svConnectionPtr cnt ); //done

void iohttpFunc_search( svConnectionPtr cnt );


void iohtmlFunc_main( ReplyDataPtr cnt );

void iohtmlFunc_menu( ReplyDataPtr cnt );

void iohtmlFunc_hq( ReplyDataPtr cnt );

void iohtmlFunc_news( ReplyDataPtr cnt );

void iohtmlFunc_council( ReplyDataPtr cnt );

void iohtmlFunc_rankings( ReplyDataPtr cnt );

void iohtmlNewsString( ReplyDataPtr cnt, int64_t *newsd );

void iohtmlFamNewsEntry( ReplyDataPtr cnt, int picture, int64_t *newsd );

void iohtmlFamNews( ReplyDataPtr cnt, int num, int64_t *newsd, dbMainEmpirePtr empired );

#endif
