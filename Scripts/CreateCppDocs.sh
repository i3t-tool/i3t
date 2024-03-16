#!/bin/bash -e

# This script is used to generate the C++ API documentation.
# This script is called from the root of the repository.

# The script requires the following tools to be installed:
# - doxygen
# - doxybook2

MARKDOWN_DIR=Docs/cpp-api-reference

function main() {
    rm -rf Docs/Doxygen/xml
    doxygen Docs/Doxygen/Doxyfile

    rm -rf Docs/cpp-api-reference && mkdir -p "$MARKDOWN_DIR"
    doxybook2 --input Docs/Doxygen/xml --output "$MARKDOWN_DIR" --config Docs/Doxygen/doxybook2.json

    # Find Markdown files in the specified directory and escape < and > in them
    find "$MARKDOWN_DIR" -type f -name "*.md" -print0 | while IFS= read -r -d '' file; do
        echo "Correcting file: $file"
        escaped_content=$(escape_markdown "$file")
        echo "$escaped_content" > "$file"
    done
}

# Function to escape < and > except within code blocks
function escape_markdown() {
    python3 - $1 << 'EOF'
import re
import sys

def escape_markdown(text):
    # Define a pattern to match code blocks
    code_block_pattern = re.compile(r'```.*?```', re.DOTALL)

    # Find code blocks and replace < and > inside them with placeholders
    def escape_code_blocks(match):
        code_block = match.group(0)
        return code_block.replace('<', '\ufffe').replace('>', '\uffff')

    # Replace < and > outside of code blocks
    escaped_text = code_block_pattern.sub(escape_code_blocks, text)
    escaped_text = escaped_text.replace('<', '&lt;').replace('>', '&gt;')

    # Restore placeholders inside code blocks
    escaped_text = escaped_text.replace('\ufffe', '<').replace('\uffff', '>')

    return escaped_text

# Read Markdown file from "$file"
with open(sys.argv[1], 'r', encoding='utf-8') as file:
    markdown_text = file.read()
    escaped_text = escape_markdown(markdown_text)
    print(escaped_text)
EOF
}

main
