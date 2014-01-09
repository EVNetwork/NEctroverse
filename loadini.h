#ifndef LOADING_INI_HEADDERS
#define LOADING_INI_HEADDERS

enum {
INI_TYPE_STRING,
INI_TYPE_DOUBLE,
INI_TYPE_BOOL,
INI_TYPE_INT,
};

typedef struct ConfigListArray {
  /**
   * Type of setting, string/int/bool
   */
  int type;

  /**
   * Origin name in INI file.
   */
  char *origin;

  /**
   * Default setting
   */
  char *ifblank;

  /**
   * Name we will associate with this value.
   */
  char *nicename;

} ConfigListArrayDef, *ConfigListArrayPtr;


typedef struct ConfigArray {
  /**
   * Index number... just in case.
   */
  int id;

  /**
   * Type headder for referance.
   */
  int type;

  /**
   * Numerical setting storage.
   */
  double num_value;

  /**
   * Origin Referance point... just in case.
   */
  char *origin;
  
  /**
   * Nice name we associated
   */
  char *name;

  /**
   * String value of setting
   */
  char *string_value;

  /**
   * Yet again, lets keep it all in a linked list.
   */  
  struct ConfigArray *next;

} ConfigArrayDef, *ConfigArrayPtr;

extern ConfigArrayPtr ConfigList;
extern ConfigArrayPtr ConfigTable[ARRAY_MAX];

extern inikey config;

ConfigArrayPtr loadfromconfig( char *name, char *sourcefile, int sourceline );
void FreeList( ConfigArrayPtr list );
void UnloadConfig();

int unloadfromconfig( char *name, char *sourcefile, int sourceline );

void DumpDefaults( char *filename );

dbUserInfoPtr ListAdmins( int num );
ConfigArrayPtr makelistfromconfig( char **list, char *sourcefile, int sourceline );

#endif
