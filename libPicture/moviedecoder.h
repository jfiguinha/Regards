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

#pragma once

#include <string>


struct AVFilterGraph;
struct AVFilterContext;
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVStream;
struct AVFrame;
struct AVPacket;
struct AVRational;

namespace Regards
{
    namespace Video
    {

        struct VideoFrame;

        class MovieDecoder
        {
        public:
            MovieDecoder(AVFormatContext* pavContext = nullptr);
            ~MovieDecoder();

           
            void seek(int timeInSeconds);
            
            cv::Mat getScaledVideoFrame(const std::string& scaledSize, bool maintainAspectRatio);

            int getWidth();
            int getHeight();
            int getDuration();
			int getFps();
            void GetAspectRatio(int& ascpectNominator, int& ascpectDenominator);
            void initialize(const std::string& filename, bool preferEmbeddedMetadata);
           
            bool IsOpened();
            bool embeddedMetaDataIsAvailable();
            int GetRotation();
            

        private:
            int32_t findPreferredVideoStream(bool preferEmbeddedMetadata);
            int32_t getStreamRotation();
            void initializeVideo(bool preferEmbeddedMetadata);
            void initializeFilterGraph(const AVRational& timeBase, const std::string& size, bool maintainAspectRatio);
            void decodeVideoFrame();
            bool decodeVideoPacket();
            bool getVideoPacket();
            std::string getCodec();
            std::string createScaleString(const std::string& size, bool maintainAspectRatio);
            void destroy();
            void checkRc(int ret, const std::string& message);

        private:
            int m_VideoStream;
            AVFormatContext* m_pFormatContext;
            AVCodecContext* m_pVideoCodecContext;
            const AVCodec* m_pVideoCodec;
            AVFilterGraph* m_pFilterGraph;
            AVFilterContext* m_pFilterSource;
            AVFilterContext* m_pFilterSink;
            AVStream* m_pVideoStream;
            AVFrame* m_pFrame;
            AVPacket* m_pPacket;
            bool m_FormatContextWasGiven;
            bool m_AllowSeek;
            bool m_UseEmbeddedData;
            bool initFilter = false;
			bool isNextEmpty = false;
            std::string scaleOld = "";
        };

    }
}

