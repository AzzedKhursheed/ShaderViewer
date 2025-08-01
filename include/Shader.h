#pragma once
#include <string>
#include <glad/gl.h>

class Shader {
public:
    GLuint ID;
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    void use() const;
    void setMat4(const std::string& name, const float* value) const;
    void setFloat(const std::string& name, float value) const;
    void reload();

private:
    GLuint m_Program;
    std::string m_VertexPath;
    std::string m_FragmentPath;
    bool compileAndLink(); 
    
};
