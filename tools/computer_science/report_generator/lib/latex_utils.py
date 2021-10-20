import re
import regex

import latex2mathml.converter

def latex_to_mathml(latex_text):
    latex_text = latex2mathml.converter.convert(latex_text)
    latex_text = re.compile(r"(\\newline)|(\\break)|(\\n)").sub("<mspace linebreak='newline'/>", latex_text)
    return latex_text


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