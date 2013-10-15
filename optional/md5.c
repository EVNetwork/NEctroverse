#ifndef CONFIGINCLUDED
#include "../config.h"
#endif
#ifndef GLOBALINCLUDED
#include "../global.h"
#endif

#include "md5.h"

char *str2md5(const char *str) {
	int n, length;
	char *out = (char*)malloc(33);
	unsigned char digest[16];
	MD5_CTX c;

length = strlen(str);
MD5_Init(&c);

while (length > 0) {
	if (length > 512) {
		MD5_Update(&c, str, 512);
	} else {
		MD5_Update(&c, str, length);
	}
	length -= 512;
	str += 512;
}

MD5_Final(digest, &c);

for (n = 0; n < 16; ++n) {
	snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
}


return out;
}


char *hashencrypt( char *passhash ) {
	int i;
	char *password;
	const char *const seedchars = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	unsigned long seed[2];
	char salted[] = "$1$";
	char urandom[9];
	char random[33];
	FILE *fh;     

srand((unsigned int) time(0));  
   

for (i = 0; i < 8; ++i) {
	urandom[i] = rand() % (126 - 33 + 1) + 33;
}
urandom[i] = '\0';

strcat(salted,urandom);

if ( (fh = fopen("/dev/urandom", "rb")) ) {
	fread( &random, 1, 32, fh );
	fclose(fh);
}

seed[0] = random;
seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);
    
for (i = 0; i < 8; i++)
	salted[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];

password = crypt(passhash, salted);

return password;
}

int checkencrypt( char *passentered, char *passcheck ) {
	char *result;
	int ok;

result = crypt(passentered, passcheck);
ok = ( strcmp(result, passcheck) == 0 );

return ok ? 1 : 0;
}


