f=dict(zip([f"p{x}.txt" for x in list("abc")],[f"p{x}.html" for x in list("abc")]));
for v in list(f.keys()):
    with open(f[v],"r") as z:
        with open(v,"w") as g: 
            g.write(__import__("bs4").BeautifulSoup(z.read(),features="html.parser").get_text())
