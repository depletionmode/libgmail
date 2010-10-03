
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "global.h"
#include "dal.h"

DAL *dal_init(char *host, uint16_t port)
{
  struct sockaddr_in s_addr;
  struct hostent *s;

  DAL *d = malloc( sizeof( DAL ) );

  d->host = host;
  d->port = port;

  SSL_library_init();

  d->ssl = SSL_new( SSL_CTX_new( SSLv23_client_method() ) );

  d->fd = socket( AF_INET, SOCK_STREAM, 0 );
  s = gethostbyname( d->host );

  memset( &s_addr, 0, sizeof( s_addr ) );
  memcpy( s->h_addr, &s_addr.sin_addr.s_addr, s->h_length );
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons( d->port );

  connect( d->fd, (const struct sockaddr *)&s_addr, sizeof( s_addr ) );

  SSL_set_fd( d->ssl, d->fd );

  return d;
}

int dal_read(DAL *d, uint8_t *buf, int len)
{
  return SSL_read( d->ssl, buf, len );
}

int dal_write(DAL *d, uint8_t *buf, int len)
{
  return SSL_write( d->ssl, buf, len );
}

void dal_kill(DAL *d)
{
  if( d ) {
    SSL_shutdown( d->ssl );

    N_FREE( d );
  }
}
