import os


def count_lines_in_files(file_list):
    total_lines = 0
    file_counts = {}

    print(f"{'File Name':<30} | {'Line Count':>10}")
    print("-" * 43)

    for file_path in file_list:
        if os.path.exists(file_path):
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    # Efficiently count lines without loading whole file
                    lines = sum(1 for _ in f)
                    file_counts[file_path] = lines
                    total_lines += lines
                    print(f"{os.path.basename(file_path):<30} | {lines:>10}")
            except Exception as e:
                print(f"Error reading {file_path}: {e}")
        else:
            print(f"{os.path.basename(file_path):<30} | {'Not Found':>10}")

    print("-" * 43)
    print(f"{'Total':<30} | {total_lines:>10}")


files_to_count = [
    'tetris.c',
    'input.c',
    'audio.c',
    'playfield.c',
    'tetromino.c',
    'render.c',

    'positions.h',
    'constants.h',

    'input.h',
    'audio.h',
    'playfield.h',
    'tetromino.h',
    'render.h'
]

if __name__ == "__main__":
    count_lines_in_files(files_to_count)
