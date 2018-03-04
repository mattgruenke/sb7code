OGLplus port of OpenGL SuperBible 7th Edition Source Code
=========================================================

This is an OGLplus port of the example source code for the 7th edition of the
OpenGL SuperBible.  This port was done for the purpose of becoming more familiar
with OGLplus and modern OpenGL, while also creating a rosetta stone for mapping
between the two.

Please note that I have no affiliation with either OGLplus or the SB7 examples,
nor has either project endorsed this effort.  With that said, I think you'll
find OGLplus greatly streamlines OpenGL code, making it a worthwhile addition to
most OpenGL projects.

For the original example source, see:

> https://github.com/openglsuperbible/sb7code

For more on OGLplus, see:

> http://oglplus.org

> https://github.com/matus-chochlik/oglplus

Note the Quickbook docs are particularly useful.  The tutorials are also worth
a look, however the Doxygen is currently out-of-date and significantly hampered
by all the macro-driven template instantiation and specialization):

> http://matus-chochlik.github.io/oglplus/doc/oglplus.html

> http://oglplus.org/oglplus/html/oglplus\_tutorials.html


## Porting Status ##

For a list of files that have been ported, open CMakeLists.txt and see the
`EXAMPLES_PLUS` variable.


## Other Changes & Differences ##

In porting these examples, I have taken care to change as little as possible.
This was done primarily to facilitate a side-by-side comparison of how OGLplus
does things vs. plain OpenGL.  As such, there are potential improvements and
non-critical errors I've not addressed.

The fixes I *have* applied are mostly from pull-requests submitted (but not
applied) to the original project:

* https://github.com/openglsuperbible/sb7code/pull/5
* https://github.com/openglsuperbible/sb7code/pull/7

Other changes include:

* Changed output directory of example programs from being relative to the
  `CMAKE_SOURCE_DIR` to being in `CMAKE_BINARY_DIR`.
* Removed extern/glfw-3.0.4/ - users are now responsible for providing glfw3.
* Removing Khronos header files, in order to ensure system-supplied versions
  are used instead.
* CMake cleanup: Changed project name, enabled C++11 using the proper method.
* Added snapshot of internal files from oglplus-0.71.0 needed to build its
  examples & borrowed to build the OGLplus ports of the SB7 examples.


## Building ##

Please read HOWTOBUILD.txt. It tells you how to build and
run the examples.


## Running ##

*Note that to run these examples, you need the associated media files which
are available from http://openglsuperbible.com/files/superbible7-media.zip*.
