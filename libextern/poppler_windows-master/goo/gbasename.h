//========================================================================
//
// gbasename.h
//
// Wrapper for libgen's basename() call which returns a std::string.
// This is a convenience method working around questionable behavior
// in the copy of basename() provided by libgen.h.
//
// This file is licensed under the GPLv2 or later
//
// Copyright (C) 2018 Greg Knight <lyngvi@gmail.com>
//
//========================================================================

#ifndef GBASENAME_H
#define GBASENAME_H

#include <string>

std::string gbasename(const char* input);

#endif // ndef GBASENAME_H
