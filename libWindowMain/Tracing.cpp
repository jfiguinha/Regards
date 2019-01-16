#ifdef WIN32
#include <stdio.h>
#ifdef __cplusplus
extern "C"
{
    void __cyg_profile_func_enter(void *this_fn, void *call_site)
        __attribute__((no_instrument_function));
    void __cyg_profile_func_exit(void *this_fn, void *call_site)
        __attribute__((no_instrument_function));
}
#endif

void __cyg_profile_func_enter(void* this_fn, void* call_site)
{
    //printf("entering %p\n", (int*)this_fn);
}

void __cyg_profile_func_exit(void* this_fn, void* call_site)
{
    //printf("exiting %p\n", (int*)this_fn);
}

#endif