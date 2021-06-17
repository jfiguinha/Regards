#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/progdlg.h>
#include <stdexcept>
#include "ffmpeg_application.h"
#include <window_id.h>
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

void CFFmpegApp::ExitFunction(int x)
{
    throw x;
}

int CFFmpegApp::ProgressBarFunction(int x, void* progressWnd)
{
    wxProgressDialog* dialog = (wxProgressDialog*)progressWnd;
    if (false == dialog->Update(x%1000, "Video Progress ..."))
        return 1;
    return 0;
}

void CFFmpegApp::ExecuteFFmpeg()
{
    arrayOfCstrings = new char* [arrayOfStrings.size()];

    for (int i = 0; i < arrayOfStrings.size(); ++i)
        arrayOfCstrings[i] = (char*)arrayOfStrings[i].c_str();

    wxProgressDialog dialog("Export File", "Checking...", 1000, nullptr, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);

    try
    {
        void (*foo)(int);
        foo = &ExitFunction;

        int (*progress)(int, void*);
        progress = &ProgressBarFunction;

        ret = ExecuteFFMpegProgram(arrayOfStrings.size(), arrayOfCstrings, foo, progress, &dialog);
    }
    catch (int e)
    {
        ret = e;
    }
    catch (...)
    {

    }

    Cleanup(ret);
}

int CFFmpegApp::TestFFmpeg(const wxString& commandline)
{
    arrayOfStrings.push_back("ffmpeg");
    arrayOfStrings.push_back("-i");
    arrayOfStrings.push_back("d:\\video\\IMG_1293.MOV");
    arrayOfStrings.push_back("-c:v");
    arrayOfStrings.push_back("vp9");
    arrayOfStrings.push_back("-c:a");
    arrayOfStrings.push_back("libvorbis");
    arrayOfStrings.push_back("d:\\video\\output.mkv");

    ExecuteFFmpeg();

}

void CFFmpegApp::Cleanup(int ret)
{
    ffmpeg_cleanup(ret);
    delete[] arrayOfCstrings;
}


int CFFmpegApp::CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension, const wxString& outputFile)
{
    arrayOfStrings.push_back("ffmpeg");
    arrayOfStrings.push_back("-stream_loop");
    arrayOfStrings.push_back("-1");
    arrayOfStrings.push_back("-i");
    arrayOfStrings.push_back(inputAudioFile.ToStdString());
    arrayOfStrings.push_back("-c:a");
    arrayOfStrings.push_back(extension.ToStdString());
    arrayOfStrings.push_back("-t");
    arrayOfStrings.push_back(convertSecondsToHHMMSS(atoi(timeVideo)).c_str());
    arrayOfStrings.push_back("-vn");
    arrayOfStrings.push_back("-y");
    arrayOfStrings.push_back(outputFile.ToStdString());
    ExecuteFFmpeg();
}

int CFFmpegApp::ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString& inputAudioFile, const wxString& timeVideo, const wxString& outputFile)
{
    arrayOfStrings.push_back("ffmpeg");
    arrayOfStrings.push_back("-ss");
    arrayOfStrings.push_back("00:00:00");
    arrayOfStrings.push_back("-t");
    arrayOfStrings.push_back(timeVideo.ToStdString());
    arrayOfStrings.push_back("-i");
    arrayOfStrings.push_back(inputVideoFile.ToStdString());
    arrayOfStrings.push_back("-ss");
    arrayOfStrings.push_back("00:00:00");
    arrayOfStrings.push_back("-t");
    arrayOfStrings.push_back(timeVideo.ToStdString());
    arrayOfStrings.push_back("-i");
    arrayOfStrings.push_back(inputAudioFile.ToStdString());
    arrayOfStrings.push_back("-c:v");
    arrayOfStrings.push_back("copy");
    arrayOfStrings.push_back("-c:a");
    arrayOfStrings.push_back("copy");
    arrayOfStrings.push_back("-map");
    arrayOfStrings.push_back("0:v:0");
    arrayOfStrings.push_back("-map");
    arrayOfStrings.push_back("1:a:0");
    arrayOfStrings.push_back("-y");
    arrayOfStrings.push_back(outputFile.ToStdString());
    ExecuteFFmpeg();
}

