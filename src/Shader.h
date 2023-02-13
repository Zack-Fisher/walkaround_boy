#pragma once
#include <string>

//a layer of abstraction over shader program blocks, which can be applied directly to the gpu rendering pipeline through their class.
//we'll read from a file that contains multiple shaders, and then compile them into one ogl program.
class Shader
{
    private:
        unsigned int m_RendererID;
        std::string m_Filepath;
        //grab the uniform id from a compiled shader.
        unsigned int GetUniformLocation(const std::string& name);

    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
};