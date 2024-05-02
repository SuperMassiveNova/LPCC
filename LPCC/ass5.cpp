#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

// Symbol table to store macro definitions
unordered_map<string, vector<string>> macroTable;

// Function to expand macros in the given code
vector<string> expandMacros(const string& code) {
    vector<string> expandedCode;
    stringstream ss(code);
    string line;

    while (getline(ss, line, '\n')) {
        if (line.empty()) {
            continue;
        }

        istringstream lineStream(line);
        string token;
        lineStream >> token;

        if (token == "MACRO") {
            // Extract macro name and parameters
            string macroName;
            lineStream >> macroName;
            vector<string> macroDefinition;
            while (getline(ss, line, '\n')) {
                if (line == "MEND") {
                    break;
                }
                macroDefinition.push_back(line);
            }
            macroTable[macroName] = macroDefinition;
        } else if (macroTable.find(token) != macroTable.end()) {
            // Macro call detected, expand the macro
            const vector<string>& macroDefinition = macroTable[token];
            for (const string& macroLine : macroDefinition) {
                expandedCode.push_back(macroLine);
            }
        } else {
            // Normal instruction, add to expanded code
            expandedCode.push_back(line);
        }
    }

    return expandedCode;
}

// Function to generate Intermediate code from expanded macro code
vector<string> generateIntermediateCode(const vector<string>& expandedCode) {
    vector<string> intermediateCode;
    int lineNum = 1;

    for (const string& line : expandedCode) {
        if (line.empty()) {
            continue;
        }

        istringstream lineStream(line);
        string token;
        lineStream >> token;

        if (token == "LOAD" || token == "STORE" || token == "ADD" || token == "SUB" || token == "MULT" || token == "END") {
            // Process instructions directly into intermediate code
            intermediateCode.push_back(to_string(lineNum) + ": " + line);
            lineNum++;
        } else if (token == "MACRO") {
            // Skip macro definitions in intermediate code generation
            continue;
        }
    }

    return intermediateCode;
}

int main() {
    // Example Macro-Expanded Code
    string code = R"(
LOAD A
MACRO ABC
LOAD p
SUB q
MEND
STORE B
MULT D
MACRO ADD1 ARG
LOAD X
STORE ARG
MEND
LOAD B
MACRO ADD5 A1, A2, A3
STORE A2
ADD1 5
ADD1 10
LOAD A1
LOAD A3
MEND
ADD1 t
ABC
ADD5 D1, D2, D3
END
)";

    // Expand macros in the code
    vector<string> expandedCode = expandMacros(code);

    // Generate Intermediate code from expanded code
    vector<string> intermediateCode = generateIntermediateCode(expandedCode);

    // Print Intermediate code
    cout << "Intermediate Code:" << endl;
    for (const string& line : intermediateCode) {
        cout << line << endl;
    }

    return 0;
}
