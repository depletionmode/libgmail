
#ifndef DAL_H_
#define DAL_H_

#include <stdint.h>

#include <openssl/ssl.h>

typedef struct dal {
  char *host;
  uint16_t port;
  int fd;
  SSL *ssl;
} DAL;

DAL *dal_init(char *host, uint16_t port);
void dal_kill(DAL *d);

int dal_read(DAL *d, char *buf, int len);
int dal_readlast( DAL *d, char *buf, int len);
int dal_readmatch( DAL *d, char *buf, int len, char *match, int timeout);
int dal_write(DAL *d, char *buf, int len);


#endif /* DAL_H_ */
