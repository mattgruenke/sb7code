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

#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include "oglplus/example.hpp"

#include <sb7.h>
#include <vmath.h>


// Remove this to draw only a single cube!
// #define MANY_CUBES


using namespace oglplus;



Mat4f Convert(const vmath::mat4 &m)
{
    Mat4f result;
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++) result.Set(i, j, m[j][i]);
    }
    return result;
}


class singlepoint_app : public Example
{
public:
    singlepoint_app()
    : mv_location(program),
      proj_location(program)
    {
        static const char * vs_source[] =
        {
            "#version 410 core                                                  \n"
            "                                                                   \n"
            "in vec4 position;                                                  \n"
            "                                                                   \n"
            "out VS_OUT                                                         \n"
            "{                                                                  \n"
            "    vec4 color;                                                    \n"
            "} vs_out;                                                          \n"
            "                                                                   \n"
            "uniform mat4 mv_matrix;                                            \n"
            "uniform mat4 proj_matrix;                                          \n"
            "                                                                   \n"
            "void main(void)                                                    \n"
            "{                                                                  \n"
            "    gl_Position = proj_matrix * mv_matrix * position;              \n"
            "    vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);      \n"
            "}                                                                  \n"
        };

        static const char * fs_source[] =
        {
            "#version 410 core                                                  \n"
            "                                                                   \n"
            "out vec4 color;                                                    \n"
            "                                                                   \n"
            "in VS_OUT                                                          \n"
            "{                                                                  \n"
            "    vec4 color;                                                    \n"
            "} fs_in;                                                           \n"
            "                                                                   \n"
            "void main(void)                                                    \n"
            "{                                                                  \n"
            "    color = fs_in.color;                                           \n"
            "}                                                                  \n"
        };

        FragmentShader  fs;
        fs.Source(fs_source).Compile();

        VertexShader    vs;
        vs.Source(vs_source).Compile();

        program.AttachShader(vs);
        program.AttachShader(fs);

        program.Link().Use();

        mv_location.BindTo("mv_matrix");
        proj_location.BindTo("proj_matrix");

        vao.Bind();

        static const GLfloat vertex_positions[] =
        {
            -0.25f,  0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f,  0.25f,

            -0.25f,  0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f,  0.25f,  0.25f,

             0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f, -0.25f
        };

        buffer.Bind(Buffer::Target::Array);
        Buffer::Data(Buffer::Target::Array, vertex_positions, BufferUsage::StaticDraw);
        VertexArrayAttrib vaa(program, "position");
        vaa.Pointer(3, DataType::Float, false, 0, NULL);
        vaa.Enable();

        gl.Enable(Capability::CullFace);
        gl.FrontFace(FaceOrientation::CW);

        gl.Enable(Capability::DepthTest);
        gl.DepthFunc(CompareFunction::LEqual);
    }

    virtual void Render(double currentTime)
    {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;

        gl.ClearColorBuffer(0, green);
        gl.ClearDepthBuffer(one);

        proj_location.Set(Convert(proj_matrix));
#ifdef MANY_CUBES
        int i;
        for (i = 0; i < 24; i++)
        {
            float f = (float)i + (float)currentTime * 0.3f;
            vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -6.0f) *
                                    vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
                                    vmath::rotate((float)currentTime * 21.0f, 1.0f, 0.0f, 0.0f) *
                                    vmath::translate(sinf(2.1f * f) * 2.0f,
                                                     cosf(1.7f * f) * 2.0f,
                                                     sinf(1.3f * f) * cosf(1.5f * f) * 2.0f);
            mv_location.Set(Convert(mv_matrix));
            gl.DrawArrays(PrimitiveType::Triangles, 0, 36);
        }
#else
        float f = (float)currentTime * 0.3f;
        vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -4.0f) *
                                vmath::translate(sinf(2.1f * f) * 0.5f,
                                                    cosf(1.7f * f) * 0.5f,
                                                    sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) *
                                vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
                                vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);
        mv_location.Set(Convert(mv_matrix));
        gl.DrawArrays(PrimitiveType::Triangles, 0, 36);
#endif
    }

    virtual void Reshape(GLuint w, GLuint h)
    {
        aspect = (float)w / (float)h;
        proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
        gl.Viewport(w, h);
    }

private:
    Context                 gl;
    Program                 program;
    VertexArray             vao;

    Buffer                  buffer;
    ProgramUniform<Mat4f>   mv_location;
    ProgramUniform<Mat4f>   proj_location;

    float                   aspect;
    vmath::mat4             proj_matrix;
};


const char *oglplus::title = "OpenGL SuperBible - Spinny Cube";


void oglplus::setupExample(ExampleParams &)
{
}


std::unique_ptr<Example> oglplus::makeExample(const ExampleParams &)
{
    return std::unique_ptr<Example>(new singlepoint_app);
}

