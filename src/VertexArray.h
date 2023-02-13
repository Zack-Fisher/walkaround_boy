#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

//this class wrangles the vertex array and matches up each vbo with the correct vertex attributes.
class VertexArray
{
    private:
        unsigned int m_RendererID;
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};