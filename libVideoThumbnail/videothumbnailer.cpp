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


#include "header.h"
#include "videothumbnailer.h"
#include "histogram.h"
#include "moviedecoder.h"
#include "stringoperations.h"
#include "filmstripfilter.h"



using namespace std;

namespace ffmpegthumbnailer
{

static const int SMART_FRAME_ATTEMPTS = 25;

VideoThumbnailer::VideoThumbnailer()
: m_ThumbnailSize(128)
, m_SeekPercentage(10)
, m_OverlayFilmStrip(false)
, m_WorkAroundIssues(false)
, m_ImageQuality(8)
, m_MaintainAspectRatio(true)
, m_SmartFrameSelection(false)
{
}

int64_t VideoThumbnailer::GetMovieDuration(const string& videoFile)
{
	int64_t duration = 0;
	AVFormatContext* pFormatCtx = nullptr;
	int ret = avformat_open_input(&pFormatCtx, videoFile.c_str(), NULL, NULL);
	if (pFormatCtx != nullptr)
	{
		duration = pFormatCtx->duration;
	}
	
	if(pFormatCtx != nullptr)
	{
		duration = pFormatCtx->duration;
		avformat_close_input(&pFormatCtx);
		avformat_free_context(pFormatCtx);
	}
	return duration;
}

VideoThumbnailer::VideoThumbnailer(int thumbnailSize, bool workaroundIssues, bool maintainAspectRatio, int imageQuality, bool smartFrameSelection)
: m_ThumbnailSize(thumbnailSize)
, m_SeekPercentage(10)
, m_WorkAroundIssues(workaroundIssues)
, m_ImageQuality(imageQuality)
, m_MaintainAspectRatio(maintainAspectRatio)
, m_SmartFrameSelection(smartFrameSelection)
{
	m_width = 0;
	m_height = 0;
	m_orientation = 0;
	m_seekTimeInSecond = 0;
}

VideoThumbnailer::~VideoThumbnailer()
{
}

MovieDecoder * VideoThumbnailer::MovieDecoderThumbnail(const string& videoFile, AVFormatContext* pAvContext)
{
	MovieDecoder * movieDecoder = new MovieDecoder(videoFile, NULL);
	movieDecoder->decodeVideoFrame(); //before seeking, a frame has to be decoded
	m_width = movieDecoder->getWidth();
	m_height = movieDecoder->getHeight();
	m_orientation = movieDecoder->getOrientation();
	return movieDecoder;
}


void VideoThumbnailer::DestroyMovieDecoder(MovieDecoder * movie)
{
	if (movie != NULL)
		delete movie;

	movie = NULL;
}

void VideoThumbnailer::setSeekPercentage(int percentage)
{
    m_SeekTime.clear();
    m_SeekPercentage = percentage > 95 ? 95 : percentage;
}

void VideoThumbnailer::setSeekTime(const std::string& seekTime)
{
    m_SeekTime = seekTime;
}

void VideoThumbnailer::setThumbnailSize(int size)
{
    m_ThumbnailSize = size;
}

void VideoThumbnailer::setWorkAroundIssues(bool workAround)
{
    m_WorkAroundIssues = workAround;
}

void VideoThumbnailer::setImageQuality(int imageQuality)
{
    m_ImageQuality = imageQuality;
}

void VideoThumbnailer::setMaintainAspectRatio(bool enabled)
{
    m_MaintainAspectRatio = enabled;
}

void VideoThumbnailer::setSmartFrameSelection(bool enabled)
{
    m_SmartFrameSelection = enabled;
}

int timeToSeconds(const std::string& time)
{
    int hours, minutes, seconds;
    sscanf(time.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);

    return (hours * 3600) + (minutes * 60) + seconds;
}

const int &  VideoThumbnailer::GetTimePosition()
{
	return m_seekTimeInSecond;
}

void  VideoThumbnailer::DecodeFile(const std::string& videoFile)
{
	MovieDecoder movieDecoder(videoFile, NULL);
	movieDecoder.decodeVideoFrame(); //before seeking, a frame has to be decoded
	m_width = movieDecoder.getWidth();
	m_height = movieDecoder.getHeight();
	m_orientation = movieDecoder.getOrientation();
}

int VideoThumbnailer::GetWidth()
{
	return m_width;
}

int VideoThumbnailer::GetHeight()
{
	return m_height;
}

int VideoThumbnailer::GetOrientation()
{
	return m_orientation;
}

cv::Mat VideoThumbnailer::GetThumbnail(const string& videoFile, MovieDecoder * movieDecoder, const int &percent, const int & thumbnailWidth, const int & thumbnailHeight, const int & rotation)
{
    cv::Mat image;
	if ((!m_WorkAroundIssues) || (movieDecoder->getCodec() != "h264")) //workaround for bug in older ffmpeg (100% cpu usage when seeking in h264 files)
	{
		try
		{
			int secondToSeekTo = (movieDecoder->getDuration() * percent) / 100;
			m_seekTimeInSecond = secondToSeekTo;
			movieDecoder->seek(secondToSeekTo);
		}
		catch (exception& e)
		{
			cerr << e.what() << ", will use first frame" << endl;
			//seeking failed, try the first frame again
			movieDecoder->destroy();
			movieDecoder->initialize(videoFile);
			movieDecoder->decodeVideoFrame();
		}
	}

	VideoFrame videoFrame;

	if (m_SmartFrameSelection)
	{
		generateSmartThumbnail(*movieDecoder, videoFrame);
	}
	else
	{
		if(thumbnailWidth > thumbnailHeight)
			m_ThumbnailSize = thumbnailWidth;
		else
			m_ThumbnailSize = thumbnailHeight;

		movieDecoder->getScaledVideoFrame(m_ThumbnailSize, m_MaintainAspectRatio, videoFrame);
	}

	applyFilters(videoFrame);

    image = cv::Mat(cv::Size(videoFrame.width, videoFrame.height), CV_8UC4, videoFrame.frameData);

	return image;
}

cv::Mat VideoThumbnailer::generateThumbnail(const string& videoFile, const int & thumbnailWidth, const int & thumbnailHeight, int & rotation, AVFormatContext* pAvContext)
{
    MovieDecoder movieDecoder(videoFile, pAvContext);
	rotation = movieDecoder.getOrientation();
	return GetThumbnail(videoFile, &movieDecoder, m_SeekPercentage, thumbnailWidth, thumbnailHeight, rotation);
}

void VideoThumbnailer::generateSmartThumbnail(MovieDecoder& movieDecoder, VideoFrame& videoFrame)
{
    vector<VideoFrame> videoFrames(SMART_FRAME_ATTEMPTS);
    vector<Histogram<int> > histograms(SMART_FRAME_ATTEMPTS);
    
    for (auto i = 0; i < SMART_FRAME_ATTEMPTS; ++i)
    {
        movieDecoder.decodeVideoFrame();
        movieDecoder.getScaledVideoFrame(m_ThumbnailSize, m_MaintainAspectRatio, videoFrames[i]);
        generateHistogram(videoFrames[i], histograms[i]);
    }
    
    int bestFrame = getBestThumbnailIndex(videoFrames, histograms);
    
    assert(bestFrame != -1);
    videoFrame = videoFrames[bestFrame];
}

string VideoThumbnailer::getMimeType(const string& videoFile)
{
    string extension = getExtension(videoFile);

    if (extension == "avi")
    {
        return "video/x-msvideo";
    }
    else if (extension == "mpeg" || extension == "mpg" || extension == "mpe" || extension == "vob")
    {
        return "video/mpeg";
    }
    else if (extension == "qt" || extension == "mov")
    {
        return "video/quicktime";
    }
    else if (extension == "asf" || extension == "asx")
    {
        return "video/x-ms-asf";
    }
    else if (extension == "wm")
    {
        return "video/x-ms-wm";
    }
    else if (extension == "mp4")
    {
        return "video/x-ms-wmv";
    }
    else if (extension == "mp4")
    {
        return "video/mp4";
    }
    else if (extension == "flv")
    {
        return "video/x-flv";
    }
    else
    {
        return "";
    }
}

string VideoThumbnailer::getExtension(const string& videoFilename)
{
    string extension;
    string::size_type pos = videoFilename.rfind('.');

    if (string::npos != pos)
    {
        extension = videoFilename.substr(pos + 1, videoFilename.size());
    }

    return extension;
}

void VideoThumbnailer::addFilter(IFilter* pFilter)
{
    m_Filters.push_back(pFilter);
}

void VideoThumbnailer::removeFilter(IFilter* pFilter)
{
    for (vector<IFilter*>::iterator iter = m_Filters.begin(); iter != m_Filters.end(); ++iter)
    {
        if (*iter == pFilter)
        {
            m_Filters.erase(iter);
            break;
        }
    }
}

void VideoThumbnailer::clearFilters()
{
    m_Filters.clear();
}

void VideoThumbnailer::applyFilters(VideoFrame& frameData)
{
    for (vector<IFilter*>::iterator iter = m_Filters.begin(); iter != m_Filters.end(); ++iter)
    {
        (*iter)->process(frameData);
    }
}

void VideoThumbnailer::generateHistogram(const VideoFrame& videoFrame, Histogram<int>& histogram)
{
    for (auto i = 0; i < videoFrame.height; ++i)
    {
        int pixelIndex = i * videoFrame.lineSize;
        for (auto j = 0; j < videoFrame.width * 3; j += 3)
        {
            ++histogram.r[videoFrame.frameData[pixelIndex + j]];
            ++histogram.g[videoFrame.frameData[pixelIndex + j + 1]];
            ++histogram.b[videoFrame.frameData[pixelIndex + j + 2]];
        }
    }
}

int VideoThumbnailer::getBestThumbnailIndex(vector<VideoFrame>& videoFrames, const vector<Histogram<int> >& histograms)
{
    Histogram<float> avgHistogram;
    for (size_t i = 0; i < histograms.size(); ++i)
    {
        for (auto j = 0; j < 255; ++j)
        {
            avgHistogram.r[j] += static_cast<float>(histograms[i].r[j]) / histograms.size();
            avgHistogram.g[j] += static_cast<float>(histograms[i].g[j]) / histograms.size();
            avgHistogram.b[j] += static_cast<float>(histograms[i].b[j]) / histograms.size();
        }
    }
    
    int bestFrame = -1;
    float minRMSE = FLT_MAX;
    for (size_t i = 0; i < histograms.size(); ++i)
    {
        //calculate root mean squared error
        float rmse = 0.0;
        for (auto j = 0; j < 255; ++j)
        {
            float error = fabsf(avgHistogram.r[j] - histograms[i].r[j])
                        + fabsf(avgHistogram.g[j] - histograms[i].g[j])
                        + fabsf(avgHistogram.b[j] - histograms[i].b[j]);
            rmse += (error * error) / 255;
        }
        
        rmse = sqrtf(rmse);
        if (rmse < minRMSE)
        {
            minRMSE = rmse;
            bestFrame = i;
        }
        
#ifdef DEBUG_MODE
        stringstream outputFile;
        outputFile << "frames/Frame" << setfill('0') << setw(3) << i << "_" << rmse << endl;
        ImageWriter* imageWriter = ImageWriterFactory<const string&>::createImageWriter(Png, outputFile.str());
        vector<uint8_t*> rowPointers;
        for (auto x = 0; x < videoFrames[i].height; ++x)
            rowPointers.push_back(&(videoFrames[i].frameData[x * videoFrames[i].lineSize]));
        imageWriter->writeFrame(&(rowPointers.front()), videoFrames[i].width, videoFrames[i].height, m_ImageQuality);
        delete imageWriter;
#endif
    }
    
#ifdef DEBUG_MODE
    cout << "Best frame was: " << bestFrame << "(RMSE: " << minRMSE << ")" << endl;
#endif

    return bestFrame;
}

}

