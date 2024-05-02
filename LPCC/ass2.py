def generate_literal_table(assembly_code):
    literal_table = {}
    current_address = 0

    # Split assembly code into lines
    lines = assembly_code.strip().split('\n')

    # Process each line of the assembly code
    for line in lines:
        tokens = line.split()

        # Check if the line has tokens
        if len(tokens) > 1:
            mnemonic = tokens[0]
            operands = tokens[1:]

            # Check if the instruction involves a literal
            if mnemonic in ['MOVER', 'ADD', 'SUB', 'COMP']:
                for operand in operands:
                    if operand.startswith("='") and operand.endswith("'"):
                        literal = operand[2:-1]  # Extract the literal value
                        if literal not in literal_table:
                            literal_table[literal] = current_address
                            current_address += 1

    return literal_table

# Example Assembly Language Code
assembly_code = """
START 100
READ A
READ B
MOVER AREG, ='50'
MOVER BREG, ='60'
ADD AREG, BREG
LOOP MOVER CREG, A
ADD CREG, ='10'
COMP CREG, B
BC LT, LOOP
NEXT SUB AREG, ='10'
COMP AREG, B
BC GT, NEXT
STOP
A DS 1
B DS 1
END
"""

# Generate literal table from assembly code
literal_table = generate_literal_table(assembly_code)

# Print literal table in the expected format
print("-----------------")
print("Literal  |  Address")
print("-----------------")
for literal, address in literal_table.items():
    print(f"'{literal}'     |  {address}")
