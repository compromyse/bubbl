#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef _BITSIZE_STDDEF_H
#define _BITSIZE_STDDEF_H

#define _SIZE_T
#if defined(__s390__) || defined(__cris__)
typedef unsigned long size_t;
#else
typedef unsigned int size_t;
#endif

#define _PTRDIFF_T
typedef signed int ptrdiff_t;

#endif /* _BITSIZE_STDDEF_H */

#undef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *) 0)
#endif

#undef offsetof
#define offsetof(t, m) ((size_t) &((t *) 0)->m)

#endif /* _STDDEF_H */
