import re
from pathlib import Path

FOLDER_WITH_SOURCE = Path('Source/')
LOCALIZATION_DIR = Path('Data/Localization/')
CHECK_ENGLISH = False  # Set to True if you want to check English localization files

RED = "\033[31m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
RESET = "\033[0m"

def find_localization_files(localization_dir):
    """
    Searches all localization files (*.txt) in the specified directory and returns a dictionary of the form:
        { language: path_to_file, ... }
    For example, for the file ru.txt the language will be 'ru'.
    """
    localization_files = {}
    for txt_file in Path(localization_dir).glob("*.txt"):
        localization_files[txt_file.name] = txt_file
    return localization_files


def load_keys_from_file(file_path):
    """
    Reads keys from a localization file where each line is in the format 'key=value',
    and returns a set of keys.
    """
    keys = set()
    with file_path.open("r", encoding="utf-8") as f:
        for line in f:
            if line and '=' in line:
                key, _ = line.split('=', 1)
                keys.add(key)
    return keys


def find_missing_keys_in_sources(folder_with_sources, localization_files):
    """
    Traverses all  files (including subfolders) in a given directory and looks for keys in macros:
      - _t(“...”) with arbitrary spaces,
      - ICON_T(ANYTHING, “...”).

    For each found key, the line number is determined, and then for each language
    it is checked for its presence in localization files.

    Returns a dictionary of the form:
        {
          'ru': { 'path/to/file.cpp': [(key, line_number), ...], ... },
          'en': { ... }
        }
    """
    # Combine the two patterns into one:
    # If the first part (_t(...)) is triggered, the key goes to group 1,
    # If ICON_T(...), it goes to group 2.
    # If _tbd(...), it goes to group 3.
    pattern = re.compile(
        r'_ts?\s*\(\s*"([^"]+)"\s*\)'
        r'|ICON_T\s*\(.*?,\s*"([^"]+)"\s*\)'
        r'|_tbd\s*\(\s*"([^"]+)"\s*\)',
        flags=re.DOTALL
    )

    # Load the keys for each language as a dictionary {language: set of keys}
    all_keys = {lang: load_keys_from_file(path) for lang, path in localization_files.items()}
    missing_keys = {lang: {} for lang in localization_files}

    # Search all .cpp files recursively
    valid_extensions = {'.cpp', '.c', '.h', '.hpp'}

    for source_file in Path(folder_with_sources).rglob("*"):
        if source_file.suffix.lower() not in valid_extensions:
            continue
        try:
            content = source_file.read_text(encoding="utf-8")
        except Exception as e:
            print(f"Failed to read file {source_file}: {e}")
            continue
        for match in pattern.finditer(content):
            # Determine which group is triggered (key from _t or from ICON_T or _tbd)
            key = match.group(1) or match.group(2) or match.group(3)
            line_number = content.count('\n', 0, match.start()) + 1
            # For each language, check for the key
            for lang, keys in all_keys.items():
                if key not in keys:
                    missing_keys[lang].setdefault(str(source_file), []).append((key, line_number))
    return missing_keys


if __name__ == '__main__':
    ret = 0

    localization_files = find_localization_files(LOCALIZATION_DIR)
    if not localization_files:
        print(f"No localization files found in {LOCALIZATION_DIR}.")
        print(YELLOW + "\nIMPORTANT! Do not forget to set working directory for this script to the root of the project.\n" + RESET)
        exit(1)

    print(f"\nLocalization files found: {', '.join(localization_files.keys())}")
    result = find_missing_keys_in_sources(FOLDER_WITH_SOURCE, localization_files)
    for lang, missing in result.items():
        print(f"\nLanguage: {lang}")
        if not CHECK_ENGLISH and lang == 'en.txt':
            print("  Validation for en.txt is disabled!")
            continue
        if not missing:
            print("  All keys found.")
        else:
            print("  The missing keys have been found:")
            for source_file, keys_info in missing.items():
                print(f"  File: {source_file}")
                for key, line_number in keys_info:
                    print(f"    - [Line {line_number}] {key}")
            ret = 1

    exit(ret)
