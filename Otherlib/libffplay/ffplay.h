#pragma once
#include "ffplay_struct.h"

int OpenMovie(const char * filename, ListFunction* pf, int argc, char **argv);
int StopMovie();
