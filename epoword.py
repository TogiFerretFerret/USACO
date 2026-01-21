import zipfile
from bs4 import BeautifulSoup
import re
import os

def count_words_raw(file_path):
    total_words = 0
    
    # Check if file exists first
    if not os.path.exists(file_path):
        print(f"Error: {file_path} not found.")
        return 0

    try:
        # Open the EPUB as a standard ZIP file
        with zipfile.ZipFile(file_path, 'r') as z:
            # Loop through every file inside the zip
            for filename in z.namelist():
                # We only care about content files (HTML/XHTML)
                if filename.lower().endswith(('.html', '.xhtml', '.htm')):
                    
                    # Read the raw bytes
                    with z.open(filename) as f:
                        content = f.read()
                        
                        # Strip HTML tags
                        soup = BeautifulSoup(content, 'html.parser')
                        text = soup.get_text()
                        
                        # Count words
                        words = re.findall(r'\w+', text)
                        total_words += len(words)
                        
    except zipfile.BadZipFile:
        print(f"Error: {file_path} is not a valid zip/epub file.")
        return 0
    except Exception as e:
        print(f"Error processing {file_path}: {e}")
        return 0

    return total_words

def main():
    # Update this list with your actual file names
    files = ["v01.epub", "v02.epub", "v03.epub"]
    grand_total = 0

    print(f"{'Filename':<30} | {'Word Count':<15}")
    print("-" * 45)

    for file in files:
        count = count_words_raw(file)
        # 0 usually means file not found or error, so we skip adding it to visual noise
        if count > 0:
            print(f"{file:<30} | {count:,}")
            grand_total += count
        else:
            print(f"{file:<30} | {'Error/Empty'}")

    print("-" * 45)
    print(f"{'TOTAL':<30} | {grand_total:,}")

if __name__ == "__main__":
    main()
