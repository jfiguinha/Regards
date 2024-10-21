#pragma once

#include "shader.h"

class GLManager {
public:
    GLManager();
    bool ok();
    void setViewport(int x, int y, int width, int height);
    void draw(int width, int height, uint8_t **data, int *linesize);

private:
    unsigned int VBO, VAO, EBO;
    unsigned int texs[3];

    float ratio{1};
    int viewWidth;
    int viewHeight;
    int texWidth{-1};
    int texHeight{-1};

    Shader shader;
};
