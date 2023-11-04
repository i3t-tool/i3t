# I3T
I3T - An Interactive Tool for Teaching Transformations

I3T is an educational application that allows the study of 3D transformations 
and their hierarchy in an illustrative way. 

It is the result of the work of students of three Parts of the Czech Technical University in Prague:
- The Department of Computer Graphics and Interaction of the Faculty of Electrical Engineering,
- The Department of Computer Science of the Faculty of Electrical Engineering,
- The Faculty of Information Technology.

Start I3T and try the Tutorials. Or read the [manual](Docs/manual.md).

## Authors
The first version of I3T was created by Michal Folta in 2016. 

- Martin Herich
- Jaroslav Holeček
- Dan Rakušan
- Adam Loucký
- Miroslav Müller
- Vít Zadina
- Filip Uhlík

Design
- Lukáš Pilka
- Jaroslav Kolář

## Info for the developers

Pay attention to [project code style](Docs/developer-guide/code-style.md).

## Build
You will need C++23 compiler with support for `std::expected`, 
see [compiler support](https://en.cppreference.com/w/cpp/compiler_support) for more information
about compiler versions.

For more information about building the project, see [build instructions](Docs/developer-guide/build.md).

### Build documentation
For local documentation build, you will need Node.js installed.
```shell
npm install
npm run docs:dev
```

## Project structure
````
Data/                       All Assets (fonts, models, scenes, textures, tutorials,...)
Dependencies/               Directory with external dependencies.
Docs/
    - assets/
    - developer-guide/
    - Doxygen/
Source/
    - Commands/             Commands for the command pattern.
    - Core/                 Core of the application.
    - GUI/                  ImGui windows, dialogs, elements.
        - Elements
            - Dialogs/      
            - Modals/
            - Nodes/       
            - Windows/      
        - ImGui/            Implementation of ImGui backend.
Test/                       Unit tests, copies structure of the Source/ directory.
````

## Contributing
Please make sure, that you include only source code and files needed for building 
or running the application (configuration, data) to version control.

### Code style
See [code style](Docs/developer-guide/code-style.md) for more information.

### Documentation
Manual is available [here](Docs/manual.md).

- See how to [add new node](Docs/developer-guide/how-to-add-new-node.md).
- See how to [add new tutorial](Docs/tutorials.md).

## Used libraries
- [assimp](https://github.com/assimp/assimp) (BSD-3-Clause)
- [glfw](https://www.glfw.org/) (Zlib)
- [glm](https://github.com/g-truc/glm) (MIT)
- [gtest](https://github.com/google/googletest) (BSD-3-Clause)
- [ImGui](https://github.com/ocornut/imgui) (MIT)
- [imgui_markdown](https://github.com/juliettef/imgui_markdown) (Zlib)
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) (MIT)
- [lua](https://github.com/lua/lua) (MIT)
- [magic_enum](https://github.com/Neargye/magic_enum) (MIT)
- [portable-file-dialogs](https://github.com/samhocevar/portable-file-dialogs) (WTFPL)
- [rttr](https://www.rttr.org/) (MIT)
- [sol2](https://github.com/ThePhD/sol2) (MIT)
- [spdlog](https://github.com/gabime/spdlog) (MIT)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) (MIT)
