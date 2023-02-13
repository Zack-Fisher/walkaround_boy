#include "VertexArray.h"

#include "renderer.h"

VertexArray::VertexArray()
{
    //this is fine i guess
    //could we make this more functional? classes suck
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);    
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

//think of passing a const as passing an immutable reference to the object.
//we can only call non-mutable (constant) methods on a const object.
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout &layout)
{
    Bind();
    //bind the vbo, then apply the attrib structure to the vbo right after, in the same method.
    //nice way to give consistent structure to our vbos

    vb.Bind();
    //like an iterator?
    //"auto" automatically deduces the type of a REFERENCE pointer.
    //we're using it for a vector iterator here, i guess.
    //why?
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];

        //starting byte, num of types, datatype, normalization, distance until next vertex
        //this all fully specifies one attribute in a vertex structure.
        glVertexAttribPointer(
            i,
            element.count,
            element.type,
            element.normalized,
            layout.GetStride(),
            //this last param is the offset, we need to accumulate the attrib byte sizes.
            //passing a read only pointer to an unsigned int? ??? why???
            //isn't this literally taking more memory on a 64-bit machine?
            (const void*)offset
        );

        glEnableVertexAttribArray(i);

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

