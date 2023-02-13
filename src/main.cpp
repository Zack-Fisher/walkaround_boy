#include <iostream>
#include <GL/glew.h>
//gives access to the gpu impls of the opengl spec
//need to include glew before any other ogl stuff
//glew bootstraps gl?
#include <GLFW/glfw3.h>
//glfw is a window handling and init library, abstraction over os stuff
#include <GL/gl.h>
//actual gl functions.

//file reading in c++
#include <fstream>
//gives access to stringstreams, which are alternatives to cout and cin, act in a very similar way.
//we use this for filereading.
#include <sstream>

#include "renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "engine/Engine.h"
#include "engine/components/Timer.h"

//define a composite datatype to return after parsing the shader file.
//we will make a given shader file return both shader types, then compile them together into one program.
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};
//structs need to be closed with a ;

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    //what we're doing is reading each line into the correct shader stringstream buffer.
    //every time we see our predefined #shader directive, we switch the type.
    //we add the line into the type's designated buffer in the else statement.
    std::string line;
    //this is an array of arbitrarily long sequences of strings.
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    //repeatedly store the lines into the mutable line std::string var.
    while (getline(stream, line))
    {
        //if it's not not positive, it is positive.
        //returns -1 if not found, in general.
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            //we use negative indexing here.
            //we grab the stringbuffer into the fragment ss or the vertex ss.
            //this is why we associated the enum type with a number, so this cast index works.
            //we can preform the same ops as cin and cout on stringstream types.
            
            //we actually can't negindex?
            //what if it's a nonetype??
            ss[(int)type] << line << '\n';
        }

    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    //convert it into a raw string(char array) rather than a c++ std string.
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //handle errors here, very finicky
    int result;
    //get the status of a given shader.
    //getshaderiv - return info from a given shader object.
    //shader ids represent a pointer to the shader in opengl, and they're referenced from that id.
    //get this prop, from this id, and dump it into this integer object.
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        int length;
        //we pass mutable references to an integer so that the function can modify it in place. very jank
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        //again, we're passing the allocated pointer of the correct message size, so it can fill the raw char array.
        //then we dump the char array to the std output.
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile shader" << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

//we'll just write the shader as a string, and then compile it to enable on the graphics card.
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //init a blank program
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //we attach the shaders to the blank program, then link it all together.
    //the program is the bulked together shader operation.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //we clean up the compiled shader objects.
    glDeleteShader(vs);
    glDeleteShader(fs);

    // return a pointer to the program with all the shaders already compiled in it.
    // this can be applied directly to the app/gpu?
    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //glfw prop setter, key -> value
    //need to do this versioning to setup core, why?
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //change to the core prof, this has no default vao which will cause problem if we don't make one.
    //compat creates and binds the vao at start
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    //we need a valid context to init glew.
    if (glewInit() != GLEW_OK)
        std::cout << "err" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    //we dump raw data into the framebuffer.
    //in a square, vertices are redundant.
    //two of them are shared.
    //we use index buffers to handle the copying of vertices.

    float positions[8] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        
        -0.5f,  0.5f,
    };

    //an out of bounds vertex index will depend on the gpu. sometimes just makes the vertex at the origin.
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    //let's make our vao, since we're not on compat.
    VertexArray vao;
    vao.Bind();

    VertexBufferLayout layout;
    layout.Push<float>(2);

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    IndexBuffer ib(indices, 6);

    
    //now, we can format the vertex position data into triangle vertices using the position attrib definition.
    //we only need a shader, a program on the graphics card that draws the buffer.

    //some gpu drivers provide a default shader, so we don't always need to manually write it. 
    //we can still write and assign our own shader.
    //use shaders for things that are faster on the gpu.
    //processing can take place in the gpu, depending on what we decide to do.

    //use the predefined functions and pipe in glsl strings to compile and send to our graphics card, for rendering.
    //write the string like this, we get really nice auto concat, which is default c++
    
    //write the string pointer relative to the file itself.
    ShaderProgramSource shader_source = ParseShader("shaders/shader.glsl");

    unsigned int shader = CreateShader(shader_source.VertexSource, shader_source.FragmentSource);
    glUseProgram(shader);

    //get the unique index of the uniform var so that we can modify and recompile the shader.
    //we reference the compiled shader program, not the individual fragment
    int location = glGetUniformLocation(shader, "u_Color");
    //put in the location, and then the values you want to overwrite the uniform with.

    float red = 0.0f;
    float increment = 0.05f;

    //initialize a basic entity scene
    Entity entity;

    Timer timer(entity, 1.0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //run all of the component frame process methods.
        processAll();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //bind the indexbuffer of the object.
        ib.Bind();

        vao.Bind();
        vao.AddBuffer(vb, layout);

        glUniform4f(location, red, 0.2f, 0.3f, 1.0f);

        if (red > 1.0f)
        {
            increment = -0.05f;
        }
        if (red < -1.0f)
        {
            increment = 0.05f;
        }
        red += increment;
        //this draws the currently bound buffer.
        //remember, ogl is a state machine. we mutate the state through layer/buffer bindings.
        //everything is contextual.

        //third arg is the number of vertices we want to read.
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //"draw these indices specified by the currently active vertex buffer"? is that specified in binding?
        //USE UNSIGNED INTS TO INDEX AN ARRAY
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
