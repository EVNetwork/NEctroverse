/*
     This file is part of libmicrohttpd
     (C) 2008 Christian Grothoff (and other contributing authors)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
 * @file platform.h
 * @brief platform-specific includes for libmicrohttpd
 * @author Christian Grothoff
 *
 * This file is included by the libmicrohttpd code
 * before "microhttpd.h"; it provides the required
 * standard headers (which are platform-specific).<p>
 *
 * Note that this file depends on our configure.ac
 * build process and the generated config.h file.
 * Hence you cannot include it directly in applications
 * that use libmicrohttpd.
 */
#ifndef MHD_PLATFORM_H
#define MHD_PLATFORM_H

#define _XOPEN_SOURCE_EXTENDED  1
#undef HAVE_CONFIG_H
#ifndef AVOIDIMPLICITS
#define AVOIDIMPLICITS
/* Get thread name visible in the kernel and its interfaces.  */
extern int pthread_getname_np (pthread_t __target_thread, char *__buf,
			       size_t __buflen)
     __THROW __nonnull ((2));

/* Set thread name visible in the kernel and its interfaces.  */
extern int pthread_setname_np (pthread_t __target_thread, const char *__name)
     __THROW __nonnull ((2));
extern int pthread_getname_np (pthread_t __target_thread, char *__buf,
			       size_t __buflen)
     __THROW __nonnull ((2));

/* Set thread name visible in the kernel and its interfaces.  */
extern int pthread_setname_np (pthread_t __target_thread, const char *__name)
     __THROW __nonnull ((2));
     
extern int asprintf (char **__restrict __ptr,
		     const char *__restrict __fmt, ...)
     __THROWNL __attribute__ ((__format__ (__printf__, 2, 3))) __wur;
     
extern int accept4 (int __fd, __SOCKADDR_ARG __addr,
		    socklen_t *__restrict __addr_len, int __flags);
		    

#endif

#define HAVE_CONFIG_H 1

#include "plibc.h"

#include "microhttpd.h"
#include "internal.h"
#include "connection.h"
#if HTTPS_SUPPORT
#include "connection_https.h"
#endif
#include "memorypool.h"
#include "response.h"
#include "reason_phrase.h"

#include "http.h"

#endif
