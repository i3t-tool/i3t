#!/bin/bash -ex

FILES="Data* LICENSE.txt"

# Assume this is the project root
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )"
cd "$SCRIPT_DIR/.."
ROOT=$(pwd)

IS_WINDOWS=0
IS_UNIX_LIKE=0

function detect_os_type() {
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
}

CONFIG_TYPE=""
ARCHIVE_NAME_POSTFIX=""

function print_usage() {
    echo "Usage: $0 [--postfix <postfix>] <config_type>"
    echo "  --postfix <postfix> - optional postfix to add to the archive name"
    echo "  <config_type> - Debug or Release"
}

function process_args() {
    while [ $# -gt 0 ]; do
        case $1 in
            --postfix)
                ARCHIVE_NAME_POSTFIX="$ARCHIVE_NAME_POSTFIX-$2"
                shift
                shift
            ;;
            Debug)
                CONFIG_TYPE="Debug"
                ARCHIVE_NAME_POSTFIX="$ARCHIVE_NAME_POSTFIX-Debug"
                shift
            ;;
            Release)
                CONFIG_TYPE="Release"
                shift
            ;;
            *)
                echo "Unknown config type $1, only Debug and Release are supported."
                echo ""
                print_usage
                exit 1
            ;;
        esac
    done

    if [ -z "$CONFIG_TYPE" ]; then
        echo "Config type is not specified."
        echo ""
        print_usage
        exit 1
    else
        echo "Config type is $CONFIG_TYPE"
    fi
}

# Zip all files given to this function, but exclude hidden ones.
function zip_cmd() {
    if [ $IS_WINDOWS = 1 ]; then
        7z a -xr'!.*' "$@"
    else
        zip -r "$@" -x "*/.*"
    fi
}

function create_bundle() {
    binaries_directory="Binaries/$CONFIG_TYPE"

    if [ ! -d "$binaries_directory" ]; then
        echo "Directory $binaries_directory does not exist. You have to build I3T first."
        exit 1
    fi

    git_version=$(git describe --tags --exact-match HEAD 2> /dev/null || git rev-parse HEAD)

    archive_name="I3T$ARCHIVE_NAME_POSTFIX.zip"

    cp Data/imgui_default.ini Data/imgui.ini
    binaries=$(find "$binaries_directory" -name "*.so" -o -name "*.dll" -o -name "I3T" -o -name "I3T.exe")
    for binary in $binaries; do
        local binary_name=$(basename $binary)

        cp -f "$binary" "$binary_name"
        zip_cmd "$archive_name" "$binary_name"
        rm -f "$binary_name"
    done

    if [ ! $? = 0 ]; then
        echo "I3T binaries were not found! Did you build I3T first?"
        exit 1
    fi

    zip_cmd "$archive_name" $FILES
}

detect_os_type
process_args "$@"
create_bundle
