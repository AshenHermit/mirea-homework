import os
import re
import math

def real_path_of_file(filepath):
    return os.path.realpath(filepath).replace("\\", "/")

def dir_name_of_file(filepath):
    return os.path.dirname(os.path.realpath(filepath)).replace("\\", "/")+"/"

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
    cells = filter_empty_elements(re.split(r'\s', text))

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