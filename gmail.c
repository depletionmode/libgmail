
#include <string.h>

#include "global.h"
#include "gmail.h"

#define MAX_LINELEN 2048
#define MAX_CMDLEN 512

#define GMAIL_SUCCESS 0
#define GMAIL_ERR -1

GMAIL *gmail_init(char *user, char *pass)
{
  GMAIL *g = malloc( sizeof( GMAIL ) );

  g->dal = NULL;

  g->user = strndup( user, 20 );
  g->pass = strndup( pass, 20 );

  return g;
}

void gmail_kill(GMAIL *g)
{
  gmail_disconnect( g );

  N_FREE( g );
}

int gmail_connect(GMAIL *g)
{
  int err;
  char *p;
  char buf[MAX_LINELEN];
  char cmd[MAX_CMDLEN];

  memset( cmd, 0, MAX_CMDLEN );

  g->dal = dal_init( GMAIL_HOST, GMAIL_PORT );
  CHK_ERR_P( g->dal, "Could not init DAL!\n" );

  err = dal_readmatch( g->dal, buf, MAX_LINELEN, "OK", 5000);
  CHK_ERR( err, "Connection failed!\n" );

  p = strstr( buf, "OK" );
  CHK_ERR_P( p, "Failed to connect!\n" );

  snprintf( cmd, MAX_CMDLEN, ". login %s@gmail.com %s\n", g->user, g->pass);
  dal_write( g->dal, cmd, strlen( cmd ) );

  memset( buf, 0, MAX_LINELEN );
  err = dal_readmatch( g->dal, buf, MAX_LINELEN, "OK", 5000);
  CHK_ERR( err, "Invalid login details!\n" );

  D_LOG("Login successful\n");

  return GMAIL_SUCCESS;

error:
  return GMAIL_ERR;
}

void gmail_disconnect(GMAIL *g)
{
  dal_kill( g->dal );
}
