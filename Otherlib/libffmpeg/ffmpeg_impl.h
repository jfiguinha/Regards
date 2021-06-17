#pragma once

void CleanupFFmpeg(int ret);

int ExecuteFFMpeg(int argc, char** argv, void (*foo)(int), int (*progress)(int, void*), void * wndProgress);
