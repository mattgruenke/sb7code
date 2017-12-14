/*
 * Copyright © 2012-2015 Graham Sellers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <cmath>
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include "oglplus/example.hpp"


using namespace oglplus;


class singlepoint_app : public Example
{
public:
    singlepoint_app()
    {
        static const char * vs_source[] =
        {
            "#version 410 core                                                 \n"
            "                                                                  \n"
            "layout (location = 0) in vec4 offset;                             \n"
            "                                                                  \n"
            "void main(void)                                                   \n"
            "{                                                                 \n"
            "    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
            "                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
            "                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
            "                                                                  \n"
            "    // Add 'offset' to our hard-coded vertex position             \n"
            "    gl_Position = vertices[gl_VertexID] + offset;                 \n"
            "}                                                                 \n"
        };

        static const char * fs_source[] =
        {
            "#version 410 core                                                 \n"
            "                                                                  \n"
            "out vec4 color;                                                   \n"
            "                                                                  \n"
            "void main(void)                                                   \n"
            "{                                                                 \n"
            "    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
            "}                                                                 \n"
        };

        FragmentShader  fs;
        fs.Source(fs_source).Compile();

        VertexShader    vs;
        vs.Source(vs_source).Compile();

        program.AttachShader(vs);
        program.AttachShader(fs);

        program.Link().Use();

        vao.Bind();
    }

    virtual void Reshape(GLuint width, GLuint height)
    {
        gl.Viewport(width, height);
    }

    virtual void Render(double currentTime)
    {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        gl.ClearColorBuffer(0, green);

            // For possible operations on VertexAttrib<>, see class ProgVar<>
            //  in oglplus/prog_var/wrapper.hpp - it's defined using
            //  OGLPLUS_DECLARE_PROG_VAR(), in oglplus/vertex_attrib.hpp.
        VertexAttrib<Vec4f>(program, 0).Set(
                             (float)sin(currentTime) * 0.5f,
                             (float)cos(currentTime) * 0.6f,
                             0.0f, 0.0f );

        gl.DrawArrays(PrimitiveType::Triangles, 0, 3);
    }

private:
    Context         gl;
    Program         program;
    VertexArray     vao;
};


const char *oglplus::title = "OpenGL SuperBible - Single Point";


void oglplus::setupExample(ExampleParams &)
{
}


std::unique_ptr<Example> oglplus::makeExample(const ExampleParams &)
{
    return std::unique_ptr<Example>(new singlepoint_app);
}

