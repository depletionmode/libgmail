

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
int dal_read(DAL *d, uint8_t *buf, int len);
int dal_write(DAL *d, uint8_t *buf, int len);
void dal_kill(DAL *d);


#endif /* DAL_H_ */
