def generate_mnt_mdt(code):
    mnt = {}  # Macro Name Table (MNT)
    mdt = {}  # Macro Definition Table (MDT)
    mdt_index = 1

    i = 0
    while i < len(code):
        line = code[i].strip()
        tokens = line.split()

        if tokens and tokens[0] == "MACRO":
            macro_name = tokens[1]
            parameters = tokens[2:] if len(tokens) > 2 else []

            # Collect macro definition
            macro_definition = []
            i += 1
            while i < len(code) and code[i].strip() != "MEND":
                macro_definition.append(code[i].strip())
                i += 1

            # Record macro details in MNT
            mnt[macro_name] = (mdt_index, parameters)

            # Record macro definition in MDT
            mdt[mdt_index] = macro_definition
            mdt_index += 1

        i += 1

    return mnt, mdt

# Input Macro-Expanded Code
code = [
    "LOAD J",
    "STORE M",
    "MACRO EST1",
    "LOAD e",
    "ADD d",
    "MEND",
    "MACRO EST ABC",
    "EST1",
    "STORE ABC",
    "MEND",
    "MACRO ADD7 P4, P5, P6",
    "LOAD P5",
    "EST 8",
    "SUB4 2",
    "STORE P4",
    "STORE P6",
    "MEND",
    "EST",
    "ADD7 C4, C5, C6",
    "END"
]

# Generate MNT and MDT
mnt, mdt = generate_mnt_mdt(code)

# Print Macro Name Table (MNT)
print("Macro Name Table (MNT):")
print("Macro Name\tMDT Index\tParameters")
print("----------------------------------------------")
for macro_name, (mdt_index, parameters) in mnt.items():
    param_str = ", ".join(parameters) if parameters else "None"
    print(f"{macro_name}\t\t{mdt_index}\t\t{param_str}")

# Print Macro Definition Table (MDT)
print("\nMacro Definition Table (MDT):")
print("MDT Index\tMacro Definition")
print("----------------------------------------------")
for mdt_index, definition in mdt.items():
    print(f"{mdt_index}\t\t{definition}")
