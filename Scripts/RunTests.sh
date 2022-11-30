#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )"
cd "$SCRIPT_DIR" && cd ..

TEST="Binaries/Debug/I3TTest"

$TEST --gtest_filter=*-StateTest.*:GLTFResourcesTest.*:UndoRedoTest.*:ResourceManagerTest.*

if [ -z ${DISPLAY+x} ]; then
  echo "Target does not have any graphical environment!"
else
  $TEST --gtest_filter=GLTFResourcesTest.AssimpCanImportGLTFFiles && \
  $TEST --gtest_filter=GLTFResourcesTest.DefaultModelAreImported && \
  $TEST --gtest_filter=StateTest.SceneCanBeSavedAndLoaded && \
  $TEST --gtest_filter=StateTest.TransformsAreSavedAndLoadedProperly && \
  $TEST --gtest_filter=ResourceManagerTest.* && \
  $TEST --gtest_filter=UndoRedoTest.Basic
fi

