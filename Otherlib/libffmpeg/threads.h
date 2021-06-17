#ifndef __MSVC_THREADS_H
#define __MSVC_THREADS_H

#if defined(_MSC_VER) && !defined(__cplusplus)

#define thread_local    __declspec(thread)

#endif

#endif

