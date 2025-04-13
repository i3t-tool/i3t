import csv
import sys

LOCALIZATION_DIR = "Data/Localization"
CSV_FILENAME = LOCALIZATION_DIR + "/localization.csv"

RED = "\033[31m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
RESET = "\033[0m"

def parse_csv_to_lang_files(csv_filename, localization_dir):
    # Open the CSV file with UTF-8 encoding
    with open(csv_filename, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)

        # Determine which columns are "language" columns (all after "Key" and "Title")
        language_columns = reader.fieldnames[2:]

        # Prepare structures to store data,
        # where the key is the language name, and the value is a list of "key=value" strings
        lang_data = {lang: [] for lang in language_columns}

        # Iterate through the rows of the CSV
        for row in reader:
            key = row["Key"]
            # If there is something in the Key cell, generate entries for all languages
            if key:
                for lang in language_columns:
                    if not row[lang] or row[lang].isspace():
                        # If the cell is empty, we skip it
                        continue
                    value = row[lang]
                    # Format "key=value"
                    lang_data[lang].append(f"{key}={value}")

    # Now create files for each language and write the collected strings into them
    for lang, lines in lang_data.items():
        with open(f"{localization_dir}/{lang}.txt", 'w', encoding='utf-8') as out_file:
            out_file.write("########################################################################################\n")
            out_file.write("#           This file is auto-generated. Do not edit it manually.\n")
            out_file.write("# If you want to modify it, do it in the CSV file (Data/Localization/localization.csv). \n")
            out_file.write("########################################################################################\n")
            out_file.write("\n")
            for line in lines:
                out_file.write(line + "\n")
        print(f"Created or rewrote {localization_dir}/{lang}.txt.")

if __name__ == "__main__":

    if len(sys.argv) < 3:
        print(YELLOW + "\nIMPORTANT! Do not forget to set working directory for this script to the root of the project.\n" + RESET)
        print(f"Using default CSV file: {CSV_FILENAME} and localization directory: {LOCALIZATION_DIR}")
        csv_filename = CSV_FILENAME
        localization_dir = LOCALIZATION_DIR
    else:
        csv_filename = sys.argv[1]
        localization_dir = sys.argv[2]

    parse_csv_to_lang_files(csv_filename, localization_dir)
    print(GREEN + "Done." + RESET)
