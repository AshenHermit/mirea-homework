import xml.etree.ElementTree as ET
import utils
import math

#TODO: make object oriented expression parser and circuit generator

def generate_task_1_circuit(output_values, empty_circuit_filepath, generated_filepath, use_disjunction=True):
    tree = ET.parse(empty_circuit_filepath)
    root = tree.getroot()
    circuit = root.find("circuit")

    offset_x = 10
    offset_y = 5*10
    wires_x_interval = 2
    # wires
    for wi in range(4):
        x = offset_x + wi*wires_x_interval*10
        y1, y2 = offset_y, offset_y+1024
        circuit.append(ET.Element('wire', attrib={"from": f"({x},{y1})", "to": f"({x},{y2})"}))

    print()
    print(" x  y  z  w   F")
    expression_parts = []
    bit_count = 4
    section_y = 0
    outputs_count = 0
    vocab = [0,1]
    for i, word in enumerate(utils.word_iterator(vocab, bit_count)):
        x,y,z,w = tuple(map(bool, word))
        result = output_values[i]

        valid = bool(int(result))
        if not use_disjunction: valid = not valid
        if valid:
            joiner = "*" if use_disjunction else "+"
            part = joiner.join(map(lambda p: ("" if (not p[1] and use_disjunction) or (p[1] and not use_disjunction) else "!") + "abcd"[p[0]], enumerate([x,y,z,w])))
            part = f"({part})"
            expression_parts.append(part)
            print(f"{part}")

            outputs_count += 1
            for wi, value in enumerate([x,y,z,w]):
                x1 = offset_x + wi*wires_x_interval*10
                x2 = offset_x + bit_count*wires_x_interval*10
                ys = offset_y + 2*10 + section_y*10 + wi*10
                y1 = ys
                x3 = x2+3*10
                x4 = x3+1*10
                if len(word) == 4 and wi >= 2:
                    y1 += 10
                circuit.append(ET.Element('wire', attrib={"from": f"({x1},{y1})", "to": f"({x2},{y1})"}))
                if (not value and use_disjunction) or (value and not use_disjunction):
                    circuit.append(ET.Element('comp', attrib={"lib": "1", "name": "NOT Gate", "loc": f"({x3},{y1})"}))
                else:
                    circuit.append(ET.Element('wire', attrib={"from": f"({x2},{y1})", "to": f"({x3},{y1})"}))
                circuit.append(ET.Element('wire', attrib={"from": f"({x3},{y1})", "to": f"({x4},{y1})"}))
            section_y+=bit_count+1

            x1 = x4
            x2 = x1 + 5*10
            y1 = ys
            y2 = y1 - 10*bit_count + 3*10
            gate = ET.Element('comp', attrib={"lib": "1", 
                "name": "AND Gate" if use_disjunction else "OR Gate", "loc": f"({x2},{y2})"})
            gate.append(ET.Element('a', attrib={"name": "inputs", "val": str(len(word))}))
            circuit.append(gate)

    # they should be more flexible, have multiple layers
    # OR components
    inputs_count = 5
    for i in range(math.ceil(outputs_count/inputs_count)):
        x1 = x2 + 5*10 + inputs_count*10
        y1 = offset_y + i*inputs_count*10*inputs_count + 3*2*10
        circuit.append(ET.Element('comp', attrib={"lib": "1", 
            "name": "OR Gate" if use_disjunction else "AND Gate", "loc": f"({x1},{y1})"}))

        for wi in range(min(i*inputs_count, outputs_count-i*inputs_count)):
            wx1 = x1 - 5*10 - inputs_count*10 + wi
            wy1 = y1 - 3*10
            wx2 = wx1 + wi
            circuit.append(ET.Element('wire', attrib={"from": f"({wx1},{wy1})", "to": f"({wx2},{wy1})"}))

    print()
    joiner = "+" if use_disjunction else "*"
    expression = joiner.join(expression_parts)
    print(f"{expression}")




    tree.write(generated_filepath)