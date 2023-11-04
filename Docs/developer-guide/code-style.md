# Code style
Please follow the rules below when writing code.
You can use the [clang-format](https://clang.llvm.org/docs/ClangFormat.html) tool to format the code
and [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) to check the code.

## C++
```cpp
/**
 * \file
 * \brief
 * \author Name Surname <email>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <string>

const int g_val = 0;

class EditBox
{
public:
  void setName(const std::string& name)
  {
    m_boxName = name;
  }

private:
  std::string m_boxName;
};
```

### Recommended conventions
- **Never** use `using namespace std;`. It can cause conflicts.
- **Never** use backslashes in paths `path\to\file`, prefer POSIX style `path/to/file` (Windows can handle these paths).

### Code structure
- **Static variables** are declared at the beginning of the `.cpp` file.
- **Header files** are included in groups separated by an empty line,
  in the following order - if possible:
    - **1.** header file associated with the given `.cpp` file,
    - **2.** STL libraries,
    - **3.** external libraries of the project,
    - **4.** internal header files.

### Code formatting
Code formatting rules are defined in the `.clang-format` file. It is not possible for each piece of code
to be indented by a different number of spaces, etc. IDE can load formatting rules and warn about their
violation.

In general, the form of our code is based on the Microsoft code style. I would like to point out in particular
writing the opening bracket of the code block on a new line (see example).

### Naming
- All **classes, structures and enumerations** `ClassName, StructName` are named in camel-case style with an initial capital letter.
- All **variables** and **functions** `myVariable` are also named in camel-case style, but with a lowercase initial letter.
- All **C++ files** `FileUtils.[cpp/h]` are named the same as classes and structures.
- **Global variables** with prefix `g_`.
- **Member variables** the same as variables, but with prefix `m_`. For purely data structures (without member functions)
  is recommended to use the `struct` keyword instead of `class` and omit the prefix `m_`.
- **Member static variables** with prefix `s_`.

## How to document code
The following file contains useful examples and tricks for Doxygen.

### Groups
It is often useful to group member functions of classes that have something in common.
Doxygen allows this using tags.
```cpp
///@{
    ...
///@}
```
Note that you cannot nest a group into a group.

[Link to Doxygen](https://www.doxygen.nl/manual/grouping.html#memgroup)
