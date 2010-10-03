
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>

#include "global.h"
#include "dal.h"

#define DAL_SUCCESS 0
#define DAL_ERR -1

DAL *dal_init(char *host, uint16_t port)
{
  int err;

  struct sockaddr_in s_addr;
  struct hostent *s = NULL;

  DAL *d = malloc( sizeof( DAL ) );

  d->host = host;
  d->port = port;

  SSL_library_init();
  SSL_load_error_strings();

  d->fd = socket( AF_INET, SOCK_STREAM, 0 );
  CHK_ERR( d->fd, "DAL: Error creating socket!\n" );

  s = gethostbyname( d->host );
  CHK_ERR_P( s, "DAL: Failed to resolve hostname!\n" );

  memset( &s_addr, 0, sizeof( s_addr ) );
  memcpy( &s_addr.sin_addr.s_addr, s->h_addr, s->h_length );
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons( d->port );

  d->ssl = SSL_new( SSL_CTX_new( SSLv23_client_method() ) );
  CHK_ERR_P( d->ssl, "DAL: SSL_new() failed!\n" );

  err = SSL_set_fd( d->ssl, d->fd );
  CHK_ERR( d->fd, "DAL: SSL_set_fd() failed!\n" );

  err = connect( d->fd, (struct sockaddr *)&s_addr, sizeof( s_addr ) );
  CHK_ERR( d->fd, "DAL: Failed to connect to %s:%d! \n", d->host, d->port );

  err = SSL_connect( d->ssl );
  CHK_ERR( d->fd, "DAL: SSL_connect() failed!\n" );

  D_LOG( "DAL: connected to %s:%d on fd = %d\n", d->host, d->port, d->fd );

  return d;

error:
  N_FREE( d );
  return NULL;
}

int dal_read(DAL *d, char *buf, int len)
{
  struct pollfd pfd;
  pfd.fd = d->fd;
  pfd.events = POLLIN | POLLERR;

  if( poll( &pfd, 1, 1) ) {
    if (pfd.revents == POLLIN)
      return SSL_read( d->ssl, buf, len );
  }

  return DAL_ERR;
}

int dal_readlast(DAL *d, char *buf, int len)
{
  int n = -1, m;

  do {
    m = n;
    n = dal_read( d, buf, len );
  } while ( n > -1 );

  return m;
}

int dal_readmatch( DAL *d, char *buf, int len, char *match, int timeout)
{
  int n = 0, m = 0, count = 0;
  char *p = NULL;

  do {
    n = dal_read( d, buf + n, len );
    if( n > 0 ) {
      p = strstr( buf, match );
      m += n;
    }
    count++;
  } while ( m < len && p == NULL && count < timeout);

  if( p )
    return m;

  return DAL_ERR;
}

int dal_write(DAL *d, char *buf, int len)
{
  D_LOG( "DAL: write(): %s", buf);
  return SSL_write( d->ssl, buf, len );
}

void dal_kill(DAL *d)
{
  if( d ) {
    SSL_shutdown( d->ssl );
    close( d->fd );

    N_FREE( d );
  }
}
