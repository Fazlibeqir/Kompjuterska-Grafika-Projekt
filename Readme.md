# [Kompjuterska-Grafika-Projekt](https://github.com/Fazlibeqir/Kompjuterska-Grafika-Projekt)


## Summary
Using [Glitter](http://polytonic.github.io/Glitter/) is a dead simple boilerplate for OpenGL, intended as a starting point for the tutorials on [learnopengl.com](http://www.learnopengl.com) and [open.gl](https://open.gl). Glitter compiles and statically links every required library, so you can jump right into doing what you probably want: how to get started with OpenGL.

## Getting Started
Glitter has a single dependency: [cmake](http://www.cmake.org/download/), which is used to generate platform-specific makefiles or project files. Start by cloning this repository.

```bash
git clone  https://github.com/Fazlibeqir/Kompjuterska-Grafika-Projekt.git
```


## Documentation
Many people overlook how frustrating it is to install dependencies, especially in environments lacking package managers or administrative privileges. For beginners, just getting set up properly set up can be a huge challenge. Glitter is meant to help you overcome that roadblock.

Glitter provides the most basic windowing example. It is a starting point, and tries very hard not to enforce any sort of directory structure. Feel free to edit the include paths in `CMakeLists.txt`. Glitter bundles most of the dependencies needed to implement a basic rendering engine. This includes:

Functionality           | Library
----------------------- | ------------------------------------------
Mesh Loading            | [assimp](https://github.com/assimp/assimp)
Physics                 | [bullet](https://github.com/bulletphysics/bullet3)
OpenGL Function Loader  | [glad](https://github.com/Dav1dde/glad)
Windowing and Input     | [glfw](https://github.com/glfw/glfw)
OpenGL Mathematics      | [glm](https://github.com/g-truc/glm)
Texture Loading         | [stb](https://github.com/nothings/stb)

## License
>The MIT License (MIT)

>Copyright (c) 2015 Kevin Fung

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
