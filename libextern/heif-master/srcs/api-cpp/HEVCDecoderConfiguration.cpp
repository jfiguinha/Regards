/*
 * This file is part of Nokia HEIF library
 *
 * Copyright (c) 2015-2021 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: heif@nokia.com
 *
 * This software, including documentation, is protected by copyright controlled by Nokia Corporation and/ or its
 * subsidiaries. All rights are reserved. Copying, including reproducing, storing, adapting or translating, any or all
 * of this material requires the prior written consent of Nokia.
 */

#include "HEVCDecoderConfiguration.h"

#include <heifreader.h>
#include <heifwriter.h>

#include <cstring>

#include "H26xTools.h"

using namespace HEIFPP;

HEVCDecoderConfiguration::HEVCDecoderConfiguration(Heif* aHeif)
    : DecoderConfig(aHeif, HEIF::FourCC("hvc1"))
{
}
HEVCDecoderConfiguration::HEVCDecoderConfiguration(Heif* aHeif, const HEIF::FourCC& aType)
    : DecoderConfig(aHeif, aType)
{
}
HEIF::ErrorCode HEVCDecoderConfiguration::convertToRawData(const HEIF::Array<HEIF::DecoderSpecificInfo>& aConfig,
                                                           std::uint8_t*& aData,
                                                           std::uint32_t& aSize) const
{
    aSize = 0;
    for (std::size_t i = 0; i < aConfig.size; i++)
    {
        if ((aConfig[i].decSpecInfoType != HEIF::DecoderSpecInfoType::HEVC_VPS) &&
            (aConfig[i].decSpecInfoType != HEIF::DecoderSpecInfoType::HEVC_SPS) &&
            (aConfig[i].decSpecInfoType != HEIF::DecoderSpecInfoType::HEVC_PPS))
        {
            return HEIF::ErrorCode::MEDIA_PARSING_ERROR;
        }


        aSize += static_cast<uint32_t>(aConfig[i].decSpecInfoData.size);
    }
    std::uint8_t* d = aData = new std::uint8_t[aSize];
    for (std::size_t i = 0; i < aConfig.size; i++)
    {
        std::memcpy(d, aConfig[i].decSpecInfoData.begin(), aConfig[i].decSpecInfoData.size);
        d += aConfig[i].decSpecInfoData.size;
    }
    return HEIF::ErrorCode::OK;
}
HEIF::ErrorCode HEVCDecoderConfiguration::convertFromRawData(const std::uint8_t* aData, std::uint32_t aSize)
{
    // ISO / IEC 14496 - 15:2017 8.3.3.1 HEVC decoder configuration record :
    // It is recommended that the arrays be in the order VPS, SPS, PPS, prefix SEI, suffix SEI.

    mConfig.decoderSpecificInfo = HEIF::Array<HEIF::DecoderSpecificInfo>(3);

    // NOTE: only VPS,PPS,SPS is saved here, and we expect all three to exist.
    NAL_State d;
    std::uint32_t flags;
    d.init_parse(aData, aSize);
    flags = 0;
    for (;;)
    {
        const std::uint8_t* nal_data = nullptr;
        std::uint64_t nal_len        = 0;
        if (!d.parse_byte_stream(nal_data, nal_len))
        {
            break;
        }
        HEIF::DecoderSpecInfoType type;
        type                = static_cast<HEIF::DecoderSpecInfoType>((nal_data[0] >> 1) & 0x3f);
        std::uint32_t index = 0;
        if (type == HEIF::DecoderSpecInfoType::HEVC_VPS)
        {
            index = 0;
        }
        else if (type == HEIF::DecoderSpecInfoType::HEVC_SPS)
        {
            index = 1;
        }
        else if (type == HEIF::DecoderSpecInfoType::HEVC_PPS)
        {
            index = 2;
        }
        else
        {
            return HEIF::ErrorCode::MEDIA_PARSING_ERROR;
        }
        if ((flags & (1u << index)) != 0)
        {
            return HEIF::ErrorCode::MEDIA_PARSING_ERROR;
        }
        flags |= 1u << index;
        mConfig.decoderSpecificInfo[index].decSpecInfoType    = type;
        mConfig.decoderSpecificInfo[index].decSpecInfoData    = HEIF::Array<uint8_t>(nal_len + 4);
        mConfig.decoderSpecificInfo[index].decSpecInfoData[0] = mConfig.decoderSpecificInfo[index].decSpecInfoData[1] =
            mConfig.decoderSpecificInfo[index].decSpecInfoData[2] = 0;
        mConfig.decoderSpecificInfo[index].decSpecInfoData[3]     = 1;
        std::memcpy(mConfig.decoderSpecificInfo[index].decSpecInfoData.elements + 4, nal_data, nal_len);
    }
    if (flags != 7)
    {
        return HEIF::ErrorCode::MEDIA_PARSING_ERROR;
    }
    return HEIF::ErrorCode::OK;
}

HEIF::ErrorCode HEVCDecoderConfiguration::setConfig(const std::uint8_t* aData, std::uint32_t aSize)
{
    delete[] mBuffer;
    mBuffer     = nullptr;
    mBufferSize = 0;
    mBuffer     = new std::uint8_t[aSize];
    std::memcpy(mBuffer, aData, aSize);
    mBufferSize = aSize;
    return convertFromRawData(mBuffer, mBufferSize);
}

void HEVCDecoderConfiguration::getConfig(uint8_t*& aData, std::uint32_t& aSize) const
{
    aData = mBuffer;
    aSize = mBufferSize;
}