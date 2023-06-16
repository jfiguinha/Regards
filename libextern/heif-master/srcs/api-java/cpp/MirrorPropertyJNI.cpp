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
 *
 *
 */

#include <jni.h>

#include "Helpers.h"
#include "TransformativeProperty.h"

#define CLASS_NAME MirrorProperty

extern "C"
{
    JNI_METHOD_ARG(jlong, createContextNative, jobject javaHEIF)
    {
        UNUSED(self);
        NATIVE_HEIF(nativeHeif, javaHEIF);
        auto* nativeObject = new HEIFPP::MirrorProperty(nativeHeif);
        return reinterpret_cast<jlong>(nativeObject);
    }

    JNI_METHOD(jboolean, getMirrorNative)
    {
        NATIVE_SELF;
        return static_cast<jboolean>(nativeSelf->mMirror.horizontalAxis);
    }

    JNI_METHOD_ARG(void, setMirrorNative, jboolean mirror)
    {
        NATIVE_SELF;
        nativeSelf->mMirror.horizontalAxis = mirror != 0;
    }
}
