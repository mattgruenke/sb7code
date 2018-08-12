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

#include <vmath.h>

#include <string>

#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/opt/smart_enums.hpp>
#include <oglplus/bound/texture.hpp>

#include "oglplus/example.hpp"



using namespace oglplus;


static const char * vs_source[] =
{
    "#version 420 core                                                              \n"
    "                                                                               \n"
    "void main(void)                                                                \n"
    "{                                                                              \n"
    "    const vec4 vertices[] = vec4[](vec4( 0.75, -0.75, 0.5, 1.0),               \n"
    "                                   vec4(-0.75, -0.75, 0.5, 1.0),               \n"
    "                                   vec4( 0.75,  0.75, 0.5, 1.0));              \n"
    "                                                                               \n"
    "    gl_Position = vertices[gl_VertexID];                                       \n"
    "}                                                                              \n"
};

static const char * fs_source[] =
{
    "#version 430 core                                                              \n"
    "                                                                               \n"
    "uniform sampler2D s;                                                           \n"
    "                                                                               \n"
    "out vec4 color;                                                                \n"
    "                                                                               \n"
    "void main(void)                                                                \n"
    "{                                                                              \n"
    "    color = texture(s, gl_FragCoord.xy / textureSize(s, 0));                   \n"
    "}                                                                              \n"
};

class simpletexture_app : public Example
{
public:
    simpletexture_app()
    {
        // Define some data to upload into the texture
        float * data = new float[256 * 256 * 4];

        // generate_texture() is a function that fills memory with image data
        generate_texture(data, 256, 256);

        // Now bind it to the context using the GL_TEXTURE_2D binding point
        gl.Bound(smart_values::_2D, texture)

        // Specify the amount of storage we want to use for the texture
        .Storage2D(
            8,                                  // 8 mipmap levels
            PixelDataInternalFormat::RGBA32F,   // 32-bit floating-point RGBA data
            256, 256)                           // 256 x 256 texels

        // Assume the texture is already bound to the GL_TEXTURE_2D target
        .SubImage2D(
            0,                          // Level 0
            0, 0,                       // Offset 0, 0
            256, 256,                   // 256 x 256 texels, replace entire image
            PixelDataFormat::RGBA,      // Four channel data
            PixelDataType::Float,       // Floating point data
            data);                      // Pointer to data

        // Free the memory we allocated before - \GL now has our data
        delete [] data;

        FragmentShader  fs;
        fs.Source(fs_source).Compile();

        std::cout << fs.GetInfoLog() << std::endl;

        VertexShader    vs;
        vs.Source(vs_source).Compile();

        std::cout << vs.GetInfoLog() << std::endl;

        program.AttachShader(vs);
        program.AttachShader(fs);

        program.Link().Use();

        vao.Bind();
    }

    virtual void Reshape(GLuint width, GLuint height)
    {
        gl.Viewport(width, height);
    }

    virtual void Render(double t)
    {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

private:
    void generate_texture(float * data, int width, int height)
    {
        int x, y;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                data[(y * width + x) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
                data[(y * width + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
                data[(y * width + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
                data[(y * width + x) * 4 + 3] = 1.0f;
            }
        }
    }

private:
    Context         gl;
    Program         program;
    VertexArray     vao;
    Texture         texture;
};


const char *oglplus::title = "OpenGL SuperBible - Simple Texturing";


void oglplus::setupExample(ExampleParams &)
{
}


std::unique_ptr<Example> oglplus::makeExample(const ExampleParams &)
{
    return std::unique_ptr<Example>(new simpletexture_app);
}

