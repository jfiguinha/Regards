#ifndef __STDALIGN_H
#define __STDALIGN_H

#if defined(_MSC_VER) && !defined(__cplusplus)

#define alignof             _Alignof
#define alignas(nBytes)     __declspec(align(nBytes))

#endif

#endif

