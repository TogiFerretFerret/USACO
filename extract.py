with open("file.txt","r") as f:
    content = f.read()
import bs4
soup = bs4.BeautifulSoup(content, 'html.parser')
print(soup.get_text())
