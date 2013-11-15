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

#include "config.h"

#define _XOPEN_SOURCE_EXTENDED  1
#if OS390
#define _OPEN_THREADS
#define _OPEN_SYS_SOCK_IPV6
#define _OPEN_MSGQ_EXT
#define _LP64
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stddef.h>
#include <limits.h>
#undef HAVE_CONFIG_H
#include <pthread.h>

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

/* different OSes have fd_set in
   a broad range of header files;
   we just include most of them (if they
   are available) */


#ifdef OS_VXWORKS
#include <sockLib.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <sys/mman.h>
#define RESTRICT __restrict__
#endif
#if HAVE_MEMORY_H
#include <memory.h>
#endif
#if HAVE_MAGIC_H
#include <magic.h>
#endif
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_SYS_MSG_H
#include <sys/msg.h>
#endif
#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif
#if HAVE_NETDB_H
#include <netdb.h>
#endif
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if HAVE_TIME_H
#include <time.h>
#endif
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#if HTTPS_SUPPORT
#include <gnutls/gnutls.h>
#endif
#if EPOLL_SUPPORT
#include <sys/epoll.h>
#endif
#include <limits.h>

#if HAVE_SEARCH_H
#include <search.h>
#else
#include "tsearch.h"
#endif

#if HTTPS_SUPPORT
#include <gcrypt.h>
#endif

#ifdef HAVE_POLL_H
#include <poll.h>
#endif

#ifdef LINUX
#include <sys/sendfile.h>
#endif

#if HAVE_NETINET_TCP_H
/* for TCP_CORK */
#include <netinet/tcp.h>
#endif

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
