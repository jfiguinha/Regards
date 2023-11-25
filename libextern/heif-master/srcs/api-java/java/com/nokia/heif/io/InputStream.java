/*
 * This file is part of Nokia HEIF library
 *
 * Copyright (c) 2015-2021 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: heif@nokia.com
 *
 * This software, including documentation, is protected by copyright controlled by Nokia Corporation and/ or its subsidiaries. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating, any or all of this material requires the prior written consent of Nokia.
 *
 *
 */

package com.nokia.heif.io;

import java.nio.ByteBuffer;

/**
 * Interface class for loading files using a stream.
 */
public interface InputStream
{
    /**
     * Size of an indeterminately sized source, ie. a network stream
     */
    long INDERMINATE_SIZE = 0x7fffffffffffffffl;
    /**
     * Called when more data is read from the stream
     * @param buffer ByteBuffer where the data should be written
     * @param size Amount of bytes requested
     * @return Total amount of bytes written, 0 signals EoS
     */
    long read(ByteBuffer buffer, long size);

    /**
     * Seeks the stream the to given location
     * @param offset Location in the stream
     * @return If the seek was succesfull
     */
    boolean seek(long offset);

    /**
     * Current position of the stream
     * @return The current position in the stream
     */
    long position();

    /**
     * The size of the stream
     * @return The current size of the file.
     * Return StreamInterface::IndeterminateSize if the file size cannot be determined.
     */
    long size();

}
