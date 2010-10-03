/*
 * gmail.h
 */

#ifndef GMAIL_H_
#define GMAIL_H_

#include "llist.h"
#include "dal.h"

#define GMAIL_HOST "imap.gmail.com"
#define GMAIL_PORT 993

#define N_MSG_FREE(X)   \
  do {                  \
    N_FREE(X->to);      \
    N_FREE(X->cc);      \
    N_FREE(X->bcc);     \
    N_FREE(X->subject); \
    N_FREE(X->body);    \
  } while( 0 )

typedef struct gmail {
  char *user,
       *pass;
  DAL *dal;
  LLIST *folders_ll;
  LLIST *labels_ll;
} GMAIL;

typedef struct gmail_folder {
    char *name,
         *prefix;
} GMAIL_FOLDER;

typedef struct gmail_msg {
  char *to,
       *cc,
       *bcc,
       *subject,
       *body;
  GMAIL_FOLDER folder;
} GMAIL_MSG;

GMAIL *gmail_init(char *user, char *pass);  /* init library obj */
void gmail_kill(GMAIL *g);                  /* free library obj */

int gmail_connect(GMAIL *g);        /* connect to gmail imap server */
void gmail_disconnect(GMAIL *g);    /* disconnect from gmail imap server */

LLIST *gmail_msg_pull(GMAIL_FOLDER folder,  /* return list of messages */
                      char *label,
                      int start_msg,
                      int end_msg);
int gmail_msg_count(GMAIL_FOLDER folder,    /* return msg count */
                    char *label);

int gmail_label_create(GMAIL *g, char *label);  /* create a new label */
int gmail_label_edit(GMAIL *g, char *label,     /* rename a label */
                     char *new_label);
int gmail_label_delete(GMAIL *g, char *label);  /* delete an existing label */

int gmail_msg_set_label(GMAIL_MSG *m);      /* tag a message with a label */
int gmail_msg_unset_label(GMAIL_MSG *m);    /* untag a message from a label */

int gmail_msg_set_starred(GMAIL_MSG *m);    /* set a message as starred */
int gmail_msg_unset_starred(GMAIL_MSG *m);  /* unset a mesage as starred */

int gmail_msg_set_spam(GMAIL_MSG *m);       /* mark a message as spam */
int gmail_msg_unset_spam(GMAIL_MSG *m);     /* unmark a message as spam */

int gmail_msg_bin(GMAIL_MSG *m);    /* send a message to the bin */

int gmail_bin_empty(GMAIL *g);      /* empty the bin */

#endif /* GMAIL_H_ */
