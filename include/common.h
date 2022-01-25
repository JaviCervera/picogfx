#ifndef PICOGFX_COMMON_H
#define PICOGFX_COMMON_H

#include <stddef.h>
#include <stdlib.h>

typedef char bool_t;

#define FALSE 0
#define TRUE 1

#define _min(A, B) (A < B ? A : B)
#define _max(A, B) (A > B ? A : B)
#define _clamp(A, B, C) _min(_max(A, B), C)
#define _alloc(T) ((T*)malloc(sizeof(T)))
#define _allocmany(T,N) ((T*)malloc(N*sizeof(T)))
#define _realloc(M,T,N) (M = (T*)realloc(M,N*sizeof(T)))

#endif /* PICOGFX_COMMON_H */
