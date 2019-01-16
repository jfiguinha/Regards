//    Copyright (C) 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef VIDEO_THUMBNAILER_H
#define VIDEO_THUMBNAILER_H


#include <map>
#include <inttypes.h>
#include "ifilter.h"
class CRegardsBitmap;

struct AVFormatContext;

class CxImage;

namespace ffmpegthumbnailer
{

struct VideoFrame;
class ImageWriter;
class MovieDecoder;
template <typename T>
struct Histogram;

class VideoThumbnailer
{
public:
    VideoThumbnailer();
    VideoThumbnailer(int thumbnailSize, bool workaroundIssues, bool maintainAspectRatio, int imageQuality, bool smartFrameSelection);
    ~VideoThumbnailer();

	static int64_t GetMovieDuration(const string& videoFile);
	int GetThumbnail(const std::string& videoFile, CRegardsBitmap * image, MovieDecoder * movieDecoder, const int &percent, const int & thumbnailWidth, const int & thumbnailHeight, const int & rotation);
	MovieDecoder * MovieDecoderThumbnail(const std::string& videoFile, AVFormatContext* pAvContext = NULL);
	void DestroyMovieDecoder(MovieDecoder * movie);
	void generateThumbnail(const std::string& videoFile, CRegardsBitmap * image, const int & thumbnailWidth, const int & thumbnailHeight, int & rotation, AVFormatContext* pAvContext = NULL);
    void setThumbnailSize(int size);
    void setSeekPercentage(int percentage);
    void setSeekTime(const std::string& seekTime);
    void setWorkAroundIssues(bool workAround);
    void setImageQuality(int imageQuality);
    void setMaintainAspectRatio(bool enabled);
    void setSmartFrameSelection(bool enabled);
    void addFilter(IFilter* pFilter);
    void removeFilter(IFilter* pFilter);
    void clearFilters();

	void DecodeFile(const std::string& videoFile);
	int GetWidth();
	int GetHeight();
	int GetOrientation();
	const int &  GetTimePosition();

private:

    void generateSmartThumbnail(MovieDecoder& movieDecoder, VideoFrame& videoFrame);

    std::string getMimeType(const std::string& videoFile);
    std::string getExtension(const std::string& videoFilename);

    void generateHistogram(const VideoFrame& videoFrame, Histogram<int>& histogram);
    int getBestThumbnailIndex(std::vector<VideoFrame>& videoFrames, const std::vector<Histogram<int> >& histograms);
    void applyFilters(VideoFrame& frameData);

private:
    int                         m_ThumbnailSize;
    uint16_t                    m_SeekPercentage;
    bool                        m_OverlayFilmStrip;
    bool                        m_WorkAroundIssues;
    int                         m_ImageQuality;
    bool                        m_MaintainAspectRatio;
    bool                        m_SmartFrameSelection;
    std::string                 m_SeekTime;
    std::vector<IFilter*>       m_Filters;
	int							m_width;
	int							m_height;
	int							m_orientation;
	int							m_seekTimeInSecond;
    friend class VideoThumbnailerTest;
};

}

#endif
