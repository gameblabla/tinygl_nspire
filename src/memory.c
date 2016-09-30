/*
 * Memory allocator for TinyGL
 */
#include "zgl.h"

/* modify these functions so that they suit your needs */

inline void gl_free(void *p)
{
    free(p);
}

inline void *gl_malloc(int size)
{
    return malloc(size);
}

inline void *gl_zalloc(int size)
{
    return calloc(1, size);
}
