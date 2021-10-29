import re
import regex
from lxml import etree
from bs4 import BeautifulSoup

import latex2mathml.converter

def add_label_to_formula(mathml, label):
    soup = BeautifulSoup(mathml, "xml")
    math = soup.find("math")
    row = list(math.children)[0]
    del math["display"]
    table_row = soup.new_tag("mtr")
    mtd = soup.new_tag("mtd")
    row.wrap(mtd)
    mtd.wrap(table_row)
    mtable = soup.new_tag("mtable")
    table_row.wrap(mtable)

    mrow = math.find("mrow")
    maligngroup = soup.new_tag("maligngroup")
    mrow.insert(0, maligngroup)

    top_mrow = soup.new_tag("mrow")
    mtable.wrap(top_mrow)
    maligngroup = soup.new_tag("maligngroup")
    top_mrow.insert(0, maligngroup)
    mtd = soup.new_tag("mtd")
    top_mrow.wrap(mtd)
    mtr = soup.new_tag("mtr")
    mtd.wrap(mtr)
    mtable = soup.new_tag("mtable")
    mtr.wrap(mtable)
    mtable["groupalign"] = "center right"

    mo = soup.new_tag("mo")
    mo.string = "#"
    top_mrow.append(mo)
    mfenced = soup.new_tag("mfenced", separators="|")
    mfenced.string = "#"
    top_mrow.append(mfenced)
    mrow = soup.new_tag("mrow")
    mfenced.append(mrow)
    mn = soup.new_tag("mn")
    mn.string = label
    mrow.append(mn)

    mathml = str(math)

    return mathml

def latex_to_mathml(latex_text, label=""):
    mathml = latex2mathml.converter.convert(latex_text)
    mathml = re.compile(r"(\\newline)|(\\break)|(\\n)").sub("<mspace linebreak='newline'/>", mathml)
    if label!="": mathml = add_label_to_formula(mathml, label)
    return mathml

class LatexExtender:
    def __init__(self, flags) -> None:
        self.flags = self._parse_flags(flags)
        self.extensions = {
            "_common_": LatexExtender.common_extension, 
            "cs": LatexExtender.computer_science_extension,
        }

    def _parse_flags(self, flags):
        flags = "_common_,"+flags 
        flags = flags.replace(" ", "").split(",")
        return flags

    def extend_latex(self, latex_text):
        for flag in self.flags:
            if flag in self.extensions:
                latex_text = self.extensions[flag](latex_text)
        return latex_text
    
    @staticmethod
    def common_extension(expr):
        expr = expr.replace("*", "{\\cdot}")
        return expr
    
    @staticmethod
    def computer_science_extension(expr):
        def format_braces(text):
            next_start_pos = 0
            def search():
                return regex.search(r"\(((?>[^\(\)]+|(?R))*)\)", text, pos=next_start_pos)
            match = search()
            while not match is None:
                span = match.span()
                sign_pos = span[0]-1
                inner_text = match.groups()[0]
                inner_text = format_braces(inner_text)
                if sign_pos>=0:
                    if text[sign_pos]=="!":
                        text = text[:span[0]-1]+"\\bar{"+inner_text+"}"+text[span[1]:]
                next_start_pos = span[1]+1
                match = search()
            return text

        expr = expr.replace("v", "{\\vee}")
        expr = expr.replace("^", "{\\wedge}")
        expr = format_braces(expr)
        expr = re.compile(r'\!(?P<var>.)').sub(r'\\bar{\g<var>}', expr)

        return expr