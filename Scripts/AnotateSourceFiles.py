import glob
import re
import subprocess
from dataclasses import dataclass
from typing import List, Dict

THRESHOLD = 0.2
SOURCE_DIRS = ["Source", "Test"]
EXTENSIONS = [".h", ".cpp"]
IGNORE_PATTERNS = ["imgui_impl_glfw", "imgui_impl_opengl3"]
FILE_HEADER = """/**
 * \\file
 * \\brief
 * \\author
 * \\copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
 """

AUTHOR_ALIASES = {
    "Adam Loucký": {"Loucký, Adam"},
    "Dan Rakušan": {"Rakušan, Dan", "DUDSS"},
}

LICENSE_HEADER = ""


@dataclass
class Authorship:
    fullname: str
    percentage: float


def collect_files() -> list[str]:
    result = []
    for source_dir in SOURCE_DIRS:
        for extension in EXTENSIONS:
            path = source_dir + f"/**/*[{extension}]"
            for filename in glob.iglob(path, recursive=True):
                result.append(filename)

    return result


def file_authors(path, names: Dict[str, str]) -> List[Authorship]:
    args = [
        "bash",
        "-c",
        f"git blame --line-porcelain {path} | grep 'author ' | sed 's,author,,' | sort | uniq -ic | sort -nr"
    ]
    # Output example:
    #      63  Martin Herich
    #       3  Rakušan, Dan
    output = subprocess.Popen(args, stdout=subprocess.PIPE)
    lines = output.stdout.read().decode("utf-8").split("\n")
    lines = [line.strip(" ") for line in lines if line]
    authors = list(map(lambda line: line.split(" ", 1), lines))

    # count total lines
    total_lines = 0
    for author in authors:
        total_lines += int(author[0].strip())

    authors_table = {}

    for lines_count, fullname in authors:
        fullname = fullname.strip()
        # find real name
        fullname = names.get(fullname, fullname)

        if fullname in authors_table:
            authors_table[fullname] += int(lines_count) / total_lines
        else:
            authors_table[fullname] = int(lines_count) / total_lines

    return list(map(lambda author: Authorship(author[0], author[1]), authors_table.items()))


def process_file(path, authors: List[Authorship]):
    content = ""
    with open(path) as file:
        content = file.read()

        match = re.findall(r"\\author|@author|\\file|@file", content)
        if match:
            match = re.findall(r"\\copyright", content)
            if not match:
                print(f"Got file without license notice: {path}")
                return
            
            match = re.findall(r"(\\author(s*)|@author(s*))\s+\w+", content)
            if not match:
                print(f"Got file without authors specified: {path}")

            if VERBOSE:
                print(f"Skipping already annotated file: {path}")
            return

        # ignore files with specific patterns
        for pattern in IGNORE_PATTERNS:
            if pattern in path:
                if VERBOSE:
                    print(f"Skipping ignored file: {path}")
                return

        if DRY_RUN:
            return

        if VERBOSE:
            print(f"Processing file: {path}")

    with open(path, "w") as file:
        # prepend file header
        file.write(FILE_HEADER + content)


if __name__ == "__main__":
    # add dry run CLI option using argparse
    import argparse
    parser = argparse.ArgumentParser(description="Annotate source files with authors.")
    parser.add_argument("-n", action="store_true", help="Dry run.")
    parser.add_argument("-v", action="store_true", help="Verbose.")
    args = parser.parse_args()

    DRY_RUN = args.n
    VERBOSE = args.v

    alias_to_author = {}
    for author, aliases in AUTHOR_ALIASES.items():
        for alias in aliases:
            alias_to_author[alias] = author

    files = collect_files()
    for file in files:
        file = file.replace("\\", "/")
        process_file(file, [])

    print("Following header was added to all files:")
    print(FILE_HEADER)
