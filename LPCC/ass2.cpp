#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cctype> // For isdigit()

using namespace std;

// Function to generate Literal table from assembly code
unordered_map<string, int> generateLiteralTable(const string& assemblyCode) {
    unordered_map<string, int> literalTable;
    int currentAddress = 0;

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

        // Check for instructions that involve literals
        if (token == "MOVER" || token == "ADD" || token == "COMP" || token == "SUB") {
            string operand1, operand2;
            lineStream >> operand1 >> operand2;

            // Check if operand is a literal (e.g., ='50')
            if (operand2.size() > 2 && operand2[0] == '=' && operand2[1] == '\'') {
                // Extract the literal value
                string literal = operand2.substr(2, operand2.size() - 3);

                // Add literal to the table if not already present
                if (literalTable.find(literal) == literalTable.end()) {
                    literalTable[literal] = currentAddress++;
                }
            }
        }
    }

    return literalTable;
}

int main() {
    // Example Assembly Language Code
    string assemblyCode = R"(
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
)";

    // Generate literal table from assembly code
    unordered_map<string, int> literalTable = generateLiteralTable(assemblyCode);

    // Print literal table
    cout << "-------------" << endl;
    cout << "Literal  |  Address" << endl;
    cout << "--------------------" << endl;
    for (const auto& entry : literalTable) {
        cout << entry.first << "  |  " << entry.second << endl;
    }

    return 0;
}
