:: Deletes all untracked files in all submodules
:: Useful to delete all temporary/cached CMake build files
:: Which can cause issues during a toolchain switch (MSVC -> WSL GCC)
git submodule foreach --recursive git clean -xfd
pause