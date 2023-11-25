#pragma once
#include "ffplay_struct.h"

int OpenMovie(const char * filename, int volume, ListFunction* pf, int argc, char **argv);
int StopMovie();
int PlayPauseMovie();
int ffplay_SetTimePosition(int64_t time);
int ffplay_SetVolumePos(int volume);
