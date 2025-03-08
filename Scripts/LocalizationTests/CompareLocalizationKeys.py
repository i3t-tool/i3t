from pathlib import Path

LOCALIZATION_DIR = Path('Data/Localization/')
CHECK_ENGLISH = False  # Set to True if you want to check English localization files

def find_localization_files(localization_dir):
    """
    Finds all localization files (*.txt) in the specified directory and returns a dictionary:
        { language: path_to_file, ... }
    For example, for the file ru.txt the language will be 'ru'.
    """
    localization_files = {}
    for txt_file in Path(localization_dir).glob("*.txt"):
        localization_files[txt_file.name] = txt_file
    return localization_files

def load_keys_from_file(file_path):
    """
    Reads keys from a localization file where each line is in the format 'key=value'.
    Returns the set of keys as they are written (without additional processing).
    """
    keys = {}
    with file_path.open("r", encoding="utf-8") as f:
        for idx, line in enumerate(f, 1):
            if line.startswith("#"):
                continue
            line = line.rstrip("\n")
            if line and "=" in line:
                key, _ = line.split("=", 1)
                keys[key] = idx
    return keys

def compare_localization_keys(localization_dir):
    """
    Compares keys in all localization files found in the specified directory.
    The first found file (base language) is used as a reference.
    If any file has missing or, on the contrary, extra keys compared to the base one,
    the differences are displayed.
    """
    ret = 0
    localization_files = find_localization_files(localization_dir)
    if not localization_files:
        print(f"No localization files found in {LOCALIZATION_DIR}.")
        exit(1)

    # Read the keys for each localization file
    keys_by_lang = {}
    for lang, file_path in localization_files.items():
        if not CHECK_ENGLISH and lang == "en.txt":
            print("Validation for en.txt is disabled!")
            continue
        keys_by_lang[lang] = load_keys_from_file(file_path)

    languages = list(keys_by_lang.keys())
    base_lang = languages[0]
    base_keys = keys_by_lang[base_lang]

    all_match = True
    differences = {}

    for lang in languages[1:]:
        current_keys = keys_by_lang[lang]
        missing_in_current = { key: base_keys[key] for key in base_keys if key not in current_keys }    # keys that are in the base but not in the current one
        extra_in_current = { key: current_keys[key] for key in current_keys if key not in base_keys }     # keys that are present in the current one, but not in the base one
        if missing_in_current or extra_in_current:
            differences[lang] = {
                "missing_in_" + lang: missing_in_current,
                "extra_in_" + lang: extra_in_current
            }
            all_match = False
            ret = 1

    if all_match:
        print("All localization files contain a matching set of keys.")
    else:
        print("Differences between localization files have been detected:")
        print(f"Base language: {base_lang}")
        for lang, diffs in differences.items():
            print(f"\nComparison {base_lang} and {lang}:")
            if diffs[f"missing_in_{lang}"]:
                print(f"   Keys not present in {lang} (present in {base_lang}):")
                for key, line in diffs[f"missing_in_{lang}"].items():
                    print(f"    - [Line {line}] {key}")
            if diffs[f"extra_in_{lang}"]:
                print(f"  Redundant keys in {lang} (not present in {base_lang}):")
                for key, line in diffs[f"extra_in_{lang}"].items():
                    print(f"    - [Line {line}] {key}")

    return ret

if __name__ == '__main__':
    ret_val = compare_localization_keys(LOCALIZATION_DIR)
    exit(ret_val)