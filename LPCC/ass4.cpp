#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

// Function to generate Intermediate code from assembly code
vector<string> generateIntermediateCode(const string& assemblyCode) {
    vector<string> intermediateCode;
    unordered_map<string, int> symbolTable;
    unordered_map<string, int> dataAllocation;
    int currentAddress = 0;
    int dataOffset = 0;

    // First Pass: Process instructions and build symbol table
    stringstream ss(assemblyCode);
    string line;
    while (getline(ss, line, '\n')) {
        if (line.empty()) {
            continue;
        }

        istringstream lineStream(line);
        string token;
        lineStream >> token;

        if (token == "START") {
            lineStream >> currentAddress;
        } else if (token == "END") {
            break;
        } else if (token == "DS") {
            string varName;
            int varSize;
            lineStream >> varName >> varSize;
            dataAllocation[varName] = dataOffset;
            dataOffset += varSize;
        } else if (token.back() == ':') {
            // Label definition
            string label = token.substr(0, token.length() - 1);
            symbolTable[label] = currentAddress;
        } else {
            // Instruction with operands
            string opcode = token;
            string operands;
            getline(lineStream >> ws, operands);
            operands.erase(remove(operands.begin(), operands.end(), ','), operands.end());

            // Replace labels with addresses in operands
            stringstream operandStream(operands);
            string operand;
            string updatedOperands;
            while (getline(operandStream, operand, ' ')) {
                if (symbolTable.find(operand) != symbolTable.end()) {
                    operand = to_string(symbolTable[operand]);
                }
                updatedOperands += operand + " ";
            }

            intermediateCode.push_back(to_string(currentAddress) + ": " + opcode + " " + updatedOperands);
            currentAddress++;
        }
    }

    // Second Pass: Update intermediate code with data allocation information
    for (auto& data : dataAllocation) {
        intermediateCode.push_back(to_string(currentAddress) + ": DS " + data.first + ", " + to_string(data.second));
        currentAddress++;
    }

    return intermediateCode;
}

int main() {
    // Example Assembly Language Code
    string assemblyCode = R"(
START 100
READ A
READ B
MOVER AREG, A
SUB AREG, B
STOP
A DS 1
B DS 1
END
)";

    // Generate intermediate code from assembly code
    vector<string> intermediateCode = generateIntermediateCode(assemblyCode);

    // Print intermediate code
    for (const string& codeLine : intermediateCode) {
        cout << codeLine << endl;
    }

    return 0;
}
