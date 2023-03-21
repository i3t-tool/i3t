#!/bin/bash

# Assume this is the project root
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )"
cd "$SCRIPT_DIR/.."

IS_WINDOWS=0
IS_UNIX_LIKE=0
CONFIG_TYPE=""

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        IS_UNIX_LIKE=1
elif [[ "$OSTYPE" == "darwin"* ]]; then
        # Mac OSX
        IS_UNIX_LIKE=1
elif [[ "$OSTYPE" == "cygwin" ]]; then
        # POSIX compatibility layer and Linux environment emulation for Windows
        IS_WINDOWS=1
elif [[ "$OSTYPE" == "msys" ]]; then
        # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
        IS_WINDOWS=1
elif [[ "$OSTYPE" == "win32" ]]; then
        # I'm not sure this can happen.
        IS_WINDOWS=1
else
        # Unknown.
        echo "Unknown OS"
        exit 1
fi

ARCHIVE_NAME_DEBUG_POSTFIX=""

case $1 in
  Debug)
    CONFIG_TYPE="Debug"
    ARCHIVE_NAME_DEBUG_POSTFIX="-Debug"
    ;;
  Release)
    CONFIG_TYPE="Release"
    ;;
  *)
    echo "Unknown config type, only Debug and Release are supported."
    exit 1
    ;;
esac

binaries_directory="Binaries/$CONFIG_TYPE"

if [ ! -d "$binaries_directory" ]; then
    echo "Directory $binaries_directory does not exist. You have to build I3T first."
    exit 1
fi

GIT_TAG=$(git describe --abbrev=0)
GIT_COMMIT_HASH=$(git log --format="%H" -n 1)

if [ $GIT_TAG = "" ]; then
    GIT_TAG="preview"
fi

ARCHIVE_NAME="I3T-$GIT_TAG-$GIT_COMMIT_HASH$ARCHIVE_NAME_DEBUG_POSTFIX.zip"
zip -r "$ARCHIVE_NAME" "Data"

if [ $IS_UNIX_LIKE = 1 ]; then
    zip --junk-paths "$ARCHIVE_NAME" \
        "$binaries_directory/I3T" \
        "$binaries_directory/*.so"
elif [ $IS_WINDOWS = 1 ]; then
    zip --junk-paths "$ARCHIVE_NAME" \
        "$binaries_directory/I3T.exe" \
        "$binaries_directory/*.dll"
fi

if [ ! $? = 0 ]; then
    echo "I3T binaries were not found! Did you build I3T first."
    exit 1
fi

zip "$ARCHIVE_NAME" Config.json config.yml LICENCE
