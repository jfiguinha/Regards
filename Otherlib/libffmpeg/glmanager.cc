#include <header.h>
#include "glmanager.h"
#include <vector>
#include <epoxy/gl.h>
#include <GL/glext.h>

const char *vertexSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)";

const char *fragmentSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;

void main()
{
    vec3 yuv, rgb;
    vec3 yuv2r = vec3(1.164, 0.0, 1.596);
    vec3 yuv2g = vec3(1.164, -0.391, -0.813);
    vec3 yuv2b = vec3(1.164, 2.018, 0.0);

    yuv.x = texture(textureY, TexCoord).r - 0.0625;
    yuv.y = texture(textureU, TexCoord).r - 0.5;
    yuv.z = texture(textureV, TexCoord).r - 0.5;

    rgb.x = dot(yuv, yuv2r);
    rgb.y = dot(yuv, yuv2g);
    rgb.z = dot(yuv, yuv2b);

    FragColor = vec4(rgb, 1.0);
}
)";

// clang-format off
    float vertices[] = {
         1.0,  1.0, 0.0,     1.0, 0.0,
         1.0, -1.0, 0.0,     1.0, 1.0,
        -1.0, -1.0, 0.0,     0.0, 1.0,
        -1.0,  1.0, 0.0,     0.0, 0.0,
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
// clang-format on

GLManager::GLManager() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices),
                 indices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(3, texs);
    for (int i = 0; i < 3; i++) {
        glBindTexture(GL_TEXTURE_2D, texs[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    shader.init(vertexSource, fragmentSource);

    shader.use();
    shader.setInt("textureY", 0);
    shader.setInt("textureU", 1);
    shader.setInt("textureV", 2);
}

bool GLManager::ok() {
    return true;
}

void GLManager::setViewport(int x, int y, int width, int height) {
    viewWidth = width;
    viewHeight = height;
    glViewport(x, y, viewWidth, viewHeight);
}

void GLManager::draw(int width, int height, uint8_t **data, int *linesize) {
    bool changed = false;
    if (texWidth != width) {
        texWidth = width;
        changed = true;
    }
    if (texHeight != height) {
        texHeight = height;
        changed = true;
    }

    float tr = (float)texWidth / texHeight;
    float vr = (float)viewWidth / viewHeight;
    float r = tr / vr;
    if (r != ratio) {
        ratio = r;
        std::vector<float> vertices;
        // clang-format off
        if (tr > vr) {
            float p = vr / tr;
            vertices = {
                1.0,  p, 0.0,     1.0, 0.0,
                1.0, -p, 0.0,     1.0, 1.0,
               -1.0, -p, 0.0,     0.0, 1.0,
               -1.0,  p, 0.0,     0.0, 0.0,
            };
        } else if (tr < vr) {
            vertices = {
                r,  1.0, 0.0,     1.0, 0.0,
                r, -1.0, 0.0,     1.0, 1.0,
               -r, -1.0, 0.0,     0.0, 1.0,
               -r,  1.0, 0.0,     0.0, 0.0,
            };
        } else {
            vertices = {
                1.0,  1.0, 0.0,     1.0, 0.0,
                1.0, -1.0, 0.0,     1.0, 1.0,
               -1.0, -1.0, 0.0,     0.0, 1.0,
               -1.0,  1.0, 0.0,     0.0, 0.0,
            };
        }
        // clang-format on
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        vertices.size() * sizeof(float),
                        vertices.data());
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs[0]);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, linesize[0]);
    if (changed) {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     width,
                     height,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     data[0]);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        width,
                        height,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        data[0]);
    }
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texs[1]);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, linesize[1]);
    if (changed) {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     width / 2,
                     height / 2,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     data[1]);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        width / 2,
                        height / 2,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        data[1]);
    }
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texs[2]);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, linesize[2]);
    if (changed) {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     width / 2,
                     height / 2,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     data[2]);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        width / 2,
                        height / 2,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        data[2]);
    }

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
