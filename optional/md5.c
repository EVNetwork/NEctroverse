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
