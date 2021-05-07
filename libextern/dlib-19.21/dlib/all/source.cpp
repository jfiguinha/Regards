// Copyright (C) 2006  Davis E. King (davis@dlib.net)
// Copyright (C) 2006  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_ALL_SOURCe_
#define DLIB_ALL_SOURCe_

#if defined(DLIB_ALGs_) || defined(DLIB_PLATFORm_)
#include "../dlib_basic_cpp_build_tutorial.txt"
#endif


#include "../cuda/cpu_dlib.cpp"
#include "../cuda/tensor_tools.cpp"
#include "../threads/multithreaded_object_extension.cpp"
#include "../threads/threaded_object_extension.cpp"
#include "../threads/threads_kernel_1.cpp"
#include "../threads/threads_kernel_2.cpp"
#include "../threads/threads_kernel_shared.cpp"
#include "../threads/thread_pool_extension.cpp"
#include "../threads/async.cpp"
#include "../misc_api/misc_api_kernel_1.cpp"
#include "../misc_api/misc_api_kernel_2.cpp"
#include "../base64/base64_kernel_1.cpp"
#include "../entropy_decoder/entropy_decoder_kernel_2.cpp"
#define DLIB_ALL_SOURCE_END

#endif // DLIB_ALL_SOURCe_