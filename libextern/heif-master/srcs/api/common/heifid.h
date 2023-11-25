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

#pragma once

namespace HEIF
{
#define IdType(Y, X)                               \
    class HEIF_DLL_PUBLIC X                        \
    {                                              \
        Y mId;                                     \
                                                   \
    public:                                        \
        constexpr X() noexcept                     \
            : mId(0)                               \
        {                                          \
        }                                          \
        constexpr X(Y a) noexcept                  \
            : mId(a)                               \
        {                                          \
        }                                          \
        Y get() const                              \
        {                                          \
            return mId;                            \
        }                                          \
    };                                             \
    inline bool operator<(const X& a, const X& b)  \
    {                                              \
        return a.get() < b.get();                  \
    }                                              \
    inline bool operator>(const X& a, const X& b)  \
    {                                              \
        return a.get() > b.get();                  \
    }                                              \
    inline bool operator!=(const X& a, const X& b) \
    {                                              \
        return a.get() != b.get();                 \
    }                                              \
    inline bool operator==(const X& a, const X& b) \
    {                                              \
        return a.get() == b.get();                 \
    }
}  // namespace HEIF
