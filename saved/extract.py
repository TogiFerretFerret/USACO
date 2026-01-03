import bs4
txt=""
with open("j1p1.html","r") as f:
    txt=f.read()
soup=bs4.BeautifulSoup(txt,features="html.parser")
print(soup.get_text())
