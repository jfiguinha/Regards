#pragma once
#ifndef TRACING_H
#define TRACING_H

#include <string>
#include <iostream>
#include <iomanip>

//#define TRACING_ENABLED

// Simple structure to handle function-call tracing.

// On debug builds, always build with tracing enabled unless explicitly disabled
#if defined(_DEBUG) && !defined(TRACING_DISABLED)
# define TRACING_ENABLED
#endif

// Define a preprocessor macro to help with the tracing
#ifdef TRACING_ENABLED
# define TRACE() tracing::tracer _tracer_object__ ## __COUNTER__ {__func__, __FILE__, __LINE__}
#else
# define TRACE() // Nothing
#endif

#ifdef TRACING_ENABLED
namespace tracing
{
    class tracer
    {
    public:
        tracer() = delete;  // Disallow default construction
        tracer(tracer const&) = delete;  // Disallow copy construction
        tracer(tracer&&) = delete;  // Disallow move construction
        tracer& operator=(tracer const&) = delete;  // Disallow copy assignment
        tracer& operator=(tracer&&) = delete;  // Disallow move assignment

        tracer(std::string const& fun, std::string const& file, int const line)
            : function_name{fun}, file_name{file}, line_number{line}
        {
            std::clog << "TRACE: Entering function " << function_name << " (" << file_name << ':' << line_number << ')' << std::endl;
        }

        ~tracer()
        {
            std::clog << "TRACE: Leaving function " << function_name << std::endl;
        }

    private:
        std::string function_name;
        std::string file_name;
        int         line_number;
    };
}
#endif // TRACING_ENABLED

#endif // TRACING_H