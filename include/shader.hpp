#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // set uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform1i(const std::string& name, int value);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void SetUinform1iv(const std::string& name, const int *v, unsigned int count);

private:
    ShaderProgramSource ParseShader();
    unsigned int CompileShader(unsigned int type, const std::string& source);

    int GetUniformLocation(const std::string& name);

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);



};


#endif /* end of include guard: SHADER_H */
