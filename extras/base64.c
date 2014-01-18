/*
* RFC 1521 base64 encoding/decoding
*
* Copyright (C) 2006-2013, Brainspark B.V.
*
* This file is part of PolarSSL (http://www.polarssl.org)
* Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
*
* All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "base64.h"

static const char  table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int   BASE64_INPUT_SIZE = 57;

bool isbase64(char c) {

return c && strchr(table, c) != NULL;
}

inline char value(char c) {
const char *p = strchr(table, c);

if(p) {
	return p-table;
} else {
	return 0;
}

}

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen) {

*dest = 0;
if(*src == 0)  {
	return 0;
}

unsigned char *p = dest;
do {
	char a = value(src[0]);
	char b = value(src[1]);
	char c = value(src[2]);
	char d = value(src[3]);
	*p++ = (a << 2) | (b >> 4);
	*p++ = (b << 4) | (c >> 2);
	*p++ = (c << 6) | d;
	if(!isbase64(src[1]))  {
		p -= 2;
		break;
	} else if(!isbase64(src[2])) {
		p -= 2;
		break;
	} else if(!isbase64(src[3])) {
		p--;
		break;
	}
	src += 4;
	while(*src && (*src == 13 || *src == 10)) {
		src++;
	}
} while(srclen-= 4);

*p = 0;
return p-dest;
}


