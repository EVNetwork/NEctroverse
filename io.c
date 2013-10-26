#ifndef GLOBALINCLUDED
#include "global.h"
#endif


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

#include "ioevm.c"



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
};




char *ioCompareWords( char *string, char *word )
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
char *ioCompareFindWords( char *string, char *word )
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

char *ioCompareFindBinwords( char *string, char *word, int size )
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

int ioCompareExact( char *s1, char *s2 )
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

int iohttpCompareExt( char *string, char *ext )
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



int iohttpCaseLow( char *dest, char *string )
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






