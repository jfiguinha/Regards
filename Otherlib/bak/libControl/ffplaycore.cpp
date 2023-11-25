#include <header.h>
#include "ffplaycore.h"
#include <window_id.h>
#include <fstream>
#include <ConvertUtility.h>
extern "C"
{
	#include <ffplay.h>
}
CFFmfc::CFFmfc()
{

}

CFFmfc::~CFFmfc()
{



}

int CFFmfc::StartMovie(void * arg)
{
	CFFmfc* ffmc = (CFFmfc*)arg;
	if (ffmc != nullptr)
	{
		int argc = 1;
		char* argv[1] = { wxString(ffmc->filename).char_str() };
		int value = OpenMovie(ffmc->filename, ffmc->volume, ffmc->functionList, argc, argv);


		if (ffmc->functionList != NULL)
			ffmc->functionList->videoEnd(ffmc->functionList->wxWindowParent);
	}



	return 0;
}


void CFFmfc::SetTimePosition(int64_t time)
{
	ffplay_SetTimePosition(time);
}

bool CFFmfc::Quit()
{
	if (startMovie != nullptr)
	{
		StopMovie();
		startMovie->join();
		delete startMovie;
		startMovie = nullptr;
	}
	return 0;
}

int CFFmfc::SetFile(const wxString& filename, ListFunction* functionList, const wxString& acceleratorHardware, const bool& isOpenGLDecoding, const int& volume)
{

	if (startMovie != nullptr)
	{
		startMovie->join();
		delete startMovie;
		startMovie = nullptr;
	}


	this->filename = filename;
	this->functionList = functionList;
	this->acceleratorHardware = acceleratorHardware;
	this->isOpenGLDecoding = isOpenGLDecoding;
	this->volume = volume;
	startMovie = new std::thread(StartMovie, this);
	return 0;
}

void CFFmfc::PlayPause()
{
	PlayPauseMovie();
}


void CFFmfc::Change_subtitle_stream(int newStreamIndex)
{

}

void CFFmfc::Change_audio_stream(int newStreamIndex)
{

}

void CFFmfc::SetVolume(const int& pos)
{
	volume = max(min(pos, 100), 0);
	ffplay_SetVolumePos(pos);
}

int CFFmfc::GetVolume()
{
	return volume;
}

void CFFmfc::VideoDisplaySize(int width, int height)
{

}

int64_t CFFmfc::GetTimePosition()
{
	return 0;
}