# OpenGL_Template
Basic OpenGL template with CMake and C++

Configured dependencies:
* GLFW 3.3-stable
* GLM 0.9.8
* GLAD core profile, no extensions
* stb_image
* EnTT
* Assimp
* Bullet3

Cloning this repository:
```bash
$> git clone --recurse-submodules <name of repo>
```

If you forgot `--recursive-submodules` or want to update the 
dependencies:
```
$> git submodule update --init --recursive
```

To build the project with no help from any IDE:
```bash
$> mkdir out
$> cmake . -B out
$> cmake --build out
```

To add source files with no IDE support: Add the paths to the files
in `CMakeLists.txt` in variable `PROJECT-SRC` like `src/MyClass.cpp`,
`src/MyClass.h`.

To add resources like images or shaders with no IDE support: Add the
files in the `resources` folder. You can address them in your code 
like `resources/my_file.glsl`.
