#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Function to generate symbol table from assembly code
unordered_map<string, int> generateSymbolTable(const string& assemblyCode) {
    unordered_map<string, int> symbolTable;
    int address = 0;

    // Mnemonic Table (Reserved Mnemonics)
    unordered_map<string, pair<string, int>> reservedMnemonics = {
        {"STOP", {"00", 0}},
        {"ADD", {"01", 2}},
        {"SUB", {"02", 2}},
        {"MUL", {"03", 2}},
        {"MOVER", {"04", 2}},
        {"MOVEM", {"05", 2}},
        {"COMP", {"06", 2}},
        {"BC", {"07", 2}},
        {"DIV", {"08", 2}},
        {"READ", {"09", 1}},
        {"PRINT", {"10", 1}},
        {"LTORG", {"05", 0}},
        {"ORIGIN", {"03", 1}},
        {"START", {"01", 1}},
        {"EQU", {"04", 2}},
        {"DS", {"01", 1}},
        {"DC", {"02", 1}},
        {"END", {"AD", 0}}
    };

    // Split assembly code into lines
    stringstream ss(assemblyCode);
    string line;
    while (getline(ss, line, '\n')) {
        if (line.empty()) {
            continue;
        }

        stringstream lineStream(line);
        string token;
        lineStream >> token;

        if (reservedMnemonics.count(token) > 0) {
            if (token == "START") {
                lineStream >> address;
            } else if (token == "END") {
                break;
            } else if (token == "LTORG" || token == "ORIGIN" || token == "EQU") {
                continue;
            } else {
                address++;
            }
        } else {
            symbolTable[token] = address;
            address++;
        }
    }

    // Second Pass: Update Address for Data Storage (DS directive)
    address--; // Adjust address for data storage after the last instruction
    stringstream ss2(assemblyCode);
    while (getline(ss2, line, '\n')) {
        if (line.empty()) {
            continue;
        }

        stringstream lineStream(line);
        string token;
        lineStream >> token;

        if (token == "DS") {
            string symbol;
            int size;
            lineStream >> symbol >> size;
            symbolTable[symbol] = address;
            address += size;
        }
    }

    return symbolTable;
}

int main() {
    // Example Assembly Language Code
    string assemblyCode = R"(
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
)";

    // Generate symbol table from assembly code
    unordered_map<string, int> symbolTable = generateSymbolTable(assemblyCode);

    // Print symbol table
    cout << "-------------" << endl;
    cout << "Symbol  |       Address" << endl;
    cout << "--------------------" << endl;
    for (const auto& entry : symbolTable) {
        cout << entry.first << "       |       " << entry.second << endl;
    }

    return 0;
}
