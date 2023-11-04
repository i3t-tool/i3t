#!/bin/bash -e

# This script is used to generate the C++ API documentation.
# This script is called from the root of the repository.

# The script requires the following tools to be installed:
# - doxygen
# - doxybook2

rm -rf Docs/Doxygen/xml
doxygen Docs/Doxygen/Doxyfile

cd Docs
rm -rf cpp-api-reference && mkdir -p cpp-api-reference
doxybook2 --input Doxygen/xml --output cpp-api-reference --config Doxygen/doxybook2.json
cd -

# Replace <br>
# find Docs/cpp-api-reference -type f -name "*.md" | xargs sed -i 's/<br>//g'
