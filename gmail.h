/*
 * gmail.h
 */

#ifndef GMAIL_H_
#define GMAIL_H_

#include "dal.h"

#define GMAIL_HOST "imap.gmail.com"
#define GMAIL_PORT 993

typedef struct gmail {
  char *user,
       *pass;
  DAL *dal;
} GMAIL;

typedef struct gmail_folder {

} GMAIL_FOLDER;

typedef struct gmail_msg {

} GMAIL_MSG;

GMAIL *gmail_init(char *user, char *pass);
void gmail_kill(GMAIL *g);

int gmail_connect(GMAIL *g);
void gmail_disconnect(GMAIL *g);

//llist *gmail_get_folders(GMAIL *g);
//llist *gmail_get_messages(GMAIL_FOLDER *folder, char *label);

int gmail_label_create(GMAIL *g, char *label);
int gmail_label_delete(GMAIL *g, char *label);

int gmail_msg_set_label(GMAIL_MSG *m);
int gmail_msg_unset_label(GMAIL_MSG *m);

int gmail_msg_set_starred(GMAIL_MSG *m);
int gmail_msg_unset_starred(GMAIL_MSG *m);

int gmail_msg_set_spam(GMAIL_MSG *m);
int gmail_msg_unset_spam(GMAIL_MSG *m);

int gmail_msg_trash(GMAIL_MSG *m);

#endif /* GMAIL_H_ */
