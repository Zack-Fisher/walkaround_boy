#include <iostream>
#include <GL/glew.h>

#include "renderer.h"

static void GLClearError()
{
    //clear the error pool
    //this function mutates the pool, pops the code out of it.
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError()
{
    while (GLenum error = glGetError())
    {
        std::cout << "[ogl err] " << error << std::endl;
    }
}