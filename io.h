#ifndef IOINCLUDES
#define IOINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.



char *ioCompareWords( char *string, char *word );
char *ioCompareFindWords( char *string, char *word );
char *ioCompareFindBinwords( char *string, char *word, int size );
int ioCompareExact( char *s1, char *s2 );
int iohttpCompareExt( char *string, char *ext );
int iohttpCaseLow( char *dest, char *string );




#endif
