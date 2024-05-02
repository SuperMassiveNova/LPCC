def generate_symbol_table(assembly_code):
    symbol_table = {}
    address = 0

    # Mnemonic Table (Reserved Mnemonics)
    # reserved_mnemonics = {
    #     'STOP', 'ADD', 'SUB', 'MUL', 'MOVER', 'MOVEM', 'COMP', 'BC',
    #     'DIV', 'READ', 'PRINT', 'LTORG', 'ORIGIN', 'START', 'EQU', 'DS', 'DC', 'END'
    # }

    reserved_mnemonics = {
        'STOP': ('00', 'IS', 0),
        'ADD': ('01', 'IS', 2),
        'SUB': ('02', 'IS', 2),
        'MUL': ('03', 'IS', 2),
        'MOVER': ('04', 'IS', 2),
        'MOVEM': ('05', 'IS', 2),
        'COMP': ('06', 'IS', 2),
        'BC': ('07', 'IS', 2),
        'DIV': ('08', 'IS', 2),
        'READ': ('09', 'IS', 1),
        'PRINT': ('10', 'IS', 1),
        'LTORG': ('05', 'AD', 0),
        'ORIGIN': ('03', 'AD', 1),
        'START': ('01', 'AD', 1),
        'EQU': ('04', 'AD', 2),
        'DS': ('01', 'DL', 1),
        'DC': ('02', 'DL', 1),
        'END': ('AD', 0)
    }


    # First Pass: Populate Symbol Table with Labels and their Addresses
    lines = assembly_code.split('\n')
    for line in lines:
        tokens = line.split()
        if len(tokens) == 0:
            continue
        if tokens[0] in reserved_mnemonics:
            if tokens[0] == "START":
                address = int(tokens[1])
            elif tokens[0] == "END":
                break
            elif tokens[0] == "LTORG" or tokens[0] == "ORIGIN" or tokens[0] == "EQU":
                continue
            else:
                address += 1
        else:
            symbol_table[tokens[0]] = address
            address += 1

    # Second Pass: Update Address for Data Storage
    for line in lines:
        tokens = line.split()
        if len(tokens) == 0:
            continue
        if tokens[0] == "DS":
            symbol_table[tokens[1]] = address
            address += int(tokens[2])

    return symbol_table

# Example Assembly Language Code
assembly_code = """
START 180
READ M
READ N
LOOP MOVER AREG, M
MOVER BREG, N
COMP BREG, ='200'
BC GT, LOOP
BACK SUB AREG, M
COMP AREG, ='500'
BC LT, BACK
STOP
M DS 1
N DS 1
END
"""

symbol_table = generate_symbol_table(assembly_code)
print("Symbol Table:")
for symbol, address in symbol_table.items():
    print(f"{symbol}: {address}")