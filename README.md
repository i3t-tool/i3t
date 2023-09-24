# I3T
An Interactive Tool for Teaching Transformations

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
Dependencies/               Directory with external dependencies.
Docs/
    - Doxygen/
Source/
    - Commands/             Commands for the command pattern.
    - Core/                 Core of the application.
    - GUI/                  ImGui windows, dialogs, elements.
        - Elements
            - Dialogs/      
            - Modals/       
            - Windows/      
        - ImGui/            Implementation of ImGui backend.
Test/                       Unit tests, copies structure of Source/ directory.
````

## Contributing
Please make sure, that you include only source code and files needed for building or running the application
(configuration, data) to version control.

### Code style
See [code style](Docs/developer-guide/code-style.md) for more information.

### Documentation
Manual is available [here](Docs/manual.md).

- See how to [add new node](Docs/developer-guide/how-to-add-new-node.md).
- See how to [add new tutorial](Docs/tutorials.md).
