def generate_mdt_mnt(code):
    # Initialize Macro Name Table (MNT) and Macro Definition Table (MDT)
    macro_name_table = {}
    macro_definition_table = {}
    mdt_index = 1

    # Split code into lines
    lines = code.strip().split('\n')
    i = 0

    # First Pass: Build Macro Name Table (MNT)
    while i < len(lines):
        line = lines[i].strip()
        tokens = line.split()

        if tokens and tokens[0] == 'MACRO':
            macro_name = tokens[1]
            parameters = tokens[2:] if len(tokens) > 2 else []

            # Collect macro definition
            macro_definition = []
            i += 1
            while i < len(lines) and lines[i].strip() != 'MEND':
                macro_definition.append(lines[i].strip())
                i += 1

            # Record macro details in MNT
            macro_name_table[macro_name] = {
                'index': mdt_index,
                'parameters': parameters
            }

            # Record macro definition in MDT
            macro_definition_table[mdt_index] = macro_definition
            mdt_index += 1

        i += 1

    return macro_name_table, macro_definition_table

# Example Macro-Expanded Code
code = """
LOAD A
STORE B
MACRO ABC
LOAD p
SUB q
MEND
MACRO ADD1 ARG
LOAD X
STORE ARG
MEND
MACRO ADD5 A1, A2, A3
STORE A2
ADD1 5
ADD1 10
LOAD A1
LOAD A3
MEND
ABC
ADD5 D1, D2, D3
END
"""

# Generate MNT and MDT
macro_name_table, macro_definition_table = generate_mdt_mnt(code)

# Print Macro Name Table (MNT)
print("Macro Name Table (MNT):")
print("Macro Name".ljust(10), "MDT Index".ljust(10), "Parameters")
print("-------------------------------------------")
for macro_name, details in macro_name_table.items():
    index = str(details['index'])
    parameters = ', '.join(details['parameters']) if details['parameters'] else 'None'
    print(macro_name.ljust(10), index.ljust(10), parameters)

# Print Macro Definition Table (MDT)
print("\nMacro Definition Table (MDT):")
print("MDT Index".ljust(10), "Macro Definition")
print("-------------------------------------------")
for index, definition in macro_definition_table.items():
    print(str(index).ljust(10), ' '.join(definition))
