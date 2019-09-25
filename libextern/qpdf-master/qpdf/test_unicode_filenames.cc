#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

static void do_copy(FILE* in, FILE* out)
{
    if ((in == 0) || (out == 0))
    {
        std::cerr << "errors opening files" << std::endl;
        exit(2);
    }
    char buf[10240];
    size_t len = 0;
    while ((len = fread(buf, 1, sizeof(buf), in)) > 0)
    {
        fwrite(buf, 1, len, out);
    }
    if (len != 0)
    {
        std::cerr << "errors reading or writing" << std::endl;
        exit(2);
    }
    fclose(in);
    fclose(out);
}

#ifdef WINDOWS_WMAIN

void copy(wchar_t const* outname)
{
#ifdef _MSC_VER
    FILE* in = 0;
    _wfopen_s(&in, L"minimal.pdf", L"rb");
    FILE* out = 0;
    _wfopen_s(&out, outname, L"wb");
#else
    FILE* in = _wfopen(L"minimal.pdf", L"rb");
    FILE* out = _wfopen(outname, L"wb");
#endif
    do_copy(in, out);
}

extern "C"
int wmain(int argc, wchar_t* argv[])
{
    // Unicode
    wchar_t const* f1 = L"auto-\xfc.pdf";
    wchar_t const* f2 = L"auto-\xf6\x03c0.pdf";
    copy(f1);
    copy(f2);
    std::cout << "created Unicode filenames" << std::endl;
    return 0;
}

#else

void copy(char const* outname)
{
    FILE* in = fopen("minimal.pdf", "rb");
    FILE* out = fopen(outname, "wb");
    do_copy(in, out);
}

int main(int argc, char* argv[])
{
    // Explicit UTF-8 encoding
    char const* f1 = "auto-\xc3\xbc.pdf";
    char const* f2 = "auto-\xc3\xb6\xcf\x80.pdf";
    copy(f1);
    copy(f2);
    std::cout << "created Unicode filenames" << std::endl;
    return 0;
}

#endif
