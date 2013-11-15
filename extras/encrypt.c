#include "../global.h"

#include "md5.c"
#include "base64.c"

//Time to add some hash protection to our passwords.
char *saltgen() {
	int i;
	const char *const seedchars = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	unsigned long seed[2];
	char salted[128], *salt;
	char urandom[9];
	char random[33];
	FILE *fh;

strcpy(salted,"$1$");
srand((unsigned int) time(0));

for(i = 0; i < 6; ++i) {
	urandom[i] = rand() % (126 - 33 + 1) + 33;
}
urandom[i] = '\0';

strcat(salted, str2md5(urandom) );

if( (fh = fopen("/dev/urandom", "rb")) ) {
	if( fread( &random, 1, 32, fh ) < 1 ) {
		loghandle(LOG_ERR, errno, "Error Getting random for MD5... Critical failure." );
	}
	fclose(fh);
}

seed[0] = (long)random;
seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

for (i = 0; i < 6; i++)
	salted[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];

salt = salted;

return salt;
}


char *hashencrypt( char *passhash ) {
	struct crypt_data data;

data.initialized = 0;
return crypt_r( str2md5(passhash), saltgen(), &data );
}

//Good stuff, now to check those encypted passwords.
int checkencrypt( char *passentered, char *passcheck ) {
	struct crypt_data data;

data.initialized = 0;
return ( ( strcmp( crypt_r( str2md5(passentered), passcheck, &data), passcheck) == 0 ) ? 1 : 0 );
}




