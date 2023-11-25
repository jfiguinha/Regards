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

#ifndef MOVIEBOX_HPP
#define MOVIEBOX_HPP

#include "bbox.hpp"
#include "customallocator.hpp"
#include "movieextendsbox.hpp"
#include "movieheaderbox.hpp"
#include "trackbox.hpp"


/**
 * Movie Box class
 * @details 'moov' box implementation as specified in the ISOBMFF specification.
 */
class MovieBox : public Box
{
public:
    MovieBox();
    ~MovieBox() override = default;

    /**
     * @brief Clear track boxes, initialize mvhd and other data.
     */
    void clear();

    /** @return Reference to the contained MovieHeaderBox. */
    MovieHeaderBox& getMovieHeaderBox();
    const MovieHeaderBox& getMovieHeaderBox() const;

    /** @return Pointers to all contained TrackBoxes. */
    const Vector<UniquePtr<TrackBox>>& getTrackBoxes() const;

    /** @return Pointer to contained TrackBox with given track id. */
    TrackBox* getTrackBox(uint32_t trackId);

    /** @return True if MovieExtendBox is present. */
    bool isMovieExtendsBoxPresent() const;

    /** @return MovieExtendsBox */
    const MovieExtendsBox* getMovieExtendsBox() const;

    /** Add MovieExtendsBox */
    void addMovieExtendsBox(UniquePtr<MovieExtendsBox> movieExtendsBox);

    /**
     * Add a TrackBox to MovieBox
     * @param trackBox TrackBox to add. */
    void addTrackBox(UniquePtr<TrackBox> trackBox);

    /**
     * @brief Serialize box data to the ISOBMFF::BitStream.
     * @see Box::writeBox()
     */
    void writeBox(ISOBMFF::BitStream& bitstr) const override;

    /**
     * @brief Deserialize box data from the ISOBMFF::BitStream.
     * @see Box::parseBox()
     */
    void parseBox(ISOBMFF::BitStream& bitstr) override;

private:
    MovieHeaderBox mMovieHeaderBox;               ///< The mandatory MovieHeaderBox
    Vector<UniquePtr<TrackBox>> mTracks;          ///< Contained TrackBoxes
    UniquePtr<MovieExtendsBox> mMovieExtendsBox;  ///< Optional Movie Extends Box
};

#endif /* end of include guard: MOVIEBOX_HPP */
