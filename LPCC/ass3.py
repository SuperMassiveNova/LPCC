def generate_pool_table(assembly_code):
    pool_table = []
    current_pool_start = None

    # Split assembly code into lines
    lines = assembly_code.strip().split('\n')

    # Process each line of the assembly code
    for line in lines:
        tokens = line.split()

        # Check if the line has tokens
        if len(tokens) > 0:
            mnemonic = tokens[0]

            # Check for LTORG directive to identify the end of a literal pool
            if mnemonic == "LTORG":
                if current_pool_start is not None:
                    # End of current pool, record the pool start address
                    pool_table.append(current_pool_start)
                    current_pool_start = None
            elif mnemonic in ['MOVER', 'MOVEM', 'ADD', 'COMP', 'SUB', 'PRINT']:
                for operand in tokens[1:]:
                    if operand.startswith("='") and operand.endswith("'"):
                        # Literal found, check if it's the start of a new pool
                        if current_pool_start is None:
                            current_pool_start = len(pool_table)  # Use index as address for simplicity

    # Check if there's an open pool at the end of the code
    if current_pool_start is not None:
        pool_table.append(current_pool_start)

    return pool_table

# Example Assembly Language Code
assembly_code = """
START 100
READ A
MOVER AREG, ='1'
MOVEM AREG, B
MOVER BREG, ='6'
ADD AREG, BREG
COMP AREG, A
BC GT, LAST
LTORG
NEXT SUB AREG, ='1' MOVER CREG, B
ADD CREG, ='8'
MOVEM CREG, B
PRINT B
LAST STOP
A DS 1
B DS 1
END
"""

# Generate pool table from assembly code
pool_table = generate_pool_table(assembly_code)

# Print pool table in the expected format
print("-------------")
print("Pool Address")
print("-------------")
for i, address in enumerate(pool_table):
    print(f"Pool {i+1}: {address}")
