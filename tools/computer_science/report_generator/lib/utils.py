import os
import re

def real_path_of_file(filepath):
    return os.path.realpath(filepath).replace("\\", "/")

def dir_name_of_file(filepath):
    return os.path.dirname(os.path.realpath(filepath)).replace("\\", "/")+"/"

def text_to_simple_table(text):
    empty_sym = text[0]
    not_empty_match = re.compile(r"(?!\s)").search(text)
    line_border_width = not_empty_match.span()[0]
    empty_area = empty_sym * line_border_width
    text = text.replace(empty_area, "\n")
    def filter_empty_elements(lst):
        return list(filter(lambda x: x!="", lst))
    rows = list(filter_empty_elements(text.split("\n")))
    table = list(map(lambda row: filter_empty_elements(re.split('\\t|    ', row)), rows))
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