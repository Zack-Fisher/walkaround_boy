#include "VertexBuffer.h"
#include "GL/glew.h"
#include "renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    //each buffer still has its own id, it's just that the object/class contains the id rather than being a vague procedural thing.
    //opengl still provides unique ids, since we're just passing it a value to modify.
    //the key difference here is using the m_rednderer object id rather than giving each buffer its own id.
    //modern ogl, use a buffer to draw a triangle.

    //buffers can be accessed with their ids.
    glGenBuffers(1, &m_RendererID);
    //specify how we're going to use the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

    //the gpu doesn't really seem to care very much about out of bounds indexing. very casual. watch out.

    //pass the size of the array, and pass a pointer to the array
    //a pointer is passed when we pass an array in c++
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    //how do we specify how this data is laid out?
    //it's just a pointer to an array in the buffer right now.
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const 
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const 
{
    //we bind nothing, effectively unbinding this class's vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
