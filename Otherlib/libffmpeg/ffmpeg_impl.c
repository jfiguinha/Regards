#include "ffmpeg.h"
#include "ffmpeg_impl.h"

int ExecuteFFMpeg(int argc, char** argv, void (*foo)(int), int (*progress)(int, void*), void * wndProgress)
{
     return ExecuteFFMpegProgram(argc, argv, foo, progress, wndProgress);
}

void CleanupFFmpeg(int ret)
{
    ffmpeg_cleanup(ret);
}