// Copyright (C) 2016  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_AsYNC_CPP_
#define DLIB_AsYNC_CPP_ 

// C++11 things don't work in old versions of visual studio 
#if !defined( _MSC_VER) ||  _MSC_VER >= 1900

#include "async.h"
#include <stdlib.h>
#include "../string.h"
#include <thread>

namespace dlib
{

// ----------------------------------------------------------------------------------------

    namespace impl
    {
        unsigned long default_num_threads()
        {
            return std::thread::hardware_concurrency();
        }
    }

// ----------------------------------------------------------------------------------------

    thread_pool& default_thread_pool()
    {
        static thread_pool tp(impl::default_num_threads());
        return tp;
    }
}

// ----------------------------------------------------------------------------------------

#endif

#endif // DLIB_AsYNC_CPP_


