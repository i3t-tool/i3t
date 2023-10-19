# I3T
I3T - An Interactive Tool for Teaching Transformations

I3T is an educational application that allows the study of 3D transformations 
and their hierarchy in an illustrative way. 

It is the result of the work of students of three Parts of the Czech Technical University in Prague:
- The Department of Computer Graphics and Interaction of the Faculty of Electrical Engineering,
- The Department of Computer Science of the Faculty of Electrical Engineering,
- The Faculty of Information Technology.

Start I3T and try the Tutorials. Or read the [manual](Docs/manual.md).

##Authors
The first version of I3T was created by Michal Folta in 2016. 

- Martin Herich
- Jaroslav Holeček
- Dan Rakušan¨
- Adam Loucký
- Miroslav Mueller
- Vít Zadina
- Filip Uhlík



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
