// RegardsSelectApplication.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#ifdef WIN32
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#endif
#include <iostream>



// AVX CPU dispatching - based on Agner Fog's C++ vector class library:
// http://www.agner.org/optimize/vectorclass.zip

#include <stdio.h>
#include <stdbool.h>
#include <string>

//------------------------------------------------------------------------------
//>> BEGIN <instrset.h>

// Detect 64 bit mode
#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) ) && ! defined(__x86_64__)
# define __x86_64__ 1  // There are many different macros for this, decide on only one
#endif

// Find instruction set from compiler macros if INSTRSET not defined
// Note: Not all compilers define these macros automatically
#ifndef INSTRSET
#if defined(__AVX2__)
# define INSTRSET 8
#elif defined(__AVX__)
# define INSTRSET 7
#elif defined(__SSE4_2__)
# define INSTRSET 6
#elif defined(__SSE4_1__)
# define INSTRSET 5
#elif defined(__SSSE3__)
# define INSTRSET 4
#elif defined(__SSE3__)
# define INSTRSET 3
#elif defined(__SSE2__) || defined(__x86_64__)
# define INSTRSET 2
#elif defined(__SSE__)
# define INSTRSET 1
#elif defined(_M_IX86_FP)           // Defined in MS compiler. 1: SSE, 2: SSE2
# define INSTRSET _M_IX86_FP
#else 
# define INSTRSET 0
#endif // instruction set defines
#endif // INSTRSET

// Include the appropriate header file for intrinsic functions
#if INSTRSET > 7                       // AVX2 and later
# ifdef __GNUC__
#  include <x86intrin.h>                 // x86intrin.h includes header files for whatever instruction 
                                       // sets are specified on the compiler command line, such as:
                                       // xopintrin.h, fma4intrin.h
# else
#  include <immintrin.h>                 // MS version of immintrin.h covers AVX, AVX2 and FMA3
# endif // __GNUC__
#elif INSTRSET == 7
# include <immintrin.h>                 // AVX
#elif INSTRSET == 6
# include <nmmintrin.h>                 // SSE4.2
#elif INSTRSET == 5
# include <smmintrin.h>                 // SSE4.1
#elif INSTRSET == 4
# include <tmmintrin.h>                 // SSSE3
#elif INSTRSET == 3
# include <pmmintrin.h>                 // SSE3
#elif INSTRSET == 2
# include <emmintrin.h>                 // SSE2
#elif INSTRSET == 1
# include <xmmintrin.h>                 // SSE
#endif // INSTRSET

// AMD  instruction sets
#ifdef __XOP__
# ifdef __GNUC__
#  include <x86intrin.h>                 // AMD XOP (Gnu)
# else
#  include <ammintrin.h>                 // AMD XOP (Microsoft)
# endif //  __GNUC__
#elif defined (__SSE4A__)              // AMD SSE4A
# include <ammintrin.h>
#endif // __XOP__ 


// Define integer types with known size
#if defined(__GNUC__) || (defined(_MSC_VER) && _MSC_VER >= 1600)
  // Compilers supporting C99 or C++0x have stdint.h defining these integer types
#include <stdint.h>
#elif defined(_MSC_VER)
  // Older Microsoft compilers have their own definitions
    typedef signed   __int8   int8_t;
typedef unsigned __int8  uint8_t;
typedef signed   __int16  int16_t;
typedef unsigned __int16 uint16_t;
typedef signed   __int32  int32_t;
typedef unsigned __int32 uint32_t;
typedef signed   __int64  int64_t;
typedef unsigned __int64 uint64_t;
#ifndef _INTPTR_T_DEFINED
#define _INTPTR_T_DEFINED
#ifdef  __x86_64__
typedef int64_t intptr_t;
#else
typedef int32_t intptr_t;
#endif
#endif
#else
  // This works with most compilers
    typedef signed   char       int8_t;
typedef unsigned char      uint8_t;
typedef signed   short int  int16_t;
typedef unsigned short int uint16_t;
typedef signed   int        int32_t;
typedef unsigned int       uint32_t;
typedef long long           int64_t;
typedef unsigned long long uint64_t;
#ifdef  __x86_64__
typedef int64_t intptr_t;
#else
typedef int32_t intptr_t;
#endif
#endif

#include <stdlib.h>                              // define abs(int)

#ifdef _MSC_VER                                  // Microsoft compiler or compatible Intel compiler
#include <intrin.h>                              // define _BitScanReverse(int), __cpuid(int[4],int), _xgetbv(int)
#endif // _MSC_VER

// functions in instrset_detect.cpp
int  instrset_detect(void);                      // tells which instruction sets are supported
bool hasFMA3(void);                              // true if FMA3 instructions supported
bool hasFMA4(void);                              // true if FMA4 instructions supported
bool hasXOP(void);                              // true if XOP  instructions supported

// GCC version
#if defined(__GNUC__) && ! defined (GCC_VERSION)
#define GCC_VERSION  ((__GNUC__) * 10000 + (__GNUC_MINOR__) * 100 + (__GNUC_PATCHLEVEL__))
#endif

//<< END <instrset.h>
//------------------------------------------------------------------------------
//>> BEGIN <instrset_detect.cpp>

// Define interface to cpuid instruction.
// input:  eax = functionnumber, ecx = 0
// output: eax = output[0], ebx = output[1], ecx = output[2], edx = output[3]
static inline void cpuid(int output[4], int functionnumber) {
#if defined (_MSC_VER) || defined (__INTEL_COMPILER)       // Microsoft or Intel compiler, intrin.h included

    __cpuidex(output, functionnumber, 0);                  // intrinsic function for CPUID

#elif defined(__GNUC__)                                    // use inline assembly, Gnu/AT&T syntax

    int a, b, c, d;
    __asm("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "a"(functionnumber), "c"(0) : );
    output[0] = a;
    output[1] = b;
    output[2] = c;
    output[3] = d;

#else                                                      // unknown platform. try inline assembly with masm/intel syntax

    __asm {
        mov eax, functionnumber
        xor ecx, ecx
        cpuid;
        mov esi, output
            mov[esi], eax
            mov[esi + 4], ebx
            mov[esi + 8], ecx
            mov[esi + 12], edx
    }

#endif
}

// Define interface to xgetbv instruction
static inline int64_t xgetbv(int ctr) {
#if (defined (_MSC_FULL_VER) && _MSC_FULL_VER >= 160040000) || (defined (__INTEL_COMPILER) && __INTEL_COMPILER >= 1200) // Microsoft or Intel compiler supporting _xgetbv intrinsic

    return _xgetbv(ctr);                                   // intrinsic function for XGETBV

#elif defined(__GNUC__)                                    // use inline assembly, Gnu/AT&T syntax

    uint32_t a, d;
    __asm("xgetbv" : "=a"(a), "=d"(d) : "c"(ctr) : );
    return a | (((uint64_t)d) << 32);

#else  // #elif defined (_WIN32)                           // other compiler. try inline assembly with masm/intel/MS syntax

    uint32_t a, d;
    __asm {
        mov ecx, ctr
        _emit 0x0f
        _emit 0x01
        _emit 0xd0; // xgetbv
        mov a, eax
            mov d, edx
    }
    return a | (uint64_t(d) << 32);

#endif
}


/* find supported instruction set
    return value:
    0           = 80386 instruction set
    1  or above = SSE (XMM) supported by CPU (not testing for O.S. support)
    2  or above = SSE2
    3  or above = SSE3
    4  or above = Supplementary SSE3 (SSSE3)
    5  or above = SSE4.1
    6  or above = SSE4.2
    7  or above = AVX supported by CPU and operating system
    8  or above = AVX2
*/
int instrset_detect(void) {

    static int iset = -1;                                  // remember value for next call
    if (iset >= 0) {
        return iset;                                       // called before
    }
    iset = 0;                                              // default value
    int abcd[4] = { 0,0,0,0 };                               // cpuid results
    cpuid(abcd, 0);                                        // call cpuid function 0
    if (abcd[0] == 0) return iset;                         // no further cpuid function supported
    cpuid(abcd, 1);                                        // call cpuid function 1 for feature flags
    if ((abcd[3] & (1 << 0)) == 0) return iset;           // no floating point
    if ((abcd[3] & (1 << 23)) == 0) return iset;           // no MMX
    if ((abcd[3] & (1 << 15)) == 0) return iset;           // no conditional move
    if ((abcd[3] & (1 << 24)) == 0) return iset;           // no FXSAVE
    if ((abcd[3] & (1 << 25)) == 0) return iset;           // no SSE
    iset = 1;                                              // 1: SSE supported
    if ((abcd[3] & (1 << 26)) == 0) return iset;           // no SSE2
    iset = 2;                                              // 2: SSE2 supported
    if ((abcd[2] & (1 << 0)) == 0) return iset;           // no SSE3
    iset = 3;                                              // 3: SSE3 supported
    if ((abcd[2] & (1 << 9)) == 0) return iset;           // no SSSE3
    iset = 4;                                              // 4: SSSE3 supported
    if ((abcd[2] & (1 << 19)) == 0) return iset;           // no SSE4.1
    iset = 5;                                              // 5: SSE4.1 supported
    if ((abcd[2] & (1 << 23)) == 0) return iset;           // no POPCNT
    if ((abcd[2] & (1 << 20)) == 0) return iset;           // no SSE4.2
    iset = 6;                                              // 6: SSE4.2 supported
    if ((abcd[2] & (1 << 27)) == 0) return iset;           // no OSXSAVE
    if ((xgetbv(0) & 6) != 6)       return iset;           // AVX not enabled in O.S.
    if ((abcd[2] & (1 << 28)) == 0) return iset;           // no AVX
    iset = 7;                                              // 7: AVX supported
    cpuid(abcd, 7);                                        // call cpuid leaf 7 for feature flags
    if ((abcd[1] & (1 << 5)) == 0) return iset;           // no AVX2
    iset = 8;                                              // 8: AVX2 supported
    return iset;
}

//<< END <instrset_detect.cpp>
//------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    int iset = instrset_detect();
    bool hasAVX = iset >= 7;
    std::string filename = "";
    std::string program = "";

    //CFFmpegApp::ExecuteFFmpegApp("-ss 00:00:00 -t 30 -i \"d:\video\file_example_MP4_1920_18MG.mp4\" -ss 0:00:00 -t 30 -i \"d:\\video\\song.mp3\" -c:v copy -c:a copy -map 0:v:0 -map 1:a:0 -y \"d:\\video\\output.mp4\"");
    
#ifdef WIN32
    if (argc > 1)
        filename = argv[1];
    if (hasAVX)
        program = "RegardsViewer_avx.exe";
    else
        program = "RegardsViewer_sse3.exe";

    if (filename != "")
        program = program + " " + filename;

    if(hasAVX)
        printf("Exec RegardsViewer with avx \n");
    else
        printf("Exec RegardsViewer with sse \n");
        
    //wxExecute(program, wxEXEC_ASYNC);
    return 0;
#else
    if (argc > 1)
        filename = argv[1];
    if (hasAVX)
        program = "./RegardsViewer_avx";
    else
        program = "./RegardsViewer_sse3";

    if (filename != "")
        program = program + " " + filename;

    if(hasAVX)
        printf("Exec RegardsViewer with avx \n");
    else
        printf("Exec RegardsViewer with sse \n");
    //wxExecute(program, wxEXEC_ASYNC);
    return system(program.c_str());
#endif
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
