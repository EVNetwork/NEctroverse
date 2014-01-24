#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "md5.h"
#include "base64.h"

#include <crypt.h>

#define SALT_SIZE 128

void saltgen( char salt[SALT_SIZE] );

char *hashencrypt( char *pass2hash );

int checkencrypt( char *passentered, char *passcheck );

#endif
