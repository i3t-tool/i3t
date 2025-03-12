#!/bin/bash -ex

FILES="Data* LICENSE.txt"

# Assume this is the project root
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )"
cd "$SCRIPT_DIR/.."
ROOT=$(pwd)

ZIP_CMD="zip"

function default_zip_cmd() {
    if [ -x "$(command -v zip)" ]; then
        ZIP_CMD="zip"
    elif [ -x "$(command -v 7z)" ]; then
        ZIP_CMD="7z"
    else
        echo "zip or 7z is not installed. Please install one of them to proceed."
        exit 1
    fi
}

CONFIG_TYPE=""
ARCHIVE_NAME_SUFFIX=""

function print_usage() {
    local script_name=$(basename $0)

    echo "Usage: $script_name [--suffix <suffix> ...] <config_type>"
    echo ""
    echo "  <config_type>       Debug, Release, MinSizeRel or RelWithDebInfo"
    echo ""
    echo "  --suffix <suffix>   optional suffix to add to the archive name"
    echo "  --zip-cmd <command name>"
    echo "                      optional zip command to use, either zip or 7z"
    echo "  --help              print this message"
    echo "  --debug             enable debug output"
}

# Print usage and exit.
function exit_and_print_usage() {
    message=$1
    if [ ! -z "$message" ]; then
        echo $message
        echo ""
    fi

    print_usage
    exit 1
}

function process_args() {
    default_zip_cmd

    while [ $# -gt 0 ]; do
        case $1 in
            --suffix)
                ARCHIVE_NAME_SUFFIX="-$2"
                shift
                shift
            ;;
            --zip-cmd)
                ZIP_CMD="$2"
                shift
                shift
            ;;
            Debug|Release|MinSizeRel|RelWithDebInfo)
                CONFIG_TYPE="$1"
                shift
            ;;
            --help)
                print_usage
                exit 0
            ;;
            --debug)
                set -x
                shift
            ;;
            *)
                exit_and_print_usage "Unknown argument $1."
            ;;
        esac
    done

    if [ -z "$CONFIG_TYPE" ]; then
        exit_and_print_usage "Config type is not specified."
    fi
}

# Zip all files given to this function, but exclude hidden ones.
function zip_cmd() {
    case $ZIP_CMD in
        zip)
            zip -r "$@" -x "*/.*"
        ;;
        7z)
            7z a -xr'!.*' "$@"
        ;;
    esac
}

function create_bundle() {
    local binaries_directory="Binaries/$CONFIG_TYPE"

    if [ ! -d "$binaries_directory" ]; then
        echo "Directory $binaries_directory does not exist. You have to build I3T first."
        exit 1
    fi

    local git_version=$(git describe --tags --exact-match HEAD 2> /dev/null || git rev-parse HEAD)

    local archive_name="I3T${ARCHIVE_NAME_SUFFIX}-${git_version}.zip"

    cp Data/imgui_default.ini Data/imgui.ini
    local binaries=$(find "$binaries_directory" -name "*.so" -o -name "*.so.*" -o -name "*.dll" -o -name "I3T" -o -name "I3T.exe")
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

process_args "$@"
create_bundle
