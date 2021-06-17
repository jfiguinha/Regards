#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "ffmpeg_application.h"

extern "C"
{
    #include "ffmpeg.h"
}

std::string convertSecondsToHHMMSS(int value) {
    std::string result;
    // compute h, m, s
    std::string h = std::to_string(value / 3600);
    std::string m = std::to_string((value % 3600) / 60);
    std::string s = std::to_string(value % 60);
    // add leading zero if needed
    std::string hh = std::string(2 - h.length(), '0') + h;
    std::string mm = std::string(2 - m.length(), '0') + m;
    std::string ss = std::string(2 - s.length(), '0') + s;
    // return mm:ss if hh is 00
    if (hh.compare("00") != 0) {
        result = hh + ':' + mm + ":" + ss;
    }
    else {
        result = mm + ":" + ss;
    }
    return result;
}

int CFFmpegApp::CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension, const wxString& outputFile)
{
    int i = 0;
    char* args[24];
    args[i] = new char[255];
    strcpy(args[i++], "ffmpeg");
    args[i] = new char[255];
    strcpy(args[i++], "-i");
    args[i] = new char[255];
    strcpy(args[i++], inputAudioFile);
    args[i] = new char[255];
    strcpy(args[i++], "-c:a");
    args[i] = new char[255];
    strcpy(args[i++], extension);
    args[i] = new char[255];
    strcpy(args[i++], "-t");
    args[i] = new char[255];
    strcpy(args[i++], convertSecondsToHHMMSS(atoi(timeVideo)).c_str());
    args[i] = new char[255];
    strcpy(args[i++], "-vn");
    args[i] = new char[255];
    strcpy(args[i++], "-y");
    args[i] = new char[255];
    strcpy(args[i++], outputFile);

    ExecuteFFMpegProgram(10, args);

    for (int i = 0; i < 10; i++)
        delete[] args[i];
}

int CFFmpegApp::ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString& inputAudioFile, const wxString& timeVideo, const wxString& outputFile)
{
    int i = 0;
    char* args[24];
    args[i] = new char[255];
    strcpy(args[i++], "ffmpeg");
    args[i] = new char[255];
    strcpy(args[i++], "-ss");
    args[i] = new char[255];
    strcpy(args[i++], "00:00:00");
    args[i] = new char[255];
    strcpy(args[i++], "-t");
    args[i] = new char[255];
    strcpy(args[i++], timeVideo);
    args[i] = new char[255];
    strcpy(args[i++], "-i");
    args[i] = new char[255];
    strcpy(args[i++], inputVideoFile);
    args[i] = new char[255];
    strcpy(args[i++], "-ss");
    args[i] = new char[255];
    strcpy(args[i++], "00:00:00");
    args[i] = new char[255];
    strcpy(args[i++], "-t");
    args[i] = new char[255];
    strcpy(args[i++], timeVideo);
    args[i] = new char[255];
    strcpy(args[i++], "-i");
    args[i] = new char[255];
    strcpy(args[i++], inputAudioFile);
    args[i] = new char[255];
    strcpy(args[i++], "-c:v");
    args[i] = new char[255];
    strcpy(args[i++], "copy");
    args[i] = new char[255];
    strcpy(args[i++], "-c:a");
    args[i] = new char[255];
    strcpy(args[i++], "copy");
    args[i] = new char[255];
    strcpy(args[i++], "-map");
    args[i] = new char[255];
    strcpy(args[i++], "0:v:0");
    args[i] = new char[255];
    strcpy(args[i++], "-map");
    args[i] = new char[255];
    strcpy(args[i++], "1:a:0");
    args[i] = new char[255];
    strcpy(args[i++], "-y");
    args[i] = new char[255];
    strcpy(args[i++], outputFile);

    ExecuteFFMpegProgram(23, args);

    for (int i = 0; i < 23; i++)
        delete[] args[i];
}

int CFFmpegApp::ExecuteFFmpegApp(const wxString& commandLine)
{
    //-ss 00:00:00  -t 30 -i "d:\video\file_example_MP4_1920_18MG.mp4" -ss 0:00:00 -t 30 -i "d:\video\song.mp3" -c:v copy -c:a copy -map 0:v:0 -map 1:a:0 -y "d:\video\output.mp4"
    int i = 0;
    char* args[24];
    args[i] = new char[255];
    strcpy(args[i++], "ffmpeg");
    args[i] = new char[255];
    strcpy(args[i++], "-ss");
    args[i] = new char[255];
    strcpy(args[i++], "00:00:00");
    args[i] = new char[255];
    strcpy(args[i++], "-t");
    args[i] = new char[255];
    strcpy(args[i++], "30");
    args[i] = new char[255];
    strcpy(args[i++], "-i");
    args[i] = new char[255];
    strcpy(args[i++], "d:\\video\\file_example_MP4_1920_18MG.mp4");
    args[i] = new char[255];
    strcpy(args[i++], "-ss");
    args[i] = new char[255];
    strcpy(args[i++], "00:00:00");
    args[i] = new char[255];
    strcpy(args[i++], "-t");
    args[i] = new char[255];
    strcpy(args[i++], "30");
    args[i] = new char[255];
    strcpy(args[i++], "-i");
    args[i] = new char[255];
    strcpy(args[i++], "d:\\video\\song.mp3");
    args[i] = new char[255];
    strcpy(args[i++], "-c:v");
    args[i] = new char[255];
    strcpy(args[i++], "copy");
    args[i] = new char[255];
    strcpy(args[i++], "-c:a");
    args[i] = new char[255];
    strcpy(args[i++], "copy");
    args[i] = new char[255];
    strcpy(args[i++], "-map");
    args[i] = new char[255];
    strcpy(args[i++], "0:v:0");
    args[i] = new char[255];
    strcpy(args[i++], "-map");
    args[i] = new char[255];
    strcpy(args[i++], "1:a:0");
    args[i] = new char[255];
    strcpy(args[i++], "-y");
    args[i] = new char[255];
    strcpy(args[i++], "d:\\video\\output.mp4");

    ExecuteFFMpegProgram(23, args);

    for (int i = 0; i < 23; i++)
        delete[] args[i];
}