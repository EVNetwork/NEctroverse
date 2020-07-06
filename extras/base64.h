#ifndef BASE64_H
#define BASE64_H

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen);


/* jude nagurney: GeoIP change, for providing proxy authorization */
/* #include <config.h> */
#define restrict __restrict

/* Get size_t. */
# include <stddef.h>

/* Get bool. */
# include <stdbool.h>

/* This uses that the expression (n+(k-1))/k means the smallest
   integer >= n/k, i.e., the ceiling of n/k.  */
# define BASE64_LENGTH(inlen) ((((inlen) + 2) / 3) * 4)

struct base64_decode_context
{
  unsigned int i;
  char buf[4];
};

extern bool isbase64 (char ch);

extern void base64_encode (const char *restrict in, size_t inlen,
                           char *restrict out, size_t outlen);

extern size_t base64_encode_alloc (const char *in, size_t inlen, char **out);

extern void base64_decode_ctx_init (struct base64_decode_context *ctx);

extern bool base64_decode_ctx (struct base64_decode_context *ctx,
                               const char *restrict in, size_t inlen,
                               char *restrict out, size_t *outlen);

extern bool base64_decode_alloc_ctx (struct base64_decode_context *ctx,
                                     const char *in, size_t inlen,
                                     char **out, size_t *outlen);

#endif /* BASE64_H */

