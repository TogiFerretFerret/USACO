with open(__import__('sys').argv[1],"r") as f:
    print(__import__("bs4").BeautifulSoup(f.read(),"html.parser").get_text())
