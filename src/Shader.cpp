#include "Shader.h"

unsigned int Shader::GetUniformLocation(const std::string &name)
{
    return 0;
}

Shader::Shader(const std::string& filepath)
    : m_Filepath(filepath), m_RendererID(0)
{
}

void Shader::Bind() const
{
}

void Shader::Unbind() const
{
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
}

Shader::~Shader()
{
}
