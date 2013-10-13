#ifndef CONFIGINCLUDED
#include "config.h"
#endif
#ifndef GLOBALINCLUDED
#include "global.h"
#endif

#include "artefact.h"
#include "db.h"
#include "sv.h"
#include "io.h"
#include "cmd.h"




void inNewHTTP( svConnectionPtr cnt );
void inNewDataHTTP( svConnectionPtr cnt );
void outSendReplyHTTP( svConnectionPtr cnt );
void inSendCompleteHTTP( svConnectionPtr cnt );
void inClosedHTTP( svConnectionPtr cnt );
void inErrorHTTP( svConnectionPtr cnt, int type );

#include "iohttp.c"




void inNewEvm( svConnectionPtr cnt );
void inNewDataEvm( svConnectionPtr cnt );
void outSendReplyEvm( svConnectionPtr cnt );
void inSendCompleteEvm( svConnectionPtr cnt );
void inClosedEvm( svConnectionPtr cnt );
void inErrorEvm( svConnectionPtr cnt, int type );

#if EVMAPENABLE == 1
#include "ioevm.c"
#endif



ioInterfaceDef ioInterface[IO_INTERFACE_NUM] =
{
  // HTTP interface definition
  {
    InitHTTP,
    EndHTTP,
    inNewHTTP,
    inNewDataHTTP,
    outSendReplyHTTP,
    inSendCompleteHTTP,
    inClosedHTTP,
    inErrorHTTP,
    TickStartHTTP,
    TickEndHTTP,
    65536,
    30000,
    40000
  }
#if EVMAPENABLE == 1
,

  // Evm protocol interface definition
  {
    InitEvm,
    EndEvm,
    inNewEvm,
    inNewDataEvm,
    outSendReplyEvm,
    inSendCompleteEvm,
    inClosedEvm,
    inErrorEvm,
    TickStartEvm,
    TickEndEvm,
    65536,
    300000,
    360000
  }
#endif
};




unsigned char *ioCompareWords( unsigned char *string, unsigned char *word )
{
  int i;
  for( i = 0 ; ; i++ )
  {
    if( !( word[i] ) )
      return &string[i];
    if( string[i] != word[i] )
      return 0;
  }
  return &string[i];
}


/*TODO add the * wildcard support*/
unsigned char *ioCompareFindWords( unsigned char *string, unsigned char *word )
{
  int i;
  for( ; ; string++ )
  {
    if( !( *string ) )
      return 0;
    for( i = 0 ; ; i++ )
    {
      if( !( word[i] ) )
        return &string[i];
      if( string[i] != word[i] && word[i] != '*' )
        break;
    }
  }
  return &string[i];
}

unsigned char *ioCompareFindBinwords( unsigned char *string, unsigned char *word, int size )
{
  int i;
  for( ; ; string++, size-- )
  {
    if( !( size ) )
      return 0;
    for( i = 0 ; ; i++ )
    {
      if( size - i <= 0 )
        return 0;
      if( !( word[i] ) )
        return &string[i];
      if( string[i] != word[i] )
        break;
    }
  }
  return &string[i];
}

int ioCompareExact( unsigned char *s1, unsigned char *s2 )
{
  int i;
  for( i = 0 ; ; i++ )
  {
  	if( s1[i] != s2[i] )
      return 0;
    if( !( s1[i] ) )
      return 1;
  }
  return 1;
}

int iohttpCompareExt( unsigned char *string, unsigned char *ext )
{
  int i;
  for( i = 0 ; ; i++ )
  {
    if( !( ext[i] ) )
    {
      if( !( string[i] ) )
        return 1;
      else
        return 0;
    }
    if( string[i] != ext[i] )
      return 0;
  }
  return 1;
}



int iohttpCaseLow( unsigned char *dest, unsigned char *string )
{
  int i;
  for( i = 0 ; string[i] ; i++ )
  {
    dest[i] = string[i];
    if( ( string[i] >= 'A' ) && ( string[i] <= 'Z' ) )
      dest[i] += 'a' - 'A';
  }
  dest[i] = 0;
  return 1;
}






