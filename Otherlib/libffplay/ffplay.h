#pragma once

int Start(int argc, char **argv);
static void stream_seek(VideoState *is, int64_t pos, int64_t rel, int by_bytes);
static void video_refresh(void *opaque, double *remaining_time);