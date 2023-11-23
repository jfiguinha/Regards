#pragma once
#include "ffplay_struct.h"

void SetListFunction(ListFunction * pf);
int OpenMovie(const char * filename, int argc, char **argv);
int StopMovie();
