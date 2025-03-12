# Build
List of supported platforms is in [table](https://github.com/i3t-tool/i3t/blob/master/README.md#Build) 
in README.md file.

## Get source code
First, you need to download the source code. The `--recursive` switch ensures that
git submodules with libraries will be downloaded together with the main repository.
```shell script
git clone --recursive https://github.com/i3t-tool/i3t.git
```

If not all submodules were initialized or if any was removed in a new version,
they must be reinitialized.
```shell
git submodule update --init --recursive
```

## Dependencies
You will need `cmake` and a C++23 compiler with support for `std::expected`.

### Linux (apt)
```shell
apt install -y libgl-dev libglx-dev \
    libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

## CMake
Project build is realized by `cmake` utility. All necessary dependencies are
contained in the `Dependencies/` directory.

The directory in which CMake is run is taken as the output directory, into which
files for build systems (Makefile, *.vxproj, ...) are generated.

**Code:** Generating configuration using cmake.
**To build a distributable binary, you need to specify the variable `CMAKE_BUILD_TYPE=Release`.**
```shell script
# In the project root create cmake output directory 'out/' and move into it.
mkdir out && cd out

# Build in the directory MSVC project files.
cmake -DCMAKE_BUILD_TYPE=Debug -A Win32 -G "Visual Studio 16 2019" ..
# or for Code-Blocks projekt on Linux
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..
```
This example of running CMake from the command line generates project files for Visual
Studio 2019 in the directory where CMake is run, for the ``Debug | x86`` configuration.
Two dots indicate the path to the main file ``CMakeLists.txt``, which is located in the root

The resulting binary files, shared libraries and the IT3(.exe) executable are
created in the ``Binaries`` directory into a subdirectory corresponding to the configuration type.

## Bundle
You can create a zip archive containing the ready-to-use I3T and its data, but first, 
you need to build the I3T.
```shell
Scripts/CreateBundle.sh Debug  # or
Scripts/CreateBundle.sh Release
```
