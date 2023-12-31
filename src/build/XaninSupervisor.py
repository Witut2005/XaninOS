import os

def count_folders(path):
    count = 0
    for root, dirs, files in os.walk(path):
        count += len(dirs)
    return count

dir_path = '../'
num_folders = count_folders(dir_path)

#print(f"How it should look like(number of md files): {num_folders}")

def count_md_files(path):
    count = 0
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith('.md'):
                count += 1
    return count

dir_path = '../'
num_md_files = count_md_files(dir_path)

#print(f"How it acctually looks: {num_md_files}")

if(num_folders != num_md_files):
    print(f"\033[91mError: Where are your {num_folders - num_md_files} md files!?\033[0m")


