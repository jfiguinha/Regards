#pragma once

#include <string>

class Shader {
public:
    unsigned int ID{};

    Shader() {}
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void init(const char *vertexSource, const char *fragmentSource);

    void use();

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;
};
