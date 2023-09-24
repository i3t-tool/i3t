#!/bin/bash -e

FILES="Data Config.json LICENSE.txt"

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

function process_args() {
  case $1 in
    Debug)
      CONFIG_TYPE="Debug"
      ARCHIVE_NAME_POSTFIX="-Debug"
      ;;
    Release)
      CONFIG_TYPE="Release"
      ;;
    *)
      echo "Unknown config type, only Debug and Release are supported."
      exit 1
      ;;
  esac
}

function zip_cmd() {
  if [ $IS_UNIX_LIKE = 1 ]; then
      zip --recurse --junk-paths "$@"
  elif [ $IS_WINDOWS = 1 ]; then
      7z a -tzip "$@"
  fi
}

function create_bundle() {
  binaries_directory="Binaries/$CONFIG_TYPE"

  if [ ! -d "$binaries_directory" ]; then
      echo "Directory $binaries_directory does not exist. You have to build I3T first."
      exit 1
  fi

  git_version=$(git describe --tags --exact-match HEAD 2> /dev/null || git rev-parse HEAD)

  archive_name="I3T-$git_version$ARCHIVE_NAME_POSTFIX.zip"

  binaries=$(find "$binaries_directory" -type f -name "*.so" -o -name "*.dll" -o -name "I3T" -o -name "I3T.exe")
  for binary in $binaries; do
      cp -f "$binary" "$(basename $binary)"
      zip_cmd "$archive_name" "$(basename $binary)"
      rm -f "$(basename $binary)"
  done

  if [ ! $? = 0 ]; then
      echo "I3T binaries were not found! Did you build I3T first?"
      exit 1
  fi

  zip_cmd "$archive_name" $FILES
}

detect_os_type
process_args $1
create_bundle
