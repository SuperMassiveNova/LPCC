def generate_intermediate_code(assembly_code):
    intermediate_code = []
    symbol_table = {}
    data_allocation = {}
    current_address = 0
    data_offset = 0

    # First Pass: Process instructions and build symbol table
    lines = assembly_code.strip().split('\n')
    for line in lines:
        tokens = line.split()
        if len(tokens) == 0:
            continue

        mnemonic = tokens[0]

        if mnemonic == 'START':
            current_address = int(tokens[1])
        elif mnemonic == 'END':
            break
        elif mnemonic == 'DS':
            var_name = tokens[1]
            var_size = int(tokens[2])
            data_allocation[var_name] = data_offset
            data_offset += var_size
        elif mnemonic.endswith(':'):
            # Label definition
            label = mnemonic[:-1]
            symbol_table[label] = current_address
        else:
            # Instruction with operands
            op_code = mnemonic
            operands = ', '.join(tokens[1:])
            intermediate_code.append(f"{current_address}: {op_code} {operands}")
            current_address += 1

    # Second Pass: Resolve symbolic addresses and update data allocation
    for i, line in enumerate(intermediate_code):
        parts = line.split()
        if len(parts) > 1:
            op_code = parts[1]
            operands = ' '.join(parts[2:])
            for token in operands.split(', '):
                if token in symbol_table:
                    operands = operands.replace(token, str(symbol_table[token]))
            intermediate_code[i] = f"{parts[0]}: {op_code} {operands}"

    # Add data allocation information to the intermediate code
    for var_name, offset in data_allocation.items():
        intermediate_code.append(f"{current_address}: DS {var_name}, {offset}")
        current_address += 1

    return intermediate_code

# Example Assembly Language Code
assembly_code = """
START 100
READ A
READ B
MOVER AREG, A
SUB AREG, B
STOP
A DS 1
B DS 1
END
"""

# Generate intermediate code from assembly code
intermediate_code = generate_intermediate_code(assembly_code)

# Print intermediate code
for line in intermediate_code:
    print(line)
