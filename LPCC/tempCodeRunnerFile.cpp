#include <iostream>
#include <sstream>
#include <unordered_map>
#include <string>

using namespace std;

// Function to generate symbol table from assembly code
unordered_map<string, int> generateSymbolTable(const string& assemblyCode) {
    unordered_map<string, int> symbolTable;
    int currentAddress = 0;

    // Use stringstream to tokenize assembly code
    stringstream ss(assemblyCode);
    string line;
    while (getline(ss, line, '\n')) {
        if (line.empty()) {
            continue;
        }

        // Tokenize each line
        stringstream lineStream(line);
        string token;
        lineStream >> token;

        // Check if the token is a label (no specific label ending)
        if (!token.empty() && isalpha(token[0])) {
            // Add label to symbol table with current address
            symbolTable[token] = currentAddress;
        } else {
            // Increment address for instructions and directives
            currentAddress++;
        }

        // Handle DS directive to allocate data storage
        if (token == "DS") {
            string symbol;
            int size;
            lineStream >> symbol >> size;
            symbolTable[symbol] = currentAddress;
            currentAddress += size;
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

    // Print symbol table in the desired format
    cout << "-------------" << endl;
    cout << "Symbol  |       Address" << endl;
    cout << "--------------------" << endl;
    for (const auto& entry : symbolTable) {
        cout << entry.first << "       |       " << entry.second << endl;
    }

    return 0;
}
