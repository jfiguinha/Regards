/* This file is part of Nokia HEIF library
 *
 * Copyright (c) 2015-2021 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: heif@nokia.com
 *
 * This software, including documentation, is protected by copyright controlled by Nokia Corporation and/ or its
 * subsidiaries. All rights are reserved.
 *
 * Copying, including reproducing, storing, adapting or translating, any or all of this material requires the prior
 * written consent of Nokia.
 */

#include "idgenerators.hpp"

namespace Context
{
    static const int INITIAL_VALUE = 1000;
    ContextId mValue               = INITIAL_VALUE;

    ContextId getValue()
    {
        return mValue++;
    }

    void reset()
    {
        mValue = INITIAL_VALUE;
    }
}  // namespace Context

namespace Track
{
    static const int INITIAL_VALUE = 1;
    uint32_t mTrackIdValue         = INITIAL_VALUE;
    uint16_t mAlternateGroupValue  = INITIAL_VALUE;

    HEIF::TrackId createTrackId()
    {
        if (mTrackIdValue < Context::INITIAL_VALUE)
        {
            return mTrackIdValue++;
        }
        else
        {
            mTrackIdValue = Context::getValue();
            return mTrackIdValue;
        }
    }

    HEIF::AlternateGroupId createAlternateGroupId()
    {
        return mAlternateGroupValue++;
    }

    void reset()
    {
        mTrackIdValue        = INITIAL_VALUE;
        mAlternateGroupValue = INITIAL_VALUE;
    }
}  // namespace Track

namespace FNVHash
{
    // Calculate FNV-1a hash for null-terminated C-string, http://isthe.com/chongo/tech/comp/fnv/
    uint64_t generate(const uint8_t* aData, uint64_t aSize)
    {
        static uint64_t offset = 2166136261u;
        static uint64_t prime  = 16777619u;

        uint64_t hash  = offset;
        uint64_t index = 0;
        while (index < aSize)
        {
            hash ^= static_cast<uint64_t>(aData[index]);
            hash *= prime;
            index++;
        }

        return hash;
    }
}  // namespace FNVHash
