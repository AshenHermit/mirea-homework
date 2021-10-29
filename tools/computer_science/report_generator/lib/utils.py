import os
import re
import math

def real_path_of_file(filepath):
    return os.path.realpath(filepath).replace("\\", "/")

def dir_name_of_file(filepath):
    return os.path.dirname(os.path.realpath(filepath)).replace("\\", "/")+"/"

def split_by_spaces(text):
    carret_state = 0
    breaks = []
    for i in range(len(text)):
        if text[i] == "<":
            carret_state = 1
        elif text[i] == ">":
            if text[i-1] == "/":
                carret_state = 0
            if carret_state == 2:
                carret_state = 0
        elif text[i] == "/" and text[i-1] == "<":
            carret_state = 2
        elif carret_state==0 and (text[i] == " " or text[i] == "\t" or text[i] == "\n"):
            breaks.append(i)
    
    elements = []
    for i in range(len(breaks)+1):
        if i==0: start = -1
        else: start = breaks[i-1]
        if i==len(breaks): end = len(text)
        else: end = breaks[i]
        elements.append(text[start+1:end])
    return elements

def text_to_simple_table(text, width=None):
    if width is None:
        empty_sym = text[0]
        not_empty_match = re.compile(r"(?!\s)").search(text)
        line_border_width = not_empty_match.span()[0]
        empty_area = empty_sym * line_border_width
        width = text.count(empty_area)
    
    def filter_empty_elements(lst):
        return list(filter(lambda x: x!="", lst))
    # re.split('\\t|    ', row)
    cells = filter_empty_elements(split_by_spaces(text))

    def get_cell_value(x,y):
        index = y*width+x
        if index >= len(cells): return ""
        return cells[index]
    table = [[get_cell_value(x,y) for x in range(width)] for y in range(math.ceil(len(cells)/width))] 
    return table

def try_turn_string_to_number(value):
    if not type(value) is str: return value
    if value.replace(".", "").isnumeric():
        if value.find(".")!=-1:
            return float(value)
        else:
            return int(value)
    else:
        return value