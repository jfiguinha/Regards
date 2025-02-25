#include "vec2.h"

/// Holds all state information relevant to a character as loaded using FreeType
class Character {
public:
    GLTexture * glTexture; // ID handle of the glyph texture
    vec2d   Size;      // Size of glyph
    vec2d   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};