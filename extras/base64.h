/*
 * This code implements the BASE64 algorithm.
 * This code is in the public domain; do with it what you wish.
 *
 * @file base64.c
 * @brief This code implements the BASE64 algorithm
 * @author Matthieu Speder
 */
#ifndef BASE64_H
#define BASE64_H

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen);

#endif /* !BASE64_H */
