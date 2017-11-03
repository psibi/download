/*-
 * Copyright (c) 2006 Aaron Griffin <aaronmgriffin@gmail.com>
 *
 * Based on original libfetch code from:
 * Copyright (c) 1998-2004 Dag-Erling Coïdan Smørgrav
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdint.h>

/* BSD seems to use this alot - we don't have it */
#define __DECONST(type, v) (type)((uintptr_t)(void *)(v))

#define FTP_DEFAULT_PORT	21
#define HTTP_DEFAULT_PORT	80
#define FTP_DEFAULT_PROXY_PORT	21
#define HTTP_DEFAULT_PROXY_PORT	3128

#ifdef WITH_SSL
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

/* Connection */
struct connection {
	int     sd;		/* socket descriptor */
	char    *buf;		/* buffer */
	size_t  bufsize;	/* buffer size */
	size_t  buflen;	/* length of buffer contents */
	int     err;		/* last protocol reply code */
    fd_set  readfds;
#ifdef WITH_SSL
	SSL		*ssl;		/* SSL handle */
	SSL_CTX		*ssl_ctx;	/* SSL context */
	X509		*ssl_cert;	/* server certificate */
	SSL_METHOD	*ssl_meth;	/* SSL method */
#endif
	int		 ref;		/* reference count */
};
typedef struct connection conn_t;

/* Structure used for error message lists */
struct downloaderr {
	const int	 num;
	const int	 cat;
	const char	*string;
};

/* for _download_writev */
struct iovec;

void _download_seterr(struct downloaderr *, int);
void _download_unseterr(void);
void _download_syserr(void);
void _download_info(const char *, ...);
int _download_default_port(const char *);
int _download_default_proxy_port(const char *);
int _download_bind(int, int, const char *);
conn_t *_download_connect(const char *, int, int, int);
conn_t *_download_reopen(int);
conn_t *_download_ref(conn_t *);
int _download_ssl(conn_t *, int);
ssize_t _download_read(conn_t *, char *, size_t);
int _download_getln(conn_t *);
ssize_t _download_write(conn_t *, const char *, size_t);
ssize_t _download_writev(conn_t *, struct iovec *, int);
int _download_putln(conn_t *, const char *, size_t);
int _download_close(conn_t *);
int _download_netrc_auth(struct url *url);

#define _ftp_seterr(n)	 _download_seterr(_ftp_errlist, n)
#define _http_seterr(n)	 _download_seterr(_http_errlist, n)
#define _netdb_seterr(n) _download_seterr(_netdb_errlist, n)
#define _url_seterr(n)	 _download_seterr(_url_errlist, n)

#ifdef DEBUG
#define DBG(x) do { if (downloadDebug) { x; } } while (0)
#else
#define DBG(x) do { } while (0)
#endif

/*
 * Check whether a particular flag is set
 */
#define CHECK_FLAG(x)	(flags && strchr(flags, (x)))

#endif /*COMMON_H_INCLUDED*/
