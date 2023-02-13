#pragma once

#include <vector>

#include "renderer.h"

//this class is heavily used to wrangle with in the vao class.

struct VertexBufferElement
{
    //the individual attributes of vertex types.
    unsigned int type;
    unsigned int count;
    bool normalized;
    
    //make the conversion here
    template<typename T>
    static GLenum ReturnGLType()
    {
        if (std::is_same<T, float>::value)
            return GL_FLOAT;
        else if (std::is_same<T, unsigned int>::value)
            return GL_UNSIGNED_INT;
        else if (std::is_same<T, unsigned char>::value)
            return GL_UNSIGNED_BYTE;
        else
            return 0;
    }

    static unsigned int GetSizeOfType(unsigned int type)
    {
        //for the attribute size allocations.
        switch (type)
        {
            case GL_FLOAT:  return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }

        return 0;
    }
};

class VertexBufferLayout
{
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;
    public:
        VertexBufferLayout()
            : m_Stride(0) {}

        template<typename T>
        void Push(int count)
        {
            GLenum gl_type = VertexBufferElement::ReturnGLType<T>();
            m_Elements.push_back({gl_type, count, true});
            m_Stride += VertexBufferElement::GetSizeOfType(gl_type);
        }

        inline const std::vector<VertexBufferElement> GetElements() const& {return m_Elements;}
        inline unsigned int GetStride() const {return m_Stride;}
};