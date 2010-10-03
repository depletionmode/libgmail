
#ifndef GLOBAL_H_
#define GLOBAL_H_

#define N_FREE(X) \
  do {            \
    if( X ) {     \
      free( X );  \
      X = NULL;   \
    }             \
  } while( 0 )

#endif /* GLOBAL_H_ */
