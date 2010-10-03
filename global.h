
#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>

#define N_FREE(X) \
  do {            \
    if( X ) {     \
      free( X );  \
      X = NULL;   \
    }             \
  } while( 0 )

#ifdef DEBUG
#  define D_LOG(X, ...)                 \
    fprintf( stderr, "libgmail: " X, ##__VA_ARGS__)
#else
#  define D_LOG(X, ...)
#endif

#define X_CHK_ERR(A, X, Y, ...) \
  if( A ) {                     \
    D_LOG(Y, ##__VA_ARGS__);    \
    goto error;                 \
  }

#define CHK_ERR(X, Y, ...)              \
  X_CHK_ERR(X < 0, X, Y, ##__VA_ARGS__)

#define CHK_ERR_P(X, Y, ...)                \
  X_CHK_ERR(X == NULL, X, Y, ##__VA_ARGS__)

#endif /* GLOBAL_H_ */
