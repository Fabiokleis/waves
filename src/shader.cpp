#include "shader.hpp"
#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>


#include <GL/glew.h>

Shader::Shader(const std::string& vertex_shader, const std::string& frag_shader)
    : vertex_shader(vertex_shader), frag_shader(frag_shader), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader();
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cerr << "Cannot get uniform location" << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    int location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::SetUniform1f(const std::string &name, float v) {
    int location = GetUniformLocation(name);
    glUniform1f(location, v);
}
void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    int location = GetUniformLocation(name);
    glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4& matrix)
{
    int location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUinform1iv(const std::string& name, const int *v, unsigned int count) {
    int location = GetUniformLocation(name);
    glUniform1iv(location, count, v);
}

ShaderProgramSource Shader::ParseShader() {
    std::ifstream vertex_stream(vertex_shader);
    std::ifstream frag_stream(frag_shader);

    std::string line;
    std::stringstream stream[2];


    while(getline(vertex_stream, line)) {
        stream[0] << line << '\n';
    }

    line = "";
    while(getline(frag_stream, line)) {
        stream[1] << line << '\n';
    }
    
    return { stream[0].str(), stream[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

