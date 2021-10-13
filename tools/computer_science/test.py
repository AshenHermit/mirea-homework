import utils
import math
import logisim_circuit_generator

#TODO: make CLI of it

vocab = [0,1]
function = "1111001101100101"

def print_table(function, for_word=False):
    print("x y z w   F")
    for i, word in enumerate(utils.word_iterator(vocab, 4)):
        x,y,z,w = tuple(map(bool, word))
        result = function[i]
        if for_word:
            print(f"{int(x)}\t{int(y)}\t{int(z)}\t{int(w)}\t{result}")
        else:
            print(f"{int(x)} {int(y)} {int(z)} {int(w)}   {result}")

def print_karno_map(function, show_ones=True):
    axis = ["00", "01", "11", "10"]
    print("karno map:")
    print("ab/cd " + " ".join(axis))
    values = { "".join(map(str,word)):function[i] for i,word in enumerate(utils.word_iterator(vocab, 4))}
    for j in range(len(axis)):
        line = f"{axis[j]}    "
        for i in range(len(axis)):
            result = values[f"{axis[j]}{axis[i]}"]
            if (result=="1" and show_ones) or (result=="0" and not show_ones): 
                line += result+"  "
            else: 
                line+="   "
        print(line)

print_table(function, for_word=True)
print()
print_karno_map(function, False)

working_directory = "./"

logisim_circuit_generator.generate_task_1_circuit(function,
    working_directory+"empty.circ",
    working_directory+"generated.circ",
    False)


import re
def expression_to_latex(expr):
    expr = expr.replace("*", "{\\bullet}")
    expr = expr.replace("v", "{\\vee}")
    expr = re.compile(r'\!(?P<var>.)').sub(r'\\bar{\g<var>}', expr)
    return expr
