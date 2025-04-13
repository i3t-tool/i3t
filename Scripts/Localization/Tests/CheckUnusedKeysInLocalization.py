import re
from pathlib import Path

FOLDER_WITH_SOURCE = Path('Source/')
LOCALIZATION_DIR = Path('Data/Localization/')

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
    Reads keys from a localization file, where each line has the format 'key=value',
    and returns a dictionary of the form { key: line_number, ... }.
    """
    keys = {}
    with file_path.open("r", encoding="utf-8") as f:
        for idx, line in enumerate(f, 1):
            if line.startswith('#'):
                continue
            if line and '=' in line:
                key, _ = line.split('=', 1)
                keys[key] = idx
    return keys


def get_used_keys_in_source(folder_with_source):
    """
    Recursively traverses all source files with extensions .cpp, .c, .h and .hpp,
    finds all keys used in the macros _t(“...”) and ICON_T(ANYTHING, “...”),
    and returns them as a set.
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
    used_keys = set()
    valid_extensions = {'.cpp', '.c', '.h', '.hpp'}

    for file in Path(folder_with_source).rglob("*"):
        if file.suffix.lower() in valid_extensions:
            try:
                content = file.read_text(encoding="utf-8")
            except Exception as e:
                print(f"Failed to read file {file}: {e}")
                continue
            for match in pattern.finditer(content):
                # Determine which group is triggered (key from _t or from ICON_T or _tbd)
                key = match.group(1) or match.group(2) or match.group(3)
                used_keys.add(key)
    return used_keys


def find_unused_keys(folder_with_source, localization_files):
    """
    For each localization file, checks which keys are not used in any of the source files.
    Returns a dictionary of the form:
      {
        'ru': { key1: string, key2: string, ... },
        'en': { ... }
      }
    """
    used_keys = get_used_keys_in_source(folder_with_source)
    unused_keys = {}

    for lang, file_path in localization_files.items():
        loc_keys_dict = load_keys_from_file(file_path)
        # Find the keys that are in the localization file, but are not used in the sources
        unused = set(loc_keys_dict.keys()) - used_keys
        # Save the line number for each unused key
        unused_keys[lang] = {key: loc_keys_dict[key] for key in unused}
    return unused_keys


if __name__ == '__main__':
    ret = 0

    print("This script might not be required to pass.\n")

    localization_files = find_localization_files(LOCALIZATION_DIR)
    if not localization_files:
        print(f"No localization files found in {LOCALIZATION_DIR}.")
        print(YELLOW + "\nIMPORTANT! Do not forget to set working directory for this script to the root of the project.\n" + RESET)
        exit(1)

    print(f"Localization files found: {', '.join(localization_files.keys())}")
    unused_keys = find_unused_keys(FOLDER_WITH_SOURCE, localization_files)
    for lang, keys in unused_keys.items():
        print(f"\nLanguage: {lang}")
        if not keys:
            print("  All keys are in use.")
        else:
            print("  Unused keys:")
            for key, line in keys.items():
                print(f"    - [Line {line}] {key}")
            ret = 1

    exit(ret)
