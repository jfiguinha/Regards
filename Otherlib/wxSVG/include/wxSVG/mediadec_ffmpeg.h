/////////////////////////////////////////////////////////////////////////////
// Name:        mediadec_ffmpeg.h
// Purpose:     FFMPEG Media Decoder
// Author:      Alex Thuering
// Created:     21.07.2007
// RCS-ID:      $Id: mediadec_ffmpeg.h,v 1.7 2011/08/02 06:48:13 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef FFMPEG_MEDIA_DECODER_H
#define FFMPEG_MEDIA_DECODER_H

#include <wx/string.h>
#include <wx/image.h>
struct AVFormatContext;
struct AVCodecContext;
struct AVFrame;
struct AVStream;

enum StreamType {
	stUNKNOWN = 0,
	stVIDEO,
	stAUDIO,
	stSUBTITLE
};

class wxFfmpegMediaDecoder {
public:
	wxFfmpegMediaDecoder();
	virtual ~wxFfmpegMediaDecoder();
	static void Init();
	
	virtual bool Load(const wxString& fileName);
	virtual void Close();
	
	unsigned int GetStreamCount();
	StreamType GetStreamType(unsigned int streamIndex);
	wxString GetCodecName(unsigned int streamIndex);
	int GetChannelNumber(unsigned int streamIndex);
	int GetSampleRate(unsigned int streamIndex);
	int GetBitrate(unsigned int streamIndex);
	
	double GetDuration();
	bool SetPosition(double pos, bool keyFrame = true);
	double GetPosition();
	
	virtual wxSize GetVideoSize();
	virtual float GetFrameAspectRatio();
	virtual float GetFps();
	
	virtual bool BeginDecode(int width = -1, int height = -1);
	virtual wxImage GetNextFrame();
	virtual void EndDecode();
	
private:
	AVFormatContext* m_formatCtx;
	int m_videoStream;
	AVCodecContext* m_codecCtx;
	AVFrame* m_frame;
	int m_width;
	int m_height;
	bool OpenVideoDecoder();
	void CloseVideoDecoder();
};

#endif //FFMPEG_MEDIA_DECODER_H
