
#include <string.h>

#include "global.h"
#include "gmail.h"

#define MAX_LINELEN 2048
#define MAX_CMDLEN 512

#define GMAIL_SUCCESS 0
#define GMAIL_ERR -1

static GMAIL_FOLDER *_folder_lookup(GMAIL *g, char *name)
{
    GMAIL_FOLDER *folder;
    LLIST_ITER *iter;

    if( ( folder = llist_iterator_init( g->folders_ll, iter ) ) )
        do {
            if( !strcmp( folder->name, name ) )
                break;
        } while( folder = llist_iterate( iter ) );

    return folder;
}

static int _is_special_folder(GMAIL_FOLDER folder)
{
    char *folder_list[] = {
        "INBOX",
        "Junk E-mail",
        "Sent Items",
        "Deleted Items",
        "Drafts"
    }

    int i;

    for( i = 0; i < 5; i++)
        if( !strcmp( folder_list[i], folder->name ) )
            return 1;

    return 0;
}

static void _populate_folder_tree(GMAIL *g)
{
    int err;
    char buf[MAX_LINELEN];
    char cmd[] = ". list \"\" \"*\"\n";

    memset( buf, 0, MAX_LINELEN );

    dal_write( g->dal, cmd, strlen( cmd ) );
    while ( err = dal_readmatch( g->dal, buf, MAX_LINELEN, "\n" ) ) {
        GMAIL *folder;
        char *ptr;
        int len;

        if( !( ptr = strstr( buf, "\"/\"" ) ) )
            break;

        ptr += 5;

        folder = calloc( 1, sizeof( GMAIL_FOLDER ) );

        if( strstr( ptr, "[" ) ) {
            ptr++;
            len = ptr - strstr( ptr, "]/" );

            folder->prefix = calloc( 1, len + 1 );
            memcpy( folder->prefix, ptr, len );

            ptr += len + 2;
        }
        
        len = ptr - strstr( ptr, "\"" );

        folder->name = calloc( 1, len + 1 );
        memcpy( folder->name, ptr, len );
        
        if( !_is_special_folder( *folder ) || folder->prefix )   /* is a folder */
            llist_append( g->labels_ll, folder );
        else {    /* is a label */
            llist_append( g->folders_ll, folder->name );

            N_FREE( folder->prefix );
            N_FREE( folder );
        }
    }

#ifdef DEBUG
    LLIST_ITER *iter;
    D_LOG( "folders:\n" );    
    if( ( folder = llist_iterator_init( g->folders_ll, iter ) ) )
        do {
            D_LOG( "%s/%s\n", folder->prefix, folder->name );
        } while ( folder = llist_iterate( iter ) );
    D_LOG( "-----\n" );

    char *label;
    D_LOG( "labels:\n");
    if( ( label = llist_iterator_init( g->labels_ll, iter ) ) )
        do {
            D_LOG( "%s\n", label );
        } while ( label = llist_iterate( iter ) );
    D_LOG( "-----\n" );
#endif
}

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

  g->folders_ll = llist_init();
  g->labels_ll = llist_init();

  _populate_folder_tree( g );

  return GMAIL_SUCCESS;

error:
  return GMAIL_ERR;
}

void gmail_disconnect(GMAIL *g)
{
  dal_kill( g->dal );

  llist_kill( g->folders_ll );
  llist_kill( g->labels_ll );
}
