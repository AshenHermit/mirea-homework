import utils
import math
import logisim_circuit_generator

#TODO: make CLI of it

vocab = [0,1]
functions = ["1111001101100101", "1011010101111100", "1101110010001111", "1111110100010011"]

def print_table(functions, for_word=False):
    rank = 4
    functions = list(map(lambda x: ("0"*(rank**2-len(x)))+x, functions))
    function_names = [f"F{i+1}" for i in range(len(functions))]
    if for_word: 
        function_names = list(map(lambda x: "<b>"+x+"</b>", function_names))
        print("<b>a</b>\t<b>b</b>\t<b>c</b>\t<b>d</b>\t"+"\t".join(function_names))
    else: print("a b c d   " + " ".join(function_names))
    for i, word in enumerate(utils.word_iterator(vocab, rank)):
        a,b,c,d = tuple(map(bool, word))
        results = list(map(lambda x: x[i], functions))
        if for_word:
            print(f"{int(a)}\t{int(b)}\t{int(c)}\t{int(d)}\t"+'\t'.join(results))
        else:
            print(f"{int(a)} {int(b)} {int(c)} {int(d)}   "+' '.join(results))

def print_karno_map(function, show_ones=True, for_word=False):
    axis = ["00", "01", "11", "10"]
    print("karno map:")
    print("ab/cd "+ ("\t" if for_word else "") +("\t" if for_word else " ").join(axis))
    values = { "".join(map(str,word)):function[i] for i,word in enumerate(utils.word_iterator(vocab, 4))}
    for j in range(len(axis)):
        line = f"{axis[j]}"+"\t" if for_word else "    "
        for i in range(len(axis)):
            result = values[f"{axis[j]}{axis[i]}"]
            if (result=="1" and show_ones) or (result=="0" and not show_ones): 
                line += result+"\t" if for_word else "  "
            else: 
                line+="\t" if for_word else "   "
        print(line) 

print_table(functions, for_word=True)
print()
print_karno_map(functions[0], show_ones=False, for_word=True)

working_directory = "C:/Users/user/Downloads/LOVT2021/projects/"

# logisim_circuit_generator.generate_task_1_circuit(function,
#     working_directory+"empty.circ",
#     working_directory+"generated.circ",
#     False)


import re
def expression_to_latex(expr):
    expr = expr.replace("*", "{\\bullet}")
    expr = expr.replace("v", "{\\vee}")
    expr = re.compile(r'\!(?P<var>.)').sub(r'\\bar{\g<var>}', expr)
    return expr
